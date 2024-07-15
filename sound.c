


//https://onestepcode.com/read-wav-header/
typedef struct WAV_HEADER {
  char RIFF[4];
  int ChunkSize;
  char WAVE[4];
  char fmt[4];
  int Subchunk1Size;
  short int AudioFormat;
  short int NumOfChan;
  int SamplesPerSec;
  int bytesPerSec;
  short int blockAlign;
  short int bitsPerSample;
  int Subchunk2Size;
  char Subchunk2ID[4];
} AWavHeader;


typedef struct WavSoundEffect
{
  int duration;
  long filesize;  
  AWavHeader* wav_header;
  int16_t* audio;
} wavSoundEffect;

typedef struct WavSoundEffectCache
{
  int16_t* audio;
} wavSoundEffectCache;

typedef struct WavSFX
{
  bool freed;
  wavSoundEffect* wavSFX;
  wavSoundEffectCache* wavSFXCache;
} AWavSFX;


typedef struct WavChannelSFX
{
  bool is_cache;
  int duration;
  long filesize;
  wavSoundEffect* wavSFX;
  wavSoundEffectCache* wavSFXCache;
} AWavChannelSFX;

#define SND_THREAD_NUM    3
AWavChannelSFX memSFX[SND_THREAD_NUM];


void InitWavSFX(AWavSFX* myWavSFX, wavSoundEffect* wavSFX, wavSoundEffectCache* wavSFXCache)
{
  myWavSFX->freed=FALSE;
  myWavSFX->wavSFX=wavSFX;
  myWavSFX->wavSFXCache=wavSFXCache;
}

void freeSoundEffectWFX(wavSoundEffect* mySoundEffect)
{
  if (mySoundEffect->wav_header!=NULL)
    free(mySoundEffect->wav_header);
}

void freeSoundEffect(wavSoundEffect* mySoundEffect) 
{
  if (mySoundEffect->audio!=NULL)
    free(mySoundEffect->audio);
}


void freeSFX(AWavSFX* mySFX)
{
  freeSoundEffect(mySFX->wavSFX);
  freeSoundEffectWFX(mySFX->wavSFX);
  if (mySFX->wavSFX!=NULL)
    free(mySFX->wavSFX);
}

//void loadSoundEffect(wavSoundEffect* mySoundEffect, const wchar_t* filename,WAVEFORMATEX wfx,bool skip_header)
void loadSoundEffect(AWavSFX* mySoundEffect, const wchar_t* filename,bool skip_header)
{
  freeSFX(mySoundEffect);
  FILE* file = _wfopen(filename, L"rb");
  int wav_header_size=0; 
  if (skip_header) {
    wav_header_size=sizeof(AWavHeader); //44
  }
  if (file) {
    fseek(file, 0, SEEK_END);
    long filesize;
    filesize = ftell(file) - wav_header_size; 

    //Alloc wav header
    fseek(file, 0, SEEK_SET);
    mySoundEffect->wavSFX->wav_header = malloc(wav_header_size);
    fread(mySoundEffect->wavSFX->wav_header,1,wav_header_size,file);


    //Alloc actual audio int16_t*
    fseek(file, wav_header_size, SEEK_SET);
    mySoundEffect->wavSFX->audio = malloc(filesize);
    fread(mySoundEffect->wavSFX->audio, 1, filesize, file); //read once filesize

    fclose(file);

    mySoundEffect->wavSFX->filesize = filesize;
    mySoundEffect->wavSFX->duration = (double)filesize / ( mySoundEffect->wavSFX->wav_header->SamplesPerSec * mySoundEffect->wavSFX->wav_header->NumOfChan * mySoundEffect->wavSFX->wav_header->bitsPerSample/8) *1000;
  }
}





void freeSoundEffectCache(wavSoundEffectCache* mySoundEffectCache) 
{
  if (mySoundEffectCache->audio!=NULL)
    free(mySoundEffectCache->audio);
}



void freeSFXCache(AWavSFX* mySFX)
{
  //freeSoundEffectCache(mySFX->wavSFXCache);
  mySFX->freed=TRUE;

  if (mySFX->wavSFXCache->audio!=NULL)
    free(mySFX->wavSFXCache->audio);


  if (mySFX->wavSFXCache!=NULL)
    free(mySFX->wavSFXCache);
}

