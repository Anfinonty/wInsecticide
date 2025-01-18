
#define SONG_NUM 10
#define SONG_FOLDER_NUM 23

int current_hour;
int current_min;
int current_sec;

int song_num=0;
int song_mode=0;
int song_rand_num=-1;
int song_audio_duration;
wchar_t song_names[2000][256];
bool is_flac[2000];
bool is_wav[2000];
bool is_mp3[2000];
bool play_new_song=TRUE;
bool stop_playing_song=FALSE;
bool toggle_stop_playing_song=FALSE;
bool playing_wav=FALSE;
bool loading_mp3=FALSE;
bool loading_flac=FALSE;
bool loading_wav=FALSE;
bool skip_song=FALSE;
bool skipping_song=FALSE;
bool clean_up_song=FALSE;
bool clean_up_sound=FALSE;
wchar_t wav_song_playing[256];
bool song_speed_up=FALSE;

unsigned long long time_song_start;
unsigned long long time_song_end;
unsigned long long current_song_time;


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
//https://www.codeproject.com/Articles/8396/Using-DirectSound-to-Play-Audio-Stream-Data
//im not using direct sound just learning its concept

//https://stackoverflow.com/questions/49605552/double-buffered-waveoutwrite-stuttering-like-hell
//https://www.vbforums.com/showthread.php?742723-RESOLVED-waveout-API-Tutorial-Example
//https://stackoverflow.com/questions/49605552/double-buffered-waveoutwrite-stuttering-like-hell

#define AUDIO_STREAM_BUFFER_NUM    2  //2mb
//#define AUDIO_STREAM_BUFFER_SIZE   524288//4096//524288//44100//16384//8192//524288  minimum //int16_t *524288 = 1mb //0.00049mb

//largest buffer reserved for slack only
#define AUDIO_STREAM_BUFFER_SIZE0    16384 //2048//8192//16384//524288//8192//4096//524288//4096   8192 or 16384
#define AUDIO_STREAM_BUFFER_SIZE1    8192  //2048//8192//16384//524288//8192//4096//524288//4096   8192 or 16384
#define AUDIO_STREAM_BUFFER_SIZE2    4096
#define AUDIO_STREAM_BUFFER_SIZE3    2048


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
  AWavHeader wav_header[44];
  HWAVEOUT hWaveOut;
  FILE *music_file;
  bool double_buffer;  

  long current_filesize; //spindle
  long filesize;
  long buffer_size;

  WAVEHDR waveHdr1;
  WAVEHDR waveHdr2;

  //int16_t audio[AUDIO_STREAM_BUFFER_NUM][AUDIO_STREAM_BUFFER_SIZE];
  //int16_t tmp_buffer1[AUDIO_STREAM_BUFFER_SIZE0];
  //int16_t tmp_buffer2[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t buffer1[AUDIO_STREAM_BUFFER_SIZE0];
  int16_t buffer2[AUDIO_STREAM_BUFFER_SIZE0];
} AudioData;
AudioData audioData;


//#define SAMPLE_RATE 44100
//#define BUFFER_SIZE 4096
//#define BUFFER_SIZE  AUDIO_STREAM_BUFFER_SIZE2 //4096
//#define OVERLAP_SIZE (BUFFER_SIZE / 2)
//#define STRETCH_RATIO 100  // For 50% faster tempo


/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SAMPLE_RATE 44100
#define BUFFER_SIZE 4096
#define OVERLAP_SIZE (BUFFER_SIZE / 2)
#define STRETCH_RATIO 1.5  // For 50% faster tempo

void applyWindowFunction(float* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] *= 0.5 * (1 - cos(2 * M_PI * i / (size - 1)));  // Hann window
    }
}

void timeStretch(float* input, float* output, int inputSize, int outputSize) {
    int inputPos = 0;
    int outputPos = 0;
    float* buffer1 = (float*)malloc(BUFFER_SIZE * sizeof(float));
    float* buffer2 = (float*)malloc(BUFFER_SIZE * sizeof(float));

    while (inputPos + BUFFER_SIZE < inputSize && outputPos + BUFFER_SIZE < outputSize) {
        // Read frames with overlap
        memcpy(buffer1, &input[inputPos], BUFFER_SIZE * sizeof(float));
        memcpy(buffer2, &input[inputPos + OVERLAP_SIZE], BUFFER_SIZE * sizeof(float));

        // Apply window function
        applyWindowFunction(buffer1, BUFFER_SIZE);
        applyWindowFunction(buffer2, BUFFER_SIZE);

        // Cross-fade and mix frames
        for (int i = 0; i < BUFFER_SIZE; i++) {
            output[outputPos + i] = (buffer1[i] * (BUFFER_SIZE - i) + buffer2[i] * i) / BUFFER_SIZE;
        }

        // Update positions
        inputPos += BUFFER_SIZE / STRETCH_RATIO;
        outputPos += BUFFER_SIZE;
    }

    free(buffer1);
    free(buffer2);
}

