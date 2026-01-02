
#include "math_randnum.c"

//Solar Hijri Time for Drawing
int64_t solar_sec=0,solar_min=0,solar_hour=0,solar_day=0,solar_month=0,solar_year=0,solar_day_of_week=0;
float solar_angle_day=0;
bool solar_leap_year;
bool solar_last_year_is_leap;
int64_t total_solar_hijri_days=0;

//Lunar Hijri Time for Drawing
int lunar_sec=0,lunar_min=0,lunar_hour=0,lunar_day=0,lunar_month=0,lunar_year=0,lunar_day_of_week=0;
float moon_angle_shift=0;
bool lunar_leap_year;

unsigned long long current_timestamp() {//https://copyprogramming.com/howto/c-sleep-in-milliseconds-in-c-code-example
//https://stackoverflow.com/questions/10301106/gettimeofday-returns-a-negative-value
  struct timeval te;
  mingw_gettimeofday(&te, NULL); //get current time
  return (unsigned long long) te.tv_sec*1000LL + te.tv_usec/1000; //calc millisecs
}

/*unsigned int int_current_timestamp() {
  struct timeval te;
  mingw_gettimeofday(&te, NULL);
  return (unsigned int) te.tv_sec;
}*/



#define SEC_PER_DAY   86400
#define SEC_PER_HOUR  3600
#define SEC_PER_MIN   60

void get_current_time(int *lhour,int *lmin, int* lsec)
{ //https://stackoverflow.com/questions/43732241/how-to-get-datetime-from-gettimeofday-in-c
//https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemtime
//  int64_t tnow = int64_current_timestamp();/*-SEC_PER_HOUR*16;*/
//tnow/SEC_PER_HOUR%24;
//(tnow%SEC_PER_HOUR)/SEC_PER_MIN;
//(tnow%SEC_PER_HOUR)%SEC_PER_MIN;

  SYSTEMTIME /*st,*/lt;
  GetLocalTime(&lt);
  //GetSystemTime(&st);

  *lhour= lt.wHour;
  *lmin= lt.wMinute;
  *lsec= lt.wSecond;
}

void get_current_time_diff(int64_t *hour_diff,int64_t *min_diff, int64_t *sec_diff)
{ //https://stackoverflow.com/questions/43732241/how-to-get-datetime-from-gettimeofday-in-c
  SYSTEMTIME st,lt;
  GetLocalTime(&lt);
  GetSystemTime(&st);

  int64_t lhour= lt.wHour;
  int64_t lmin= lt.wMinute;
  int64_t lsec= lt.wSecond;

  int64_t shour= st.wHour;
  int64_t smin= st.wMinute;
  int64_t ssec= st.wSecond;

  *hour_diff=shour-lhour;
  *min_diff=smin-lmin;
  *sec_diff=ssec-lsec;
}



int64_t int64_current_timestamp()
{//https://stackoverflow.com/questions/13804095/get-the-time-zone-gmt-offset-in-c
 //https://www.unixtimestamp.com/ returns time where GMT+0000
  struct timeval te;
  mingw_gettimeofday(&te, NULL);
  __time64_t t;
  _time64(&t); //GMT 0 time

  int64_t h_diff;
  int64_t m_diff;
  int64_t _l;

  get_current_time_diff(&h_diff,&m_diff,&_l); //Get GMT <x> difference

  //printf("\nhdiff:%lld, mdiff:%lld\n",h_diff,m_diff); //Print difference


  return (int64_t) t-  (SEC_PER_HOUR*h_diff) - (SEC_PER_MIN*m_diff);
}


unsigned long long long_current_timestamp() {
  struct timeval te;
  //struct timezone tz;
  //mingw_gettimeofday(&te,&tz);2037
  mingw_gettimeofday(&te,NULL);
  return (unsigned long long) te.tv_sec;
}


// Source - https://stackoverflow.com/questions/13804095/get-the-time-zone-gmt-offset-in-c
// Posted by Hill, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-12, License - CC BY-SA 4.0

