int fib(int x){
  if(x==0||x==1){
    return 0;
  }
  return fib(x-1)+fib(x-2);
}

int main(){
  int a=9;
  fib(++a);
  int b=3;
  b++;
  return 0;
}