int main() {
    FILE* inputFile = fopen("input.wav", "rb");
    FILE* outputFile = fopen("output.wav", "wb");

    // Read input file and allocate buffers
    fseek(inputFile, 0, SEEK_END);
    int inputSize = ftell(inputFile) / sizeof(float);
    fseek(inputFile, 0, SEEK_SET);
    float* inputBuffer = (float*)malloc(inputSize * sizeof(float));
    fread(inputBuffer, sizeof(float), inputSize, inputFile);

    int outputSize = inputSize / STRETCH_RATIO;
    float* outputBuffer = (float*)malloc(outputSize * sizeof(float));

    // Apply time-stretching
    timeStretch(inputBuffer, outputBuffer, inputSize, outputSize);

    // Write output file
    fwrite(outputBuffer, sizeof(float), outputSize, outputFile);

    // Clean up
    free(inputBuffer);
    free(outputBuffer);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
*/


/*
void applyWindowFunction(int16_t* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] *= 0.5 * (1 - cos(2 * M_PI * i / (size - 1)));  // Hann window
    }
}

void timeStretch(int16_t* input, int16_t* output,int BUFFER_SIZE) {
    int OVERLAP_SIZE = BUFFER_SIZE/2;
    int16_t* buffer1 = (int16_t*)malloc(BUFFER_SIZE * sizeof(int16_t));
    int16_t* buffer2 = (int16_t*)malloc(BUFFER_SIZE * sizeof(int16_t));

        // Read frames with overlap
    memcpy(buffer1, &input[0], BUFFER_SIZE * sizeof(int16_t));
    memcpy(buffer2, &input[OVERLAP_SIZE], BUFFER_SIZE * sizeof(int16_t));

    // Apply window function
    applyWindowFunction(buffer1, BUFFER_SIZE);
    applyWindowFunction(buffer2, BUFFER_SIZE);
    for (int i = 0; i < BUFFER_SIZE; i++) {
      output[i] = (buffer1[i] * (BUFFER_SIZE - i) + buffer2[i] * i) / BUFFER_SIZE;
    }
    free(buffer1);
    free(buffer2);
}*/

//memcpy(output, input, BUFFER_SIZE);

/*
int main() {
    FILE* inputFile = fopen("input.wav", "rb");
    FILE* outputFile = fopen("output.wav", "wb");

    // Read input file and allocate buffers
    fseek(inputFile, 0, SEEK_END);
    int inputSize = ftell(inputFile) / sizeof(float);
    fseek(inputFile, 0, SEEK_SET);
    float* inputBuffer = (float*)malloc(inputSize * sizeof(float));
    fread(inputBuffer, sizeof(float), inputSize, inputFile);

    int outputSize = inputSize / STRETCH_RATIO;
    float* outputBuffer = (float*)malloc(outputSize * sizeof(float));

    // Apply time-stretching
    timeStretch(inputBuffer, outputBuffer, inputSize, outputSize);

    // Write output file
    fwrite(outputBuffer, sizeof(float), outputSize, outputFile);

    // Clean up
    free(inputBuffer);
    free(outputBuffer);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

*/


void applyWindowFunction(int16_t* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] *= 0.5 * (1 - cos(2 * M_PI * i / (size - 1)));  // Hann window
    }
}

void timeStretch(int16_t* input, int16_t* output, int input_size, float stretch_factor) {
    int output_size = (int)(input_size * stretch_factor);
    int overlap_size = input_size / 2; // Overlap size set to half of input size
    int16_t* buffer1 = (int16_t*)malloc(input_size * sizeof(int16_t));
    int16_t* buffer2 = (int16_t*)malloc(input_size * sizeof(int16_t));

    for (int i = 0; i < output_size; i += input_size - overlap_size) {
        int j = i / stretch_factor;
        if (j + input_size > input_size * 2) break; // Avoid going out of bounds

        memcpy(buffer1, &input[j], input_size * sizeof(int16_t));
        memcpy(buffer2, &input[j + overlap_size], input_size * sizeof(int16_t));

        // Apply window function
        applyWindowFunction(buffer1, input_size);
        applyWindowFunction(buffer2, input_size);

        for (int k = 0; k < input_size; k++) {
            int index = i + k;
            if (index >= output_size) break;

            output[index] = (buffer1[k] * (input_size - k) + buffer2[k] * k) / input_size;
        }
    }

    free(buffer1);
    free(buffer2);
}

