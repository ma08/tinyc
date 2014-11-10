int printi(int n);
int prints(char* c);
int readi(int* eP);
int fib(int num);
 
int main() {
  char *c="\nPlease Enter number: ";
  char *d="\nThe fibonacci output is: ";
  char *n="\n";
  prints(c);
   int err,num,i,j,k;
    num = readi(&err);
    prints(d);
    printi(fib(num));
    prints(n);
  return 0;

}
 
int fib(int num) {
    if(num <= 1) return 1;
    else return fib(num-1)+fib(num-2);

}
