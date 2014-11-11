int prints(char *c);
int printi(int n);
int readi(int* eP);

int main()
{	
  int a,b,err;
  char *c1 = "\nEnter a:";
  char *c2 = "\nEnter b:";
  char *c3 = "\na+b:";
  char *c4 = "\na*b:";
  char *c5 = "\na/b:";
  char *c6 = "\na%b:";
  char *c7 = "\na-b:";
  char* c="\n";
  prints(c1);
  a = readi(&err);
  prints(c2);
  b = readi(&err);
  prints(c3);
  printi(a+b);
  prints(c4);
  printi(a*b);
  prints(c7);
  printi(a-b);
  prints(c5);
  printi(a/b);
  prints(c6);
  printi(a%b);
  prints(c);
	return 0;
}
