/*int prints(char *a);
int printi(int a);
int readi(int *eP); // *eP is for error, if the input is not an integer
int readf(float *a); // return value is error or OK
int printd(float a);*/

int printi(int a);

int fib(int x){
int i=0;
if(x==i || x==1){
return 1;
}
return fib(x-1)+fib(x-2);
}

int main(){
  int a=5;
  int d;
  int *c;
  int **b;
  c=&a;
  b=&c;
  d=**b;
  printi(d);
/*for(i=0;i<2;i++){
  for(j=0;j<2;j++){
    b[i][j]=i+j;
  }
}
for(i=0;i<2;i++){
  for(j=0;j<2;j++){
    printi(b[i][j]);
  }
}*/
/*for (i = 0; i < 9; ++i)
{
  b[i]=i;
  
}
int x;
for (i = 0; i < 9; ++i)
{
  x=(b[i]);
  printi(x);
}*/
return 0;
}
