
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



int leap_years[11]={2,5,7,10,13,16,18,21,24,26,29}; //Kuwaiti Algorithm, Tabular algortihm also by Al-Khwarizmi
//https://en.wikipedia.org/wiki/Tabular_Islamic_calendar
//int leap_years[11]={2,5,8,11,13,16,19,21,24,27,0}; //Habash al-Hasib last nuber is 30 but modulo %30 is 0




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




int64_t GetLunarHijriDays(int day0,int month0, int64_t year0)
{
  int64_t year_i=1389;
  int month=10;          //lunar hijri unix start day is  1389-10-22  //Gregorian is 1970-1-1
  int64_t lh_days=0;

  if (year0<1389) { //less than jan 1 1970, count day backwards
    lh_days+=21; //-1 day epoch for direct days to work
    month--;    
    while (month>0) { // 9, 8 , 7, 6,  |5, 4, 3, 2, 1
      if (month%2==0)
        lh_days+=29;
      else
        lh_days+=30; 
      month--;
    }
    year_i--;
 // years inbetween
    while (year_i>year0) {    
      //check leap year 
      bool leap=FALSE;
      int lyr=year_i%30;
      for (int i=0;i<11;i++) {
        if (abs(lyr)==leap_years[i]) {
          leap=TRUE;
          break;
        }
      }
      lh_days+=30*6 + 29*5;
      if (leap) {                                                    // x | x | x | x | x |  x
        lh_days+=30;          //odd = 30 *6, even= 29*5, leap=29/30,,,, 1,2,3,4,5,6,7,8,9,10,11
      } else {
        lh_days+=29;
      }
      year_i--;
    }

    // final year
    month=12;
    //check leap year 
    bool leap=FALSE;
    int lyr=year_i%30;
    for (int i=0;i<11;i++) {
      if (abs(lyr)==leap_years[i]) {
        leap=TRUE;
        break;
      }
    }
    if (leap) {                                                    // x | x | x | x | x |  x
      lh_days+=30;          //odd = 30 *6, even= 29*5, leap=29/30,,,, 1,2,3,4,5,6,7,8,9,10,11
    } else {
      lh_days+=29;
    }
    month--;

    //check months
    while (month>month0) { //11, 10, 9, 8 , 7, 6,  |5, 4, 3, 2, 1
      if (month%2==0) //even 29, odd 30
        lh_days+=29;
      else
        lh_days+=30; 
      month--;
    }

    if (month0%2==0) { //1 day epoch, day 0 isnt a thing i.e. 29-day0 where day0 is 29
      lh_days+=30-day0;
    } else {
      lh_days+=31-day0;
    }    
    //
  } else {  //count forwards (TBA)

  }
  return lh_days;
}

