#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>

#define MAXBUFSIZE 2000
#define PORT 1234

int server_init(int *sock){

  struct sockaddr_in server;
  bzero(&server,sizeof(struct sockaddr_in));               //zero the struct
	server.sin_family = AF_INET;                 //address family
	server.sin_port = htons(PORT);      //sets port to network byte order
	server.sin_addr.s_addr = INADDR_ANY; //sets remote IP address

  if ((*sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("unable to create socket");
	}

  if (bind(*sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("unable to bind socket\n");
	}
}

int server_put(char *buffer, int *sock, struct sockaddr_in **remote){

  int nbytes;
  unsigned int remote_length = sizeof(*remote);
  nbytes = sendto(*sock, buffer ,strlen(buffer)+1,0,(struct sockaddr *)*remote,sizeof(struct sockaddr_in));
  if(nbytes<0){
    perror("sendto");
  }
}

int server_get(char *buffer, int *sock, struct sockaddr_in **remote){

  int nbytes;
  *remote = malloc(sizeof(struct sockaddr_in));
  unsigned int remote_length = sizeof(*remote);
  nbytes = recvfrom(*sock, buffer, MAXBUFSIZE*sizeof(char), 0, (struct sockaddr *)*remote,&remote_length );
  printf("nbytes: %d\n",nbytes);
  if(nbytes<0){
    perror("recvfrom");
  }
}

int server_file_get(char *buffer, int *sock, struct sockaddr_in **remote){

int nbytes;
FILE *fpw;
fpw = fopen("op", "w");
// server_get(buffer, sock, remote);
*remote = malloc(sizeof(struct sockaddr_in));
unsigned int remote_length = sizeof(*remote);
nbytes = recvfrom(*sock, buffer, MAXBUFSIZE*sizeof(char), 0, (struct sockaddr *)*remote,&remote_length );
printf("nbytes: %d\n",nbytes);
if(nbytes<0){
  perror("recvfrom");
}
fwrite(buffer,1, nbytes, fpw);
fclose(fpw);
}


int main(){

int sock;
struct sockaddr_in *remote;
char buffer_recv[MAXBUFSIZE];
server_init(&sock);
// server_get(buffer_recv, &sock, &remote);
// char *buffer_send = "server_to_client";
// server_put(buffer_send, &sock, &remote);
//
//
// printf("\n received %s\n",buffer_recv);
server_file_get(buffer_recv, &sock, &remote);
}
