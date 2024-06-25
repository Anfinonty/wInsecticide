



#define SND_THREAD_NUM    3
int mem_snd_play=0;
HANDLE hMemSndArray[SND_THREAD_NUM];
bool mem_snd_playing[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
bool mem_snd_stopped[SND_THREAD_NUM]={FALSE,FALSE,FALSE};
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


WAVEFORMATEX wfx1 = {
    .wFormatTag = WAVE_FORMAT_PCM,
    .nChannels = 1, // Stereo
    .nSamplesPerSec = 11025L,//22050L,//11025L, // Sample rate
    .nAvgBytesPerSec = 11025L, // Sample rate
    .nBlockAlign = 4,//(wfx1.nChannels * wfx1.wBitsPerSample) / 8,
    .wBitsPerSample = 16, // 16-bit audio
    .cbSize = 0
    //.nAvgBytesPerSec = wfx1.nSamplesPerSec * wfx1.nBlockAlign
};





int16_t* adjustVolumeA(int16_t* src, long filesize, double volumeFactor)
{
  int16_t* dest=(int16_t*) malloc(filesize*2/*filesize+512+64+4+1*/);
  if (!dest) {
    printf("malloc failed");
    return NULL;
  }

  for (long i=0; i<filesize; i++) {
    //printf("\nfilesize: %ld/%ld",i,filesize);
    if (i<21) { //header with 20 bytes
      dest[i]=src[i];
    } else {
      double scaled_value=(double)src[i]*volumeFactor;
      if (scaled_value >= INT16_MAX) {
        dest[i] = INT16_MAX;
      } else if (scaled_value <= INT16_MIN) {
        dest[i] = INT16_MIN;
      } else {
        dest[i] = (int16_t)scaled_value;
      }
    }
  }
  return dest;
}


int16_t* adjustVolume(int16_t* src, long filesize, double volumeFactor)
{
  int16_t* dest=(int16_t*) malloc(filesize*2/*filesize+512+64+4+1*/);
  if (!dest) {
    printf("malloc failed");
    return NULL;
  }

  for (long i=0; i<filesize; i++) {
    //printf("\nfilesize: %ld/%ld",i,filesize);
    double scaled_value=(double)src[i]*volumeFactor;
    if (scaled_value >= INT16_MAX) {
      dest[i] = INT16_MAX;
    } else if (scaled_value <= INT16_MIN) {
      dest[i] = INT16_MIN;
    } else {
      dest[i] = (int16_t)scaled_value;
    }
  }
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

    sounddata = (int16_t*)malloc(file_size);
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
    *duration=2000;
  }
  return NULL;
}








// Play stereo audio from a buffer
//void PlayStereoAudio(const int16_t* audioBuffer, long *bufferSize) {
//https://learn.microsoft.com/en-us/windows/win32/multimedia/using-the-waveformatex-structure
void PlayStereoAudio(const int16_t* audioBuffer, long bufferSize, int duration, int id) 
{
    /*WAVEFORMATEX wfx1 = {
        .wFormatTag = WAVE_FORMAT_PCM,
        .nChannels = 2, // Stereo
        .nSamplesPerSec = 44100L, // Sample rate
        .nAvgBytesPerSec = 176400L, // Sample rate
        .nBlockAlign = 4,//(wfx1.nChannels * wfx1.wBitsPerSample) / 8,
        .wBitsPerSample = 16, // 16-bit audio
        .cbSize = 0
        //.nAvgBytesPerSec = wfx1.nSamplesPerSec * wfx1.nBlockAlign
    };*/

    HWAVEOUT hWaveOut;
    WAVEHDR whdr;
    // Open the audio output device

    // Prepare the audio buffer
    whdr.lpData = (LPSTR) audioBuffer;
    whdr.dwBufferLength = bufferSize;

    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx1, 0, 0, CALLBACK_NULL);


    waveOutPrepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));

    // Write the audio data
    waveOutWrite(hWaveOut, &whdr, sizeof(WAVEHDR));

    // Wait for playback to finish (you can add more logic here)
    while (duration>0) {
      duration--;
      if (mem_snd_interrupt[id]) {
        mem_snd_interrupt[id]=FALSE;
        break;
      }
      Sleep(1);
    }

    // Cleanup
    waveOutUnprepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}




