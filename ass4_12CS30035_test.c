int printi(int n);
int readi(int* eP);
int prints(char* c);
void mergeSort(int *arr,int low,int mid,int high);
void partition(int *arr,int low,int high);

int main(){
   char *c = "\nEnter number of entries:";
   char *ne = "\n";

   prints(c);
    int merge[10000],i,n,err;
    n = readi(&err);
   char *c1 = "\nEnter numbers to be sorted:";
   prints(c1);
   prints(ne);

    if(err == 0){
        for(i = 0;i<n;i++){
            merge[i] = readi(&err);
        }
    }

    partition(merge,0,n-1);
    prints(ne);
   char *c2 = "\nSorted numbers:";
   char *sp =" ";
    prints(c2);
    for(i=0;i<n;i++){
         printi(merge[i]);
          prints(sp);
    }
    prints(ne);

   return 0;
}

void partition(int *arr,int low,int high){
    int mid;
    printi(low);
    printi(high);

    if(low<high){
         mid=(low+high)/2;
         partition(arr,low,mid);
         partition(arr,mid+1,high);
         mergeSort(arr,low,mid,high);
    }
}

void mergeSort(int *arr,int low,int mid,int high){

    int i,m,k,l,temp[10000];

    l=low;
    i=low;
    m=mid+1;

    while((l<=mid)&&(m<=high)){
         if(arr[l]<=arr[m]){
             temp[i]=arr[l];
             l++;
         }
         else{
             temp[i]=arr[m];
             m++;
         }
         i++;
    }

    if(l>mid){
         for(k=m;k<=high;k++){
             temp[i]=arr[k];
             i++;
         }
    }
    else{
         for(k=l;k<=mid;k++){
             temp[i]=arr[k];
             i++;
         }
    }
   
    for(k=low;k<=high;k++){
         arr[k]=temp[k];
    }
}
