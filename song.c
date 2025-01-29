
#define SONG_NUM 10
#define SONG_FOLDER_NUM 23

int current_hour;
int current_min;
int current_sec;


int gct=0;//global choose track
int song_num=0;
int song_mode[2]={0,0};
int song_rand_num[2]={-1,-1};
int song_audio_duration[2];
wchar_t song_names[2000][256];
bool is_flac[2000];
bool is_wav[2000];
bool is_mp3[2000];
bool play_new_song[2]={TRUE,TRUE};
bool stop_playing_song[2]={FALSE,FALSE};
bool toggle_stop_playing_song[2]={FALSE,FALSE};
bool playing_wav[2]={FALSE,FALSE};
bool loading_mp3[2]={FALSE,FALSE};
bool loading_flac[2]={FALSE,FALSE};
bool loading_wav[2]={FALSE,FALSE};
bool skip_song[2]={FALSE,FALSE};
bool skipping_song[2]={FALSE,FALSE};
bool clean_up_song[2]={FALSE,FALSE};
//bool song_speed_up[2]=FALSE;
bool clean_up_sound=FALSE;
wchar_t wav_song_playing[256];
wchar_t wav_song_playing2[256];


//unsigned long long time_song_start[2];
unsigned long long time_song_end[2];
unsigned long long current_song_time[2];


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

WAVEFORMATEX wfx_wav_sfx_rain;

//https://github.com/audiojs/sample-rate

//https://riptutorial.com/winapi/example/5736/create-a-file-and-write-to-it
//https://stackoverflow.com/questions/51864328/unable-to-create-dlls-getting-dll-is-not-a-valid-win32-application
//https://learn.microsoft.com/en-us/windows/win32/fileio/creating-and-opening-files
//https://learn.microsoft.com/en-us/windows/win32/coreaudio/capturing-a-stream
//https://stackoverflow.com/questions/3774343/what-libraries-how-to-play-wav-file-on-windows32-in-c
//https://stackoverflow.com/questions/1565439/how-to-playsound-in-c-using-windows-api
//https://stackoverflow.com/questions/59483071/trying-to-create-a-wav-file-with-capturing-a-stream-from-win32-wasapi-c

//https://stackoverflow.com/questions/2302841/win32-playsound-how-to-control-the-volume
//https://stackoverflow.com/questions/16905458/reduce-the-volume-of-a-wav-audio-file-using-c// Adjusts the volume of the WAV samples by a percentage
//https://web.archive.org/web/20090827003349/http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
//https://blog.devgenius.io/how-to-increase-volume-of-an-audio-file-with-c-6d23e04a03a6
//http://soundfile.sapp.org/doc/WaveFormat/



//https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const wchar_t *get_filename_ext(const wchar_t *filename) 
{
  const wchar_t *dot = wcsrchr(filename, '.');
  if(!dot || dot == filename) return L"";
  return dot + 1;
}


int CountSongsInDir(const wchar_t *dirname,const wchar_t *indirname, int song_num)
{
  _WDIR *d;
  struct _wdirent *dir; //https://castor-project.org/doc/castor3_0/struct__wdirent.html
  d = _wopendir(dirname);//
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for songs in folder
        wchar_t indir2[256];
        swprintf(indir2,256,L"%s/%s",indirname,dir->d_name);
        song_num=CountSongsInDir(indir,indir2,song_num);
      } else {
        const wchar_t *ext=get_filename_ext(dir->d_name);
        if (_wcsicmp(ext,L"wav")==0 || 
            _wcsicmp(ext,L"mp3")==0 || 
            _wcsicmp(ext,L"flac")==0) {
          song_num++;
          if (song_num>=2000) {
            break;
          }
        }      
      }
    }
    _wclosedir(d);
  }
  return song_num;
}