/*int main() {
    // Example usage:
    int input_size = 1024;
    float stretch_factor = 1.5; // Increase duration by 1.5 times
    int output_size = (int)(input_size * stretch_factor);
    int16_t* input = (int16_t*)malloc(input_size * sizeof(int16_t));
    int16_t* output = (int16_t*)malloc(output_size * sizeof(int16_t));

    // Initialize input buffer with example data
    for (int i = 0; i < input_size; i++) {
        input[i] = (int16_t)(32767 * sin(2 * M_PI * i / input_size));
    }

    // Perform time-stretching
    timeStretch(input, output, input_size, stretch_factor);

    // Free allocated memory
    free(input);
    free(output);

    return 0;
}*/


void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
    AudioData* audioData = (AudioData *)dwInstance;
    if (playing_wav) {
      if (uMsg == WOM_DONE) {
        WAVEHDR *waveHdr = (WAVEHDR *)dwParam1;
        if (waveHdr == &audioData->waveHdr1) {
            audioData->double_buffer=TRUE;
            audioData->current_filesize+=chosen_buffer_size;
            //fread(audioData->tmp_buffer1, sizeof(BYTE), chosen_buffer_size, audioData->music_file);
            //timeStretch(audioData->tmp_buffer1,audioData->buffer1,chosen_buffer_size,1.5);
            fread(audioData->buffer1, sizeof(BYTE), chosen_buffer_size, audioData->music_file);
            //memcpy(audioData->buffer1, audioData->tmp_buffer1, chosen_buffer_size);
            waveOutWrite(audioData->hWaveOut, &audioData->waveHdr1, sizeof(WAVEHDR));
        } else if (waveHdr == &audioData->waveHdr2) {
            audioData->double_buffer=FALSE;
            audioData->current_filesize+=chosen_buffer_size;
            //fread(audioData->tmp_buffer2, sizeof(BYTE), chosen_buffer_size, audioData->music_file);
            //timeStretch(audioData->tmp_buffer2,audioData->buffer2,chosen_buffer_size,1.5);
            fread(audioData->buffer2, sizeof(BYTE), chosen_buffer_size, audioData->music_file);
            //memcpy(audioData->buffer2, audioData->tmp_buffer2, chosen_buffer_size);
            waveOutWrite(audioData->hWaveOut, &audioData->waveHdr2, sizeof(WAVEHDR));
        }
      }
    }
}


void LiveWaveClose()
{
  waveOutReset(audioData.hWaveOut);
  waveOutUnprepareHeader(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));
  waveOutUnprepareHeader(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));
  waveOutClose(audioData.hWaveOut);

}

void LiveWaveReOpen()
{

     waveOutOpen(&audioData.hWaveOut, WAVE_MAPPER, &wfx_wav_music, (DWORD_PTR)waveOutProc, (DWORD_PTR)&audioData, CALLBACK_FUNCTION);

      long int vol=VolumeValue(wav_out_volume*100,1);
      waveOutSetVolume(audioData.hWaveOut,vol);

      audioData.waveHdr1.lpData = (LPSTR)audioData.buffer1;
      audioData.waveHdr1.dwBufferLength = chosen_buffer_size;//AUDIO_STREAM_BUFFER_SIZE;
      audioData.waveHdr1.dwFlags = 0;
      audioData.waveHdr1.dwLoops = 0;
      waveOutPrepareHeader(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));
      waveOutWrite(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));


      audioData.waveHdr2.lpData = (LPSTR)audioData.buffer2;
      audioData.waveHdr2.dwBufferLength = chosen_buffer_size;//AUDIO_STREAM_BUFFER_SIZE;
      audioData.waveHdr2.dwFlags = 0;
      audioData.waveHdr2.dwLoops = 0;
      waveOutPrepareHeader(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));
      waveOutWrite(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));

}


