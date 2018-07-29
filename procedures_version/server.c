#include "myheader.h" /* sumperulamvanoume to arxeio me tis bibliothhkes,ths dhlwseis statherwn ta structs kai tis global metablhtes*/
#include "serverfunctions.h" /* sumperulamvanoume to arxeio me tis sunarthseis pou xrhsimopoiei o server*/

int main( int argc, char **argv ) /*kurio programma*/
{
      	pid_t childpid1,childpid2;
      	socklen_t clilen;
       	struct sockaddr_un cliaddr, servaddr; /* Structs for the client and server socket addresses. */
	struct timeval start1,start2,end1,end2; /*metablhtes pou xrhshmopoiountai gia thn xronometrhsh (telos kai arxh)*/
	/*set the signals in min programm*/
       	signal( SIGCHLD, sig_chld ); /* Avoid "zombie" process generation. */
	signal(SIGINT, catch_int); /*Me auth thn entolh dinetai o xeirismos tou shmatos SIGINT(Ctrl+C) sth sunarthsh catch_int.*/
	signal(SIGALRM, catch_alarm); /* set a signal handler for ALRM signals */

/*======================Sockets===============================*/
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

/*==========================Shared Memory======================*/
	/*dhmiourgia shared memory theatrou*/
	shm_id=shmget(shm_key,shm_size,0600|IPC_CREAT); /*dhmiourgia shared memory me key to 5399 kai megethos iso me to megethos enos struct tupou zones.*/
	data=shmat(shm_id,NULL,0); /*O pointer data deixnei stin arxi tis shared memory.*/

/*============================================================*/

/*======================Semaphores============================*/

/*Dhmiourgia semaphores*/
/* my_sem1 --> xrhsimopoietai gia na prostateusei th shared memory apo koines eggrafes. */
	my_sem1=sem_open(SEM_NAME1,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR,1); /*mono mia diergasia mporei na tropopoihsei kathe fora dedomena sthn koinh mnhmh*/

	if (my_sem1==SEM_FAILED)
	{  
		printf("Could not open semaphore no1!\n"); 
		exit(1);
	}/*elegxos gia swsto anoigma ths semaphore*/

/* my_sem2 --> xrhsimopoietai giati uparxei periorimos thlefwnhtwn pou eksuphretoun tous pelates me orio 10. */
	my_sem2=sem_open(SEM_NAME2,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR,10); /*mono 10 thlefwnhtes einai diathesimoi*/

	if (my_sem2==SEM_FAILED)
	{  
		printf("Could not open semaphore no2!\n");
		exit(1);
	}/*elegxos gia swsto anoigma ths semaphore*/

/* my_sem3 --> xrhsimopoietai giati uparxei periorimos stis aithseis pou mporei na eksuphrethsei h trapeza me orio 10. */
	my_sem3=sem_open(SEM_NAME3,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR,4); /*mono 4 diergasies mporoun na briskontai sthn trapeza kathe xronikh stigmh*/

	if (my_sem3==SEM_FAILED)
	{  
		printf("Could not open semaphore no3!\n");
		exit(1);
	}/*elegxos gia swsto anoigma ths semaphore*/

/*===========================================================*/

/*======================arxikopoihseis============================*/

	/*arxikopoihsh tou pinaka metaforwn*/	
	for(i=0;i<100;i++) T[i]=0;

	/*arxikopoihsh tou pinaka me tis trexouses kleismenes theseis kai tou pinaka me ta eishthria pou epilegei o xrhsths*/
	for(i=0;i<4;i++)
	{
		data->c_s[i]=1; /*pinakas me tis trexouses kleismenes theseis*/
		app.tic_num[i]=0; /*pinaka me ta eishthria pou epilegei o xrhsths*/
	}

	/*arxikopoihsh tou pinaka me tis zwnes(thn megisth timh thesewn pou mporei na dexthei h kathe zwnh)*/
	data->zon[0]=100;
	data->zon[1]=130;
	data->zon[2]=180;
	data->zon[3]=230;

	/*arxikopoihsh tou pinaka me tis times gia kathe zwnh*/
	data->pr[0]=50;
	data->pr[1]=40;
	data->pr[2]=35;
	data->pr[3]=30;

	/*arxikopoihsh twn upoloipwn pediwn ths struct ths shared memory*/
	data->cust=0;
	data->comp_acc=0;
	data->notokord=0;
	data->orders=0;
	data->time1=0;
	data->time2=0;

	/*arxikopoihsh tou pointer gia na deixnei sto pedio company_account (comp_acc)*/
	ptr=&data->comp_acc;

	/*arxikopoihsh twn pediwn ths struct gia thn krathsh kathe pelath*/
	booking.pelatis_id=0;
	booking.kostos=0;
	booking.not_ok=0;
	booking.zwnh=0;
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
                            fprintf( stderr, "Accept Error\n" );
                            exit(0);
			}
              	}

              	childpid1 = fork(); /* Spawn a child. */ /*dhmiourgia neou thlefwnhth*/
		
              	if ( childpid1 == 0 ) 
		{ /* Child process. (process thlefwnhth)*/
			gettimeofday(&start1, NULL); /*arxizei h xronometrhsh apo thn stigmh pou o client stelnei to aithma tou*/
			sem_wait(my_sem2); /*o shmafore (thlefwnhtwn) anamenei kathws o thlefwnhths eksuphretei enan pelath*/
			gettimeofday(&end1, NULL); /*termatizei h xronometrhsh apo thn stigmh pou o server antapokrinetai sto aithma tou client*/
			write(connfd, &t, sizeof(int)); /*stelnetai mhnuma ston client oti h eksuphrethsh tou exei ksekinhsei*/
			data->time1+=((end1.tv_sec)-(start1.tv_sec)); /*ekxwrhsh tou parapanw xronou sthn metablhth time1 gia na upologistei meta o mesos xronos*/
			gettimeofday(&start2, NULL); /*arxizei h xronometrhsh apo thn stigmh pou o client arxizei na milaei me enan ekproswpo*/
                     	close( listenfd ); /* Close listening socket. */
			/*dhmiourgia neas shared memory metaksu diergasias thlefwnhth kai trapezas gia epibebaiwsh ths egkurothtas ths pistwtikhs kartas*/
			key_t shm_key2=getpid(); /*dhmiourgeitai mia shared memory me key iso me to pid ths ekastote diergasias.*/	
			size_t shm_size2=(sizeof(int)); /*megethos shared memory*/
			int shm_id2=shmget(shm_key2,shm_size2,0600|IPC_CREAT); 
			int *sm2;
			sm2=shmat(shm_id2,NULL,0); /*O pointer sm2 deixnei stin arxi tis shared memory.*/

			/*dhmiourgia semaphore gia shered memory no2!*/
			/* my_sem4 --> xrhsimopoietai gia na anamenei h diergasia tou thlefwnhth oso ginetai o elegxos gia thn egkurothta pistotikhs kartas kai na mhn paei na prospelasei ta dedomena ths shared memory pou dhmiourghthhke parapanw prin prolabei h trapeza na ta tropopoihsei. */
			sem_t *my_sem4;
			my_sem4=sem_open(SEM_NAME4,O_CREAT|O_RDWR,S_IRUSR|S_IWUSR,0);

			if (my_sem4==SEM_FAILED)  /*elegxos gia swsto anoigma ths semaphore*/
			{  
				printf("Could not open semaphore no4!\n");
				exit(1);
			}

                     	while ( read( connfd, &app, sizeof( app ) ) > 0 ) /*elegxos gia swsth klhsh ths read*/
			{						  /*diabasma tou aithmatos pou exei steilei o client*/
				childpid2 = fork(); /* Spawn a child. */ /*dhmiourgia 2 diergasiwn, mia gia ton thlefwnhth kai mia gia thn trapeza*/
              			if ( childpid2 == 0 ) 
					{ /* Child process*//*bank process */
                				sem_wait(my_sem3); /*o semaphore anamenei kathws h trapeza elegxei thn egkurothta ths pistwtikhs kartas*/
						printf("Please wait to check the validity of your credit card...\n");
						sleep(2); /*anamonh gia elegxo egkurothtas pistotikhs kartas*/
						int val = cr_crd_rand();  /*timh egkurothtas 'h mh ths pistwtikhs*/
						(*sm2)=val;
						sem_post(my_sem4); /*o semaphor ths shared memory gia thn pistotikh karta auksanetai kai twra h diergasia thlefwnhth mporei na diabasei ta dedomena auths ths shared memory*/
						sem_post(my_sem3); /*o semaphor den anamenei pleon giati h diergasia ths trapezas exei teliwsei*/
					}/*end of bank process*/
				else 
				{
					sem_wait(my_sem4); /*o semaphor anamenei kathws h diergasia trapezas elegxei thn egkurothta ths pistotikhs kartas*/
					printf("Please wait to find the seats you requested...\n");
					sleep(6); /*anamonh gia elegxo*/
					sem_wait(my_sem1); /*o semafore anamenei kathws koina dedomena tropopoiountai sthn shared memory tou theatrou*/
					data->orders++; /*oi sunolikes paragkelies auksanontai kata 1*/
					for(i=0;i<4;i++)
					{
						if(data->zon[i]-app.tic_num[i]<0)
						{
							booking.not_ok=2; /*elegxos gia periptwsh mh diathesimwn thesewn gia th sugkekrimenh zwnh*/
							break;
						}
						if(app.tic_num[i]!=0) booking.zwnh=i; /*krathsh ths zwnhs krathshs pou exei zhthsei kathe pelath*/
					}

					if((*sm2)==1) printf("H pistwtikh sas karta einai egkurh.\n"); /*emfanish mhnumatos gia egkurothta pistotikhs kartas*/

					if((*sm2)==0)
					{
						printf("H pistwtikh sas karta den einai egkurh.\n"); /*emfanish mhnumatos gia mh egkurothta pistotikhs kartas*/
						data->notokord++; /*oi apotuxhmenes paraggelies auksanontai kata 1*/
						write(connfd,&notvalied_buf,sizeof(notvalied_buf)); /*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
					}
					/*elegxos gia diathesimes theseis se olo to theatro*/
					else if((data->zon[0]==0)&&(data->zon[1]==0)&&(data->zon[2]==0)&&(data->zon[3]==0))
					{	
						printf("Den uparxoun KATHOLOU diathesimes theseis (to theatro exei gemisei).\n");
						data->notokord++; /*oi apotuxhmenes paraggelies auksanontai kata 1*/
						write(connfd,&full_buf,sizeof(full_buf)); /*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
						booking.not_ok=3; /*thetoume to pedio not_ok ths struct booking iso me 3 gia na dhlwsoume oti to theatro exei gemisei kai den exei katholou diathesimes theseis*/
						closing_server(); /*kaleitai h sunarthshs closing_server gia kleisimo tou server*/
					}
					else if(booking.not_ok==2) /*periptwsh mh diathesimwn thesewn gia th sugkekrimenh zwnh*/
					{
						printf("Den uparxoun diathesimes theseis gia thn sugkekrimenh zwnh.\n");
						data->notokord++; /*oi apotuxhmenes paraggelies auksanontai kata 1*/
						write(connfd,&noseats_buf,sizeof(noseats_buf)); /*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
					}
					else if(((*sm2)==1)&&(booking.not_ok==0)) /*periptwsh egkurhs pistotikhs kartas kai uparksh diathesimwn thesewn sto theatro*/
					{
						data->cust++; /*to sunolo twn pelatwn auksanetai kata 1*/
						for(i=0;i<4;i++) data->zon[i]-=app.tic_num[i]; /*afairesh twn diatheshmwn thesewn apo thn kathe zwnh*/
						booking.kostos=data->pr[booking.zwnh]*app.tic_num[booking.zwnh];/*euresh tou kostous ths krathshs tou kathe pelath*/
						data->comp_acc+=booking.kostos; /*aukshsh tou posou tou company_account kata to poso ths ekastote krathshs*/
						/*tupwsh ths timhs ths theshs ana zwnh*/
						printf("price of zone A per sit :%d\n",data->pr[0]);
						printf("price of zone B per sit :%d\n",data->pr[1]);
						printf("price of zone C per sit :%d\n",data->pr[2]);
						printf("price of zone D per sit :%d\n",data->pr[3]);
						/*tupwsh tou arithou eishthriwn ana zwnh pou zhththhkan apo ton client*/
						printf("arithmos eishthriwn zwnhs A :%d\n",app.tic_num[0]);
						printf("arithmos eishthriwn zwnhs B :%d\n",app.tic_num[1]);
						printf("arithmos eishthriwn zwnhs C :%d\n",app.tic_num[2]);
						printf("arithmos eishthriwn zwnhs D :%d\n",app.tic_num[3]);
						booking.pelatis_id=data->cust; /*anathesh timhs sto anagnwristiko krathshs*/
						/*tupwsh katallhlou mhnumatos gia thn proodo ths krathshs*/
						printf("H krathsh oloklhrwthhke epituxws!\n");
						printf("To anagnwristiko ths krathshs einai: %d\n",booking.pelatis_id);
						printf("to kostos sunallaghs einai :%d \nkai oi theseis sas einai :",booking.kostos);
						int c=0;/*arxikopoihsh tou counter gia ton arithmo twn eishthriwn tou kathe pelath*/
						if(booking.zwnh==0) /*periptwsh pou h zwnh pou epilexthhke einai h A*/
						{
							for(i=1;i<=app.tic_num[0];i++)
							{
								data->A[data->c_s[0]-1]=data->cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs A*/
								data->c_s[0]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh A (keli 0)*/
							}
							
							for(i=1;i<=100;i++)
							{
								if(booking.pelatis_id==data->A[i-1])
								{
									printf("A%d ",i); /*tupwsh twn thesewn tou pelath*/
									c++; /*aukshsh tou counter*/
								}
								if(c==app.tic_num[0]) break;
							}
						}
						if(booking.zwnh==1) /*periptwsh pou h zwnh pou epilexthhke einai h B*/
						{
							for(i=1;i<=app.tic_num[1];i++)
							{
								data->B[data->c_s[1]-1]=data->cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs B*/
								data->c_s[1]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh B (keli 1)*/
							}
							for(i=1;i<=130;i++)
							{
								if(booking.pelatis_id==data->B[i-1])
								{
									printf("B%d ",i); /*tupwsh twn thesewn tou pelath*/
									c++;  /*aukshsh tou counter*/
								}
								if(c==app.tic_num[1]) break;
							}
						}
						if(booking.zwnh==2) /*periptwsh pou h zwnh pou epilexthhke einai h C*/
						{
							for(i=1;i<=app.tic_num[2];i++)
							{
								data->C[data->c_s[2]-1]=data->cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs C*/
								data->c_s[2]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh C (keli 2)*/
							}
							for(i=1;i<=180;i++)
							{
								if(booking.pelatis_id==data->C[i-1])
								{
									printf("C%d ",i);/*tupwsh twn thesewn tou pelath*/
									c++; /*aukshsh tou counter*/
								}
								if(c==app.tic_num[2]) break;
							}
						}
						if(booking.zwnh==3) /*periptwsh pou h zwnh pou epilexthhke einai h D*/
						{
							for(i=1;i<=app.tic_num[3];i++)
							{
								data->D[data->c_s[3]-1]=data->cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs D*/
								data->c_s[3]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh C (keli 3)*/
							}
							for(i=1;i<=230;i++)
							{
								if(booking.pelatis_id==data->D[i-1])
								{
									printf("D%d ",i);/*tupwsh twn thesewn tou pelath*/
									c++; /*aukshsh tou counter*/
								}
								if(c==app.tic_num[3]) break;
							}
						} 
						printf("\n");
						write(connfd,&ok_buf,sizeof(ok_buf));/*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
					}
					gettimeofday(&end2, NULL); /*termatizei h xronometrhsh apo thn stigmh pou o client exei labei to mhnuma gia thn proodo ths krathshs kai termatizetai h klhsh*/
					data->time2+=((end2.tv_sec)-(start2.tv_sec)); /*ekxwrhsh tou parapanw xronou sthn metablhth time2 gia na upologistei meta o mesos xronos*/
					sem_post(my_sem1); /*o semaphor den anamenei pleon kai mporoun pleon na xrhsimopoioun to kommati ths shared memory tou theatrou kai alles dierasies*/
				}/*end of process no2(thlefwnhths)*//*end of else*/
			}/*end of while*/
			sem_post(my_sem2); /*o semaphor den anamenei pleon giati h diergasia tou thlefwnhth exei teleiwsei*/
			int unmalloc2 = shmdt(sm2); /*apodesmeush ths shared memory no2(shared memory gia karta)*/
			shmctl(shm_id2,IPC_RMID,NULL); /*svinoume th shared memory no2(shared memory gia elegxo ekurothtas pistotikhs kartas)*/
			sem_close(my_sem4); /*kleisimo tou semaphor my_sem4*/
			sem_unlink(SEM_NAME4); /*diagrafh tou semaphor my_sem4*/
                        exit(0); /* Terminate child process. */
		}/*end of child process no1*/ /*fork no1*/
		close(connfd); /* Parent closes connected socket */
	}/*end of for infinite loop*/	
}/*end of main*/
