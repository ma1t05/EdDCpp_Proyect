
#include "uniform.h"

/*
float unif(){
  return rand()/float(RAND_MAX);
}
*/
float unif(float a,float b){
  return (b-a)*rand()/(RAND_MAX)+a;
}
/*
int unif(int a){
  if(a<0)a*=-1;
  if(!a)return 0;
  return int(unif()*a)+1;
}
*/
void seed(){
  srand(time(0));
}
