
U0 DrawTriFill(CTask *,CDC *dc,I64 tri_color,F64 x1,F64 y1,F64 x2,F64 y2,F64 x3,F64 y3)
{
  F64 x_1,x_2,
      gradient_middle1,gradient_middle2,gradient_largest,
      c_middle1,c_middle2,c_largest,
      smallest=9999,largest=0,
      x_arr[3],y_arr[3];
  x_arr[0]=x1;
  x_arr[1]=x2;
  x_arr[2]=x3;
  y_arr[0]=y1;
  y_arr[1]=y2;
  y_arr[2]=y3;
  I64 i,saved_largest=0,saved_smallest=0,saved_middle=0;
  for (i=0;i<3;i++) {
    if (y_arr[i]<smallest) {
      smallest=y_arr[i];
      saved_smallest=i;
    }
  }
  for (i=0;i<3;i++) {
    if (y_arr[i]>largest) {
      largest=y_arr[i];
      saved_largest=i;
    }
  }
  for (i=0;i<3;i++) {
    if (i!=saved_smallest && i!=saved_largest) {
      saved_middle=i;
    }
  }
  gradient_middle1=GetGradient(x_arr[saved_smallest],y_arr[saved_smallest],x_arr[saved_middle],y_arr[saved_middle]); //Gradient of main line
  c_middle1=GetGroundC(x_arr[saved_smallest],y_arr[saved_smallest],gradient_middle1);

  gradient_middle2=GetGradient(x_arr[saved_largest],y_arr[saved_largest],x_arr[saved_middle],y_arr[saved_middle]);
  c_middle2=GetGroundC(x_arr[saved_largest],y_arr[saved_largest],gradient_middle2);

  gradient_largest=GetGradient(x_arr[saved_largest],y_arr[saved_largest],x_arr[saved_smallest],y_arr[saved_smallest]);
  c_largest=GetGroundC(x_arr[saved_smallest],y_arr[saved_smallest],gradient_largest);
  dc->color=tri_color;
  for (i=smallest;i<y_arr[saved_middle];i+=1) {//small to middle
    x_1=GetX(i,gradient_middle1,c_middle1);
    x_2=GetX(i,gradient_largest,c_largest);
    GrLine(dc,x_1,i,x_2,i);
  }
  for (i=y_arr[saved_middle];i<largest;i+=1) {//middle to largest
    x_1=GetX(i,gradient_middle2,c_middle2);
    x_2=GetX(i,gradient_largest,c_largest);
    GrLine(dc,x_1,i,x_2,i);
  }
}
