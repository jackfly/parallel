/*
Project 1: Part 2
Group 2
*/
//static int crackLabel = 0;
static const int alphanumerical[36] = {
	97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,49,50,51,52,53,54,55,56,57,58
};
//"abcdefghijklmnopqrstuvwxyz"
//"0123456789"

static int password[5] = {
	98,118,51,55,113
};
//bv37q

//http://www.cse.yorku.ca/~oz/hash.html
//sdbm hash function
static unsigned int64 inline hash(int str[], int len)
{
    unsigned int64 hash = 0;
    int c;

    for(uniform int i = 0; i< len; i++){
    	  c = str[i];
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

static void inline permutation(int prefix[], int length, uniform int index, unsigned int64 hashPassword, int constLength, uniform int output[]){
  if(length == 0){
  if(hash(prefix,constLength)==hashPassword){
    print("======Successfully Craked！！！====== \n");
    const int lastone = 5;
    output[lastone] = constLength;
    for (int n = 0; n<constLength; n++){
      output[n] = prefix[n];
    }
   }
   return;
  }else{
   for (uniform int i = 0; i < sizeof(alphanumerical)/sizeof(int); i++) {
   	prefix[index] = alphanumerical[i];
    permutation(prefix, length-1,index+1, hashPassword, constLength, output);
   }
   
  }
 }

export void pwdcraker_ispc(uniform int maxLen,  uniform int output[])
{
  unsigned int64 hashPassword = hash(password,5);
  int temp[5] = {0};
  print("======Start cracking using ISPC (balanced distributed)======\n");
  for(uniform int length=1;length<=maxLen;length++){
    foreach(m=1...37){  
      temp[0] = alphanumerical[m-1];
      permutation(temp,length-1,1,hashPassword,length,output);
    } 
  }
}

export void pwdcraker_ispc_unbalanced(uniform int maxLen, uniform int output[])
{
  unsigned int64 hashPassword = hash(password,5);
  int temp[5] = {0};
  print("======Start cracking using ISPC (unbalanced distributed)====== \n");
    foreach(length=1...maxLen+1){  
      permutation(temp,length,0,hashPassword,length,output);
    } 
}

task void pwdcraker_ispc_task(uniform int maxLen,  uniform int output[], uniform int range, uniform int length)
{
  unsigned int64 hashPassword = hash(password,5);
  uniform int start = taskIndex * range;
  uniform int end = min((int)36, start + range);
  int temp[5] = {0};
    foreach(m=start...end){  
      temp[0] = alphanumerical[m-1];
      permutation(temp,length-1,1,hashPassword,length,output);
    } 
}

export void pwdcraker_ispc_task_exe(uniform int taskNum, uniform int maxLen, uniform int output[]){
    uniform int times = 36;
    uniform int range = (int) times / taskNum;
    print("======Start cracking using ISPC_task (balanced distributed)======\n");
    for(uniform int length=1;length<=maxLen;length++){
      launch[times/range] pwdcraker_ispc_task(maxLen, output, range, length);
    }
}
