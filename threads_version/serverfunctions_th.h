#ifndef __SERVER_FUNCTIONS__
#define __SERVER_FUNCTIONS__

/*sunarthseis pou xrhshmopoiounte ston server*/

/*sunarthsh gia egkurothta pistwtikhs kartas me tuxaio tropo*/
int cr_crd_rand()
{
	srand(pthread_self());
	int i=rand()%9;
	if (i==0) return 0; /*10% apotuxia (mh egkurh pistwtikh karta)*/
	else return 1; /* 90% epituxia (egkurh pistwtikh karta)*/
}

void closing_server();

/*sunarthsh tou thread ths trapezas*/
void *bank_thread(void *null) 
{
	printf("Please wait to check the validity of your credit card...\n");
	sleep(2); /*anamonh gia elegxo egkurothtas pistotikhs kartas*/
	pthread_mutex_lock(&mutex_bank); /*o mutex "kleidwnei" gia na elegxthei posa threads "briskontai" sthn trapeza*/
   	if (count_bank == 4) {  /*elegxetai mesw twn condition variables posa threads einai mesa se auto to block kwdika kai an den einai epitreptos o arithmos ta threads anamenoun*/
      	pthread_cond_wait(&cond_bank, &mutex_bank); /*ginetai wait twn thread pou den mporoun na eksuphreththoun afou den uparxoun arketoi diathesimoi "trapezikoi upallhloi"*/
   	}
	count_bank++; /*afou einai epitreptos o arithmos twn threads pou briskontai sthn trapeza tote to neo aitoumeno thread mporei na "mpei" kai etsi auksanetai o metrhths ths trapezas*/
    	pthread_mutex_unlock(&mutex_bank); /*o mutex "ksekleidwnei" afou exei oloklhrwthei o elegxos*/
	int *p; /*dhlwnetai enas pointer opou tha parei timh apo thn sunarthsh tuxaias apanthshs gia thn egkurwthta kartas*/
	p = malloc(sizeof(int)); /*inetai desmeush enos int ston pointer*/
	*p = cr_crd_rand();/*to periexomeno tou pointer pairnei thn timh pou epistrefei h sunarthsh*/
	pthread_mutex_lock(&mutex_bank); /*o mutex den "kleidwnei" pali gia na apodesmeuthei o poros ths trapezas kai na eksuphreththei h epomen h aithsh*/
	count_bank--;/*afou h ergasia tou thread oloklhrwthhke o metrhths meiwnetai gia na mporesei na "mpei" kainourio thread pros eksuphrethsh*/
      	pthread_cond_signal(&cond_bank); /*ginetai "afupnhsh" enos apo ta thread pou exoun ginei wait giati pleon mporoun na eksuphreththoun*/
	pthread_mutex_unlock(&mutex_bank); /*o mutex den "ksekleidwnei" giati o elegxos kai to thread ths trapezas exei teliwsei*/
  	pthread_exit(p);/* Terminate thread.*/ /*h timh tou p pernaei apo to thread ths trapezas sto thread tou thlefwnhths gia na proskomistei h timh egkurothtas wste na proxwrisei h diadikasia ths krathshs*/
}

