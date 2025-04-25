


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

typedef struct WavSoundEffectCache //for sound whoese volume that needs to be adjustable
{
  int16_t* audio;
} wavSoundEffectCache;





//largest buffer reserved for slack only
//#define AUDIO_STREAM_BUFFER_SIZE     32768
#define AUDIO_STREAM_BUFFER_SIZE0    16384 //2048//8192//16384//524288//8192//4096//524288//4096   8192 or 16384

/*typedef struct SFXData
{
  AWavHeader wav_header[1];
  WAVEFORMATEX awfx_sfx;
  HWAVEOUT hWaveOut;
  WAVEHDR waveHdr1;
  WAVEHDR waveHdr2;

  int16_t buffer1[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t buffer2[AUDIO_STREAM_BUFFER_SIZE0];
} SFXData;

SFXData sfxData;


void CALLBACK waveOutProcSFX(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    SFXData* sfxData = (SFXData *)dwInstance;
    if (uMsg == WOM_DONE) {
      WAVEHDR *waveHdr = (WAVEHDR *)dwParam1;
      if (waveHdr == &sfxData->waveHdr1) {
        waveOutWrite(sfxData->hWaveOut, &sfxData->waveHdr1, sizeof(WAVEHDR));
      } else if (waveHdr == &sfxData->waveHdr2) {
        waveOutWrite(sfxData->hWaveOut, &sfxData->waveHdr2, sizeof(WAVEHDR));
      }
    }
}


void ResetSFXBuffer()
{
  memset(sfxData.buffer1, 0, sizeof(sfxData.buffer1));
  memset(sfxData.buffer2, 0, sizeof(sfxData.buffer2));
}


void InitSFXBuffer()
{
  //reset buffers
  ResetSFXBuffer();
  waveOutWrite(sfxData.hWaveOut, &sfxData.waveHdr1, sizeof(WAVEHDR));
  waveOutWrite(sfxData.hWaveOut, &sfxData.waveHdr2, sizeof(WAVEHDR));
}*/




#define SND_THREAD_NUM    7
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


#define SND_MEM_STACK_SIZE  500000  // to be new fixed size for each allowed audio
int16_t SND_MEM_STACK[SND_MEM_STACK_SIZE]; //for adjusting volume because access via heap is finicky!!, 5 megabyte 500k KB Ram allowed max
HANDLE hMemSndArray[SND_THREAD_NUM];
bool mem_snd_interrupt[SND_THREAD_NUM]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};//,FALSE};

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
double wav_out_volume=0.50;
DWORD wav_out_original_volume;


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
    WAVEFORMATEX wfxi;

    mem_snd_interrupt[id]=FALSE;
    whdr[id].lpData = (LPSTR)myChannelSFX->audio;//(LPSTR);
    whdr[id].dwBufferLength = bufferSize;
    if (!is_cache) {
      wfxi.wFormatTag = WAVE_FORMAT_PCM;
      wfxi.nChannels = myChannelSFX->wav_header->NumOfChan;
      wfxi.nSamplesPerSec = myChannelSFX->wav_header->SamplesPerSec;
      wfxi.nAvgBytesPerSec = myChannelSFX->wav_header->bytesPerSec;
      wfxi.nBlockAlign = myChannelSFX->wav_header->blockAlign;
      wfxi.wBitsPerSample = myChannelSFX->wav_header->bitsPerSample;
      wfxi.cbSize = 0;
      waveOutOpen(&hWaveOut[id], WAVE_MAPPER, &wfxi, 0, 0, CALLBACK_NULL);
      waveOutPrepareHeader(hWaveOut[id], &whdr[id], sizeof(WAVEHDR));
      waveOutSetVolume(hWaveOut[id], VolumeValue(wav_out_volume*100,1));
    }
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
}

DWORD WINAPI PlayMemSnd2(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[1],1);
}


DWORD WINAPI PlayMemSnd4(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[3],3);
}


DWORD WINAPI PlayMemSnd5(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[4],4);
}


DWORD WINAPI PlayMemSnd6(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[5],5);
}

void PlayMemSnd(wavSoundEffect* mySoundEffect,wavSoundEffectCache* mySoundEffectCache,bool play_cache,int thread_id) //thread 0,1,2
{
  mem_snd_interrupt[thread_id]=TRUE;
  waveOutReset(hWaveOut[thread_id]);
  CloseHandle(hMemSndArray[thread_id]);

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
       // case 2:
         // hMemSndArray[2] = CreateThread(NULL,0,PlayMemSnd3,NULL,0,NULL);
         // break;
        case 3:
          hMemSndArray[3] = CreateThread(NULL,0,PlayMemSnd4,NULL,0,NULL);
          break;
        case 4:
          hMemSndArray[4] = CreateThread(NULL,0,PlayMemSnd5,NULL,0,NULL);
          break;
        case 5:
          hMemSndArray[5] = CreateThread(NULL,0,PlayMemSnd6,NULL,0,NULL);
          break;
      }
  }
}



