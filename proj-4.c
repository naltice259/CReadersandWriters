//Nicholas Altice
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "threads.h"


int a = 0;
int b = 0;
int e = 0;
int size = 0;
Semaphore *Reader;
Semaphore *Writer;
int rc = 0;
int wc = 0;
int rwc = 0;
int wwc = 0;

void reads(int readerID);
void write(int writerID);

int main()
{
	
    char c[17];
    fgets(c, 17, stdin); //get input
    int i = 0;
	while(c[i] >= 48 && c[i] <= 57 && i < 17){
    	b = (b * 10) + (c[i] - '0'); //get readers number
    	i++;
	}
	i++;
	while(c[i] >= 48 && c[i] <= 57 && i < 17){
    	e = (e * 10) + (c[i] - '0'); //get writers number
    	i++;
	}
	Reader = (struct Semaphore*)malloc(sizeof(struct Semaphore));
	Writer = (struct Semaphore*)malloc(sizeof(struct Semaphore));
	
	InitSem(&Reader, a);
	InitSem(&Writer, a);
	size = b+e;
	int q[size];
    for(int z = 0;z < size; z++){
    	scanf("%d", &q[z]);
	}
	for(int z = 0;z < size; z++){
		if(q[z] > 0){
			start_thread(reads, q[z]);
		}else{
			start_thread(write, q[z]);
		}
	}
    run();
    return 0;
}

void reads(int readerID)
{
	if(rwc>0 || wc>0 || wwc>0){
		rwc++;
		P(&Reader);
		rwc--;
	}
	rc++;
	printf("\n This is the %d th reader reading value i = %d for the first time \n", readerID, a);
	yield();
	printf("\n This is the %d th reader reading value i = %d for the second time \n", readerID, a);
	rc--;
	if(rc==0 && wwc>0){
		V(&Writer);
	}
	yield();
	for(int z = 0;z < 300; z++){
    	yield();
	}
	return;
}

void write(int writerID)
{
	int wID = -1 * writerID;
	if(rc>0 || wc>0 || rwc>0 || wwc>0){
		wwc++;
		P(&Writer);
		wwc--;
	}
	wc++;
	a = wID;
	printf("\n This is the %d th writer writing value i = %d \n", wID, a);
	yield();
	printf("\n This is the %d th writer verifying value i = %d \n", wID, a);
	wc--;
	if(rwc>0){
		for(int i=1;i<=rwc;i++){
			V(&Reader);
		}
	}else if(wwc>0){
		V(&Writer);
	}
	yield();
	for(int z = 0;z < 300; z++){
    	yield();
	}
	return;
}
