int prints(char *c);
int printi(int n);
int readi(int* eP);

int main(){
  int mat[100][100];
  char *c="\nPlease Enter Dimension of matrixr: ";
  char *d="\nPlease Enter the matrix row-wise: ";
  char *e="\nThe Entered matrix is: ";
  char *f="\nThe Transposed matrix is: ";
  char *sp=" ";
  char *ne="\n";
  prints(c);
  int err,n,i,j,k;
  n = readi(&err);
  int temp;
  prints(d);
  for (i = 0; i < n; ++i)
  {
    for (j = 0; j < n; ++j)
    {
      mat[i][j]=readi(&err);
    }
  }
  prints(e);
  prints(ne);
  for (i = 0; i < n; ++i)
  {
    prints(ne);
    for (j = 0; j < n; ++j)
    {
      printi(mat[i][j]);
      prints(sp);
    }
  }
  prints(f);
  prints(ne);

  for (i = 0; i < n; ++i)
  {
    for (j = i+1; j < n; ++j)
    {
      temp=mat[j][i];
      mat[j][i]=mat[i][j];
      mat[i][j]=temp;
    }
    
  }
  for (i = 0; i < n; ++i)
  {
    prints(ne);
    for (j = 0; j < n; ++j)
    {
      printi(mat[i][j]);
      prints(sp);
    }
  }
  prints(ne);
return 0;
}
