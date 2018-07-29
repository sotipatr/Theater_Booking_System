#ifndef __MY_HEADER__
#define __MY_HEADER__

/* Dhlwsh vivliothikwn*/
#include <stdio.h>/*input-output*/
#include <sys/types.h>/* basic system data types */
#include <string.h>/*alfarithmhtika*/
#include <stdlib.h> 
#include <unistd.h>
#include <sys/socket.h> /* xrhsimopoieitai gia ton oriso ths socket*/ /* basic socket definitions */
#include <sys/ipc.h> /*epikoiwnia diergasiwn*/
#include <sys/shm.h> /*shared memory*/
#include <pthread.h>/*xrhsimh gia ton orismo twn threads*/
#include <sys/stat.h> /* diaxeirish signals */
#include <signal.h> /*diaxeirish signals */
#include <errno.h> /*xrhsimopoieitai gia thn perror */ /* for the EINTR constant */
#include <time.h> /* aparaithth gia na pairnoume to xrono se kapoia stigmh */
#include <sys/un.h> /* for Unix domain sockets */
#include <sys/time.h> /*xrhshmopoieitai gia thn xronometrhsh*/

/*Dhlwsh statherwn*/
#define UNIXSTR_PATH "/tmp/unix.str"
#define LISTENQ 20 // Size of the request queue.

/*orismos twn structs pou xreiazontai gia thn leitourgia tou server*/ 
/* struct gia thn apothhkeush kai thn enhmerwsh ths shared memory */
struct zones{   
		int zon[4];/*pinakas me ta eishthria se kathe zwnh(keli [0]->zwnh A, [1]->zwnh B, ...)*/
		int pr[4];/*pinakas me tis times gia kathe zwnh(keli [0]->zwnh A, [1]->zwnh B, ...)*/
		int cust;/*pelates, kratame to sunolo twn pelatwn pou exoun kanei krathsh pelatis_id*/
		int comp_acc;/*kratame ta posa apo thn agora eishthriwn(company_account)*/
		int notokord;/*sunolo apotuxhmenwn paraggeliwn*/
		int orders;/*sunolikes paraggelies*/
		float time1; /*xronos anamonhs paraggeliwn*/
		float time2; /*xronos eksuphrethshs paraggeliwn*/
		/*pinakes gia zwnes*/
		int A[100];  /*theseis zwnhs A*/
		int B[130];  /*theseis zwnhs B*/
		int C[180];  /*theseis zwnhs C*/
		int D[230];  /*theseis zwnhs D*/
		int c_s[4];/*trexouses theseis ths kathe zwnhs.se kathe thesh tou pinaka apothhkeuontai oi theseis pou exoun karaththei mexri stigmhs gia na tupwthoun ston pelath oi theseis pou kleisthkan sthn krathsh tou.*/
};	

/*struct gia tupwsh twn dedomenwn gia kathe krathsh*/
struct krathsh{   
		int pelatis_id;/*id tou kathe pelath*/
		int kostos;/*kostos sunallaghs*/
		int not_ok;/*periptwsh pou h karta den einai egkurh(timh 1),den uparxun diatheshmes theshs sthn zwnh(timh 2),den uparxoun katholou theseis(timh 3)*/
		int zwnh;/*zwnh krathshs*/
		
};

struct aithsh{   
		int tic_num[4];  /*arithmos eishthriwn pou epithumei na agorasei o pelaths(keli [0]->zwnh A, [1]->zwnh B, ...)*/
		};

/*metablhtes genikou skopou*/
pthread_attr_t attr, attr2; /*orizoume to attribute tou thread */
pthread_mutex_t mutex_mem,mutex_bank,mutex_tel;/*Orizoume mutexes gia na kanoume blocking tmhmata kwdika wste na ekteleitai kathe fora auto to tmhma apo sugkekrimeno arithmo apo threads*/
pthread_cond_t cond_bank, cond_tel ;/*Orizoume ta condition variables pou tha xrhshmopoihthoun se sundiasmo me tous mutexes gia thn antikatastash twn semaphores*/
int count_bank=0, count_tel=0; /*metrhtes pou xrhsimopoiountai se sundiasmo me ta mutexes kai ta condition variables*/
struct timeval start1,start2,end1,end2; /*metablhtes pou xrhshmopoiountai gia thn xronometrhsh (telos kai arxh)*/
struct zones data;    /*Orizoume enan pointer pros struct zones data*/
int listenfd; /* Socket descriptor. */
int *ptr,rc,rc2,th_acc=0,t=0,T[100],i,count=0; /*diafores metablhtes tupou int*/

/*Oi 4 buffers pou akolouthoun einai mhnumata pou stelnei o server ws apanthseis ston client gia na enhmerwsei to xrhsth gia thn poreia ths krathshs.*/
char ok_buf[]="H krathsh oloklhrwthike epituxws!";
char notvalied_buf[]="H pistwtikh karta den einai egkurh.";
char noseats_buf[]="Den uparxoun diathesimes theseis gia thn sugkekrimenh zwnh.";
char full_buf[]="Den uparxoun KATHOLOU diathesimes theseis (to theatro exei gemisei).";

#endif
