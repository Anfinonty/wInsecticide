


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

int64_t int64_current_timestamp() {
  struct timeval te;
  mingw_gettimeofday(&te, NULL);
  return (int64_t) te.tv_sec;
}


unsigned long long long_current_timestamp() {
  struct timeval te;
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
  "shanbeh",
  "yekkeshanbeh",
  "doshanbeh",
  "seshanbeh",
  "chaharshanbeh",
  "panjshanbeh",
  "jom'eh"
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
    //01-Jan-1970

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


  //Break Down the different time parts
  int year=1348;
  int month=9;          //start day     //solar hijri offset: 8 hours from 1970-1-1 Gregorian
  int64_t seconds=_seconds+day_seconds*11+8*60*60; //Offsets
  while (seconds>0) {
    //Get months
    if (month<6) {   //First 6 months have 31 days          0,1,2,3,4,5
      if (seconds-days31_seconds<0) {
        break;
      } else {
        seconds-=days31_seconds;
        month++;
      }
    } else if (month>5 && month<11) {  //6 months have 30 days      6,7,8,9,10
      if (seconds-days30_seconds<0) {
        break;
      } else {
        seconds-=days30_seconds;
        month++;
      }
    } else { //12th month   //leap year at last month, 30 days = leap year       29 days = common year      ,11
      if (year%4==0) {//Leap year
        if (seconds-days30_seconds<0) {
          break;
        } else {
          seconds-=days30_seconds;
          month++;
        }
      } else {//Common Year
        if (seconds-days29_seconds<0) {
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
    if (year%4==0) {
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
  if (year%4==0) {
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
  *_solar_day_of_week=(days+3)%7;
  *_solar_angle_day=__solar_angle;
}



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
  int64_t lunar_day_start=-(day_seconds*22-16*60*60);

  //Break Down the different time parts
  int year=1389;
  int month=9;          //start day is  1389-10-22    //lunar hijri hours offset from 1970-1-1 gregorian is 16 hrs
  int64_t seconds=_seconds+day_seconds*22-16*60*60; //Offset

  while (seconds>0) {
    //Get months
    if (month<11) {   //0,1,2,3,4,5
      if ((month+1)%2==0) { //29 Days, Even number months
        if (seconds-days29_seconds<0) {
          break;
        } else {
          seconds-=days29_seconds;
          lunar_day_start+=days29_seconds;
          month++;
        }        
      } else { //30 Days, Odd Number months
        if (seconds-days30_seconds<0) {
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
        if (seconds-days30_seconds<0) {
          break;
        } else {
          seconds-=days30_seconds;
          lunar_day_start+=days30_seconds;
          month++;
        }
      } else {//Common Year
        if (seconds-days29_seconds<0) {
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
    if ((month+1)%2==0) { //Even number months
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
  *_lunar_day_of_week=days%7+1;
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


