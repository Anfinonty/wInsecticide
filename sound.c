


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
WAVEFORMATEX wfx_wav_music = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 2, // Stereo
    .nSamplesPerSec = 44100L, // Sample rate
    .nAvgBytesPerSec = 176400L, // Sample rate
    .nBlockAlign = 4,//(wfx1.nChannels * wfx1.wBitsPerSample) / 8,
    .wBitsPerSample = 16, // 16-bit audio
    .cbSize = 0
};


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
  freeSoundEffectCache(mySFX->wavSFXCache);
  mySFX->wavSFXCache->audio = adjustSFXVol( mySFX->wavSFX->audio, mySFX->wavSFX->filesize, game_volume, skipped_header);
}




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
}
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
}

DWORD WINAPI PlayMemSnd2(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[1],1);
}

DWORD WINAPI PlayMemSnd3(LPVOID lpParam)
{
  PlayThreadSound(&memSFX[2],2);
}


void PlayMemSnd(AWavSFX* myWavSFX,bool play_cache,int thread_id) //thread 0,1,2
{
  mem_snd_interrupt[thread_id]=TRUE;
  waveOutReset(hWaveOut[thread_id]);
  DWORD exitCode;
  CloseHandle(hMemSndArray[thread_id]);
  //TerminateThread(hMemSndArray[thread_id],exitCode);

  //const int16_t* read_audio;
  

  if (thread_id>=0 && thread_id<=2) {
      if (play_cache) {
        //read_audio=myWavSFX->wavSFXCache->audio; //!!reading, not creating new!!
        memSFX[thread_id].wavSFXCache=myWavSFX->wavSFXCache;
        memSFX[thread_id].is_cache=TRUE;
      } else {
        //read_audio=myWavSFX->wavSFX->audio; //!!reading, not creating new!!
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