int time_offset()
{
    time_t gmt, rawtime = time(NULL);
    struct tm *ptm;

#if !defined(WIN32)
    struct tm gbuf;
    ptm = gmtime_r(&rawtime, &gbuf);
#else
    ptm = gmtime(&rawtime);
#endif
    // Request that mktime() looksup dst in timezone database
    ptm->tm_isdst = -1;
    gmt = mktime(ptm);

    return (int)difftime(rawtime, gmt);
}









//Maths
//public_seed = rand() in WM_PAINT
//minmax inclusive
int RandNum(int min, int max, int *global_rand_i,int seed) {
  int m=0;
  if (min<0) {
    m=min-1;
  }
  if (seed!=-1) { //random not from table
    return (rand()+seed)%(max-m + 1) + m;
  }
  return (myrand(global_rand_i))%(max-m + 1) + m;
}



float RandAngle(int min, int max, int *global_rand_i,int seed) {
  return RandNum(min,max,global_rand_i,seed)*M_PI/180;
}


//----------------------------------------
float GetGradient(float x1,float y1,float x2,float y2) 
{
  // gradient = rise/run, m = (y1-y2)/(x1-x2)
  float denominator=(x2-x1);
  if (denominator==0) {
    return DBL_MAX;
  }
  return (y2-y1)/denominator;
}

float GetGroundC(float x,float y,float gradient)
{
  //y=mx+c, c = y-mx
  return y-gradient*x; 
}

float GetX(float y,float gradient,float c)
{
  return (y-c)/gradient;
}


float GetY(float x,float gradient,float c)
{
  return x*gradient+c;
}


//https://stackoverflow.com/questions/1349542/john-carmacks-unusual-fast-inverse-square-root-quake-iii
// Source - https://stackoverflow.com/q
// Posted by Alex, modified by community. See post 'Timeline' for change history
// Retrieved 2025-11-13, License - CC BY-SA 3.0

/*float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}*/


float GetDistance(float x1,float y1,float x2,float y2)
{
  //float d=Y*Y+X*X;
  //return d*Q_rsqrt(d);
  float Y=y2-y1;
  float X=x2-x1;
  float dist=sqrt(Y*Y+X*X);//sqrt(pow(y2-y1,2)+pow(x2-x1,2));
  return dist;
}

float GetSinAngle(float height,float length)
{
  return asin(height/length);
}

float GetCosAngle(float vertical_distance,float length)
{
  return acos(vertical_distance/length);
}

float GetRotateAngle(float x,float y) 
{
  float adj=0,hypo=0,E=0;
  adj=x;
  hypo=GetDistance(x,y,0,0);
  E=acos(adj/hypo);
  if (x<0) {
    if (y<0) {
      E=M_PI-E;
    } else {
      E=M_PI+E;
    }
  } else if (y<0) {
    E=2*M_PI-E;
  }
  return E;
}

//------------------------------------------------------------
bool IsOutOfBounds(float x,float y,int r,int max_width,int max_height)
{
  if (x<=r || x>=max_width-r ||  //more than or less than
      y<=r || y>=max_height-r) { //
    return TRUE;
  }
  return FALSE;
}



int LimitValueInt(int value, int min, int max)
{
  int num=value;
  if (value>=max) {
    num=min;
  } else if (value<min) {
    num=max-1;
  }
  return num;
}


float LimitValue(float value,float min,float max)
{
  float num=value;
  if (value>=max) {
    num=min;
  } else if (value<min) {
    num=max-1;
  }
  return num;
}

//------------------------------------------------------



DWORD VolumeValue(const int percentage, const int reduce) {
    // Clamp percentage value
    int hpercentage = percentage/reduce;
    int p = (hpercentage < 0) ? 0 : (hpercentage > 100) ? 100 : hpercentage;
    // Calculate scaled value for one channel
    const WORD wVol = (WORD)((65535 * p) / 100);
    // Construct return value for both channels
    const DWORD retVal = ((wVol << 16) | wVol);
    return retVal;
}



