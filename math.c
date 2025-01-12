
//Solar Hijri Time for Drawing
int solar_sec=0,solar_min=0,solar_hour=0,solar_day=0,solar_month=0,solar_year=0,solar_day_of_week=0;
double solar_angle_day=0;

//Lunar Hijri Time for Drawing
int lunar_sec=0,lunar_min=0,lunar_hour=0,lunar_day=0,lunar_month=0,lunar_year=0,lunar_day_of_week=0;
double moon_angle_shift=0;


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



int64_t int64_current_timestamp() {
  //struct timeval te;
  //mingw_gettimeofday(&te, NULL);
  __time64_t t;
  _time64(&t);
  int64_t h_diff;
  int64_t m_diff;
  int64_t _l;

  get_current_time_diff(&h_diff,&m_diff,&_l);
  return (int64_t) t - (SEC_PER_HOUR*h_diff) - (SEC_PER_MIN*m_diff);
}


unsigned long long long_current_timestamp() {
  struct timeval te;
  //struct timezone tz;
  //mingw_gettimeofday(&te,&tz);2037
  mingw_gettimeofday(&te,NULL);
  return (unsigned long long) te.tv_sec;
}


int leap_years[11]={2,5,7,10,13,16,18,21,24,26,29};




char *lunar_months_en_txt[12]={
"Muharram", 
"Safar", 
"Rabi' al-Awwal",
"Rabi' al-Thani",
"Jumada al-Awwal", 
"Jumada al-Thani", 
"Rajab", 
"Sha'ban",
"Ramadan", 
"Shawwal", 
"Dhu al-Qa'dah",
"Dhu al-Hijjah"
};

char *solar_months_en_txt[12]={
"Farvardin",
"Ordibehesht",
"Khordad",
"Tir",
"Mordad/Amordad",
"Shahrivar",
"Mehr",
"Aban",
"Azar",
"Dey",
"Bahman",
"Esfand"
};



char *lunar_days_en_txt[7]={
  "yawm al-Khamis", //Thursday
  "yawm al-Jum'ah", //Friday
  "yawm as-Sabt", //Saturday
  "yawm al'-Ahdad",
  "yawm al-lthnayn",
  "yawm ath-Thulatha'",
  "yawm al-'Aribi'a'",
};

char *solar_days_en_txt[7]={
  "panjshanbeh", //Thursday
  "jom'eh",  //Friday
  "shanbeh", //Saturday
  "yekkeshanbeh", //Sunday
  "doshanbeh", //Monday
  "seshanbeh", //Tuesday
  "chaharshanbeh", //Wednesday
};
//-589 Farvardin 12/ Johmeh



//https://en.wikipedia.org/wiki/Solar_Hijri_calendar
wchar_t *solar_months_txt[12]={
L"فروردین",
L"اردیبهشت",
L"خرداد",
L"تیر",
L"مرداد/امرداد",
L"شهریور",
L"مهر",
L"آبان",
L"آذر",
L"دی",
L"بهمن",
L"اسفند/اسپند"
};

wchar_t *lunar_months_txt[12]={
   L"ٱلْمُحَرَّم",
   L"صَفَر",
   L"ربيع الأول",
   L"رَبِيع ٱلثَّانِي",
   L"جُمَادَىٰ ٱلْأُولَىٰ",
   L"جُمَادَىٰ ٱلثَّانِيَة",
   L"رَجَب",
   L"شَعْبَان",
   L"رَمَضَان",
   L"شَوَّال",
   L"ذُو ٱلْقَعْدَة",
   L"ذُو ٱلْحِجَّة"    
};

wchar_t *lunar_days_txt[7]={
  L"يوم الخميس", //Thursday
  L"يوم الجُمُعة",  //Friday
  L"يوم السَبْت", //Saturday
  L"يوم الأَحَد", //Sunday
  L"يوم الاِثْنَين", //Monday
  L"يوم الثُلاثاء", //Tuesday
  L"يوم الأَرْبِعاء", //Wednesday
};

//https://en.wikipedia.org/wiki/Shanbeh
wchar_t *solar_days_txt[7]={
    L"پنج‌شنبه", //Thursday
    L"جمعه", //Friday
    L"شنبه", //Saturday
    L"یک‌شنبه", //Sunday
    L"دوشنبه",
    L"سه‌شنبه",
    L"چهارشنبه"
};

