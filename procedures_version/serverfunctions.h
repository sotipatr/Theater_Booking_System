#ifndef __SERVER_FUNCTIONS__
#define __SERVER_FUNCTIONS__

/*sunarthseis pou xrhshmopoiounte ston server*/

/*sunarthsh gia egkurothta pistwtikhs kartas me tuxaio tropo*/
int cr_crd_rand()
{
	srand(getpid());
	int i=rand()%9;
	if (i==0) return 0; /*10% apotuxia (mh egkurh pistwtikh karta)*/
	else return 1; /* 90% epituxia (egkurh pistwtikh karta)*/
}

/*sunarthsh gia kleisimo tou server*/
void closing_server()
{
	/*ektupwsh tou planou twn thesewn tou theatrou!*/
	printf("\nZwnh A: [");
	int j;
	for(j=0;j<100;j++)
	{
		if(data->A[j]!=0) printf("P%d  ",data->A[j]); /*tupwsh twn thesewn ths zwnhs A*/
		else break;
	}
	printf("]\n");

	printf("Zwnh B: [");
	for(j=0;j<130;j++)
	{
		if(data->B[j]!=0) printf("P%d  ",data->B[j]); /*tupwsh twn thesewn ths zwnhs B*/
		else break;
	}
	printf("]\n");

	printf("Zwnh C: [");
	for(j=0;j<180;j++)
	{
		if(data->C[j]!=0) printf("P%d  ",data->C[j]); /*tupwsh twn thesewn ths zwnhs C*/
		else break;
	}
	printf("]\n");

	printf("Zwnh D: [");
	for(j=0;j<230;j++)
	{
		if(data->D[j]!=0) printf("P%d  ",data->D[j]); /*tupwsh twn thesewn ths zwnhs D*/
		else break;
	}
	printf("]\n");
	printf("O pinakas me tis metafores twn poswn (se eurw) \n"); 
	printf("pou eginan apo ton logariasmo ths etairias ston \n");
	printf("logariasmo tou theatrou einai o parakatw:\n");
	printf("[");
	for(j=0;j<100;j++)
	{
		if(T[j]!=0) printf("%d  ",T[j]); /*tupwsh tou pinaka metaforwn*/
		else break;
	}
	printf("]\n");
	printf("To teliko poso pou sugkentrwthhke apo thn pwlhsh twn "); /*tupwsh tou telikou posou pou sugkentrwthhke*/
	printf("eishthriwn einai:%d eurw.\n",th_acc);
	printf("Oi sunolikes paraggelies einai:%d.\n",data->orders);  /*tupwsh twn sunolikwn paraggeliwn*/
	printf("Oi apotuxhmenes paraggelies einai:%d.\n",data->notokord); /*tupwsh twn apotuximenwn paraggeliwn*/
	printf("To pososto apotuximenwn paraggeliwn se sxesh\n"); 
	float y=((data->notokord)*100)/data->orders;     
 	printf("me tis sunolikes paraggelies");
	printf(" einai:%f tois ekato.\n",y);  /*tupwsh tou posostou twn apotuximenwn paraggeliwn se sxesh metis sunolikes (apotuximenes/sunolikes)*/
	printf("O mesos xronos anamonhs twn paraggeliwn htan:%f sec.\n",data->time1/data->orders);
	printf("O mesos xronos eksuphrethshs twn paraggeliwn htan:%f sec.\n",data->time2/data->orders);
	close (listenfd); /*kleisimo tou socket*/

        unmalloc = shmdt(data); /*apodesmeuoume thn koinh mnhmh*/
	
	if(unmalloc == -1)  /*elegxoume an egine swsta h apeleutherwsh kai an oxi tupwnoume katallhlo mhnuma*/
 	{
		printf( " Shared memory could not be detached");
		exit(1);
	}

	shmctl(shm_id,IPC_RMID,NULL); /*svinoume thn koinh mnhmh*/ 
	sem_close(my_sem1); /*kleinoume thn shmaforo my_sem1*/
	sem_close(my_sem2); /*kleinoume thn shmaforo my_sem2*/
	sem_close(my_sem3); /*kleinoume thn shmaforo my_sem3*/
	sem_unlink(SEM_NAME1); /*diagrafoume thn shmaforo my_sem1*/
	sem_unlink(SEM_NAME2); /*diagrafoume thn shmaforo my_sem2*/
	sem_unlink(SEM_NAME3); /*diagrafoume thn shmaforo my_sem3*/
	unlink(UNIXSTR_PATH); /*Remove any previous socket with the same filename. */
	
	printf( "Server terminated.\n"); /*sxetiko mhnuma gia ton termatismo tou server*/
	exit(0);
}

/*===========================*signal handlers *=============================*/

/*The use of this functions avoids the generation of "zombie" processes.*/
void sig_chld( int signo )
{
       	pid_t pid;
       	int stat;
	while ( ( pid = waitpid( -1, &stat, WNOHANG ) ) > 0 ) 
	{
              	printf( "Child %d terminated.\n", pid );
       	}
}

/* define a signal handler to terminate the server. */
void catch_int(int sig_num)
{
 	signal(SIGINT, catch_int); /*re-set the signal handler again to catch_int, for next time*/
	closing_server();
}

/* define an alarm signal handler. */
void catch_alarm(int sig_num)
{
	signal(SIGALRM, catch_alarm); /*reset the signal handler*/
	th_acc+=(*ptr); /*ginetai h metafora twn poswn apo to company_account sto theater_account*/
	T[count]=(*ptr); /*to poso pou metaferetai sto theater_account eisagetai ston pinaka metaforwn*/
	count++; /*o deikths pou xrhshmopoihtai gia na gemisei pinakas metaforwn stadiaka auksanetai*/
	*ptr=0;  /*o deikths pou deixnei sto company_account mhdenizetai(mhdenizetai to periexomeno ths metablhths pou deixei o deikths) etsi wste na dexthei th nea timh mexri na ksanaklhthei o handler*/
	alarm(30); /*stelnoume shma gia na ksekinhsei h xronometrhsh ksana*/
}

/*============================================================*/

#endif