//https://www.geeksforgeeks.org/c-program-copy-contents-one-file-another-file/
int myCopyFile(const wchar_t* filename_dest,const wchar_t* filename_src)
{
    FILE *fptr1, *fptr2;
    int c;

    // Open one file for reading
    fptr1 = _wfopen(filename_src, L"r");
    if (fptr1 == NULL)
      return 0;

    // Open another file for writing
    fptr2 = _wfopen(filename_dest, L"w");
    if (fptr2 == NULL)
      return 0;

    // Read contents from file
    while ((c = fgetwc(fptr1)) != WEOF)
    {
        fputwc(c, fptr2);
    }

    fclose(fptr1);
    fclose(fptr2);
    return 0;
}




int stickyTo(int num, int sticky) {
  if (sticky==0)
    return num;
  return sticky * ((num + (sticky-1)) /sticky);
}


int GetXFromId(int id,int width) 
{
  if (width>0) {
    return id%width;
  } else {
    return 0;
  }
}

int GetYFromId(int id,int width)
{ 
  if (width>0) {
    return id/width;
  } else {
    return 0;
  }
}


//https://www.baeldung.com/cs/check-if-point-is-in-2d-triangle

//https://stackoverflow.com/questions/9513107/find-if-4-points-form-a-quadrilateral
//https://www.geeksforgeeks.org/geometric-algorithms/
//https://www.geeksforgeeks.org/c-cpp-programs-for-geometric-algorithms/
//https://stackoverflow.com/questions/18084065/find-order-of-points-to-make-a-quadrilateral

// Function to calculate the area of a triangle given its vertices
float triangleArea(float x1, float y1, float x2, float y2, float x3, float y3) {
    float area=(x1*(y2 - y3) + x2*(y3 - y1) + x3*(y1 - y2)) / 2.0;
    return fabs(area);
}

// Function to check if a point is inside a quadrilateral
bool isPointInQuadrilateral(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
    // Calculate the area of the quadrilateral
    float quadArea = triangleArea(x1, y1, x2, y2, x3, y3) + triangleArea(x2, y2, x3, y3, x4, y4);

    // Calculate the area of triangles formed with the point and the vertices of the quadrilateral
    float area1 = triangleArea(x, y, x1, y1, x2, y2);
    float area2 = triangleArea(x, y, x2, y2, x4, y4);
    float area3 = triangleArea(x, y, x1, y1, x3, y3);
    float area4 = triangleArea(x, y, x3, y3, x4, y4);
    float total_area=area1+area2+area3+area4;

    // Check if the sum of the areas of the triangles is equal to the area of the quadrilateral
    if (total_area>=quadArea-1 && total_area<=quadArea+1) {
      return true;
    }
    return false;
}


void RemoveFolderRecursive(const wchar_t* dirname)
{
  _WDIR *d;
  struct _wdirent *dir;
  d = _wopendir(dirname);
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%ls/%ls",dirname,dir->d_name);
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for 
        RemoveFolderRecursive(indir);
      } else {
        _wremove(indir);
      }
    }
    _wrmdir(dirname);
  }
}


bool DirExists(const wchar_t* filename)
{
    _WDIR *d = _wopendir(filename);

    if (d) {
        // Directory exists
        //printf("Directory exists.\n");
        _wclosedir(d);
        return TRUE;
    } /*else if (ENOENT == errno) {
        // Directory does not exist
        printf("Directory does not exist.\n");
    } else {
        // opendir() failed for some other reason.
        printf("Failed to open directory.\n");
    }*/
    return FALSE;      
}



bool FileExists(const wchar_t* filename)
{
    FILE *fptr1;
    // Open one file for reading
    fptr1 = _wfopen(filename, L"r");
    if (fptr1 == NULL)
      return 0;
    fclose(fptr1);
    return TRUE;
}