#define SND_MEM_STACK_SIZE  100000
int16_t SND_MEM_STACK[SND_MEM_STACK_SIZE]; //for adjusting volume because access via heap is finicky!!, 1 megabyte 100k KB Ram allowed max
//int mem_snd_play=0;
HANDLE hMemSndArray[SND_THREAD_NUM];
//bool mem_snd_playing[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
//bool mem_snd_stopped[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
bool mem_snd_interrupt[SND_THREAD_NUM]={FALSE,FALSE,FALSE};

//Custom Audio format
WAVEFORMATEX wfx_wav_sfx = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 1, // Mono
    .nSamplesPerSec = 11025L,//22050L,//11025L, // Sample rate
    .wBitsPerSample = 16, // 16-bit audio
    .nBlockAlign = (1 *  16) / 8,
    .cbSize = 0,
    .nAvgBytesPerSec = 11025L * (1 *  16) / 8
};


//https://learn.microsoft.com/en-us/windows/win32/multimedia/using-the-waveformatex-structure
/*WAVEFORMATEX wfx_wav_music = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 2, // Stereo
    .nSamplesPerSec = 44100L, // Sample rate
    .nAvgBytesPerSec = 176400L, // Sample rate
    .nBlockAlign = 4,//(wfx1.nChannels * wfx1.wBitsPerSample) / 8,
    .wBitsPerSample = 16, // 16-bit audio
    .cbSize = 0
};*/


//Responsible for handling audio async in different threads
HWAVEOUT hWaveOut[SND_THREAD_NUM];
WAVEHDR whdr[SND_THREAD_NUM];

//to restore on shutdown (what if crash)?
double wav_out_volume=0.10;
DWORD wav_out_original_volume;


int16_t* adjustSFXVol(const int16_t* src, long filesize, double volumeFactor,bool skipped_header)
{
  memset(SND_MEM_STACK, 0, SND_MEM_STACK_SIZE);
  memcpy(SND_MEM_STACK,src,filesize);  
  long start=21;
  if (skipped_header)
    start=0;
  for (long i=start; i<filesize; i++) {
    double scaled_value=(double)SND_MEM_STACK[i]*volumeFactor;
    if (scaled_value >= INT16_MAX) {
      SND_MEM_STACK[i] = INT16_MAX;
    } else if (scaled_value <= INT16_MIN) {
      SND_MEM_STACK[i] = INT16_MIN;
    } else {
      SND_MEM_STACK[i] = (int16_t)scaled_value;
    }
  }
  int16_t* dest=malloc(filesize);
  memcpy(dest,SND_MEM_STACK,filesize);
  return dest;
}


void adjustSFXVolume(AWavSFX *mySFX, double game_volume,bool skipped_header)
{
  //keySoundEffectCache[i].audio=adjustSFXVolume(keySoundEffect[i].audio,keySoundEffect[i].filesize,game_volume);  
  //if (mySFX->wavSFXCache->audio!=NULL)
    //free(mySFX->wavSFXCache->audio);
  if (!mySFX->freed)
    freeSFXCache(mySFX);
  mySFX->freed=FALSE;
  mySFX->wavSFXCache->audio = adjustSFXVol( mySFX->wavSFX->audio, mySFX->wavSFX->filesize, game_volume, skipped_header);
}



/*
int16_t* LoadMusicWavW(const wchar_t* filename,long *datasize,int *duration)
{
  int16_t* sounddata;
  FILE* file = _wfopen(filename, L"rb");
  if (file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file) - 44; //<-- 44 is the size of the header
    fseek(file, 44, SEEK_SET);

    sounddata = malloc(file_size);
    fread(sounddata, 1, file_size, file); //read once filesize
    
    fclose(file);

    *datasize=file_size;
    //https://forum.lazarus.freepascal.org/index.php?topic=24547.0
    //duration = filesize in bytes / (samplerate * #of channels * (bitspersample/eight))
    *duration = (double)file_size / (44100L * 2 * 16/8) *1000;
    return sounddata;
  } else {
    *datasize=0;
    *duration=1;
  }
  return NULL;
}*/
// Play stereo audio from a buffer
//void PlayStereoAudio(const int16_t* audioBuffer, long *bufferSize) {
//https://learn.microsoft.com/en-us/windows/win32/multimedia/using-the-waveformatex-structure