void PersiaSolarTime(int64_t _seconds,
  int *_solar_sec,
  int *_solar_min,
  int *_solar_hour,
  int *_solar_day,
  int *_solar_month,
  int *_solar_year,
  int *_solar_day_of_week,
  double *_solar_angle_day
)
{  
  //Beginning date:
  //Gegorian
    //01-Jan-1970 THURSDAY

  //Persian
    //11-Dey-1348 <----not considering timezone offsets

  //seconds in a year
  //365 (days in a year) * 24 (hours in a day) * 60 (minutes in an hour) * 60 (seconds in a minute)
  //366 (days in a LEAP year) * 24 (hours in a day) * 60 (minutes in an hour) * 60 (seconds in a minute)

  //---X    ---X    ---X    ---X-
  const int day_seconds=60*60*24;
  const int days31_seconds=day_seconds*31;
  const int days30_seconds=day_seconds*30;
  const int days29_seconds=day_seconds*29;

  //1346 is a leap year
  int leap_year=1350;

  //Break Down the different time parts
  int solar_hijri_cycle=28; //28/33
  int year=1348;        //gregorian unix is 1970-1-1 THURSDAY
  int month=9;          //start day     //solar hijri unix date is 1348-10-11 //month 0 is month 1
  int64_t seconds_static=_seconds; //Begins on thursday solar time
  int64_t seconds=_seconds+day_seconds*10;
  while (seconds>0) {
    //Get months
    if (month<6) {   //First 6 months have 31 days          0,1,2,3,4,5
      if (seconds-days31_seconds<=0) {
        break;
      } else {
        seconds-=days31_seconds;
        month++;
      }
    } else if (month>5 && month<11) {  //6 months have 30 days      6,7,8,9,10
      if (seconds-days30_seconds<=0) {
        break;
      } else {
        seconds-=days30_seconds;
        month++;
      }
    } else { //12th month   //leap year at last month, 30 days = leap year       29 days = common year      ,11
      if (year==leap_year) {//Leap year
        if (seconds-days30_seconds<=0) {
          break;
        } else {
          seconds-=days30_seconds;
          month++;
        }
      } else {//Common Year
        if (seconds-days29_seconds<=0) {
          break;
        } else {
          seconds-=days29_seconds;
          month++;
        }
      }
    }


    //new year
    if (month==12) {
      if (year==leap_year) {
        if (solar_hijri_cycle!=17) {
          leap_year+=4;
        } else {
          leap_year+=5;
        }
      }
      month=0;
      year++;
      solar_hijri_cycle++;
      if (solar_hijri_cycle>33) {
        solar_hijri_cycle=1;
      }
    }
  }



  int64_t print_seconds=seconds%60; //60 seconds in a minute
 
  //Minutes
  int64_t min=seconds/60;
  int64_t print_min=min%60; //60 minutes in a second

  //Hours
  int64_t hours=min/60;
  int64_t print_hours=hours%24; //24 hours in a day

  //Days
  int64_t days=hours/24;


  //31 or 30 or 29 days in a month
  int print_days=0;
  if (month<6) {
    print_days=days%31;
  } else if (month>5 && month<11) {
    print_days=days%30;
  } else {
    if ((year-1346)%4==0) {
      print_days=days%30;
    } else {
      print_days=days%29;
    }
  }

  int __solar_day=0;
  if (month<6) {
    __solar_day=31*month;
  }  else if (month>5 && month<11) {
    __solar_day=31*6;
    __solar_day+=30*(month-6);
  } else {
    __solar_day=31*6;
    __solar_day+=30*5;
  }
  __solar_day+=days;
  //printf("SD~%d~\n",__solar_day);


  double __solar_angle=0;
  if (year==leap_year) {
    __solar_angle=(M_PI*2)*__solar_day/366;
  } else {
    __solar_angle=(M_PI*2)*__solar_day/365;
  }


  *_solar_year=year;
  *_solar_month=month+1;
  *_solar_day=(int)print_days+1;
  *_solar_hour=(int)print_hours;
  *_solar_min=(int)print_min;
  *_solar_sec=(int)print_seconds;
  *_solar_day_of_week=seconds_static/SEC_PER_DAY%7;
  *_solar_angle_day=__solar_angle;
}


