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

int reverse(char *c){
  prints(c);
  int i=0,j;
  while(c[i]!=0)
    i++;
  printi(i);
  i--;
  j=0;
  /*char temp;
  while(i>=j){
    temp=c[j];
    c[j++]=c[i];
    c[i--]=temp;
  }*/
  c[0]='i';
  prints(c);
  return 0;
}

int ccc(char x,int i){
  int j;
  char h;
  j=i++;
  h=x--;
  return 0;
}

int main(){
  int i,j;
  /*int b[2][2];*/
  /*int a=5;
  int d;
  int *c;
  int **b;
  c=&a;
  b=&c;
  d=**b;
  printi(d);*/
  char *d = "Adfaaajkafasfd";
  char *e = "Adfaaaaaaaaajkafasfd";
  char c[4];
  c[0]='a';
  c[1]='b';
  c[2]='c';
  c[3]=0;
  /*reverse(c);*/
  reverse(d);

  
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
