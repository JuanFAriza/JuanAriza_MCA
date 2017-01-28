#include <stdio.h>
#include <math.h>

#define start (double)1
#define steps 72
#define delta (double)pow(10,1.0/3.0)

int main(void){
  float pos = start;
  int cod;
  float precision;

  int i;

  for (i=0;i<(steps + 2)/2;i++){
    cod = (*(int*)&pos) - 1;	   
    precision = pos - (*(float*)&cod);
    printf("%.25f %.25f\n",pos,precision);
    pos = pos*delta;
  }
  
  pos = start;

  for (i=0;i<(steps + 2)/2;i++){
    cod = (*(int*)&pos) - 1;	   
    precision = pos - (*(float*)&cod);
    printf("%.25f %.25f\n",pos,precision);
    pos = pos/delta;
  }
  return 0;
}
