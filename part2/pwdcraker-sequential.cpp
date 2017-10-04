/*
Project 1: Part 2
Group 2
*/

#include <iostream>
#include <stdio.h>
#include <algorithm>
#include "timing.h"
#include <string.h>
#include "md5.h"
using namespace std;

extern string md5(string str);
//PWD to be craked
static string authPwd = "bv37q";
static string authMD5 = md5(authPwd);
static int crackLabel = 0;

// The main recursive method to print all possible strings of length k
static void checkAllKLengthRec(char set[], string prefix, int n, int k) {
    if(crackLabel == 1) return;

// Base case: k is 0, print prefix
    if (k == 0) {
        //cout << prefix << endl;
        string md5PWD = md5(prefix);
        if(md5PWD==authMD5){
            cout << "Successfully cracked the password:" << prefix << endl;
            crackLabel = 1;
        }

        return;
    }
// One by one add all characters from set and recursively 
// call for k equals to k-1
    for (int i = 0; i < n; ++i) { 
    // Next character of input added
        string newPrefix = prefix + set[i]; 
        // k is decreased, because we have added a new character
        checkAllKLengthRec(set, newPrefix, n, k - 1); 
    }
}
 // The method that check the md5() of all possible strings of length k with the md5() of the authenticate password.  It is mainly a wrapper over recursive function printAllKLengthRec()
static void checkAllKLength(char set[], int k) {

    int n = strlen(set);        
    checkAllKLengthRec(set, "", n, k);
}



int main() {
    cout << "Authenticated password: " << authPwd << endl;
    cout << "MD5 of the password: " << authMD5 << endl;
    reset_and_start_timer();
    for(int i = 1; i <= 8; i++){
        if(crackLabel==1) break;
        checkAllKLength(alphanum, i);
    }
     
    double dt = get_elapsed_mcycles();
    printf("@time of ISPC run:\t\t\t[%.3f] million cycles\n", dt);
    return 0;
}