void LoadBufferSFX(const wchar_t* filename)
{
  //audioData.double_buffer=FALSE;
  if (audioData.music_file)
    fclose(audioData.music_file);

  audioData.music_file = _wfopen(filename, L"rb");
  int wav_header_size=sizeof(AWavHeader); //44
  audioData.current_filesize=wav_header_size;


  //reset buffers
  memset(audioData.buffer1, 0, sizeof(audioData.buffer1));
  memset(audioData.buffer2, 0, sizeof(audioData.buffer2));

  if (audioData.music_file) {
    fseek(audioData.music_file, 0, SEEK_END);
    long filesize;
    filesize = ftell(audioData.music_file);// - wav_header_size; 

    //Alloc wav buffer header
    fseek(audioData.music_file, 0, SEEK_SET);
    fread(audioData.wav_header,1,wav_header_size,audioData.music_file);

    wfx_wav_music.wFormatTag = WAVE_FORMAT_PCM;
    wfx_wav_music.nChannels = audioData.wav_header->NumOfChan;
    wfx_wav_music.nSamplesPerSec = audioData.wav_header->SamplesPerSec;
    wfx_wav_music.nAvgBytesPerSec = audioData.wav_header->bytesPerSec;
    wfx_wav_music.nBlockAlign = audioData.wav_header->blockAlign;
    wfx_wav_music.wBitsPerSample = audioData.wav_header->bitsPerSample;
    wfx_wav_music.cbSize = 0;

    audioData.buffer_size=chosen_buffer_size;
    audioData.filesize=filesize;
    song_audio_duration=(double)filesize / (audioData.wav_header->SamplesPerSec * audioData.wav_header->NumOfChan * audioData.wav_header->bitsPerSample/8) *1000;



    // Initialize AudioData structure
     waveOutOpen(&audioData.hWaveOut, WAVE_MAPPER, &wfx_wav_music, (DWORD_PTR)waveOutProc, (DWORD_PTR)&audioData, CALLBACK_FUNCTION);

    long int vol=VolumeValue(wav_out_volume*100,1);
    waveOutSetVolume(audioData.hWaveOut,vol);

    if (audioData.current_filesize>audioData.filesize) {
      audioData.current_filesize=audioData.filesize;
    }
    fread(audioData.buffer1,1,chosen_buffer_size,audioData.music_file);
    audioData.current_filesize+=chosen_buffer_size;
    audioData.waveHdr1.lpData = (LPSTR)audioData.buffer1;
    audioData.waveHdr1.dwBufferLength = chosen_buffer_size;//AUDIO_STREAM_BUFFER_SIZE;
    audioData.waveHdr1.dwFlags = 0;
    audioData.waveHdr1.dwLoops = 0;
  //  waveOutUnprepareHeader(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));
    waveOutPrepareHeader(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));
    waveOutWrite(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));

    if (audioData.current_filesize>audioData.filesize) {
       audioData.current_filesize=audioData.filesize;
    }
    fread(audioData.buffer2,1,chosen_buffer_size,audioData.music_file);
    audioData.current_filesize+=chosen_buffer_size;
    audioData.waveHdr2.lpData = (LPSTR)audioData.buffer2;
    audioData.waveHdr2.dwBufferLength = chosen_buffer_size;//AUDIO_STREAM_BUFFER_SIZE;
    audioData.waveHdr2.dwFlags = 0;
    audioData.waveHdr2.dwLoops = 0;
   // waveOutUnprepareHeader(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));
    waveOutPrepareHeader(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));
    waveOutWrite(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));

  }
}





