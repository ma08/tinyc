int prints(char *c);
int main(){
char k,c='s',*d;
d = &c;
char a[2];
a[0]=*d;
a[1]=0;
prints(a);
return 0;
}
