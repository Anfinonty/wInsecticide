


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


#define SND_THREAD_NUM    4
typedef struct threadSFX
{
  bool is_cache;
  int duration;
  long filesize;
  AWavHeader* wav_header;
  int16_t* audio;
} AWavChannelSFX;

AWavChannelSFX memSFX[SND_THREAD_NUM];



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


void freeSoundEffectCache(wavSoundEffectCache* mySoundEffectCache) 
{
  if (mySoundEffectCache->audio!=NULL)
    free(mySoundEffectCache->audio);
}


//void loadSoundEffect(wavSoundEffect* mySoundEffect, const wchar_t* filename,WAVEFORMATEX wfx,bool skip_header)
void loadSoundEffect(wavSoundEffect* mySoundEffect,const wchar_t* filename,bool skip_header)
{
  FILE* file = _wfopen(filename, L"rb");
  int wav_header_size=0; 
  //printf("%d\n",sizeof(AWavHeader));
  if (skip_header) {
    wav_header_size=sizeof(AWavHeader); //44
  }
  if (file) {
    fseek(file, 0, SEEK_END);
    long filesize;
    filesize = ftell(file) - wav_header_size; 

    //Alloc wav header
    fseek(file, 0, SEEK_SET);
    mySoundEffect->wav_header = malloc(wav_header_size);
    fread(mySoundEffect->wav_header,1,wav_header_size,file);


    //Alloc actual audio int16_t*
    fseek(file, wav_header_size, SEEK_SET);
    mySoundEffect->audio = malloc(filesize);
    fread(mySoundEffect->audio, 1, filesize, file); //read once filesize

    fclose(file);

    mySoundEffect->filesize = filesize;
    mySoundEffect->duration = (double)filesize / ( mySoundEffect->wav_header->SamplesPerSec * mySoundEffect->wav_header->NumOfChan * mySoundEffect->wav_header->bitsPerSample/8) *1000;
  }
}


#define SND_MEM_STACK_SIZE  100000
int16_t SND_MEM_STACK[SND_MEM_STACK_SIZE]; //for adjusting volume because access via heap is finicky!!, 1 megabyte 100k KB Ram allowed max
//int mem_snd_play=0;
HANDLE hMemSndArray[SND_THREAD_NUM];
//bool mem_snd_playing[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
//bool mem_snd_stopped[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
bool mem_snd_interrupt[SND_THREAD_NUM]={FALSE,FALSE,FALSE,FALSE};

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

WAVEFORMATEX wfx_wav_music;
WAVEFORMATEX wfx_wav_sfx2;
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


void adjustSFXVolume(wavSoundEffectCache* mySoundEffectCache, wavSoundEffect* mySoundEffect, double game_volume,bool skipped_header)
{
  //keySoundEffectCache[i].audio=adjustSFXVolume(keySoundEffect[i].audio,keySoundEffect[i].filesize,game_volume);
  mySoundEffectCache->audio = adjustSFXVol( mySoundEffect->audio, mySoundEffect->filesize, game_volume, skipped_header);
}

// Play stereo audio from a buffer
//void PlayStereoAudio(const int16_t* audioBuffer, long *bufferSize) {
//https://learn.microsoft.com/en-us/windows/win32/multimedia/using-the-waveformatex-structure
//https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/nf-mmeapi-waveoutunprepareheader

//void PlayThreadSound(const int16_t* audioBuffer, long bufferSize, int duration, int id) 
void PlayThreadSound(AWavChannelSFX* myChannelSFX, int id) 
{
    long bufferSize=myChannelSFX->filesize;
    int duration=myChannelSFX->duration;
    bool is_cache=myChannelSFX->is_cache;

    mem_snd_interrupt[id]=FALSE;
    whdr[id].lpData = (LPSTR)myChannelSFX->audio;//(LPSTR);
    whdr[id].dwBufferLength = bufferSize;
    if (!is_cache) {
      wfx_wav_music.wFormatTag = WAVE_FORMAT_PCM;
      wfx_wav_music.nChannels = myChannelSFX->wav_header->NumOfChan;
      wfx_wav_music.nSamplesPerSec = myChannelSFX->wav_header->SamplesPerSec;
      wfx_wav_music.nAvgBytesPerSec = myChannelSFX->wav_header->bytesPerSec;
      wfx_wav_music.nBlockAlign = myChannelSFX->wav_header->blockAlign;
      wfx_wav_music.wBitsPerSample = myChannelSFX->wav_header->bitsPerSample;
      wfx_wav_music.cbSize = 0;
      waveOutOpen(&hWaveOut[id], WAVE_MAPPER, &wfx_wav_music, 0, 0, CALLBACK_NULL);
      waveOutPrepareHeader(hWaveOut[id], &whdr[id], sizeof(WAVEHDR));
      waveOutSetVolume(hWaveOut[id], VolumeValue(wav_out_volume*100,1));
    }
    // Write the audio data
    waveOutWrite(hWaveOut[id], &whdr[id], sizeof(WAVEHDR));    
    while (duration>0 && !mem_snd_interrupt[id]) {
      duration--;
      Sleep(1);
    }
    //free(whdr[id].lpData);
    mem_snd_interrupt[id]=FALSE;
}



DWORD WINAPI PlayMemSnd1(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[0],0);
  //ExitThread(0);
}

DWORD WINAPI PlayMemSnd2(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[1],1);
  //ExitThread(0);
}

DWORD WINAPI PlayMemSnd3(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[2],2);
  //ExitThread(0);
}


DWORD WINAPI PlayMemSnd4(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[3],3);
  //ExitThread(0);
}


void PlayMemSnd(wavSoundEffect* mySoundEffect,wavSoundEffectCache* mySoundEffectCache,bool play_cache,int thread_id) //thread 0,1,2
{
  //if (hMemSndArray[thread_id]!=NULL) {
  mem_snd_interrupt[thread_id]=TRUE;
  waveOutReset(hWaveOut[thread_id]);
  //if (hMemSndArray[thread_id]!=NULL)
    CloseHandle(hMemSndArray[thread_id]);
  //DWORD exitCode;
  //closeHandleSafely(hMemSndArray[thread_id]); //WARNING CAUSES CRASH
  //}
  //TerminateThread(hMemSndArray[thread_id],exitCode);

  if (thread_id>=0 && thread_id<SND_THREAD_NUM) {
      memSFX[thread_id].audio=NULL;
      memSFX[thread_id].wav_header=NULL;
      //free(memSFX[thread_id].audio);
      //free(memSFX[thread_id].wav_header);

      if (play_cache) {
        memSFX[thread_id].audio=mySoundEffectCache->audio;
        memSFX[thread_id].is_cache=TRUE;
      } else {
        memSFX[thread_id].audio=mySoundEffect->audio;
        memSFX[thread_id].wav_header=mySoundEffect->wav_header;
        memSFX[thread_id].is_cache=FALSE;
      }
      memSFX[thread_id].duration=mySoundEffect->duration;
      memSFX[thread_id].filesize=mySoundEffect->filesize;
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
        case 3:
          hMemSndArray[3] = CreateThread(NULL,0,PlayMemSnd4,NULL,0,NULL);
          break;
      }
  }
}



