


unsigned long long current_timestamp() {//https://copyprogramming.com/howto/c-sleep-in-milliseconds-in-c-code-example
//https://stackoverflow.com/questions/10301106/gettimeofday-returns-a-negative-value
  struct timeval te;
  mingw_gettimeofday(&te, NULL); //get current time
  return (unsigned long long) te.tv_sec*1000LL + te.tv_usec/1000; //calc millisecs
}

unsigned int int_current_timestamp() {
  struct timeval te;
  mingw_gettimeofday(&te, NULL);
  return (unsigned int) te.tv_sec;
}


void PersiaSolarTime(int _seconds)
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


  //Filter out the different time parts
  int year=1348;
  int month=9;          //start day     //timezone diff 8:30 (America to Iran)
  int seconds=_seconds+(day_seconds*12+day_seconds/2)-(8*60*60+30*60); //Offsets
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



  int print_seconds=seconds%60; //60 seconds in a minute
 
  //Minutes
  int min=seconds/60;
  int print_min=min%60; //60 minutes in a second

  //Hours
  int hours=min/60;
  int print_hours=hours%24; //24 hours in a day

  //Days
  int days=hours/24;


  //31 or 30 or 29 days in a month
  int print_days;
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

  printf("\n~::Solar Hijiri Time- Year~%d Month~%d Day~%d - %d:%d:%d ::~\n",year,(month+1),print_days,print_hours,print_min,print_seconds);
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