//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
int GetSongsInDir(const wchar_t *dirname,const wchar_t *indirname, int song_num)
{
  _WDIR *d;
  struct _wdirent *dir; //https://castor-project.org/doc/castor3_0/struct__wdirent.html
  d = _wopendir(dirname);//
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
      //printf("status: %d\n",PathIsDirectoryA(indir)); //https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathisdirectorya
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for songs in folder
        wchar_t indir2[256];
        swprintf(indir2,256,L"%s/%s",indirname,dir->d_name);
        song_num=GetSongsInDir(indir,indir2,song_num);
      } else {
        const wchar_t *ext=get_filename_ext(dir->d_name);
        /*wchar_t lowext[6];
        for (int i=0;i<6;i++) {
          lowext[i]=tolower(ext[i]);
        }*/
      //printf("\n--%s ;; %s",ext,lowext);
        if (_wcsicmp(ext,L"wav")==0 || 
            _wcsicmp(ext,L"mp3")==0 || 
            //_wcsicmp(ext,L"wma")==0 || 
            //_wcsicmp(ext,L"midi")==0 || 
            _wcsicmp(ext,L"flac")==0/* ||
            strcmp(lowext,"mpg")==0 ||
            strcmp(lowext,"mpeg")==0*/) {
        //printf("%d|-> %s\n",song_num,dir->d_name);
        //song_names[song_num]=dir->d_name;
          if (_wcsicmp(ext,L"flac")==0) {
            is_flac[song_num]=TRUE;
          } else {
            is_flac[song_num]=FALSE;
          }


          if (_wcsicmp(ext,L"wav")==0) {
            is_wav[song_num]=TRUE;
          } else {
            is_wav[song_num]=FALSE;
          }


          if (_wcsicmp(ext,L"mp3")==0) {
            is_mp3[song_num]=TRUE;
          } else {
            is_mp3[song_num]=FALSE;
          }


          wchar_t indir[256];
          swprintf(indir,256,L"%s/%s",indirname,dir->d_name);
          wcsncpy(song_names[song_num],indir,256);
          //printf("%d|-> %s\n",song_num,song_names[song_num]);
          song_num++;
          if (song_num>=2000) {
            break;
          }
        }      
      }
    }
    _wclosedir(d);
  }
  return song_num;
}

void ResetSongBank() {
  for (int i=0;i<song_num;i++) {
    song_names[i][0]='\0';
    is_flac[i]=is_mp3[i]=is_wav[i]=FALSE;
  }
  song_num=0;
}


void InitSongBank() {
  ResetSongBank();
  song_num=GetSongsInDir(L"music",L"",0);
  swprintf(src_music_dir,64,L"music"); //set dir of music
}

/*
void InitSongBank2() {
  ResetSongBank();
  song_num=GetSongsInDir(L"music",L"",0);
  swprintf(src_music_dir2,64,L"music"); //set dir of music
}*/


//https://www.codeproject.com/Articles/8396/Using-DirectSound-to-Play-Audio-Stream-Data
//im not using direct sound just learning its concept

//https://stackoverflow.com/questions/49605552/double-buffered-waveoutwrite-stuttering-like-hell
//https://www.vbforums.com/showthread.php?742723-RESOLVED-waveout-API-Tutorial-Example
//https://stackoverflow.com/questions/49605552/double-buffered-waveoutwrite-stuttering-like-hell

//#define AUDIO_STREAM_BUFFER_NUM    2  //2mb
//#define AUDIO_STREAM_BUFFER_SIZE   524288//4096//524288//44100//16384//8192//524288  minimum //int16_t *524288 = 1mb //0.00049mb



//largest buffer reserved for slack only
//#define AUDIO_STREAM_BUFFER_SIZE     32768
//#define AUDIO_STREAM_SAMPLE_SIZE     44100
#define AUDIO_STREAM_BUFFER_SIZE0    16384 //2048//8192//16384//524288//8192//4096//524288//4096   8192 or 16384
#define AUDIO_STREAM_BUFFER_SIZE1    8192  //2048//8192//16384//524288//8192//4096//524288//4096   8192 or 16384
#define AUDIO_STREAM_BUFFER_SIZE2    4096
#define AUDIO_STREAM_BUFFER_SIZE3    2048
#define READ_BUFFER_NUM    20

const long _buffer_size0=AUDIO_STREAM_BUFFER_SIZE0*sizeof(int16_t);
const long _buffer_size1=AUDIO_STREAM_BUFFER_SIZE1*sizeof(int16_t);
const long _buffer_size2=AUDIO_STREAM_BUFFER_SIZE2*sizeof(int16_t);
const long _buffer_size3=AUDIO_STREAM_BUFFER_SIZE3*sizeof(int16_t);

long buffer_size_arr[3]={_buffer_size1,_buffer_size2,_buffer_size3};
long buffer_length_arr[3]={AUDIO_STREAM_BUFFER_SIZE1,AUDIO_STREAM_BUFFER_SIZE2,AUDIO_STREAM_BUFFER_SIZE3};

int casbs_i=0;
int chosen_buffer_length_o;
long chosen_buffer_size_o;
int chosen_buffer_length;
long chosen_buffer_size;


