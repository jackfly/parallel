/*
Project 1: Part 2
Group 2
*/

#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "timing.h"
#include <string.h>
#include "pwdcraker_ispc.h"
using namespace std;
using namespace ispc;


static uint8_t alphanumerical[36] = {
    97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,49,50,51,52,53,54,55,56,57,58
};
//"abcdefghijklmnopqrstuvwxyz"
//"0123456789"

static uint8_t password[5] = {
    98,118,51,55,113
};
//bv37q

//http://www.cse.yorku.ca/~oz/hash.html
//sdbm hash function
static uint64_t inline hash(uint8_t str[], int len)
{
    uint64_t hash = 0;
    int c;
    int i = 0;

    for(i = 0; i< len; i++){
          c = str[i];
        //hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

static void inline permutation(uint8_t prefix[], int length, int index, uint64_t hashPassword, int constLength){
  if(length == 0){
  if(::hash(prefix,constLength)==hashPassword){
    printf("Successfully craked!! \n Password length: %d\n", constLength);
    //printf("Cracked password:\n");
    //for (int j = 0; j< constLength; j++){
    //printf("%d \n", prefix[j]);
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
    
   for (int i = 0; i < sizeof(alphanumerical)/sizeof(int8_t); i++) {
    prefix[index] = alphanumerical[i];
    permutation(prefix, length-1,index+1, hashPassword, constLength);
   }
   
  }
 }

void pwdcraker_serial( int maxLen,  uint64_t output[])
{
  //printf("Length of the password: %d\n", sizeof(password)/sizeof(int8_t));
  //printf("Length of alphanumerical pool: %d\n", sizeof(alphanumerical)/sizeof(int8_t));
  int i = 0;
  uint64_t hashPassword = ::hash(password,5);
  output[i] = hashPassword;
 
  for(int length=1;length<=5;length++){
    uint8_t temp[5] = {0};
    permutation(temp,length,0,hashPassword,length);
  }
}



int main() {
    int maxLen = 5;
    // We assume that the password is not case-sensitive, and can be only set from the following 46 characters.
    //char *password = "bv37q";
    //cout << "The password to be cracked: " << password << endl;
    //unsigned long result = ::hash((unsigned char*)password);
    //cout << "Hash of the password: " << result << endl;
    unsigned long *output = new unsigned long[5];

    double minISPC = 1e30;

    for(unsigned int i = 0; i < 3; i++){
        reset_and_start_timer();
        pwdcraker_ispc(maxLen, (uint64_t *)output);
        double dt = get_elapsed_mcycles();
        printf("@time of ISPC run:\t\t\t[%.3f] million cycles\n", dt);
        minISPC = std::min(minISPC, dt); 
    }
    printf("[root calculate ISPC]:\t\t[%.3f] million cycles\n", minISPC);


    double minSerial = 1e30;
    for(unsigned int i = 0 ; i < 3; i ++){
        reset_and_start_timer();
        pwdcraker_serial(maxLen, (uint64_t *)output);
        double dt = get_elapsed_mcycles();
        printf("@time of serial run:\t\t\t[%.3f] million cycles\n", dt);
        minSerial = std::min(minSerial, dt); 
    }
    printf("[root calculate serial]:\t\t[%.3f] million cycles\n", minSerial);


    printf("\t\t\t\t(%.2fx speedup from ISPC)\n", minSerial/minISPC);

    return 0;
}