void *tel_thread( void *acc_des) 
{
	int as=*((int *)acc_des); /*ginetai cast tou pointer (pou phre h sunarthsh san orisma) apo (void*) se (int*) kai to periexomeno tou apothhkeuetai se mia metabphth int outws wste oi sunarthseis read(), write() kai close() na xrhshmopoihsoun auth thn timh gia tis leitourgies tou*/
	struct aithsh app; /*dhmiourgoume mia struct tupou aithsh pou apothhkeuoume ta dedomena kathe aithshs*/
	struct krathsh booking;  /*dhmiourgoume mia struct tupou krathsh gia na kratame ta dedomena kathe krathshs*/

	/*arxikopoihseis*/
	/*arxikopoihsh twn pediwn ths struct gia thn krathsh kathe pelath*/
	booking.pelatis_id=0;
	booking.kostos=0;
	booking.not_ok=0;
	booking.zwnh=0;

	/*arxikopoihsh tou pinaka me ta eishthria pou epilegei o xrhsths*/
	for(i=0;i<4;i++)
	{
		app.tic_num[i]=0; 
	}
	gettimeofday(&start1, NULL); /*arxizei h xronometrhsh apo thn stigmh pou o client stelnei to aithma tou*/
	pthread_mutex_lock(&mutex_tel); /*o mutex (thlefwnhtwn) "kleidwnei" gia na elegxthei posa threads thlefwnhtwn exoun dhmiourghthei*/
   	if (count_tel == 10) {  /*elegxetai mesw twn condition variables posa threads einai mesa se auto to block kwdika kai den einai epitreptos o arithmos ta threads anamenoun*/
      	pthread_cond_wait(&cond_tel ,&mutex_tel);/*ginetai wait twn thread pou den mporoun na eksuphreththoun afou den uparxoun arketoi diathesimoi thlefwnhtes*/
   	}
	count_tel++;/*afou einai epitreptos o arithmos twn threads-thlefwnhtwn tote to neo aitoumeno thread mporei na "mpei" kai etsi auksanetai o metrhths twn thlefwnhtwn*/
    	pthread_mutex_unlock(&mutex_tel); /*o mutex den "ksekleidwnei" afou o elegxos exei teliwsei*/
	gettimeofday(&end1, NULL); /*termatizei h xronometrhsh apo thn stigmh pou o server antapokrinetai sto aithma tou client*/
	write(as, &t, sizeof(int)); /*stelnetai mhnuma ston client oti h eksuphrethsh tou exei ksekinhsei*/
	data.time1+=((end1.tv_sec)-(start1.tv_sec)); /*ekxwrhsh tou parapanw xronou sthn metablhth time1 gia na upologistei meta o mesos xronos*/
	gettimeofday(&start2, NULL); /*arxizei h xronometrhsh apo thn stigmh pou o client arxizei na milaei me enan ekproswpo*/

        while ( read( as, &app, sizeof( app ) ) > 0 ) /*elegxos gia swsth klhsh ths read*/
		{		/*diabasma tou aithmatos pou exei steilei o client*/
				pthread_t thread_bank;  /*dhlwsh tou thread tou trapezas*/
				pthread_attr_init(&attr2); /*arxikopoioume to neo attribute*/
				pthread_attr_setdetachstate(&attr2, PTHREAD_CREATE_JOINABLE); /*thetoume to to thread se katastash opou na mporei na ginei "join"*/
				rc2 = pthread_create(&thread_bank, &attr2, bank_thread, NULL); /*ekteleitai h pthread_create gia th dhmiourgia neou thread-trapezas opou auto ektelei th sunarthsh ths trapezas*/
				if (rc2) {  /*ginetai elegxos gia to an epistrafhke egkurh timh meta thn klhsh ths pthread_create*/
						printf("ERROR: return code from pthread_create() is %d\n", rc2); 		
						exit(-1);
				}
				void *status;/*dhlwnetai enas pointer ston opoio tha epistrafei h timh egkurothtas kartas apo to thread ths trapezas*/
				int jc=pthread_join(thread_bank, &status);/*to thread tou thlefwnhth anamenei kathws to thread ths trapezas na oloklhrwthei me thn xrhsh ths pthread_join*/
	        		if (jc) {   /*ginetai elegxos gia to an epistrafhke egkurh timh meta thn klhsh ths join*/
	            				printf("ERROR: return code from pthread_join() is %d\n", jc);
	            				exit(-1);
	        		}
				int value=*((int *)status);/*h timh pou perasthke apo thn trapezas ston pointer "status" apothhkeuetai afou ginei cast tou pointer se (int*) sthn metablhth "value" gia na xrhshmopoihthei pio katw gia elgxo*/
				free(status); /*eleutherwnetai o pointer "status" pou exei desmeuthei gia thn lhpsh ths timhs apo to thread ths trapezas*/
				printf("Please wait to find the seats you requested...\n");
				sleep(6); /*anamonh gia elegxo*/
				pthread_mutex_lock(&mutex_mem); /*o mutex "kleidwnei" kathws koina dedomena tropopoiountai sth mnhmh*/
				data.orders++; /*oi sunolikes paragkelies auksanontai kata 1*/
				for(i=0;i<4;i++)
					{
						if(data.zon[i]-app.tic_num[i]<0)
						{
							booking.not_ok=2; /*elegxos gia periptwsh mh diathesimwn thesewn gia th sugkekrimenh zwnh*/
							break;
						}
						if(app.tic_num[i]!=0) booking.zwnh=i; /*krathsh ths zwnhs krathshs pou exei zhthsei kathe pelath*/
					}
					
					if(value==1) printf("H pistwtikh sas karta einai egkurh.\n"); /*emfanish mhnumatos gia egkurothta pistotikhs kartas*/

					if(value==0)
					{
						printf("H pistwtikh sas karta den einai egkurh.\n"); /*emfanish mhnumatos gia mh egkurothta pistotikhs kartas*/
						data.notokord++; /*oi apotuxhmenes paraggelies auksanontai kata 1*/
						write(as,&notvalied_buf,sizeof(notvalied_buf)); /*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
					}
					/*elegxos gia diathesimes theseis se olo to theatro*/
					else if((data.zon[0]==0)&&(data.zon[1]==0)&&(data.zon[2]==0)&&(data.zon[3]==0))
					{	
						printf("Den uparxoun KATHOLOU diathesimes theseis (to theatro exei gemisei).\n");
						data.notokord++; /*oi apotuxhmenes paraggelies auksanontai kata 1*/
						write(as,&full_buf,sizeof(full_buf)); /*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
						booking.not_ok=3; /*thetoume to pedio not_ok ths struct booking iso me 3 gia na dhlwsoume oti to theatro exei gemisei kai den exei katholou diathesimes theseis*/
						closing_server(); /*kaleitai h sunarthshs closing_server gia kleisimo tou server*/
					}
					else if(booking.not_ok==2) /*periptwsh mh diathesimwn thesewn gia th sugkekrimenh zwnh*/
					{
						printf("Den uparxoun diathesimes theseis gia thn sugkekrimenh zwnh.\n");
						data.notokord++; /*oi apotuxhmenes paraggelies auksanontai kata 1*/
						write(as,&noseats_buf,sizeof(noseats_buf)); /*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
					}
					else if((value==1)&&(booking.not_ok==0)) /*periptwsh egkurhs pistotikhs kartas kai uparksh diathesimwn thesewn sto theatro*/
					{
						data.cust++; /*to sunolo twn pelatwn auksanetai kata 1*/
						for(i=0;i<4;i++) data.zon[i]-=app.tic_num[i]; /*afairesh twn diatheshmwn thesewn apo thn kathe zwnh*/
						booking.kostos=data.pr[booking.zwnh]*app.tic_num[booking.zwnh];/*euresh tou kostous ths krathshs tou kathe pelath*/
						data.comp_acc+=booking.kostos; /*aukshsh tou posou tou company_account kata to poso ths ekastote krathshs*/
						/*tupwsh ths timhs ths theshs ana zwnh*/
						printf("price of zone A per sit :%d\n",data.pr[0]);
						printf("price of zone B per sit :%d\n",data.pr[1]);
						printf("price of zone C per sit :%d\n",data.pr[2]);
						printf("price of zone D per sit :%d\n",data.pr[3]);
						/*tupwsh tou arithou eishthriwn ana zwnh pou zhththhkan apo ton client*/
						printf("arithmos eishthriwn zwnhs A :%d\n",app.tic_num[0]);
						printf("arithmos eishthriwn zwnhs B :%d\n",app.tic_num[1]);
						printf("arithmos eishthriwn zwnhs C :%d\n",app.tic_num[2]);
						printf("arithmos eishthriwn zwnhs D :%d\n",app.tic_num[3]);
						booking.pelatis_id=data.cust; /*anathesh timhs sto anagnwristiko krathshs*/
						/*tupwsh katallhlou mhnumatos gia thn proodo ths krathshs*/
						printf("H krathsh oloklhrwthhke epituxws!\n");
						printf("To anagnwristiko ths krathshs einai: %d\n",booking.pelatis_id);
						printf("to kostos sunallaghs einai :%d \nkai oi theseis sas einai :",booking.kostos);
						int c=0;/*arxikopoihsh tou counter gia ton arithmo twn eishthriwn tou kathe pelath*/
						if(booking.zwnh==0) /*periptwsh pou h zwnh pou epilexthhke einai h A*/
						{
							for(i=1;i<=app.tic_num[0];i++)
							{
								data.A[data.c_s[0]-1]=data.cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs A*/
								data.c_s[0]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh A (keli 0)*/
							}
							
							for(i=1;i<=100;i++)
							{
								if(booking.pelatis_id==data.A[i-1])
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
								data.B[data.c_s[1]-1]=data.cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs B*/
								data.c_s[1]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh B (keli 1)*/
							}
							for(i=1;i<=130;i++)
							{
								if(booking.pelatis_id==data.B[i-1])
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
								data.C[data.c_s[2]-1]=data.cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs C*/
								data.c_s[2]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh C (keli 2)*/
							}
							for(i=1;i<=180;i++)
							{
								if(booking.pelatis_id==data.C[i-1])
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
								data.D[data.c_s[3]-1]=data.cust; /*eisagwgh tou id tou pelath ston pinaka me tis theseis ths zwnhs D*/
								data.c_s[3]++; /*aukshsh tou pinka me tis trexouses theseis sthn zwnh C (keli 3)*/
							}
							for(i=1;i<=230;i++)
							{
								if(booking.pelatis_id==data.D[i-1])
								{
									printf("D%d ",i);/*tupwsh twn thesewn tou pelath*/
									c++; /*aukshsh tou counter*/
								}
								if(c==app.tic_num[3]) break;
							}
						} 
						printf("\n");
						write(as,&ok_buf,sizeof(ok_buf));/*apostolh katallhlou mhnumatos ston client gia na enhmerwthei gia tn katastash ths paraggelias*/
					}
					gettimeofday(&end2, NULL); /*termatizei h xronometrhsh apo thn stigmh pou o client exei labei to mhnuma gia thn proodo ths krathshs kai termatizetai h klhsh*/
					data.time2+=((end2.tv_sec)-(start2.tv_sec)); /*ekxwrhsh tou parapanw xronou sthn metablhth time2 gia na upologistei meta o mesos xronos*/
					pthread_mutex_unlock(&mutex_mem); /*o mutex "ksekleidwnei" kai mporoun pleon na xrhsimopoioun to kommati ths mnhmhs kai alla threads*/
				
			}/*end of while*/
			pthread_mutex_lock(&mutex_tel); /*o mutex den "kleidwnei" pali gia na apodesmeuthei o poros tou thlefwnhth kai na eksuphreththei h epomen h aithsh*/
			count_tel--;/*afou h ergasia tou thread oloklhrwthhke o metrhths meiwnetai gia na mporesei na "mpei" kainourio thread pros eksuphrethsh*/
      			pthread_cond_signal(&cond_tel);/*ginetai "afupnhsh" enos apo ta thread pou exoun ginei wait giati pleon mporoun na eksuphreththoun*/
			pthread_mutex_unlock(&mutex_tel); /*o mutex den "ksekleidwnei" giati o elegxos kai to thread tou thlefwnhth exei oloklhrwthei*/
			/* Clean up and exit */
			/* Free attribute and wait for the other threads */
  			pthread_attr_destroy(&attr2);
			close(as); /* Parent closes connected socket */
  			pthread_exit(NULL);/* Terminate thread. */
}

/*sunarthsh gia kleisimo tou server*/
void closing_server()
{
	/*ektupwsh tou planou twn thesewn tou theatrou!*/
	printf("\nZwnh A: [");
	int j;
	for(j=0;j<100;j++)
	{
		if(data.A[j]!=0) printf("P%d  ",data.A[j]); /*tupwsh twn thesewn ths zwnhs A*/
		else break;
	}
	printf("]\n");

	printf("Zwnh B: [");
	for(j=0;j<130;j++)
	{
		if(data.B[j]!=0) printf("P%d  ",data.B[j]); /*tupwsh twn thesewn ths zwnhs B*/
		else break;
	}
	printf("]\n");

	printf("Zwnh C: [");
	for(j=0;j<180;j++)
	{
		if(data.C[j]!=0) printf("P%d  ",data.C[j]); /*tupwsh twn thesewn ths zwnhs C*/
		else break;
	}
	printf("]\n");

	printf("Zwnh D: [");
	for(j=0;j<230;j++)
	{
		if(data.D[j]!=0) printf("P%d  ",data.D[j]); /*tupwsh twn thesewn ths zwnhs D*/
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
	printf("To teliko poso pou sugkentrwthhke apo thn pwlhsh twn eishthriwn einai:%d eurw.\n",th_acc); /*tupwsh tou telikou posou pou sugkentrwthhke*/
	printf("Oi sunolikes paraggelies einai:%d.\n",data.orders);  /*tupwsh twn sunolikwn paraggeliwn*/
	printf("Oi apotuxhmenes paraggelies einai:%d.\n",data.notokord); /*tupwsh twn apotuximenwn paraggeliwn*/
	float y;
	if(data.orders==0) y=0; /*elegxos an den exou ginei katholou paraggelies*/
	else y=((data.notokord)*100)/data.orders;
	printf("To pososto apotuximenwn paraggeliwn se sxesh\n me tis sunolikes paraggelies einai:%f tois ekato.\n",y);  /*tupwsh tou posostou twn apotuximenwn paraggeliwn se sxesh me tis sunolikes (apotuximenes/sunolikes)*/
	printf("O mesos xronos anamonhs twn paraggeliwn htan:%f sec.\n",data.time1/data.orders);
	printf("O mesos xronos eksuphrethshs twn paraggeliwn htan:%f sec.\n",data.time2/data.orders);
	close (listenfd); /*kleisimo tou socket*/
	/* Clean up and exit */
	/*katastrofh twn mutexes, attributes kai condition variables*/
	pthread_attr_destroy(&attr2);
  	pthread_mutex_destroy(&mutex_tel);
	pthread_mutex_destroy(&mutex_bank);
	pthread_mutex_destroy(&mutex_mem);
  	pthread_cond_destroy(&cond_bank);
  	pthread_cond_destroy(&cond_tel);
	unlink(UNIXSTR_PATH); /*Remove any previous socket with the same filename. */
	printf( "Server terminated.\n"); /*sxetiko mhnuma gia ton termatismo tou server*/
	exit(0);
}

/*===========================*signal handlers *=============================*/

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