int64_t GetSolarHijriDays(int64_t day0,int64_t month0,int64_t year0)
{
  //1348-10-11 
  int solar_hijri_cycle=28;
  int month=10;
  int64_t sh_days=0;
  int64_t year_i=1348;
  int leap_year=1350;
  //1348-10-11 , 1348 Dey 11 is Jan 01 1970 aka start of unix
  if (year0<1348) { //count day backwards
    leap_year=1346;
    // current year, 10
    sh_days+=12; //reach new month //+epoch 1 day for direct days calc to work
    month--;
    while (month>0) { // 9, 8 , 7, 6,  |5, 4, 3, 2, 1
      if (month>5)
        sh_days+=31;
      else
        sh_days+=30; 
      month--;
    }
    
    year_i--;
    solar_hijri_cycle--;

    // years inbetween
    while (year_i>year0) {     
      if (year_i==leap_year) {
        if (solar_hijri_cycle!=17) {
          leap_year-=4;
        } else {
          leap_year-=5;
        }
        sh_days+=31*6+ 30*5 +30;
      } else {
        sh_days+=31*6+ 30*5 +29;
      }

      year_i--;
      solar_hijri_cycle--;
      if (solar_hijri_cycle<1) {
        solar_hijri_cycle=33;
      }
    }

    // final year
    month=12;
    if (year_i==leap_year) {
      sh_days+=30;
    } else {
      sh_days+=29;
    }
    month--;


    while (month>month0) { //11, 10, 9, 8 , 7, |6, 5, 4, 3, 2, 1
      if (month>=7)
        sh_days+=30;
      else
        sh_days+=31; 
      month--;
    }

    //final day
    if (month0>=7 && month0<=12) {
      sh_days+=31-day0;
    } else if (month0>=1 && month0>=6){ //first 6 months
      sh_days+=32-day0;
    }


  } else { //count solar hijri day forwards
    //leap_year=1350;
    // current year, 10
    sh_days+=(30-10);
    month++;
    while (month<12) {
      if (month<=6)
        sh_days+=31;
      else
        sh_days+=30; 
      month++;
    }
    //non-leapyear
    sh_days+=29;    
    year_i++;
    solar_hijri_cycle++;

    // years inbetween
    while (year_i<year0) {     
      if (year_i==leap_year) {
        if (solar_hijri_cycle!=17) {
          leap_year+=4;
        } else {
          leap_year+=5;
        }
        sh_days+=31*6+ 30*5+ 30;
      } else {
        sh_days+=31*6+ 30*5+ 29;
      }

      year_i++;
      solar_hijri_cycle++;
      if (solar_hijri_cycle>33) {
        solar_hijri_cycle=1;
      }
    }

    // final year
    month=1;
    while (month<month0) { //1, 2, 3, 4, 5, 6 | 7, 8, 9 ,10 ,11 ,12
      if (month<=6)
        sh_days+=31;
      else
        sh_days+=30; 
      month++;
    }

    sh_days+=day0; //day at target month

  }
  return sh_days;
}




/*struct SolarHijri
{
  int sec;
  int min;
  int hour;

  int day;
  int month;
  int64_t year;

  int solar_day_of_week;

  int64_t total_days;
  bool solar_leap_year;
  bool last_year_is_leap;
} SolarHijri;*/

//   *Omar Khayyam

