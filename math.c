//Maths
int RandNum(int min, int max) {return rand()%(max-min + 1) + min;}


long long current_timestamp() {//https://copyprogramming.com/howto/c-sleep-in-milliseconds-in-c-code-example
  struct timeval te;
  mingw_gettimeofday(&te, NULL); //get current time
  long long millisec = te.tv_sec*1000LL + te.tv_usec/1000; //calc millisecs
  return millisec;
}

int machinespeed() {
  struct timeval time_start,time_now;
  int tick,i,j,c=2048,d=5;
  double sum=0,sum2=0;
  for (j=0;j<d;j++) { //5 times & generate avg
    for (i=0;i<c;i++) { //count to 65546 2048 times & generate avg
      mingw_gettimeofday(&time_start, NULL); //get time in msec
      for (tick=0;tick<65536;tick++);
      mingw_gettimeofday(&time_now, NULL); //get time in msec after for count
      sum += (time_now.tv_sec - time_start.tv_sec) * 1000000 + time_now.tv_usec - time_start.tv_usec;
    }
    sum2+=sum/c;
    sum=0;
  }
  return (int) sum2/d; //return machine speed
}


/*int machinespeed() {
  int tick=0;
  long long start_time=current_timestamp();
  long long current_time=LLONG_MAX;

  long long time_diff;
  while (true) {
    time_diff=current_time-start_time;
    if (time_diff<100) {
      current_time=current_timestamp();
      tick++;
    } else {
      return tick/100;
    }
  }
}*/


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
//------------------------------------------------------
