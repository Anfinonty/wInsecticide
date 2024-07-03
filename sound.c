


#define SND_MEM_STACK_SIZE  100000
int16_t SND_MEM_STACK[SND_MEM_STACK_SIZE]; //for adjusting volume because access via heap is finicky!!, 1 megabyte 100k KB Ram

#define SND_THREAD_NUM    3
//int mem_snd_play=0;
HANDLE hMemSndArray[SND_THREAD_NUM];
//bool mem_snd_playing[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
//bool mem_snd_stopped[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
bool mem_snd_interrupt[SND_THREAD_NUM]={FALSE,FALSE,FALSE};

int mem_snd_duration1;
int mem_snd_duration2;
int mem_snd_duration3;
long mem_snd_filesize1;
long mem_snd_filesize2;
long mem_snd_filesize3;
int16_t* mem_snd_audio1;
int16_t* mem_snd_audio2;
int16_t* mem_snd_audio3;



//Custom Audio format
WAVEFORMATEX wfx1 = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 1, // Stereo
    .nSamplesPerSec = 11025L,//22050L,//11025L, // Sample rate
    .wBitsPerSample = 16, // 16-bit audio
    .nBlockAlign = (1 *  16) / 8,
    .cbSize = 0,
    .nAvgBytesPerSec = 11025L * (1 *  16) / 8
};


//https://learn.microsoft.com/en-us/windows/win32/multimedia/using-the-waveformatex-structure
WAVEFORMATEX wfx_wav = {
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

/*
WAVEFORMATEX wfx1 = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 2, // Stereo
    //.nSamplesPerSec = 11025L,//22050L,//11025L, // Sample rate
    //.nAvgBytesPerSec = 11025L, // Sample rate
    .nSamplesPerSec = 44100L, // Sample rate
    .nAvgBytesPerSec = 176400L, // Sample rate
    .nBlockAlign = 4,//(wfx1.nChannels * wfx1.wBitsPerSample) / 8,
    .wBitsPerSample = 16, // 16-bit audio
    .cbSize = 0
    //.nAvgBytesPerSec = wfx1.nSamplesPerSec * wfx1.nBlockAlign
};*/




int16_t* adjustVolumeA(const int16_t* src, long filesize, double volumeFactor)
{  
  memset(SND_MEM_STACK, 0, SND_MEM_STACK_SIZE);
  memcpy(SND_MEM_STACK,src,filesize);  
  for (long i=21; i<filesize; i++) {
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


int16_t* adjustVolume(const int16_t* src, long filesize, double volumeFactor)
{
  memset(SND_MEM_STACK, 0, SND_MEM_STACK_SIZE);
  memcpy(SND_MEM_STACK,src,filesize);  
  for (long i=0; i<filesize; i++) {
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


int16_t* LoadWavA(const char* filename,long *filesize)
{
  int16_t* sounddata;
  FILE* file = fopen(filename, "rb");
  if (file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    sounddata = malloc(file_size);
    fread(sounddata, 1, file_size, file); //read once filesize
    
    fclose(file);

    *filesize=file_size;
    return sounddata;
  } else {
    *filesize=0;
  }
  return NULL;
}



int16_t* LoadWav(const char* filename,long *datasize,int *duration)
{
  int16_t* sounddata;
  FILE* file = fopen(filename, "rb");
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
    *duration = (double)file_size / (11025L * 1 * 16/8) *1000;
    return sounddata;
  } else {
    *datasize=0;
    *duration=1;
  }
  return NULL;
}


/*int16_t* LoadMusicWav(const char* filename,long *datasize,int *duration)
{
  int16_t* sounddata;
  FILE* file = fopen(filename, "rb");
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

void PlayThreadSound(const int16_t* audioBuffer, long bufferSize, int duration, int id) 
{
    mem_snd_interrupt[id]=FALSE;
    whdr[id].lpData = (LPSTR) audioBuffer;
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
  PlayThreadSound(mem_snd_audio1,mem_snd_filesize1,mem_snd_duration1,0);
}

DWORD WINAPI PlayMemSnd2(LPVOID lpParam)
{
  PlayThreadSound(mem_snd_audio2,mem_snd_filesize2,mem_snd_duration2,1);
}

DWORD WINAPI PlayMemSnd3(LPVOID lpParam)
{
  PlayThreadSound(mem_snd_audio3,mem_snd_filesize3,mem_snd_duration3,2);
}


void PlayMemSnd(const int16_t* read_audio,const long read_audio_size,const int read_audio_duration,int thread_id) //thread 0,1,2
{
  mem_snd_interrupt[thread_id]=TRUE;
  waveOutReset(hWaveOut[thread_id]);
  CloseHandle(hMemSndArray[thread_id]);

  switch (thread_id) {
    case 0:
      mem_snd_audio1=(int16_t*) read_audio; //!!reading, not creating new!!
      mem_snd_duration1=read_audio_duration;
      mem_snd_filesize1=read_audio_size;
      hMemSndArray[0] = CreateThread(NULL,0,PlayMemSnd1,NULL,0,NULL);
      break;
    case 1:
      mem_snd_audio2=(int16_t*) read_audio;
      mem_snd_duration2=read_audio_duration;
      mem_snd_filesize2=read_audio_size;
      hMemSndArray[1] = CreateThread(NULL,0,PlayMemSnd2,NULL,0,NULL);
      break;
    case 2:
      mem_snd_audio3=(int16_t*) read_audio;
      mem_snd_duration3=read_audio_duration;
      mem_snd_filesize3=read_audio_size;
      hMemSndArray[2] = CreateThread(NULL,0,PlayMemSnd3,NULL,0,NULL);
      break;
  } 
}



