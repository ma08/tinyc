#define BUFF 30 //Max length of buffer
#define LIM 6 //Precision of float
int printi(int n) {
	char buff[BUFF], zero='0';
    int i=0, j, k ;
    if(n == 0) buff[i++]=zero;
    else{
       if(n < 0) {
       	  // - for negative integer
          buff[i++]='-';
          //getting magnitude
          n = -n;
       }
       while(n){
       	   //putting each digit into buffer in reverse order
          int dig = n%10;
          buff[i++] = (char)(zero+dig);
          n /= 10;
       }
       if(buff[0] == '-') j = 1;
       else j = 0;
       k=i-1;
       //reversing the digits
       while(j<k){
          char temp=buff[j];
          buff[j++] = buff[k];
          buff[k--] = temp;
       }
    } 
    //system call to write buff upto i characters
    __asm__ __volatile__ (
          "movl $4, %%eax \n\t"
          "movl $1, %%ebx \n\t"
          "int $128 \n\t"
          :
          :"c"(buff), "d"(i)
    ) ;  // $4: write, $1: on stdin
    return i;
}

int prints(char *str) {
	int i=0;
	//counting number of characters
	while(str[i]!='\0'){
		i++;
	}
	//system call to write str on stdin upto i characters
	__asm__ __volatile__ (
          "movl $4, %%eax \n\t"
          "movl $1, %%ebx \n\t"
          "int $128 \n\t"
          :
          :"c"(str), "d"(i)
    ) ;  // $4: write, $1: on stdin
    //returning number of characters printed
    return i;
}

int printd(float f){
	char buff[BUFF], zero='0';
	int i=0;
	int count=0;
	//mag is magnintued of f
	int mag=f;
	//magn is number of characters printed  for mag
	//printing the integer part
	int magn=printi(mag);
	//for negative numbers, getting the absolute
	if(f<0){
		f=-f;
		mag=-mag;
	}
	//getting the fraction part
	f=f-mag;
	buff[count++]='.';
	/*
	 * To not print the last zeroes
	int lastn0=-1;
	for(i=2;i<=LIM+1;i++){
		int x=f*10;
		f=f*10-x;
		buff[count++]=x+zero;
		if(x>0){
			lastn0=i;
		}
	}
	if(lastn0==-1){
		return magn;
	}
	__asm__ __volatile__ (
          "movl $4, %%eax \n\t"
          "movl $1, %%ebx \n\t"
          "int $128 \n\t"
          :
          :"c"(buff), "d"(lastn0)
    ) ;  // $4: write, $1: on stdin
    */
	//printing each digit in the fractional part upto LIM characters	
	for(i=0;i<LIM;i++){
		int x=f*10;
		f=f*10-x;
		buff[count++]=x+zero;
	}
	//system call to write buff upto LIM+1 character on stdin
	//printing the fractional part here
	__asm__ __volatile__ (
          "movl $4, %%eax \n\t"
          "movl $1, %%ebx \n\t"
          "int $128 \n\t"
          :
          :"c"(buff), "d"(LIM+1)
    ) ;  // $4: write, $1: on stdin
    //returning number of characters printed
    return magn+LIM+1;
}

int readi(int *eP){ // *eP is for error, if the input is not an integer
	char buff[BUFF], zero='0', space=' ';
	int length;
	//length is the number of characters read. Including '\n'
	//reading from stdin into buff upto maximum of BUFF characters
	//number of characters read is given in length
	__asm__ __volatile__ (
          "movl $3, %%eax \n\t"
          "movl $0, %%ebx \n\t"
          "int $128 \n\t"
          :"=a" (length)
          :"c"(buff), "d" (BUFF): "bx"
    ) ; //$3 to read, $1 on stdout
	//defualt is ERR
    *eP=1;
    //only new line character
    if(length<=1){
    	return 0;
    }
    int n=0;
	int i=0;
	int negative=0;
	//ignoring starting spaces
	while(buff[i]==space)
		i++;
	//checking for negative		
	if(buff[0]=='-'){
		negative=1;
		i=1;
		//only - and \n
		if(length==2)
			return 0;
		//non digit succeeding -
		if(buff[1]-zero>9||buff[1]-zero<0)
			return 0;
	}else if(buff[0]-zero>9||buff[0]-zero<0){ //checking if first character is invalid
		return 0;
	}

    for(;i<length-1;i++){
    	if(buff[i]==space) //if space is found, exit reading, return read number 
    		break;
    	if(buff[i]-zero<0||buff[i]-zero>9){ //if nond digit character is found
    		//exit reading, return read number
    		break;
    		//*eP=1;
    		//return -1;
    	}
    	n=n*10+buff[i]-zero;
    }
    //OK
    *eP=0;
    if(negative) //for negative numbers
    	n=-n;
    return n;
}


int readf(float *f){ // return value is error or OK
	char buff[BUFF], zero='0', space=' ';
	int length;
	//length is the number of characters read. Including '\n'
	*f=0;
	//reading from stdin into buff upto maximum of BUFF characters
	//number of characters read is given in length
	__asm__ __volatile__ (
          "movl $3, %%eax \n\t"
          "movl $0, %%ebx \n\t"
          "int $128 \n\t"
          :"=a" (length)
          :"c"(buff), "d" (BUFF): "bx"
    ) ; //$3 to read, $1 on stdout
    //only new line character
	if(length<=1){
    	return 1;
    }
	int negative=0;
	//decimal is position of decimal
	int decimal=-1;
	int i=0;
	//ignoring starting spaces
	while(buff[i]==space)
		i++;
	//checking for negative		
	if(buff[0]=='-'){
		negative=1;
		i=1;
		//only - and \n
		if(length==2)
			return 1;
		//non digit succeeding -
		if(buff[1]-zero>9||buff[1]-zero<0){
			return 1;
		}
	}else if(buff[0]-zero>9||buff[0]-zero<0){//checking if first character is invalid
		return 1;
	}

   	int n=0;
	for(;i<length-1;i++){
		//stop reading when space is found
		if(buff[i]==space)
    		break;
		//stop reading when non digit character is found except . 
		if((buff[i]-zero<0||buff[i]-zero>9)&&!(buff[i]=='.')){
			//return the number
			*f=n;
    		return 0;
    	}
    	
		if(buff[i]=='.'){ //decimal point is found
			//get positon of decimal
			decimal=i;
			break;
		}
    	n=n*10+buff[i]-zero;
	}
	*f=0;
	if(decimal==-1||decimal==length-2){ //no decimal point or it is found at end
		*f=n;
		return 0;
	}
	//firstinvalid is the position of the first invalid character the fraction part
	//ignoring all characters from firstinvalid to end
	int firstinvalid=-1;
	//finding firstinvalid
	for(i=length-2;i>decimal;i--)
		if(buff[i]-zero>9||buff-zero<0)
			firstinvalid=i;
	//constructing the fractional part
	for(i=firstinvalid==-1?length-2:firstinvalid-1;i>decimal;i--){
		(*f)=(*f+buff[i]-zero)/10;
	}
	//adding integer part
	*f=*f+n;
	if(negative)
		*f=-(*f);
	//OK
	return 0;
}
	


