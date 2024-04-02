


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
//  double tnow = int64_current_timestamp();/*-SEC_PER_HOUR*16;*/
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

void get_current_time_diff(int *hour_diff,int *min_diff, int *sec_diff)
{ //https://stackoverflow.com/questions/43732241/how-to-get-datetime-from-gettimeofday-in-c
  SYSTEMTIME st,lt;
  GetLocalTime(&lt);
  GetSystemTime(&st);

  int lhour= lt.wHour;
  int lmin= lt.wMinute;
  int lsec= lt.wSecond;

  int shour= st.wHour;
  int smin= st.wMinute;
  int ssec= st.wSecond;

  *hour_diff=shour-lhour;
  *min_diff=smin-lmin;
  *sec_diff=ssec-lsec;
}



double double_current_timestamp() {
  //struct timeval te;
  //mingw_gettimeofday(&te, NULL);
  time_t t;
  time(&t);
  int h_diff;
  int m_diff;
  int _l;

  get_current_time_diff(&h_diff,&m_diff,&_l);
  return (double) t - (SEC_PER_HOUR*h_diff) - (SEC_PER_MIN*m_diff);
}


unsigned long long long_current_timestamp() {
  struct timeval te;
  //struct timezone tz;
  //mingw_gettimeofday(&te,&tz);
  mingw_gettimeofday(&te,NULL);
  return (unsigned long long) te.tv_sec;
}



int leap_years[11]={2,5,7,10,13,16,18,21,24,26,29};
/*char *lunar_months_txt[12]={
    "ٱلْمُحَرَّم",
    "صَفَر",
    "رَبِيع ٱلْأَوَّل",
    "رَبِيع ٱلثَّانِي",
    "جُمَادَىٰ ٱلْأُولَىٰ",
    "جُمَادَىٰ ٱلثَّانِيَة",
    "رَجَب",
    "شَعْبَان",
    "رَمَضَان",
    "شَوَّال",
    "ذُو ٱلْقَعْدَة",
    "ذُو ٱلْحِجَّة"    
};*/



char *lunar_months_txt[12]={
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

char *solar_months_txt[12]={
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

char *lunar_days_txt[7]={
  "yawm al'-Ahdad",
  "yawm al-lthnayn",
  "yawm ath-Thulatha'",
  "yawm al-'Aribi'a'",
  "yawm al-Khamis",
  "yawm al-Jum'ah",
  "yawm as-Sabt"
};

char *solar_days_txt[7]={
  "yekkeshanbeh",
  "doshanbeh",
  "seshanbeh",
  "chaharshanbeh",
  "panjshanbeh",
  "jom'eh",
  "shanbeh",
};
//-589 Farvardin 12/ Johmeh


void PersiaSolarTime(double _seconds,
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

  //Break Down the different time parts
  //int count_year_leap=2;
  int year=1348;        //gregorian unix is 1970-1-1 THURSDAY
  int month=9;          //start day     //solar hijri unix date is 1348-10-11 //month 0 is month 1
  double seconds_static=_seconds+day_seconds*3; //Begins on thursday solar time
  double seconds=_seconds+day_seconds*11;
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
      if (fmod((year-1346),4)==0) {//Leap year
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
      month=0;
      year++;
    }
  }



  double print_seconds=fmod(seconds,60); //60 seconds in a minute
 
  //Minutes
  double min=seconds/60;
  double print_min=fmod(min,60); //60 minutes in a second

  //Hours
  double hours=min/60;
  double print_hours=fmod(hours,24); //24 hours in a day

  //Days
  double days=hours/24;


  //31 or 30 or 29 days in a month
  int print_days=0;
  if (month<6) {
    print_days=fmod(days,31);
  } else if (month>5 && month<11) {
    print_days=fmod(days,30);
  } else {
    if (fmod((year-1346),4)==0) {
      print_days=fmod(days,30);
    } else {
      print_days=fmod(days,29);
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
  if (fmod((year-1346),4)==0) {
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
  *_solar_day_of_week=fmod(seconds_static/SEC_PER_DAY,7);
  *_solar_angle_day=__solar_angle;
}



void PersiaLunarTime(double _seconds,
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
  double lunar_day_start=-day_seconds*21; //lunar hijri unix start day is  1389-10-22 //Gregorian is 1970-1-1 //1 day offset, account for day start at evening

  //Break Down the different time parts
  int year=1389;
  int month=9;          //lunar hijri unix start day is  1389-10-22  //Gregorian is 1970-1-1 //month 0 is month 1
  double seconds=_seconds+day_seconds*21;
  double seconds_static=_seconds+day_seconds*3; //Begins on thursday

  while (seconds>0) {
    //Get months
    if (month<11) {   //0,1,2,3,4,5
      if (fmod((month+1),2)==0) { //29 Days, Even number months
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
  double print_seconds=fmod(seconds,60); //60 seconds in a minute
  //Minutes
  double min=seconds/60;
  double print_min=fmod(min,60); //60 minutes in a second
  //Hours
  double hours=min/60;
  double print_hours=fmod(hours,24); //24 hours in a day
  //Days
  double days=hours/24;


  //31 or 30 or 29 days in a month
  bool leap=FALSE;
  int print_days=0;
  if (month<11) {
    if (fmod((month+1),2)==0) { //Even number months, 29 days
      print_days=fmod(days,29);
    } else { //Odd number months
      print_days=fmod(days,30);
    }
  } else { //Leap Year
    int lyr=fmod(year,30);
    for (int i=0;i<11;i++) {
      if (lyr==leap_years[i]) {
        leap=TRUE;
        break;
      }
    }

    if (leap) {
      print_days=fmod(days,30);
    } else {
      print_days=fmod(days,29);
    }
  }

  int _start_solar_day=0;
  int _start_solar_year=0;
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
  *_lunar_day_of_week=fmod(seconds_static/SEC_PER_DAY,7);
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


