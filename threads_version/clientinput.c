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
	int zonh,i,t=0,tickets=0;
	char buf[128] , ch;

       	sockfd = socket( AF_LOCAL, SOCK_STREAM, 0 ); /* Create the client's endpoint. */

       	bzero( &servaddr, sizeof( servaddr ) ); /* Zero all fields of servaddr. */
       	servaddr.sun_family = AF_LOCAL; /* Socket type is local (Unix Domain). */
       	strcpy( servaddr.sun_path, UNIXSTR_PATH ); /* Define the name of this socket. */

	/* Connect the client's and the server's endpoint. */
	/*Me thn connect sundeetai sto socket(praktika stelnei mia aithsh) gia na epikoinwnei me to server*/
       	connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	for(i=0;i<4;i++) app.tic_num[i]=0; /*arxikopoihsh tou pinaka me ta eishthria*/
	printf("Select zone: A or B or C or D (please write whith capital letters):\n");
	scanf("%c", &ch);  /*lhpsh epiloghs tou xrhsth gia to poia zwnh thelei*/
	while((ch!='A')&&(ch!='B')&&(ch!='C')&&(ch!='D')) /*elegxos egkurothtas epiloghs tou xrhsth gia epilogh zwnhs*/
	{
		printf("Your choice is not valid.Please try again\n");
		scanf("%c", &ch); /*lhpsh epiloghs tou xrhsth ksana*/
	}
	printf("Select the number of tickets you want (You can only choose one to four tickets):\n");
	scanf("%d", &tickets); /*lhpsh epiloghs tou xrhsth gia to poso eishthria thelei*/
	while((tickets>4)&&(tickets<0)) /*elegxos egkurothtas epiloghs tou xrhsth gia epilogh eishthriwn*/
	{
		printf("Your choice is not valid.Please try again\n");
		scanf("%d", &tickets);/*lhpsh epiloghs tou xrhsth ksana*/
	}
	/*apothhkeush ths epiloghs eishthriwn tou xrhsth ston pinaka eishthriwn*/
	if(ch=='A') app.tic_num[0]=tickets;
	else if(ch=='B') app.tic_num[1]=tickets;
	else if(ch=='C') app.tic_num[2]=tickets;
	else if(ch=='D') app.tic_num[3]=tickets;
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
