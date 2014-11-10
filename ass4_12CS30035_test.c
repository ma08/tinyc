/*int prints(char *a);
int printi(int a);
int readi(int *eP); // *eP is for error, if the input is not an integer
int readf(float *a); // return value is error or OK
int printd(float a);*/
int prints(char *a);

int printi(int a);

int fib(int x){
int i=0;
if(x==i || x==1){
return 1;
}
return fib(x-1)+fib(x-2);
}

int reverse(int *a,int n){
  /*prints(c);*/
  int i=n-1,j=0;
  int temp;
  while(i>=j){
    temp=a[j];
    a[j++]=a[i];
    a[i--]=temp;
  }
  /*printi(b);*/
  /*printi(c);*/
  return 0;
}



int main(){
  int i,j;
  int a[8];
  for (i = 0; i < 8; ++i)
  {
    a[i]=i;
    
  }
  reverse(a,8);
  for (i = 0; i < 8; ++i)
  {
    printi(a[i]);
    
  }
  /*int b[2][2];*/
  /*int a=5;
  int d;
  int *c;
  int **b;
  c=&a;
  b=&c;
  d=**b;
  printi(d);*/
    
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