//void PlayThreadSound(const int16_t* audioBuffer, long bufferSize, int duration, int id) 
void PlayThreadSound(AWavChannelSFX* myChannelSFX, int id) 
{
    long bufferSize=myChannelSFX->filesize;
    int duration=myChannelSFX->duration;
    bool is_cache=myChannelSFX->is_cache;


    mem_snd_interrupt[id]=FALSE;
    if (is_cache) {
      const int16_t* audio_cache=myChannelSFX->wavSFXCache->audio;
      whdr[id].lpData = (LPSTR)audio_cache;//(LPSTR);
    } else {
      const int16_t* audio=myChannelSFX->wavSFX->audio;
      whdr[id].lpData = (LPSTR)audio;//(LPSTR) myChannelSFX->wavSFX->audio;
      WAVEFORMATEX wfx_wav_music = {
        .wFormatTag = WAVE_FORMAT_PCM,
        .nChannels = myChannelSFX->wavSFX->wav_header->NumOfChan,
        .nSamplesPerSec = myChannelSFX->wavSFX->wav_header->SamplesPerSec,
        .nAvgBytesPerSec = myChannelSFX->wavSFX->wav_header->bytesPerSec,
        .nBlockAlign = myChannelSFX->wavSFX->wav_header->blockAlign,
        .wBitsPerSample = myChannelSFX->wavSFX->wav_header->bitsPerSample,
        .cbSize = 0
      };
      waveOutOpen(&hWaveOut[id], WAVE_MAPPER, &wfx_wav_music, 0, 0, CALLBACK_NULL);
      waveOutPrepareHeader(hWaveOut[id], &whdr[id], sizeof(WAVEHDR));
      waveOutSetVolume(hWaveOut[id], VolumeValue(wav_out_volume*100,1));
    }
    whdr[id].dwBufferLength = bufferSize;

    // Write the audio data
    waveOutWrite(hWaveOut[id], &whdr[id], sizeof(WAVEHDR));    

    while (duration>0 && !mem_snd_interrupt[id]) {
      duration--;
      Sleep(1);
    }
    mem_snd_interrupt[id]=FALSE;
}





DWORD WINAPI PlayMemSnd1(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[0],0);
  ExitThread(0);
}

DWORD WINAPI PlayMemSnd2(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[1],1);
  ExitThread(0);
}

DWORD WINAPI PlayMemSnd3(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[2],2);
  ExitThread(0);
}




void PlayMemSnd(AWavSFX* myWavSFX,bool play_cache,int thread_id) //thread 0,1,2
{
  mem_snd_interrupt[thread_id]=TRUE;
  waveOutReset(hWaveOut[thread_id]);
  //DWORD exitCode;
  CloseHandle(hMemSndArray[thread_id]);
  //TerminateThread(hMemSndArray[thread_id],exitCode);

  if (thread_id>=0 && thread_id<=2) {
      if (play_cache) {
        //read_audio=myWavSFX->wavSFXCache->audio; //!!reading, not creating new!!
        if (memSFX[thread_id].wavSFXCache!=NULL)
          free(memSFX[thread_id].wavSFXCache);
        memSFX[thread_id].wavSFXCache=myWavSFX->wavSFXCache;
        memSFX[thread_id].is_cache=TRUE;
      } else {
        //read_audio=myWavSFX->wavSFX->audio; //!!reading, not creating new!!
       if (memSFX[thread_id].wavSFX!=NULL)
         free(memSFX[thread_id].wavSFX);
        memSFX[thread_id].wavSFX=myWavSFX->wavSFX;
        memSFX[thread_id].is_cache=FALSE;
      }
      //memSoundEffect[thread_id].audio=read_audio; //!!reading, not creating new!!
      //memSoundEffect[thread_id].duration=myWavSFX->wavSFX->duration;
      memSFX[thread_id].duration=myWavSFX->wavSFX->duration;
      memSFX[thread_id].filesize=myWavSFX->wavSFX->filesize;
      switch (thread_id) {
        case 0:
          hMemSndArray[0] = CreateThread(NULL,0,PlayMemSnd1,NULL,0,NULL);
          break;
        case 1:
          hMemSndArray[1] = CreateThread(NULL,0,PlayMemSnd2,NULL,0,NULL);
          break;
        case 2:
          hMemSndArray[2] = CreateThread(NULL,0,PlayMemSnd3,NULL,0,NULL);
          break;
      }
  }

}



