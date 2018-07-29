/* Dhlwsh vivliothikwn*/
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h> /* basic socket definitions */
#include <sys/types.h> /* basic system data types */
#include <sys/un.h> /* for Unix domain sockets */
#include <stdlib.h>				
#include <string.h>
#include <sys/stat.h> /* diaxeirish signals */
#include <signal.h> /*diaxeirish signals */
#include <time.h> /* aparaithth gia na pairnoume to xrono se kapoia stigmh */
#include <sys/time.h> /*xrhshmopoieitai gia thn xronometrhsh*/

/*Dhlwsh statherwn*/
#define UNIXSTR_PATH "/tmp/unix.str"

/*orismos tou struct gia thn aithsh tou client ston server*/
struct aithsh{   
		int tic_num[4];  /*arithmos eishthriwn pou epithumei na agorasei o pelaths*/
		}app;

/*sunarthsh gia random epilogh zwnhs*/
int rand_select()
{
	srand(getpid());
	int i=rand()%9;
	if (i==0) return 0; /*10% pithanothta na epilegei h zwnh A*/
	else if (i==1||i==2) return 1; /*20% pithanothta na epilegei h zwnh B*/
	else if (i>=3&&i<=5) return 2; /*30% pithanothta na epilegei h zwnh C*/
	else if (i>=6&&i<=9) return 3; /*40% pithanothta na epilegei h zwnh D*/
}

/*signal handler gia tupwsh tou mhnumatos sugnwmhs*/
void catch_alarm(int sig_num)
{
	/*reset the signal handler*/
	signal(SIGALRM, catch_alarm);
 	printf("Sas zhtoume sugnwmh gia thn anamonh.\n");
	printf("Briskeste se grammh protereothtas.\n");
	printf("Suntoma tha eksuphreththeitai apo enan apo tous thlefwnhtes mas.\n");
	printf("Parakalw perimenete...\n\n");
	alarm(10);

}

int main( int argc, char **argv )
{
	/*set the signal handler*/
	signal(SIGALRM, catch_alarm);
	/*metablhtes kuriou programmatos*/
	int sockfd;
       	struct sockaddr_un servaddr; /* Struct for the server socket address. */
       	int pid;
	int zonh,i,t=0;
	char buf[128];

       	sockfd = socket( AF_LOCAL, SOCK_STREAM, 0 ); /* Create the client's endpoint. */

       	bzero( &servaddr, sizeof( servaddr ) ); /* Zero all fields of servaddr. */
       	servaddr.sun_family = AF_LOCAL; /* Socket type is local (Unix Domain). */
       	strcpy( servaddr.sun_path, UNIXSTR_PATH ); /* Define the name of this socket. */

	/* Connect the client's and the server's endpoint. */
	/*Me thn connect sundeetai sto socket(praktika stelnei mia aithsh) gia na epikoinwnei me to server*/
       	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	for(i=0;i<4;i++) app.tic_num[i]=0; /*arxikopoihsh tou pinaka me ta eishthria*/
	zonh=rand_select();  /*klhsh sunarthshs gia tuxaia euresh zwnhs*/
	srand(getpid()); /*kaleitai h srand gia na mhn epanalambanontai oi times pou mas dinei h sunarthsh rand*/
	app.tic_num[zonh]=(rand()%3)+1; /*bazoume sthn tuxaia thesh(zwnh) tou pinaka me ta eishthria apo ena mexri 4 eishthria me tuxaio 						tropo*/
	printf("client number of seats in every zone:\n");
	for(i=0;i<4;i++) printf("%d\n",app.tic_num[i]); /*tupwnoume tis theseis kai ta eishthria pou zhthse o pelaths se kathe zwnh(thesh tou 								pianaka)*/
        write( sockfd, &app , sizeof( app ) );  /*stelnoume thn aithsh (struct) ston server mesw tous socket sockfd*/
	alarm(10);  /*arxizei h xronometrhsh mexri na antapokrithei o server sto aithma mas*/
	read (sockfd, &t, sizeof(int));  /*ginetai read tou mhnhmatos pou stelnei o server oti o pelaths eksuphreteitai*/
	alarm(0);  /*to xronometro stamataei edw mias kai o pelaths eksuphreteitai*/	
	printf("Please wait to check the validity of your credit card and find the seats you requested...\n");
	read(sockfd,&buf,sizeof(buf));  /*ginetai read tou mhnumatos tou server gia thn proodo ths krathshs*/
	printf("%s\n",buf); /*to mhnuma tou server tupwnetai*/
       	close( sockfd );  /*ginetai kleishmo tou socket*/
	exit(0);/*Terminate process*/
}
