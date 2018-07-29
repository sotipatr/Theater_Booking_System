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
#include <semaphore.h>/*xrhsimh gia ton orismo twn shmaforwn*/
#include <sys/stat.h> /* diaxeirish signals */
#include <signal.h> /*diaxeirish signals */
#include <sys/wait.h> /*diaxeirish shmaforou*/
#include <errno.h> /*xrhsimopoieitai gia thn perror */ /* for the EINTR constant */
#include <fcntl.h> /*xrhsimopoieitai sthn dhmiourgia ths shmaforou */
#include <time.h> /* aparaithth gia na pairnoume to xrono se kapoia stigmh */
#include <sys/un.h> /* for Unix domain sockets */
#include <sys/time.h> /*xrhshmopoieitai gia thn xronometrhsh*/

/*Dhlwsh statherwn*/
#define UNIXSTR_PATH "/tmp/unix.str"
#define LISTENQ 20 // Size of the request queue.
#define SEM_NAME1 "sem_mem"
#define SEM_NAME2 "sem_thl"
#define SEM_NAME3 "sem_bank"
#define SEM_NAME4 "card"

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
sem_t *my_sem1,*my_sem2,*my_sem3; /*Orizoume pointers gia thn dhmiourgia twn semaphors*/
struct aithsh app; /*dhmiourgoume mia struct tupou aithsh pou apothhkeuoume ta dedomena kathe aithshs*/
struct krathsh booking;  /*dhmiourgoume mia struct tupou krathsh gia na kratame ta dedomena kathe krathshs*/
struct zones *data;    /*Orizoume enan pointer pros struct zones data*/
int listenfd, connfd; /* Socket descriptors. */
int *ptr,shm_id,unmalloc,th_acc=0,t=0,T[100],i,count=0/**/,value1,value2,value3/**/; /*diafores metablhtes tupou int*/
size_t shm_size=(sizeof(struct zones)); /*megethos shared memory*/
key_t shm_key=5399; /*to key gia to orima sthn dhmiourgia ths shared memory gia tis zwnes

/*Oi 4 buffers pou akolouthoun einai mhnumata pou stelnei o server ws apanthseis ston client gia na enhmerwsei to xrhsth gia thn poreia ths krathshs.*/
char ok_buf[]="H krathsh oloklhrwthike epituxws!";
char notvalied_buf[]="H pistwtikh karta den einai egkurh.";
char noseats_buf[]="Den uparxoun diathesimes theseis gia thn sugkekrimenh zwnh.";
char full_buf[]="Den uparxoun KATHOLOU diathesimes theseis (to theatro exei gemisei).";

#endif
