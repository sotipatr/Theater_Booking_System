#include "myheader_th.h" /* sumperulambanoume to arxeio me tis bibliothhkes,ths dhlwseis statherwn ta structs kai tis global metablhtes*/
#include "serverfunctions_th.h" /* sumperulambanoume to arxeio me tis sunarthseis pou xrhsimopoiei o server*/

int main( int argc, char **argv ) /*kurio programma*/
{
      	pthread_t thread_tel;/*dhlwsh tou thread tou thlefwnhth*/
      	socklen_t clilen;
       	struct sockaddr_un cliaddr, servaddr; /* Structs for the client and server socket addresses. */

	/*set the signals in min programm*/
	signal(SIGINT, catch_int); /*Me auth thn entolh dinetai o xeirismos tou shmatos SIGINT(Ctrl+C) sth sunarthsh catch_int.*/
	signal(SIGALRM, catch_alarm); /* set a signal handler for ALRM signals */

/*======================Sockets===============================*/
	int *pointer;
	int connfd;
       	listenfd = socket( AF_LOCAL, SOCK_STREAM, 0 ); /* Create the server's endpoint */

	/* ATTENTION!!! THIS ACTUALLY REMOVES A FILE FROM YOUR HARD DRIVE!!! */
       	unlink( UNIXSTR_PATH ); /* Remove any previous socket with the same filename. */

       	bzero( &servaddr, sizeof( servaddr ) ); /* Zero all fields of servaddr. */
       	servaddr.sun_family = AF_LOCAL; /* Socket type is local (Unix Domain). */
       	strcpy( servaddr.sun_path, UNIXSTR_PATH ); /* Define the name of this socket. */

	/* Create the file for the socket and register it as a socket. */
       	bind( listenfd, ( struct sockaddr* ) &servaddr, sizeof( servaddr ) );

       	listen( listenfd, LISTENQ ); /* Create request queue. */
/*============================================================*/

/*==========================Attribute======================*/
	/* Initialize and set thread detached attribute in a joinable state*/ 
	pthread_attr_init(&attr); 
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
/*============================================================*/

/*==========================Mutexes======================*/
	/* Initialize mutexes objects */
	pthread_mutex_init(&mutex_mem, NULL);
	pthread_mutex_init(&mutex_tel, NULL);
	pthread_mutex_init(&mutex_bank, NULL);
/*============================================================*/

/*======================Condition Variables============================*/
	/* Initialize Condition Variables objects */
	pthread_cond_init (&cond_bank, NULL);
	pthread_cond_init (&cond_tel, NULL);
/*===========================================================*/

/*======================arxikopoihseis============================*/

	/*arxikopoihsh tou pinaka metaforwn*/	
	for(i=0;i<100;i++) T[i]=0;

	/*arxikopoihsh tou pinaka me tis trexouses kleismenes theseis */
	for(i=0;i<4;i++)
	{
		data.c_s[i]=1;
	}

	/*arxikopoihsh tou pinaka me tis zwnes(thn megisth timh thesewn pou mporei na dexthei h kathe zwnh)*/
	data.zon[0]=100;
	data.zon[1]=130;
	data.zon[2]=180;
	data.zon[3]=230;

	/*arxikopoihsh tou pinaka me tis times gia kathe zwnh*/
	data.pr[0]=50;
	data.pr[1]=40;
	data.pr[2]=35;
	data.pr[3]=30;

	/*arxikopoihsh twn upoloipwn pediwn ths struct ths shared memory*/
	data.cust=0;
	data.comp_acc=0;
	data.notokord=0;
	data.orders=0;
	data.time1=0;
	data.time2=0;

	/*arxikopoihsh tou pointer gia na deixnei sto pedio company_account (comp_acc)*/
	ptr=&data.comp_acc;

	pointer = malloc(sizeof(int));/*ginetai malloc ston pointer gia na desmeuthei xwros gia thn timh tou socket pou tha labei meta*/

/*===========================================================*/

	alarm(30);
	printf("The server is running...\n");
       	for ( ; ; ) /*enter to infinite loop*/
	{
             	clilen = sizeof( cliaddr );

		/* Copy next request from the queue to connfd and remove it from the queue. */
              	connfd = accept( listenfd, ( struct sockaddr * ) &cliaddr, &clilen );
		/*elegxoume an egine swsta h klhsh ths accept kai an oxi tupwnoume katallhlo mhnuma*/
              	if ( connfd < 0 ) 
		{
                     	if ( errno == EINTR ) /* Something interrupted us. */
                            	continue; /* Back to for()... */
                     	else 
			{
			    perror("for: accept" );
                            fprintf( stderr, "Accept Error\n" );
                            exit(0);
			}
              	}
		*pointer=connfd; /*o pointer pairnei thn timh tou connfd gia thn perasei san orisma sthn sunarthsh tou thread*/
		/*dhmiourgia tou thread thlefwnhths me thn xrhsh ths pthread_create*/
		rc = pthread_create(&thread_tel, &attr, tel_thread, (void *) pointer);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc); 		
			exit(-1); 
		} 
		/* Free attribute and wait for the other threads */ 
	}/*end of for infinite loop*/
	free(pointer);
	/*katastrofh twn mutexes, attributes kai condition variables*/
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&mutex_tel);
  	pthread_cond_destroy(&cond_tel);
	pthread_mutex_destroy(&mutex_bank);
  	pthread_cond_destroy(&cond_bank);
	pthread_mutex_destroy(&mutex_mem);	
}/*end of main*/