DWORD WINAPI SoundTask(LPVOID lpArg) {
  srand(time(NULL));
  while (TRUE) {
    //SONG Tasks
    if (song_num>0) {
      if (!stop_playing_song) {

        if (playing_wav) { //hi im currently playing a flac/wav
          
          current_song_time=current_timestamp();
          if (current_song_time>time_song_end ||  play_new_song) { //stop playing flac when duration is over //second delay
            play_new_song=TRUE;
            playing_wav=FALSE;
            loading_mp3=FALSE;
            loading_flac=FALSE;
            loading_wav=FALSE;
            time_song_start=0;
            time_song_end=-1;
            current_song_time=-1;
          }
        }


        if (!playing_wav) {//im not playing a wav music
          if ((loading_flac || loading_mp3 || loading_wav)) {//loading flac or wav, flac
            call_help_timer=0;
            current_song_time=-1;
            if (loading_wav) {
              swprintf(wav_song_playing,256,L"%s/%s",src_music_dir,song_names[song_rand_num]);
            } else {
              swprintf(wav_song_playing,256,L"music/tmp/tmp.wav");
            }

            LoadBufferSFX(wav_song_playing);
            time_song_start=current_timestamp();
            time_song_end=time_song_start+song_audio_duration;//songAudio->duration;
            loading_flac=FALSE;
            loading_mp3=FALSE;
            loading_wav=FALSE;
            play_new_song=FALSE;
            playing_wav=TRUE;


          } else if (!loading_flac && !loading_mp3 && !loading_wav) { //not loading song 
            if (play_new_song) //song status: stopped
            {
              LiveWaveClose();
            //play new music
              call_help_timer=0;
              if (audioData.music_file)
                fclose(audioData.music_file);
              remove("music/tmp/tmp.wav");
              rmdir("music/tmp"); //remove tmp, manually because C is like that

              if (!skip_song && !skipping_song) {
                switch (song_mode) {
                  case 0: //play songs shuffle
                    song_rand_num=RandNum(0,song_num-1,1);
                    break;
                  case 1: //Play Songs acending
                    song_rand_num=LimitValue(song_rand_num+1,0,song_num);
                    break;
                  case 2: //Play songs decending
                    song_rand_num=LimitValue(song_rand_num-1,0,song_num);
                    break;
                }
              } else {
                skip_song=FALSE;
              }

              if (skipping_song) {
                skipping_song=FALSE;
              }

              //stop .wav player
              current_song_time=-1;
              time_song_end=-1;

              if (is_flac[song_rand_num]) { //loaded song is a flac
                wchar_t my_command[512];
                loading_flac=TRUE;
                system("mkdir \"music/tmp\""); //make new tmp
                swprintf(my_command,512,L"flac.exe --totally-silent -d -f \"%s/%s\" -o music/tmp/tmp.wav",src_music_dir,song_names[song_rand_num]);
                _wsystem(my_command);
              } else if (is_mp3[song_rand_num]) {
                wchar_t my_command[512];
                loading_mp3=TRUE;
                system("mkdir \"music/tmp\""); //make new tmp
                //http://mpg123.de/download/win32/mpg123-1.10.1-static-x86.zip //currently used to decode mp3
                //swprintf(my_command,512,L"madplay.exe -b 16 -Q -R 44100  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]); //not compatible with unicode/utf16
                //swprintf(my_command,512,L"lame.exe --decode  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]); //unable to decode to a specific desired sample rate

                swprintf(my_command,512,L"mpg123.exe -q -w \"music/tmp/tmp.wav\"  \"%s/%s\"",src_music_dir,song_names[song_rand_num]);
                _wsystem(my_command);
              } else if (is_wav[song_rand_num]) {
                loading_wav=TRUE;
              }
              play_new_song=FALSE;
            }
          } //else { //song status: playing
            //LoadBufferSFX(wav_song_playing);
            //waveOutWrite(audioData.hWaveOut, &audioData.waveHdr1, sizeof(WAVEHDR));
            //waveOutWrite(audioData.hWaveOut, &audioData.waveHdr2, sizeof(WAVEHDR));
          //}
        }  
      }
    }
    //End of song Task


    if (toggle_stop_playing_song) {
      LiveWaveClose();
      if (audioData.music_file)
        fclose(audioData.music_file);

    //play new music
      call_help_timer=0;
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp, manually because C is like that

      playing_wav=FALSE;
      loading_wav=
      loading_flac=
      loading_mp3=FALSE;
      play_new_song=FALSE;

      stop_playing_song=TRUE;
      toggle_stop_playing_song=FALSE;
    }

    if (flag_adjust_wav_out_audio) {
      long int vol=VolumeValue(wav_out_volume*100,1);
      waveOutSetVolume(audioData.hWaveOut,vol);
      flag_adjust_wav_out_audio=FALSE;
    }


    //SOUND Tasks
    if (!in_main_menu) { //NOT in main menu, gaming
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
      Sleep(6); //fast loop
    } else { //In Main Menu
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
              if (audioData.music_file)
                fclose(audioData.music_file);
              remove("music/tmp/tmp.wav");
              rmdir("music/tmp"); //remove tmp, manually because C is like that
              lvl_has_song=FALSE;
              InitSongBank();
              play_new_song=TRUE;
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
      Sleep(1000); //eepy loop
    }
  }
}



