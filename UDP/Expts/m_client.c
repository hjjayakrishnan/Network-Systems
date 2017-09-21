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

#define MAXBUFSIZE 100
#define SERVER_IP "128.138.201.66"
// #define SERVER_IP "128.138.201.66"
#define PORT 1234

int client_init(int *sock, struct sockaddr_in **remote){

  *remote = malloc(sizeof(struct sockaddr_in));
  bzero(*remote,sizeof(struct sockaddr_in));               //zero the struct
	(*remote)->sin_family = AF_INET;                 //address family
	(*remote)->sin_port = htons(PORT);      //sets port to network byte order
	(*remote)->sin_addr.s_addr = inet_addr(SERVER_IP); //sets remote IP address

  if ((*sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("unable to create socket");
	}
}

int client_put(char *buffer, int *sock, struct sockaddr_in **remote){

  int nbytes;
  unsigned int remote_length = sizeof(*remote);
  printf("buffer length before sending: %d\n", strlen(buffer));
  // printf("buffer: %s\n", buffer);
  nbytes = sendto(*sock, buffer ,strlen(buffer)+1,0,(struct sockaddr *)*remote,sizeof(struct sockaddr_in));
  printf("nbytes: %d \n", nbytes);
  if(nbytes<0){
    perror("sendto");
  }
}

int client_get(char *buffer, int *sock, struct sockaddr_in **remote){

  int nbytes;
  unsigned int remote_length = sizeof(*remote);
  nbytes = recvfrom(*sock, buffer, MAXBUFSIZE*sizeof(char), 0, (struct sockaddr *)*remote,&remote_length );
  if(nbytes<0){
    perror("recvfrom");
  }
}

int client_file_put(int *sock, struct sockaddr_in **remote){

  FILE *fp;
  char *buffer;
  int length;
  int nbytes;
  size_t rc;

  fp = fopen("foo2", "r");
  fseek(fp, 0, SEEK_END);
  length = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  buffer = malloc(length + 1);
  rc = fread(buffer, 1, length, fp);
  // buffer[length+1] = '\0';
  printf("length of foo2: %d\n", strlen(buffer));
  // buffer = malloc(5);
  // buffer = "test";
  // client_put(buffer,sock, remote);
  nbytes = sendto(*sock, buffer ,length,0,(struct sockaddr *)*remote,sizeof(struct sockaddr_in));
  printf("nbytes: %d", nbytes);
  free(buffer);

}

int main(){

int sock;
struct sockaddr_in *remote;
client_init(&sock, &remote);

// char *buffer_send = "client_to_server";
// client_put(buffer_send, &sock, &remote);
char buffer_recv[MAXBUFSIZE];
// client_get(buffer_recv, &sock, &remote);
// printf("\n received %s\n",buffer_recv);
client_file_put(&sock, &remote);
// client_put("damn", &sock, &remote);
free(remote);
}
