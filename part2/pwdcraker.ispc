/*
Project 1: Part 2
Group 2
*/

static const unsigned int8 alphanumerical[36] = {
	97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,49,50,51,52,53,54,55,56,57,58
};
//"abcdefghijklmnopqrstuvwxyz"
//"0123456789"

static unsigned int8 password[5] = {
	98,118,51,55,113
};
//bv37q

//http://www.cse.yorku.ca/~oz/hash.html
//sdbm hash function
static unsigned int64 inline hash(uniform unsigned int8 str[], int len)
{
    unsigned int64 hash = 0;
    int c;

    for(uniform int i = 0; i< len; i++){
    	  c = str[i];
        //hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

static void permutation(uniform unsigned int8 prefix[], int length, uniform int index, unsigned int64 hashPassword, int constLength){
  if(length == 0){
  if(hash(prefix,constLength)==hashPassword){
    print("Successfully craked!! \n Password length: %\n", constLength);
    //print("Cracked password:\n");
   	//for (int j = 0; j< constLength; j++){
    //print("% \n", prefix[j]);
    //}
   }
   return;
  }else{
    /*
    foreach(i=0...sizeof(alphanumerical)/sizeof(int8)){
      prefix[index] = alphanumerical[i];
      permutation(prefix, length-1,index+1, hashPassword, constLength);
    }
    */
    
   for (uniform int i = 0; i < sizeof(alphanumerical)/sizeof(int8); i++) {
   	prefix[index] = alphanumerical[i];
    permutation(prefix, length-1,index+1, hashPassword, constLength);
   }
   
  }
 }

export void pwdcraker_ispc(uniform int maxLen,  uniform unsigned int64 output[])
{
  //print("Length of the password: %\n", sizeof(password)/sizeof(int8));
  //print("Length of alphanumerical pool: %\n", sizeof(alphanumerical)/sizeof(int8));
  //uniform int i = 0;
  unsigned int64 hashPassword = hash(password,5);
  //output[i] = hashPassword;
 
/*
  for(uniform int length=1;length<=5;length++){
    unsigned int8 temp[5] = {0};
    permutation(temp,length,0,hashPassword,length);
  }
  */

 unsigned uniform int8 temp[5] = {0};
  foreach(length=1...6){  
    permutation(temp,length,0,hashPassword,length);
  }
  

    

}
