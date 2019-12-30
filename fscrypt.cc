//
//  a.cpp
//  stream
//
//  Created by BO ZHANG on 10/7/19.
//  Copyright © 2019 BO ZHANG. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include "openssl/blowfish.h"
#include <string.h>
#include "fscrypt.h"

   BF_KEY key;
   char initialization_vector[]="00000000";

void *fs_encrypt(void *plaintext, int bufsize, char *keystr, int *resultlen){
    
    //BF_KEY key;
    //unsigned char *result = NULL;
    //char initialization_vector[]="00000000";
    
    unsigned char *output = (unsigned char *)malloc(100*sizeof(char));
    unsigned char cipher_text[bufsize];
    
    //bf set key
    BF_set_key(&key,16,(const unsigned char*)keystr);
    
    unsigned char *plain=(unsigned char*)plaintext;
    
    int i=0;
    while(i<8){
        cipher_text[i]=(*plain)^initialization_vector[i];
//    printf("cipher:%d %d %d  %d\n",i, *plain, initialization_vector[i],cipher_text[i]);
        plain++;
        i++;
    }
    unsigned char *temp =output;
    
    BF_ecb_encrypt(&cipher_text[0],output,&key,BF_ENCRYPT);
    
    bufsize-=8;
    int index1=8;
    int index2=0;
    int index3=0;
    
    //unsigned char *temp=output;
    
    while(bufsize>0){
        //printf("while\n");
        while(index2<8){
            cipher_text[index1]=(*output)^(*plain);
//        printf(" cipher: %d %d %d %d\n",index1,*output,*plain,cipher_text[index1]);
            index1++;
            output=output+1;
	    plain=plain+1;
            index2+=1;
        }
        index3+=8;
        
        //BF ECB
        BF_ecb_encrypt(&cipher_text[index3],output,&key,BF_ENCRYPT);
        
//        printf("after %d\n",cipher_text);
        
        bufsize-=8;
        index2=0;
    }
    
    *resultlen=strlen((const char *)temp);
    return (void *)temp;
}

void *fs_decrypt(void *ciphertext,int bufsize, char *keystr, int *resultlen){
    
    //BF_KEY key;
    //unsigned char *result = NULL;
    //char initialization_vector[]="00000000";
    
    unsigned char *output = (unsigned char *)malloc(100*sizeof(char));
    unsigned char *plain_text = (unsigned char *)malloc((bufsize+1)*sizeof(plain_text));
    //unsigned char plaintext[bufsize];
    
    unsigned char *tempc=(unsigned char*)ciphertext;
    unsigned char *cipher=(unsigned char*)ciphertext;
    unsigned
    int i=0;
    
    BF_ecb_encrypt(tempc,output,&key,BF_DECRYPT);
    while(i<8){
        plain_text[i]=(*output)^initialization_vector[i];
//    printf("decrypt: %d %d %d %d\n",i,*output,initialization_vector[i],plain_text[i]);
        output++;
        i++;
    }
    
    bufsize-=8;
    
    int index1=0;
    int index2=8;
    while(bufsize>0){
        tempc+=8;
        BF_ecb_encrypt(tempc,output,&key,BF_DECRYPT);
//    printf("cipher %d\n",tempc);
        while(index1<8){
            plain_text[index2]=(*cipher)^(*output);
//        printf("decrypt: %d %d %d %d\n",index2,*cipher,*output,plain_text[index2]);
            output=output+1;
            cipher=cipher+1;
            index1+=1;
            index2+=1;
        }
        index1=0;
        bufsize-=8;
    }
    
    *resultlen=strlen((const char*)plain_text)+1;
    
    //return (void*)plain_text;
    unsigned char * result=(unsigned char*)malloc(sizeof(plain_text));
    result=plain_text;
    return (void*) result;
}