DWORD WINAPI PlayMemSnd1(LPVOID lpParam)
{
  PlayStereoAudio(mem_snd_audio1,mem_snd_filesize1,mem_snd_duration1,0);
  //if (mem_snd_audio1!=NULL)
    //free(mem_snd_audio1);
  mem_snd_filesize1=0;
  mem_snd_duration1=2000;
  mem_snd_interrupt[0]=FALSE;
  mem_snd_stopped[0]=TRUE;
}

DWORD WINAPI PlayMemSnd2(LPVOID lpParam)
{
  PlayStereoAudio(mem_snd_audio2,mem_snd_filesize2,mem_snd_duration2,1);
  //if (mem_snd_audio2!=NULL)
    //free(mem_snd_audio2);
  mem_snd_filesize2=0;
  mem_snd_duration2=2000;
  mem_snd_interrupt[1]=FALSE;
  mem_snd_stopped[1]=TRUE;
}

DWORD WINAPI PlayMemSnd3(LPVOID lpParam)
{
  PlayStereoAudio(mem_snd_audio3,mem_snd_filesize3,mem_snd_duration3,2);
  //if (mem_snd_audio3!=NULL)
    //free(mem_snd_audio3);
  mem_snd_filesize3=0;
  mem_snd_duration3=2000;
  mem_snd_interrupt[2]=FALSE;
  mem_snd_stopped[2]=TRUE;
}


void PlayMemSnd(const int16_t* read_audio,const long read_audio_size,const long read_duration)
{
  if (!mem_snd_playing[mem_snd_play]) {
    mem_snd_playing[mem_snd_play]=TRUE;
    switch (mem_snd_play) {
      case 0:
        //if (mem_snd_audio1==NULL) {
          mem_snd_audio1=(int16_t*) read_audio; //!!reading, not creating new!!
          mem_snd_filesize1=read_audio_size;
          mem_snd_duration1=read_duration;
          hMemSndArray[0] = CreateThread(NULL,0,PlayMemSnd1,NULL,0,NULL);
        //}
        break;
      case 1:
        //if (mem_snd_audio2==NULL) {
          mem_snd_audio2=(int16_t*) read_audio;
          mem_snd_filesize2=read_audio_size;
          mem_snd_duration2=read_duration;
          hMemSndArray[1] = CreateThread(NULL,0,PlayMemSnd2,NULL,0,NULL);
        //}
        break;
      case 2:
        //if (mem_snd_audio3==NULL) {
          mem_snd_audio3=(int16_t*) read_audio;
          mem_snd_filesize3=read_audio_size;
          mem_snd_duration3=read_duration;
          hMemSndArray[2] = CreateThread(NULL,0,PlayMemSnd3,NULL,0,NULL);
        //}
        break;
    }
  } else { //is playing
    mem_snd_interrupt[mem_snd_play]=TRUE;

    /*switch (mem_snd_play) {
      case 0:
        //if (mem_snd_audio1==NULL) {
          mem_snd_audio1=(int16_t*) read_audio; //!!reading, not creating new!!
          mem_snd_filesize1=read_audio_size;
          mem_snd_duration1=read_duration;
          hMemSndArray[0] = CreateThread(NULL,0,PlayMemSnd1,NULL,0,NULL);
        //}
        break;
      case 1:
        //if (mem_snd_audio2==NULL) {
          mem_snd_audio2=(int16_t*) read_audio;
          mem_snd_filesize2=read_audio_size;
          mem_snd_duration2=read_duration;
          hMemSndArray[1] = CreateThread(NULL,0,PlayMemSnd2,NULL,0,NULL);
        //}
        break;
      case 2:
        //if (mem_snd_audio3==NULL) {
          mem_snd_audio3=(int16_t*) read_audio;
          mem_snd_filesize3=read_audio_size;
          mem_snd_duration3=read_duration;
          hMemSndArray[2] = CreateThread(NULL,0,PlayMemSnd3,NULL,0,NULL);
        //}
        break;
    }*/

  }
  mem_snd_play++;
  if (mem_snd_play>SND_THREAD_NUM-1)
    mem_snd_play=0;
}