//Rebounding/ricochet
/*
         Negative                                                     Positive


            /           /                                     \
           /         /                                         \
          /        /                                            \       
         /      /                                                \
        /     /                                                   \
       /   /                                                       \
      / /                                                           \
     /                                                               \


                    (Clockwise)


                    0 . M_PI_2     |       -M_PI_2 . 0
            M_PI . M_PI+M_PI_2     |       M_PI+M_PI_2. 2*M_PI       
                    Positive        |       Negative
                    --------------/m.\\---------------
                    Negative        |       Positive
                    -M_PI_2 . 0    |       0 . M_PI_2
                     M_PI_2 . M_PI         0 . M_PI_2

                        left              !left

                                            



     \          \                                                  /           /
       \        \                                                  /         /
         \       \                                                /        /
           \     \                                                /      /
             \    \                                              /     /
               \  \                                              /   /
                 \ \                                            / /
                   \                                            /
            
         Positive gradient                                  Negative Gradient
          Positive Angle                                     Negative Angle


        


   (Clockwise)
    Right Side:
        Upwards: M_PI+M_PI_2 . 2*M_PI
      Downwards: 0 . M_PI_2

    Left Side:
      Downwards: M_PI_2 . M_PI 
        Upwards: M_PI . M_PI+M_PI_2




Ascii art woo!! :D

                            ________________________
        _ _ _ _ _ _ _ _ _ _\  _/  _ _ _ _ _ _ _ _ _ |
 Ground _______________     \ ground_angle          |
                        -----------------           |
                         -- / \ /        -----------|   
                      --   /   \   j                |
                   --     /     \                   |
                --    i  /  i    \                  |
             --         /         \                 |
          --           /           \                |
       (O)2           /             \               |
    --               /               \              |
                    /                 \             |
                   /                   (O)1---------|




    O1 = Original Bullet Angle
    O2 = New Bullet Angle
    i = angle of incidence
    j = outer angle of incidence
    ground_angle = angle of ground


    Find O2

    j = 2pi - pi/2 - pi/2 -  (2pi - O1) - ground_angle
      = 2pi - pi - 2pi + O1 - ground_angle
      = -pi + O1 - ground_angle

    i = pi/2 - j
      = pi/2 - (-pi + O1 - ground_angle)
      = pi/2 + pi - O1 + ground_angle
      = 3/2 * pi - O1 + ground_angle


    O2 = ground_angle + j + i + i
       = ground_angle + j + 2i
       = ground_angle + (-pi + O1 - ground_angle) + 2*(3/2 * pi - O1 + ground_angle)
       = ground_angle -pi + O1 - ground_angle + 3pi - 2*O1 + 2*ground_angle
       = 2pi -O1 + 2*ground_angle


   *Al-Khwarizmi
*/

float GetBounceAngle(float angle1,float angle2)
{
  float ans;
  ans=2*M_PI-angle1+2*angle2;
  //if (ans>2*M_PI || ans<0) {
  if (ans>0) {
    ans=fmod(ans,(4*M_PI));
  } 

  if (ans<0) {
    ans=fmod(ans,(-4*M_PI));
  }
  //}
  /*if (ans<0) {
    ans=abs(fmod(ans,(2*M_PI)));
  }*/

  /*if (ans>2*M_PI) {
    ans-=2*M_PI;
  }
  if (ans<0) {
    ans+=2*M_PI;
  }*/

  return ans;
}



float GetMarbleAngle(float angle1,float angle2)
{
  float x1,y1,x2,y2,rx,ry,ans;
  x1=cos(angle1);
  y1=sin(angle1);
  x2=cos(angle2);
  y2=sin(angle2);
  rx=x1+x2;
  ry=y1+y2;
  ans=atan2(ry,rx);
  if (ans>0) {
    ans=fmod(ans,(2*M_PI));
  } else {
    ans=fmod(ans,(-2*M_PI));
  }
  return ans;
}


void LoadRngTable(wchar_t *rng_table_name)
{
  if (!FileExists(rng_table_name)) {
    for (int i=0;i<RAND_NUM;i++) {
      rng_table[i]=0;
    }
  } else {
    int c,row=0; //each character
    int int_val=0,int_saved_val=0;
    FILE *fptr;
    fptr = _wfopen(rng_table_name,L"r");

    while ((c=fgetwc(fptr))!=WEOF) {
      if (c!=',') {
        if (c>='0' && c<='9') {
          int_val=c-'0';
          int_saved_val*=10;
          int_saved_val+=int_val;
        }
      } else {
        rng_table[row]=int_saved_val;
        int_saved_val=0;
        row++;
      }
    } 
    fclose(fptr);
  }
}


// Function to swap two elements
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Choosing the last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// Quicksort function
void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}