void PersiaSolarTime(int64_t _seconds,
  int64_t *_solar_sec,
  int64_t *_solar_min,
  int64_t *_solar_hour,
  int64_t *_solar_day,
  int64_t *_solar_month,
  int64_t *_solar_year,
  int64_t *_solar_day_of_week,
  float *_solar_angle_day//,
//  bool *_solar_leap_year,
//  bool *_solar_last_year_is_leap,
//  int64_t *total_solar_hijri_days
)
{  
  //https://en.wikipedia.org/wiki/Solar_Hijri_calendar
  //I am sticking with the traditional 33 year cycle by Omar Khayyam as the
  //proposed new system by Birashk and Zabih Behruz are refuted
    //https://web.archive.org/web/20160224162520/http://aramis.obspm.fr/~heydari/divers/ir-cal-eng.html
    //https://iranologia.es/en/2020/09/23/a-concise-review-of-the-iranian-calendar/
  //expect to see small day drifts after 10k centuries from results online
  //Also it'll be less complex for me haha

  //2025-12-23 it has be revealed to me that the 20,33,33,33,37 cycle was erronously guessed by an individual
  //who was extremely passionate about pre-Islamic Iran - Behris Zabih or Zabih Behruz
  //https://www.iranicaonline.org/articles/behruz-dabih-1889-1971-persian-satirist-son-of-the-physician-and-calligrapher-abul-fazl-savaji/
  //Birashk's leap year proposal is based on that and it has been refuted.

  //2026-01-01 Most online resources use the code seen in this website (viewed using Ctrl-S as .html and Save Page As...)
  //https://www.muqawwim.com/
  //https://www.fourmilab.ch/documents/calendar/
  //It does not use the 33-year cycle leading to some days to misalign with my results many years later
  //The logic used here is consistent with no magic number epochs, any instances of them you may interpret it as so is to
  //allow for the printf be correct but mathematically it is consistent.


  //https://web.archive.org/web/20160318112553/http://payvand.com/calendar/ 
  //(not working though)

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
/*
https://en.wikipedia.org/wiki/Solar_Hijri_calendar

26   1346* 
27   1367
28   1348 <--------- starting year
29   1349
30   1350*
31   1351
32   1352
33   1353

1    1354 (-4)
*/
  //1346 is a leap year
  int leap_year=1350;

  //Break Down the different time parts
  int solar_hijri_cycle=28; 
  int year=1348;        //gregorian unix is 1970-1-1 THURSDAY
  int month=10;          //start day     //solar hijri unix date is 1348-10-11
  int64_t seconds_static=_seconds; //Begins on thursday solar time
  int64_t seconds=_seconds+day_seconds*11; // 11 days offset, move to next month

  int64_t _total_solar_hijri_days=0;
  if (_seconds>0) { //positive UNIX time
  while (seconds>0) {
    //Get months
    if (month>=1 && month<=6) {   //First 6 months have 31 days          1,2,3,4,5
      if (seconds-days31_seconds<=0) {
        break;
      } else {
        seconds-=days31_seconds;
        month++;
        _total_solar_hijri_days+=31;
      }
    } else if (month>=7 && month<=11) {  //last 5 months have 30 days      7,8,9,10,11
      if (seconds-days30_seconds<=0) {
        break;
      } else {
        seconds-=days30_seconds;
        month++;
        _total_solar_hijri_days+=30;
      }
    } else { //12th month   //leap year at last month, 30 days = leap year       29 days = common year      ,12
      if (year==leap_year) {//Leap year
        if (seconds-days30_seconds<=0) {
          break;
        } else {
          seconds-=days30_seconds;
          month++;
          _total_solar_hijri_days+=30;
        }
      } else {//Common Year
        if (seconds-days29_seconds<=0) {
          break;
        } else {
          seconds-=days29_seconds;
          month++;
          _total_solar_hijri_days+=29;
        }
      }
    }


    //new year
    if (month==13) {
      if (year==leap_year) {
        if (solar_hijri_cycle!=17) {
          leap_year+=4;
        } else {
          leap_year+=5;
        }
      }
      month=1;
      year++;
      solar_hijri_cycle++;
      if (solar_hijri_cycle>33) {
        solar_hijri_cycle=1;
      }
    }
  }

  //negative unix time
  } else if (_seconds<0) {
    leap_year=1346;
    seconds_static=_seconds+day_seconds;

    if (abs(_seconds)/day_seconds>10) { //1348 // Dey (10th month, 30 days)// 11  1348-10-11
      seconds=_seconds+day_seconds*10; // 10 days offset, move to 9th month
      month--;
      _total_solar_hijri_days+=10;
    } else {
      seconds=-abs( (30-10)*day_seconds -_seconds); //30 days in 10th month
    }

    while (seconds<0) {
    //Get months
    if (month<=6 && month>=1) {   //First 6 months have 31 days          
      if (seconds+days31_seconds>=0) {
        break;
      } else {
        seconds+=days31_seconds;
        month--;
        _total_solar_hijri_days+=31;
      }
    } else if (month>=7 && month<=11) {  //last 5 months have 30 days
      if (seconds+days30_seconds>=0) {
        break;
      } else {
        seconds+=days30_seconds;
        month--;
        _total_solar_hijri_days+=30;
      }
    } else { //12th month  //leap year at last month, 30 days = leap year       29 days = common year      ,12
      if (year==leap_year) {//Leap year
        if (seconds+days30_seconds>=0) {
          break;
        } else {
          seconds+=days30_seconds;
          month--;//month=11
          _total_solar_hijri_days+=30;
        }
      } else {//Common Year
        if (seconds+days29_seconds>=0) {
          break;
        } else {
          seconds+=days29_seconds;
          month--; //month=11
          _total_solar_hijri_days+=29;
        }
      }
    }

    //new year
    if (month==0) {
      if (year==leap_year) {
        if (solar_hijri_cycle!=17) {
          leap_year-=4;
        } else {
          leap_year-=5;
        }
      }
      year--;
      month=12;
      solar_hijri_cycle--;
      if (solar_hijri_cycle<1) {
        solar_hijri_cycle=33;
      }
    } //end of month
  } //end of while loop
  } //end of else if + or - seconds

  seconds=abs(seconds);

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
  if (_seconds>0) {
    if (month<=6) {
      print_days=days%32;
    } else if (month>=7 && month<=11) {
      print_days=days%31;
    } else { //print leap year
      //if ((year-1346)%4==0) {
      if (year==leap_year) {
        print_days=days%31;
      } else {
        print_days=days%30;
      }
    }
  } else if (_seconds<0) { //1 day epoch day 0 not existing
    if (month>=1 && month<=6) {
      print_days=32-days;
    } else if (month>=7 && month<=11) {
      print_days=31-days;
    } else { //print leap year        
      if (year==leap_year) {
        print_days=31-days;
      } else {
        print_days=30-days;
      }
    }
  }
 
  //int64_t lol=(int64_t) print_days;
  //printf("\nprintdays: %d,%lld\n",days,lol);
  //printf("total days: %lld\n", _total_solar_hijri_days+lol);


  //misc
  //Check this year's solar_day since 1 Farvadin
  int __solar_day=0;
  if (month>=1 && month<=6) {
    __solar_day=31*(month-1);
  }  else if (month>=7 && month<=11) {
    __solar_day=31*6;
    __solar_day+=30*(month-1-6);
  } else {
    __solar_day=31*6;
    __solar_day+=30*5;
  }
  __solar_day+=days;
  _total_solar_hijri_days+=__solar_day;
  //printf("SD~%d~\n",__solar_day);

  //*total_solar_hijri_days=_total_solar_hijri_days;

  bool _is_solar_leap_year=FALSE;
  float __solar_angle=0;
  if (year==leap_year) { //this year is a leap year
    _is_solar_leap_year=TRUE;
    __solar_angle=(M_PI*2)*(__solar_day)/366;
  } else {
    __solar_angle=(M_PI*2)*(__solar_day)/365;
  }

  //if (year-1==leap_year) {//check if last year is a leap year
    //*_solar_last_year_is_leap=TRUE;
  //} else {
    //*_solar_last_year_is_leap=FALSE;
  //}

  *_solar_year=year;
  *_solar_month=abs(month);
  *_solar_day=(int)print_days;
  *_solar_hour=(int)print_hours;
  *_solar_min=(int)print_min;
  *_solar_sec=(int)print_seconds;
  if (_seconds>0)
    *_solar_day_of_week=abs(seconds_static/SEC_PER_DAY%7);
  else 
    *_solar_day_of_week=6-abs(seconds_static/SEC_PER_DAY%7);



  *_solar_angle_day=__solar_angle;
  //*_solar_leap_year=_is_solar_leap_year;
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
  float *_moon_angle_shift,
  bool *_lunar_leap_year
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
  int64_t lunar_day_start=-day_seconds*21; //lunar hijri unix start day is  1389-10-22 //Gregorian is 1970-1-1

  //Break Down the different time parts
  int year=1389;
  int month=10;          //lunar hijri unix start day is  1389-10-22  //Gregorian is 1970-1-1
  int64_t seconds=_seconds+day_seconds*22;
  int64_t seconds_static=_seconds+(60*30); //Begins on thursday


  if (_seconds>0) {
  while (seconds>0) { //positive seconds
    //Get months
    if (month<12) {   //0,1,2,3,4,5
      if ((month)%2==0) { //29 Days, Even number months
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
        if (abs(lyr)==leap_years[i]) {
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
    if (month==13) {
      month=1;
      year++;
    }    
  }
  }else if (_seconds<0) { //before 1970 jan 1
  //JAN-01-1970 IS 22-10(SHAWWAL)-1389                             1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17  18  19  20  21  22  23               
  //EPOCH 22-10-1389 IS 01-JAN-2027, MAX SHAWAL DAYS IS 29       (22,21,20,19,18,17,18,15,14,13,12,11,10,9, 8, 7, 6 ,  5,  4,  3,  2,  1,  30, 29, 28, 27)

  lunar_day_start=-day_seconds*(29-21); //lunar hijri unix start day is  1389-10-22 //Gregorian is 1970-1-1

  //22 days to move 22-10-1389 to 30-09-1389
  if (abs(_seconds/day_seconds) > 21) { //post starting month date
    seconds=_seconds+day_seconds*21;
    month--; //shift from shawwal (10) to start from rammadan (9)
  } else { //inside starting month date
    seconds=abs(-day_seconds*(29-21)+_seconds);  //29-22 = 22 -> 29 = 7 days, 29 days in shawall (even number month) 
    //printf("\nless than 22\n");
  }

  while (seconds<0) {
    //Get months
    if (month<12) {   //0,1,2,3,4,5
      if ((month)%2==0) { //29 Days, Even number months
        if (seconds+days29_seconds>=0) {
          break;
        } else {
          seconds+=days29_seconds;
          lunar_day_start-=days29_seconds;
          month--;
        }        
      } else { //30 Days, Odd Number months
        if (seconds+days30_seconds>=0) {
          break;
        } else {
          seconds+=days30_seconds;
          lunar_day_start-=days30_seconds;
          month--;
        }
      }
    } else { //12th month   //leap year at last month, 30 days = leap year       29 days = common year      ,11
      //30 yrs, 11 leap years
      //2,5,7,10,13,16,18,21,24,26,29
      bool leap=FALSE;
      int lyr=year%30;
      for (int i=0;i<11;i++) {
        if (abs(lyr)==leap_years[i]) {
          leap=TRUE;
          break;
        }
      }

      if (leap) {//Leap year
        if (seconds+days30_seconds>=0) {
          break;
        } else {
          seconds+=days30_seconds;
          lunar_day_start-=days30_seconds;
          month--;
        }
      } else {//Common Year
        if (seconds+days29_seconds>=0) {
          break;
        } else {
          seconds+=days29_seconds;
          lunar_day_start-=days29_seconds;
          month--;
        }
      }
    }


    //new year
    if (month==0) {
      month=12;
      year--;
    }    
  } //end of while


  //change to positive
  seconds=abs(seconds);

  //Muhammed's (PBUH) Son, Ibrahim, passed away on 10AH Shawwal 29,  
  //Solar Eclipse Notes:
  //https://www.sid.ir/FileServer/JE/12392000603
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
  if (month<12) {

    if (_seconds>0) {
      if ((month)%2==0) { //Even number months, 29 days
        print_days=days%30; //1 day epoch
      } else { //Odd number months
        print_days=days%31; //1 day epoch
      }
    } else { //negative unix time
      if ((month)%2==0) { //Even number months, 29 days
        print_days=30-days; //1day epoch
      } else { //Odd number months
        print_days=31-days;//1day epoch
      }
    }

  } else { //Leap Year
    int lyr=year%30;
    for (int i=0;i<11;i++) {
      if (lyr==leap_years[i]) {
        leap=TRUE;
        break;
      }
    }

    if (_seconds>0) {
      if (leap) {
        print_days=days%31;//1day epoch from 30
      } else {
        print_days=days%30; //1day epoch from 29
      }
    } else { //negative unix time
      if (leap) {
        print_days=31-days; //1day epoch from 30
      } else {
        print_days=30-days;//1day epoch from 29
      }
    }
  }

  int64_t _;
  bool b_;
  if (print_days>=28) { //new moon
    lunar_day_start+=(print_days*day_seconds);
  }

  float moon_angle_shift=0;
  PersiaSolarTime(lunar_day_start,&_,&_,&_,&_,&_,&_,&_,&moon_angle_shift);//,&b_,&b_,&_);
  
  if (print_days>=28) //new moon
    moon_angle_shift-=2*M_PI/27;


  //Assign to variables
  *_lunar_year=year;
  *_lunar_month=abs(month);
  *_lunar_day=(int)print_days;
  *_lunar_hour=(int)print_hours;
  *_lunar_min=(int)print_min;
  *_lunar_sec=(int)print_seconds;
  if (_seconds>0)
    *_lunar_day_of_week=abs(seconds_static/SEC_PER_DAY%7);
  else
    *_lunar_day_of_week=6-abs(seconds_static/SEC_PER_DAY%7);

  *_moon_angle_shift=moon_angle_shift;
  *_lunar_leap_year=leap;
}




/***
JAN 1 ----> JAN 31
FEB 1 ----> FEB 28 (normal) or 29 (leap year)       ESFAND      
MAR 1 ----> MAR 19/20/21                            FAVRADIN    1
***/

// sun: compute sunrise/sunset times
// (Note: reference only)
// https://github.com/oreparaz/sun
// (c) 2022 Oscar Reparaz <firstname.lastname@esat.kuleuven.be>

typedef struct sun_ctx_t {
    double in_latitude;
    double in_longitude;
    double out_sunrise_mins; // minutes after midnight (UTC)
    double out_sunset_mins;
    int in_yday;
    int in_hour;
} sun_ctx_t;



typedef struct Earth
{
  double orbital_eccentricity;
  double perihelion;
  double axial_tilt;  
} Earth;


Earth planet_earth;


#define arcsec2rad(arcsec)    ((arcsec*(M_PI/648000.0)))
#define deg2rad(deg) ((deg) * (M_PI / 180.0))
#define rad2deg(rad) ((rad) * (180.0 / M_PI))

void sun_compute(sun_ctx_t *ctx, Earth *_earth,int solar_hijri_day, int solar_hijri_month, int64_t solar_hijri_year)
{
  /*
  Simple Calculation, features a shifting perihelion,  although not 100% accurate as it does not account for the other factors
  that would only be of concern 20 generations later.
  */


    int64_t last_yalda_night=GetSolarHijriDays(1,10,solar_hijri_year-1); //Last Year Yalda Night (Winter Solstice)
    int64_t this_yalda_night=GetSolarHijriDays(1,10,solar_hijri_year);  //Last Year Yalda Night (Winter Solstice)

    int64_t solar_hijri_days=GetSolarHijriDays(solar_hijri_day,solar_hijri_month,solar_hijri_year)+1; //Total Number of Solar Hijri Days Today 
    //(+1 offset as astronomical calcs start with 1)
    
    //earth orbits 360 degs or 2pi radians exactly 365.25 years 
    double A;

    //Days since last Winter Solstice * 360degs/DaysPerYear,,,  Days Since Yalda Night, Dey 1
    if (solar_hijri_days<this_yalda_night) { //include last year yalda night until today
      A = (solar_hijri_days-last_yalda_night)* 2*M_PI/365.25;
    } else { //
      A = (solar_hijri_days-this_yalda_night)* 2*M_PI/365.25;
    }



    //Alternative Calculation
    //https://en.wikipedia.org/wiki/Equation_of_time    

    //~~ https://web.archive.org/web/20120323231813/http://www.green-life-innovators.org/tiki-index.php?page=The%2BLatitude%2Band%2BLongitude%2Bof%2Bthe%2BSun%2Bby%2BDavid%2BWilliams
    //https://www.youtube.com/user/GreenLifeInnovators

    //original: A = (D+9)*n where 9 is Days since winter-solstice and D is days since Jan 1
    //(D (Jan 1)-> D-9 (Winter solstice Date))

    /*
      0.0167 is the Earth's orbital eccentricity

      0.0167 * 360degs/PI * sin((D-12)*n) //12 Days after Winter Solstice is Perihelion
        =1.914degs*sin((D-12)*n)
        =0.033405602 RAD * sin((D-12)*n)

      B is the angle the Earth moves from the Winter solstice to date D
    */
    //original: B = A + 1.1914degs*sin((D-3)*n) where D is days since Jan 1 and 3 is approx. days from 31 Dec to current  date of earth's perihelion' (Jan 3)
    //(D-3) -> ((D-9)-3) (Winter Solstice Date instead of Jan 1 Date)-> (D-12)

    //double B=A + 0.033405602*sin((D_Dey-12)*n); //Perihelion //Approx. 2 weeks after Yalda Night or 12 Days, 0.033405602 radians is 1.914degs -> shifts after 100 years thoe



    //~~ https://astronomy.stackexchange.com/questions/6555/advancement-of-perihelion-data
    //https://farside.ph.utexas.edu/teaching/336k/Newton/node115.html 
    //https://iranchamber.com/calendar/converter/iranian_calendar_converter.php
    //https://astronomy.stackexchange.com/questions/6625/why-earths-perihelion-occurs-3rd-january-rather-than-1st
    //https://astropixels.com/ephemeris/perap/perap1801.html
       
    //Axial Precession
    /*~~https://en.wikipedia.org/wiki/Axial_precession
        //https://owd.tcnj.edu/~pfeiffer/AST261/AST261Chap4,Preces.pdf
    //50.29 arcseconds per year
    //pA = 5,028.796195 T + 1.1054348 T2 ...  (5028 arcseconds per century)

        axial precession changes seasons closer to periphelion, currently its winter solstice

        P         P         P
        A       A   B       B
      D * B       *       A * C
        C       D   C       D
    */

    //Apsidal Precession
    /*
      //https://en.wikipedia.org/wiki/Apsidal_precession

      //https://dictionary.obspm.fr/index.php?showAll=1&formSearchTextfield=apsidal
      //  https://www.iau.org/Profile?ID=29332

      //  11.6 arc-seconds every year
      112000 years for Earth's Apsidal cycle
      1 degree = 3600 arcseconds
      arcseconds per year = (360 degrees * 3600 arcsec perdegree)/112000 = ~11.571428571 or ~11.6
                                    
                             A          A
         B             B                
      C  *    A          *           B  *  D
         D            C    D            C

      //https://www.timeanddate.com/astronomy/perihelion-aphelion-solstice.html
      //At year 625 Dey 1 or 1246 AD Dec 22 (JULIAN), they both occur on the same day
    */
    //1 day shift every 58 years from the effects of both Axial Precession and Apsidal Precession
    //General precession:
    //E is the number of days from Dey 1 until the next periphelion day. right now it is 13.5 days apart
    //double E=fmod(  (solar_hijri_year-625)*arcsec2rad((50.28796195+11.571428571)) ,2*M_PI);  


    //~~ https://github.com/php/php-src/blob/61249b01257929e96ce3e2ad7b9f0067a6c44aa4/ext/date/lib/astro.c#L207
    //~~ https://stjarnhimlen.se/comp/tutorial.html
    /*
        //Try factoring solar days instead of years.
        Other things:
        1.151E-9 <----- change of earth's eccentricity per day
        4.70935E-5 <----- change of earth's perihelion per day
        3.563E-7 <----- change of earth's axial tilt per day

        1246AD Dec 22 to 1970 Jan 01 is 264,080 days

            625 Dey 1 to 1348 Dey 11 is &? days


        1246AD Dec 22 to 2000 Jan 01 is 275,037 days

            625 Dey 1  to 1348 Dey 11 +
            1348 Dey 11 to 1378 Dey11  
                                            is ^? days

        F0 = 0.01671022  + 1.151E-9 * ^? days      

        sh_days = &? + solarhijri days
    */
    int64_t jan2000_sh_days = GetSolarHijriDays(11,10,1378); //shjri:10957 greg:10957
    int64_t sh_days0 = GetSolarHijriDays(1 +3,10,625); //shjri:264080 greg: 264080 //3day offset as this occured before swap from julian to gregorian
    int64_t sh_days = sh_days0 + solar_hijri_days;
    double perihelion_wsolstice_ang = fmod(deg2rad(4.70935E-5 * sh_days),2*M_PI); //axial and apsidal preccedent movement per day
    _earth->perihelion=perihelion_wsolstice_ang;

    //printf("\n!~days since 1jan2000: %lld\n",(solar_hijri_days-jan2000_sh_days));
    //on 625 Dey 1, 0.0170267876 is its eccentricity
    //on Jan 1 2000, it is 0.01671022  
    //decreases over centuries, slowly - 0 is a perfect circle
    //*Johannes Kepler
    //https://en.wikipedia.org/wiki/Milankovitch_cycles

    double earth_orbital_eccentricity= 0.01671022+1.151E-9*(jan2000_sh_days - solar_hijri_days); //orbital accentricity per day
    //Earth oribital eccentricity Bounce back after reaching limit and vice versa

    //http://profharwood.x10host.com/GEOL102/Study/Eccentricity.htm
    double minEcc = 0.0058;
    double maxEcc = 0.057;
    double Ecc_range  = maxEcc - minEcc; 


    // Wrap into oscillating range
    double in_Ecc_range = fmod(earth_orbital_eccentricity - minEcc, 2 * Ecc_range);
    if (in_Ecc_range < 0) in_Ecc_range += 2 * Ecc_range; // keep positive

    if (in_Ecc_range < Ecc_range) {
        earth_orbital_eccentricity = minEcc + in_Ecc_range;
    } else {
        earth_orbital_eccentricity = maxEcc - (in_Ecc_range - Ecc_range);
    }
    _earth->orbital_eccentricity=earth_orbital_eccentricity;


    double F = deg2rad(earth_orbital_eccentricity*360/M_PI); //convert to radians 


    double earth_axial_tilt=23.4393 + 3.563E-7*(jan2000_sh_days-solar_hijri_days);


    //Axial Tilt Bounce back after reaching limit and vice versa
    //https://science.nasa.gov/earth/earth-observatory/milutin-milankovitch/
    double minTilt = 22.1;
    double maxTilt = 24.5;
    double range   = maxTilt - minTilt; // 2.4

    // Wrap into oscillating range
    double in_range = fmod(earth_axial_tilt - minTilt, 2 * range);

    if (in_range < 0) in_range += 2 * range; // ensure positive

    if (in_range < range) {
        earth_axial_tilt = minTilt + in_range;
    } else {
        earth_axial_tilt = maxTilt - (in_range - range);
    }
    _earth->axial_tilt=deg2rad(earth_axial_tilt);


    //printf("\nsh_days0: %lld, sh_days: %lld, solar_hijri_days: %lld\n",sh_days0,sh_days,solar_hijri_days);
    //printf("\ndays between 1 jan 1970 and 1 jan 2000: %lld\n" , jan2000_sh_days);
    //1 year = 61.89arcsec

    //printf("\n\n/************[EARTH]***************/\n");
    //printf("**Days from Dey 1 (Winter Solstice) to Perihelion: %5.4f, \n",  perihelion_wsolstice_ang/(2*M_PI/365.25) /*,(solar_hijri_year-625)/58.0*//*12.0*n*/ );
    //printf("** -- Caused by the combined effects of Aspidal and Axial Precession.\n");
    //printf("**Earth Orbital Eccentricity: %10.10f\n",earth_orbital_eccentricity);
    //printf("**Earth Axial Tilt: %10.10f\n",earth_axial_tilt);
    //printf("/****************************/\n");

    //B is the angle the Earth moves from the solstice to date D
    double B= A + F*sin(A - perihelion_wsolstice_ang);


    //Difference between angle moved at mean speed
    double C = (A - atan( tan(B)/cos(deg2rad(earth_axial_tilt)) ))/M_PI; //23.44 is earth's  tilt


    //720 is 12hours * 60 minutes
    double eqtime = 720 * (C-(int)(C+0.5)); //720(C - nint C) nint is nearest integer to C.


    //Solar Declination
    double decl = asin(sin(-deg2rad(earth_axial_tilt))*cos(B)); //23.44 is earth's  tilt


    //~~ https://gml.noaa.gov/grad/solcalc/solareqns.PDF
    //90.833 is the zenith angle
    // hour angle [radians]
    double ha_sunrise = acos(((cos(deg2rad(90.833))) / (cos(deg2rad(ctx->in_latitude)) * cos(decl))) - tan(deg2rad(ctx->in_latitude)) * tan(decl));
    double ha_sunset = -ha_sunrise;

    // sunrise [minutes]
    double sunRise = 720 - 4 * (ctx->in_longitude + rad2deg(ha_sunrise)) - eqtime;
    double sunSet = 720 - 4 * (ctx->in_longitude + rad2deg(ha_sunset)) - eqtime;

    ctx->out_sunrise_mins = sunRise;
    ctx->out_sunset_mins = sunSet;
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


