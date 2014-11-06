int b[10][10];

void transpose(int m, int n);

int foo(){
  int i,j;
  for (i = 0; i < 5; ++i)
  {
    for (j = 0; j < 5; ++j)
    {
      b[i][j]=i+j;
    }
  }
  transpose(5,5);
  return 0;
}

void transpose(int m, int n){
  int i,j;
  int temp;
  for (i = 0; i < 5; ++i)
  {
    for (j = i+1; j < 5; ++j)
    {
      temp=b[i][j];
      b[i][j]=b[j][i];
      b[j][i]=temp;
      
    }
  }
  return;

}