typedef struct AudioData
{
  AWavHeader wav_header[1];
  WAVEFORMATEX awfx_music;
  HWAVEOUT hWaveOut;
  FILE *music_file;
  bool double_buffer;
  bool saved_double_buffer;
  bool song_rewind;
  int read_size;
  int jump1;
  int jump2;
  int saved_read_buffer1;
  int saved_play_buffer1;
  int saved_read_buffer2;
  int saved_play_buffer2;


  long current_filesize; //spindle
  long filesize;
  long buffer_size;

  WAVEHDR waveHdr1;
  WAVEHDR waveHdr2;
  double tempo;

  int queue_play_buffer;
  int queue_read_buffer;
  int16_t read_buffer[READ_BUFFER_NUM][AUDIO_STREAM_BUFFER_SIZE0];
  int16_t buffer1[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t buffer2[AUDIO_STREAM_BUFFER_SIZE0];
} AudioData;

AudioData audioData[2];

/*
#include <complex.h>
#include <mmsystem.h>

// Define constants
#define PI 3.14159265358979323846
#define FRAME_SIZE 1024
#define HOP_SIZE 256
#define OVERLAP (FRAME_SIZE - HOP_SIZE)

// Function prototypes
void stft(const int16_t* input, int input_size, float complex* output);
void istft(const float complex* input, int input_size, int16_t* output);
void phase_vocoder(const int16_t* input, int input_size, int16_t* output, int16_t speed);

void stft(const int16_t* input, int input_size, float complex* output) {
    // Implement STFT
    for (int i = 0; i < input_size; i += HOP_SIZE) {
        for (int j = 0; j < FRAME_SIZE; j++) {
            output[i + j] = input[i + j] * cexp(-I * 2.0 * PI * j / FRAME_SIZE);
        }
    }
}

void istft(const float complex* input, int input_size, int16_t* output) {
    // Implement inverse STFT
    for (int i = 0; i < input_size; i += HOP_SIZE) {
        for (int j = 0; j < FRAME_SIZE; j++) {
            output[i + j] += creal(input[i + j] * cexp(I * 2.0 * PI * j / FRAME_SIZE));
        }
    }
}

void phase_vocoder(const int16_t* input, int input_size, int16_t* output, int16_t speed) {
    float complex stft_output[input_size];
    float complex processed_stft[input_size];
    stft(input, input_size, stft_output);
    // Process phases and magnitudes
    for (int i = 0; i < input_size; i += HOP_SIZE) {
        for (int j = 0; j < FRAME_SIZE; j++) {
            int16_t magnitude = cabs(stft_output[i + j]);
            int16_t phase = carg(stft_output[i + j]);
            // Adjust phase
            phase *= speed;
            processed_stft[i + j] = magnitude * cexp(I * phase);
        }
    }
    istft(processed_stft, input_size, output);
}*/


/*int main() {
    // Load audio data into input array
    // This part depends on your specific audio file reading implementation

    // Example input audio data
    int input_size = 44100; // 1 second of audio at 44.1 kHz sample rate
    int16_t input[input_size];
    int16_t output[input_size * 2]; // Assuming maximum 2x speed change

    // Initialize the input array with example data (e.g., a sine wave)
    for (int i = 0; i < input_size; i++) {
        input[i] = sin(2.0 * PI * 440.0 * i / 44100.0); // A4 note
    }

    // Apply the phase vocoder
    int16_t speed = 1.5f; // Change the tempo to 1.5x
    phase_vocoder(input, input_size, output, speed);

    // Output the processed data
    // This part depends on your specific audio file writing implementation

    return 0;
}*/

//0 1 2 3 [4] 
//[5] 6 7 8 9 


void CALLBACK waveOutProc1(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    AudioData* audioData = (AudioData *)dwInstance;
    //if (playing_wav[z]) {
      if (uMsg == WOM_DONE) {
        WAVEHDR *waveHdr = (WAVEHDR *)dwParam1;
        if (waveHdr == &audioData->waveHdr1) {
            //frad(audioData->buffer1, sizeof(BYTE), audioData->read_size, audioData->music_file);
            audioData->double_buffer=TRUE;
            audioData->read_size=(double)chosen_buffer_size*audioData->tempo;
            if (!audioData->song_rewind) {
                audioData->current_filesize+=audioData->read_size;
                fread(audioData->read_buffer[audioData->queue_read_buffer], sizeof(BYTE), audioData->read_size, audioData->music_file);  
                if (audioData->queue_read_buffer<=18) {audioData->queue_read_buffer++;} else {audioData->queue_read_buffer=0;}
                if (audioData->queue_play_buffer>-1 && audioData->queue_play_buffer<READ_BUFFER_NUM) {memcpy(audioData->buffer1,audioData->read_buffer[audioData->queue_play_buffer],audioData->read_size);}
                if (audioData->queue_play_buffer<=18) {audioData->queue_play_buffer++;} else {audioData->queue_play_buffer=0;}
            }
            waveOutWrite(audioData->hWaveOut, &audioData->waveHdr1, sizeof(WAVEHDR));
        } else if (waveHdr == &audioData->waveHdr2) {
            //fread(audioData->buffer2, sizeof(BYTE), audioData->read_size, audioData->music_file);
            audioData->double_buffer=FALSE;
            audioData->read_size=(double)chosen_buffer_size*audioData->tempo;
            if (!audioData->song_rewind) {
                audioData->current_filesize+=audioData->read_size;
                fread(audioData->read_buffer[audioData->queue_read_buffer], sizeof(BYTE), audioData->read_size, audioData->music_file);  
                if (audioData->queue_read_buffer<=18) {audioData->queue_read_buffer++;} else {audioData->queue_read_buffer=0;}                
                if (audioData->queue_play_buffer>-1 && audioData->queue_play_buffer<READ_BUFFER_NUM) {memcpy(audioData->buffer2,audioData->read_buffer[audioData->queue_play_buffer],audioData->read_size);}
                if (audioData->queue_play_buffer<=18) {audioData->queue_play_buffer++;} else {audioData->queue_play_buffer=0;}
            } 
            waveOutWrite(audioData->hWaveOut, &audioData->waveHdr2, sizeof(WAVEHDR));
        }
      //}
    }
}


  //int16_t audio[AUDIO_STREAM_BUFFER_NUM][AUDIO_STREAM_BUFFER_SIZE];
  /*int16_t _tmp_buffer1[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t _tmp_buffer2[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t tmp_buffer1[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t tmp_buffer2[AUDIO_STREAM_BUFFER_SIZE0];*/


//audioData->current_filesize+=chosen_buffer_size;
//fread(audioData->tmp_buffer1, sizeof(BYTE), chosen_buffer_size, audioData->music_file);
//timeStretch(audioData->tmp_buffer1,audioData->buffer1,AUDIO_STREAM_BUFFER_SIZE1,0.1);
//memcpy(audioData->buffer1, audioData->tmp_buffer1, chosen_buffer_size);


//audioData->current_filesize+=chosen_buffer_size;
//fread(audioData->tmp_buffer2, sizeof(BYTE), chosen_buffer_size, audioData->music_file);
//timeStretch(audioData->tmp_buffer2,audioData->buffer2,AUDIO_STREAM_BUFFER_SIZE1,0.1);
//memcpy(audioData->buffer2, audioData->tmp_buffer2, chosen_buffer_size);
/*void BelieveWaveClose()
{
  waveOutReset(hWaveOut[2]);
  //memset(audioData.buffer3, 0, sizeof(audioData.buffer3));
  waveOutUnprepareHeader(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
  waveOutClose(hWaveOut[2]);
}



void BelieveWaveReOpen(int buffer_length)
{
   waveOutOpen(&hWaveOut[2], WAVE_MAPPER, &wfx_wav_music, 0, 0, CALLBACK_FUNCTION);
   long int vol=VolumeValue(wav_out_volume*100,1);
   waveOutSetVolume(hWaveOut[2],vol);

   //whdr[2].lpData = (LPSTR)audioData.buffer3;
   whdr[2].dwBufferLength = buffer_length;//AUDIO_STREAM_BUFFER_SIZE;
   whdr[2].dwFlags = 0;
   whdr[2].dwLoops = 0;

   waveOutPrepareHeader(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
   waveOutWrite(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
}*/


void LiveWaveClose(int z)
{
  waveOutReset(audioData[z].hWaveOut);
  memset(audioData[z].buffer1, 0, sizeof(audioData[z].buffer1));
  memset(audioData[z].buffer2, 0, sizeof(audioData[z].buffer2));
  waveOutUnprepareHeader(audioData[z].hWaveOut, &audioData[z].waveHdr1, sizeof(WAVEHDR));
  waveOutUnprepareHeader(audioData[z].hWaveOut, &audioData[z].waveHdr2, sizeof(WAVEHDR));
  waveOutClose(audioData[z].hWaveOut);
  //waveOutPause(audioData[z].hWaveOut);
}


void LiveWaveReOpen(int z)
{

        waveOutOpen(&audioData[z].hWaveOut, WAVE_MAPPER, &audioData[z].awfx_music, (DWORD_PTR)waveOutProc1, (DWORD_PTR)&audioData[z], CALLBACK_FUNCTION);

      //waveOutOpen(&audioData[z].hWaveOut, WAVE_MAPPER, &audioData[z].awfx_music, (DWORD_PTR)waveOutProc1, (DWORD_PTR)&audioData[z], CALLBACK_FUNCTION);

     //waveOutOpen(&audioData[0].hWaveOut, WAVE_MAPPER, &audioData[0].awfx_music, (DWORD_PTR)waveOutProc1, (DWORD_PTR)&audioData[0], CALLBACK_FUNCTION);

      waveOutRestart(audioData[z].hWaveOut);
      long int vol=VolumeValue(wav_out_volume*100,1);
      waveOutSetVolume(audioData[z].hWaveOut,vol);

      audioData[z].waveHdr1.lpData = (LPSTR)audioData[z].buffer1;
      audioData[z].waveHdr1.dwBufferLength = chosen_buffer_size;//AUDIO_STREAM_BUFFER_SIZE;
      audioData[z].waveHdr1.dwFlags = 0;
      audioData[z].waveHdr1.dwLoops = 0;
      waveOutPrepareHeader(audioData[z].hWaveOut, &audioData[z].waveHdr1, sizeof(WAVEHDR));
      waveOutWrite(audioData[z].hWaveOut, &audioData[z].waveHdr1, sizeof(WAVEHDR));


      audioData[z].waveHdr2.lpData = (LPSTR)audioData[z].buffer2;
      audioData[z].waveHdr2.dwBufferLength = chosen_buffer_size;//AUDIO_STREAM_BUFFER_SIZE;
      audioData[z].waveHdr2.dwFlags = 0;
      audioData[z].waveHdr2.dwLoops = 0;
      waveOutPrepareHeader(audioData[z].hWaveOut, &audioData[z].waveHdr2, sizeof(WAVEHDR));
      waveOutWrite(audioData[z].hWaveOut, &audioData[z].waveHdr2, sizeof(WAVEHDR));

}



void ResetAudioBuffer(int z)
{
  //audioData[z].double_buffer=FALSE;
  //audioData[z].queue_read_buffer=0;
  //audioData[z].queue_play_buffer=0;
  memset(audioData[z].buffer1, 0, sizeof(audioData[z].buffer1));
  memset(audioData[z].buffer2, 0, sizeof(audioData[z].buffer2));
  for (int i=0;i<READ_BUFFER_NUM;i++) {
    memset(audioData[z].read_buffer[i], 0, sizeof(audioData[z].buffer1));
  }
}


void InitAudioBuffer(int z)
{
  //reset buffers
  //int a1,b1,a2,b2,pivot;
  ResetAudioBuffer(z);
  audioData[z].queue_read_buffer=audioData[z].queue_play_buffer-READ_BUFFER_NUM/2;

  if (audioData[z].queue_read_buffer<0) {
    audioData[z].queue_read_buffer+=READ_BUFFER_NUM;
  }

  for (int i=0;i<READ_BUFFER_NUM;i++) {    
    fread(audioData[z].read_buffer[audioData[z].queue_read_buffer], sizeof(BYTE), audioData[z].read_size, audioData[z].music_file);  
    audioData[z].queue_read_buffer++;
    if (audioData[z].queue_read_buffer==READ_BUFFER_NUM) {
      audioData[z].queue_read_buffer=0;
    }
  }
}


void LoadBufferSFX(const wchar_t* filename, int z)
{
  audioData[z].double_buffer=FALSE;
  audioData[z].queue_read_buffer=0;
  audioData[z].queue_play_buffer=0;
  if (audioData[z].music_file)
    fclose(audioData[z].music_file);

  audioData[z].jump1=44;
  audioData[z].jump2=44;

  audioData[z].music_file = _wfopen(filename, L"rb");
  int wav_header_size=sizeof(AWavHeader); //44
  audioData[z].current_filesize=wav_header_size;


  //reset buffers
  memset(audioData[z].buffer1, 0, sizeof(audioData[z].buffer1));
  memset(audioData[z].buffer2, 0, sizeof(audioData[z].buffer2));
  for (int i=0;i<READ_BUFFER_NUM;i++) {
    memset(audioData[z].read_buffer[i], 0, sizeof(audioData[z].buffer1));
  }

  if (audioData[z].music_file) {
    fseek(audioData[z].music_file, 0, SEEK_END);
    long filesize;
    filesize = ftell(audioData[z].music_file);// - wav_header_size; 

    //Alloc wav buffer header
    fseek(audioData[z].music_file, 0, SEEK_SET);
    fread(audioData[z].wav_header,1,wav_header_size,audioData[z].music_file);

    audioData[z].awfx_music.wFormatTag = WAVE_FORMAT_PCM;
    audioData[z].awfx_music.nChannels = audioData[z].wav_header->NumOfChan;
    audioData[z].awfx_music.nSamplesPerSec = audioData[z].wav_header->SamplesPerSec;
    audioData[z].awfx_music.nAvgBytesPerSec = audioData[z].wav_header->bytesPerSec;
    audioData[z].awfx_music.nBlockAlign = audioData[z].wav_header->blockAlign;
    audioData[z].awfx_music.wBitsPerSample = audioData[z].wav_header->bitsPerSample;
    audioData[z].awfx_music.cbSize = 0;

    audioData[z].buffer_size=chosen_buffer_size;
    audioData[z].filesize=filesize;
    song_audio_duration[z]=(double)filesize / (audioData[z].wav_header->SamplesPerSec * audioData[z].wav_header->NumOfChan * audioData[z].wav_header->bitsPerSample/8) *1000;



    // Initialize audioData[z] structure
    /*fread(audioData[z].buffer1,1,chosen_buffer_size,audioData[z].music_file);
    audioData[z].current_filesize+=chosen_buffer_size;
    if (audioData[z].current_filesize>audioData[z].filesize) {
      audioData[z].current_filesize=audioData[z].filesize;
    }
    fread(audioData[z].buffer2,1,chosen_buffer_size,audioData[z].music_file);
    audioData[z].current_filesize+=chosen_buffer_size;
    if (audioData[z].current_filesize>audioData[z].filesize) {
      audioData[z].current_filesize=audioData[z].filesize;
    }*/
    //fill buffers on start
    audioData[z].read_size=(double)chosen_buffer_size*audioData[z].tempo;
//    for (int i=0;i<6;i++) {
    for (int i=0;i<11;i++) {
      fread(audioData[z].read_buffer[i], sizeof(BYTE), audioData[z].read_size, audioData[z].music_file);  
      audioData[z].queue_read_buffer++;
    }
    memcpy(audioData[z].buffer1,audioData[z].read_buffer[0],audioData[z].read_size);
    audioData[z].current_filesize+=audioData[z].read_size;
    memcpy(audioData[z].buffer2,audioData[z].read_buffer[1],audioData[z].read_size);
    audioData[z].current_filesize+=audioData[z].read_size;
    audioData[z].queue_play_buffer=2;
    LiveWaveReOpen(z);
  }
}






DWORD WINAPI SoundTask(LPVOID lpArg) {
  srand(time(NULL));
  while (TRUE) {
    //SONG Tasks
    if (song_num>0) {
      for (int z=0;z<2;z++) {
          if (!stop_playing_song[z]) {

            if (playing_wav[z]) { //hi im currently playing a flac/wav
              
              //current_song_time=current_timestamp();
              current_song_time[z]=(double)audioData[z].current_filesize / (audioData[z].wav_header->SamplesPerSec * audioData[z].wav_header->NumOfChan * audioData[z].wav_header->bitsPerSample/8) *1000;
              if (current_song_time[z]>time_song_end[z] ||  play_new_song[z]) { //stop playing flac when duration is over //second delay
                play_new_song[z]=TRUE;
                playing_wav[z]=FALSE;
                loading_mp3[z]=FALSE;
                loading_flac[z]=FALSE;
                loading_wav[z]=FALSE;
                time_song_end[z]=-1;
                current_song_time[z]=-1;
              }
            }


            if (!playing_wav[z]) {//im not playing a wav music
              if ((loading_flac[z] || loading_mp3[z] || loading_wav[z])) {//loading flac or wav, flac
                call_help_timer=0;
                current_song_time[z]=-1;
                
                if (loading_wav[z]) {
                  switch (z) {
                    case 0:swprintf(wav_song_playing,256,L"%s/%s",src_music_dir,song_names[song_rand_num[0]]);break;
                    case 1:swprintf(wav_song_playing2,256,L"%s/%s",src_music_dir,song_names[song_rand_num[1]]);break;
                  }
                } else {
                  switch (z) {
                    case 0:swprintf(wav_song_playing,256,L"music_tmp/tmp/tmp.wav");break;
                    case 1:swprintf(wav_song_playing2,256,L"music_tmp/tmp2/tmp.wav");break;
                  }
                }

                switch (z) {
                  case 0:LoadBufferSFX(wav_song_playing,0);break;
                  case 1:LoadBufferSFX(wav_song_playing2,1);break;                
                  break;
                }
                time_song_end[z]=song_audio_duration[z];//time_song_start+song_audio_duration[z];//songAudio->duration;
                loading_flac[z]=FALSE;
                loading_mp3[z]=FALSE;
                loading_wav[z]=FALSE;
                play_new_song[z]=FALSE;
                playing_wav[z]=TRUE;


              } else if (!loading_flac[z] && !loading_mp3[z] && !loading_wav[z]) { //not loading song 
                if (play_new_song[z]) //song status: stopped
                {
                  LiveWaveClose(z);
                //play new music
                  call_help_timer=0;
                  if (audioData[z].music_file) {
                    fclose(audioData[z].music_file);
                  }
                  switch (z) {
                    case 0:
                       remove("music_tmp/tmp/tmp.wav");
                       rmdir("music_tmp/tmp"); //remove tmp, manually because C is like that
                       break;
                    case 1:
                      remove("music_tmp/tmp2/tmp.wav");
                      rmdir("music_tmp/tmp2"); //remove tmp, manually because C is like that
                      break;
                  }

                  if (!skip_song[z] && !skipping_song[z]) {
                    switch (song_mode[z]) {
                      case 0: //play songs shuffle
                        song_rand_num[z]=RandNum(0,song_num-1,1);
                        break;
                      case 1: //Play Songs acending
                        song_rand_num[z]=LimitValue(song_rand_num[z]+1,0,song_num);
                        break;
                      case 2: //Play songs decending
                        song_rand_num[z]=LimitValue(song_rand_num[z]-1,0,song_num);
                        break;
                    }
                  } else {
                    skip_song[z]=FALSE;
                  }

                  if (skipping_song[z]) {
                    skipping_song[z]=FALSE;
                  }

                  //stop .wav player
                  current_song_time[z]=-1;
                  time_song_end[z]=-1;

                  if (is_flac[song_rand_num[z]]) { //loaded song is a flac
                    wchar_t my_command[512];
                    loading_flac[z]=TRUE;
                    switch (z) {
                      case 0:
                        system("mkdir \"music_tmp/tmp\""); //make new tmp
                        swprintf(my_command,512,L"flac.exe --totally-silent -d -f \"%s/%s\" -o music_tmp/tmp/tmp.wav",src_music_dir,song_names[song_rand_num[z]]);
                        _wsystem(my_command);
                        break;
                      case 1:
                        system("mkdir \"music_tmp/tmp2\""); //make new tmp
                        swprintf(my_command,512,L"flac.exe --totally-silent -d -f \"%s/%s\" -o music_tmp/tmp2/tmp.wav",src_music_dir,song_names[song_rand_num[z]]);
                        _wsystem(my_command);
                        break;
                    }

                  } else if (is_mp3[song_rand_num[z]]) {
                    wchar_t my_command[512];
                    loading_mp3[z]=TRUE;
                    //http://mpg123.de/download/win32/mpg123-1.10.1-static-x86.zip //currently used to decode mp3
                    //swprintf(my_command,512,L"madplay.exe -b 16 -Q -R 44100  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num[z]]); //not compatible with unicode/utf16
                    //swprintf(my_command,512,L"lame.exe --decode  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num[z]]); //unable to decode to a specific desired sample rate
                    switch (z) {
                      case 0:
                        system("mkdir \"music_tmp/tmp\""); //make new tmp
                        swprintf(my_command,512,L"mpg123.exe -q -w \"music_tmp/tmp/tmp.wav\"  \"%s/%s\"",src_music_dir,song_names[song_rand_num[z]]);
                        _wsystem(my_command);
                        break;
                      case 1:
                        system("mkdir \"music_tmp/tmp2\""); //make new tmp
                        swprintf(my_command,512,L"mpg123.exe -q -w \"music_tmp/tmp2/tmp.wav\"  \"%s/%s\"",src_music_dir,song_names[song_rand_num[z]]);
                        _wsystem(my_command);
                        break;
                    }
                  } else if (is_wav[song_rand_num[z]]) {
                    loading_wav[z]=TRUE;
                  }
                  play_new_song[z]=FALSE;
                }
              } //else { //song status: playing
                
              //}
            }  
          }
        //End of song Task
         if (toggle_stop_playing_song[z]) {
           LiveWaveClose(z);
           if (audioData[z].music_file) {
             fclose(audioData[z].music_file);
           }
        //play new music
           call_help_timer=0;
           switch (z) {
             case 0:
               remove("music_tmp/tmp/tmp.wav");
               rmdir("music_tmp/tmp"); //remove tmp, manually because C is like that
               break;
             case 1:
               remove("music_tmp/tmp2/tmp.wav");
               rmdir("music_tmp/tmp2"); //remove tmp, manually because C is like that
               break;
           }
           playing_wav[z]=FALSE;
           loading_wav[z]=
           loading_flac[z]=
           loading_mp3[z]=FALSE;
           play_new_song[z]=FALSE;
           stop_playing_song[z]=TRUE;
           toggle_stop_playing_song[z]=FALSE;
         }

       } // end of for loop


         if (flag_adjust_wav_out_audio) {
           long int vol=VolumeValue(wav_out_volume*100,1);
           waveOutSetVolume(audioData[0].hWaveOut,vol);
           waveOutSetVolume(audioData[1].hWaveOut,vol);
           flag_adjust_wav_out_audio=FALSE;
         }
     } //end of if song>0





    //Game sound tasks
  if (!in_main_menu) {
      //Play Game Souond
      if (game_audio && level_loaded) {
        if (!player.time_breaker) { //player sounds made by player bullets
          for (int i=0;i<player.bullet_shot_num;i++) {
            BulletSndAct(player.bullet[i]);
          }
        }
        if (player.bullet_shot!=-1) { //player sounds made by sniper player bullets
          BulletSndAct(player.bullet_shot);
        }
        if (level_loaded) {
          if (!flag_restart_audio) {
            for (int i=0;i<ENEMY_NUM;i++) {
              EnemySndAct(i);
            }
            PlayerSndAct();       
          } else {
            for (int i=0;i<ENEMY_NUM;i++) {
              PlaySound(NULL, NULL, SND_ASYNC);
            }
            flag_restart_audio=FALSE;
          }
        }
      }

      if (call_help_timer<2000/*5000*/) {
        call_help_timer+=6;
      }
      Sleep(6);

  } else { //in main menu
      //persian time update if day change
      //get_current_time(&current_hour,&current_min,&current_sec);

        if (clean_up_sound) {
          for (int i=0;i<MAX_ENEMY_NUM;i++) {
            PlaySound(NULL, NULL, SND_ASYNC);
          }

          waveOutReset(hWaveOut[0]);
          mem_snd_interrupt[0]=TRUE;

          waveOutReset(hWaveOut[1]);
          mem_snd_interrupt[1]=TRUE;

          waveOutReset(hWaveOut[3]);
          mem_snd_interrupt[3]=TRUE;

          waveOutReset(hWaveOut[4]);
          mem_snd_interrupt[4]=TRUE;

          waveOutReset(hWaveOut[5]);
          mem_snd_interrupt[5]=TRUE;

          if (lvl_has_song) {
            //play new music
              call_help_timer=0;
              //if (audioData[z].music_file)
                //fclose(audioData.music_file);
              //remove("music/tmp/tmp.wav");
              //rmdir("music/tmp"); //remove tmp, manually because C is like that
              lvl_has_song=FALSE;
              InitSongBank();
              play_new_song[0]=TRUE;
              play_new_song[1]=TRUE;
          }


          for (int i=0;i<SPAM_SFX_NUM;i++) {
            freeSoundEffectCache(&spamSoundEffectCache[i]);
          }
          for (int i=0;i<CHANNEL_SFX_NUM;i++) {
            freeSoundEffectCache(&channelSoundEffectCache[i]);
          }
          clean_up_sound=FALSE;
        }

      if (current_hour==0 && current_min==0 && current_sec<=1) {//next day
        int64_t timenow=int64_current_timestamp();
        PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day);
        PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift);
      }

      Sleep(1000);
    }
  } //end of while loop
}