//   *Al-Khwarizmi
void PersiaLunarTime(int64_t _seconds,
  int *_lunar_sec,
  int *_lunar_min,
  int *_lunar_hour,
  int *_lunar_day,
  int *_lunar_month,
  int *_lunar_year,
  int *_lunar_day_of_week,
  double *_moon_angle_shift
)
{
  //622 C.E. = Beginning of Year


  //Each year has 12 months
    //odd number months have 30 days
    //even number months have 29 days
    //At the last 12th months,
      //30 days during leap year
      //29 days during common year

  const int day_seconds=60*60*24;
  const int days30_seconds=day_seconds*30;
  const int days29_seconds=day_seconds*29; 
  int64_t lunar_day_start=-day_seconds*21; //lunar hijri unix start day is  1389-10-22 //Gregorian is 1970-1-1 //1 day offset, account for day start at evening

  //Break Down the different time parts
  int year=1389;
  int month=9;          //lunar hijri unix start day is  1389-10-22  //Gregorian is 1970-1-1 //month 0 is month 1
  int64_t seconds=_seconds+day_seconds*21;
  int64_t seconds_static=_seconds; //Begins on thursday

  while (seconds>0) {
    //Get months
    if (month<11) {   //0,1,2,3,4,5
      if ((month+1)%2==0) { //29 Days, Even number months
        if (seconds-days29_seconds<=0) {
          break;
        } else {
          seconds-=days29_seconds;
          lunar_day_start+=days29_seconds;
          month++;
        }        
      } else { //30 Days, Odd Number months
        if (seconds-days30_seconds<=0) {
          break;
        } else {
          seconds-=days30_seconds;
          lunar_day_start+=days30_seconds;
          month++;
        }
      }
    } else { //12th month   //leap year at last month, 30 days = leap year       29 days = common year      ,11
      //30 yrs, 11 leap years
      //2,5,7,10,13,16,18,21,24,26,29
      bool leap=FALSE;
      int lyr=year%30;
      for (int i=0;i<11;i++) {
        if (lyr==leap_years[i]) {
          leap=TRUE;
          break;
        }
      }

      if (leap) {//Leap year
        if (seconds-days30_seconds<=0) {
          break;
        } else {
          seconds-=days30_seconds;
          lunar_day_start+=days30_seconds;
          month++;
        }
      } else {//Common Year
        if (seconds-days29_seconds<=0) {
          break;
        } else {
          seconds-=days29_seconds;
          lunar_day_start+=days29_seconds;
          month++;
        }
      }
    }


    //new year
    if (month==12) {
      month=0;
      year++;
    }    
  }
  
  //::
  //Get Seconds, Minutes, Hours and Days
  int64_t print_seconds=seconds%60; //60 seconds in a minute
  //Minutes
  int64_t min=seconds/60;
  int64_t print_min=min%60; //60 minutes in a second
  //Hours
  int64_t hours=min/60;
  int64_t print_hours=hours%24; //24 hours in a day
  //Days
  int64_t days=hours/24;


  //31 or 30 or 29 days in a month
  bool leap=FALSE;
  int print_days=0;
  if (month<11) {
    if ((month+1)%2==0) { //Even number months, 29 days
      print_days=days%29;
    } else { //Odd number months
      print_days=days%30;
    }
  } else { //Leap Year
    int lyr=year%30;
    for (int i=0;i<11;i++) {
      if (lyr==leap_years[i]) {
        leap=TRUE;
        break;
      }
    }

    if (leap) {
      print_days=days%30;
    } else {
      print_days=days%29;
    }
  }

  int _;
  if (print_days+1>=29) { //new moon
    lunar_day_start+=(print_days*day_seconds);
  }

  double moon_angle_shift=0;
  PersiaSolarTime(lunar_day_start,&_,&_,&_,&_,&_,&_,&_,&moon_angle_shift);
  
  if (print_days+1==28) //new moon
    moon_angle_shift+=2*M_PI/27;

  //Assign to variables
  *_lunar_year=year;
  *_lunar_month=month+1;
  *_lunar_day=(int)print_days+1;
  *_lunar_hour=(int)print_hours;
  *_lunar_min=(int)print_min;
  *_lunar_sec=(int)print_seconds;
  *_lunar_day_of_week=seconds_static/SEC_PER_DAY%7;
  *_moon_angle_shift=moon_angle_shift;
}


//Maths
//public_seed = rand() in WM_PAINT
int RandNum(int min, int max,int seed) {
  //srand(int_current_timestamp());
  int m=0;
  if (min<0) {
    m=min-1;
  }
  return (rand()*seed)%(max-m + 1) + m;
}



double RandAngle(int min, int max, int seed) {
  return RandNum(min,max,seed)*M_PI/180;
}


//----------------------------------------
double GetGradient(double x1,double y1,double x2,double y2) 
{
  // gradient = rise/run, m = (y1-y2)/(x1-x2)
  return (y2-y1)/(x2-x1);
}

double GetGroundC(double x,double y,double gradient)
{
  //y=mx+c, c = y-mx
  return y-gradient*x; 
}

double GetX(double y,double gradient,double c)
{
  return (y-c)/gradient;
}


double GetY(double x,double gradient,double c)
{
  return x*gradient+c;
}

double GetDistance(double x1,double y1,double x2,double y2)
{
  double dist=sqrt(pow(y2-y1,2)+pow(x2-x1,2));
  return dist;
}

double GetSinAngle(double height,double length)
{
  return asin(height/length);
}

double GetCosAngle(double vertical_distance,double length)
{
  return acos(vertical_distance/length);
}

double GetRotateAngle(double x,double y) 
{
  double adj=0,hypo=0,E=0;
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
bool IsOutOfBounds(double x,double y,int r,int max_width,int max_height)
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


double LimitValue(double value,double min,double max)
{
  double num=value;
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
 //w=5
 //id=19
 //ans: 3 + 1
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
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for 
        RemoveFolderRecursive(indir);
      } else {
        _wremove(indir);
      }
    }
    _wrmdir(dirname);
  }
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

double GetBounceAngle(double angle1,double angle2)
{
  double ans;
  ans=2*M_PI-angle1+2*angle2;
  //if (ans>2*M_PI || ans<0) {
  if (ans>0) {
    ans=fmod(ans,(2*M_PI));
  } else {
    ans=fmod(ans,(-2*M_PI));
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



double GetMarbleAngle(double angle1,double angle2)
{
  double x1,y1,x2,y2,rx,ry,ans;
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

