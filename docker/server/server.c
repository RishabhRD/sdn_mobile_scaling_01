#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>
#include <linux/if_packet.h>
#include <netinet/ether.h>
char client_message[2000];
char buffer[1024];
int count = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void * socketThread(void *arg)
{
  int newSocket = *((int *)arg);
  count++;
  recv(newSocket , client_message , 2000 , 0);
  // Send message to the client socket 
  pthread_mutex_lock(&lock);
  char *message = malloc(sizeof(client_message)+20);
  strcpy(message,"Hello Client : ");
  strcat(message,client_message);
  strcat(message,"\n");
  strcpy(buffer,message);
  free(message);
  pthread_mutex_unlock(&lock);
  sleep(1);
  printf("%s\n",buffer);
  while(1);
  printf("Exit socketThread \n");
  close(newSocket);
  pthread_exit(NULL);
}
void sendScale(){
	int sockfd;
	char sendbuf[sizeof(struct ether_header)+6];
	struct ether_header *eth = (struct ether_header *) sendbuf;
	memset(sendbuf,0,sizeof(sendbuf));
	char* data = sendbuf+sizeof(struct ether_header);
	strcpy(data,"Scale");
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}
	struct sockaddr_ll socket_address;
	eth->ether_shost[0] = 0x00;
	eth->ether_shost[1] = 0x00;
	eth->ether_shost[2] = 0x00;
	eth->ether_shost[3] = 0x00;
	eth->ether_shost[4] = 0x00;
	eth->ether_shost[5] = 0x00;
	eth->ether_dhost[0] = 0xaa;
	eth->ether_dhost[1] = 0xaa;
	eth->ether_dhost[2] = 0xaa;
	eth->ether_dhost[3] = 0xaa;
	eth->ether_dhost[4] = 0xaa;
	eth->ether_dhost[5] = 0xaa;
	socket_address.sll_halen = ETH_ALEN;
	//socket_address.sll_ifindex = if_idx.ifr_ifindex;
	socket_address.sll_addr[0] = 0xaa;
	socket_address.sll_addr[1] = 0xaa;
	socket_address.sll_addr[2] = 0xaa;
	socket_address.sll_addr[3] = 0xaa;
	socket_address.sll_addr[4] = 0xaa;
	socket_address.sll_addr[5] = 0xaa;
	if (sendto(sockfd, sendbuf, sizeof(struct ether_header)+6 , 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0)
		printf("Send failed\n");

}	

int main(){
  int serverSocket, newSocket;
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  //Create the socket. 
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  // Configure settings of the server address struct
  // Address family = Internet 
  serverAddr.sin_family = AF_INET;
  //Set port number, using htons function to use proper byte order 
  serverAddr.sin_port = htons(11111);
  //Set IP address to localhost 
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //Set all bits of the padding field to 0 
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  //Bind the address struct to the socket 
  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  //Listen on the socket, with 40 max connection requests queued 
  if(listen(serverSocket,11111)==0)
    printf("Listening\n");
  else
    printf("Error\n");
    pthread_t tid[60];
    int i = 0;
    while(1)
    {
	if(count>=3){
		printf("Scaling...\n");
		count =0;
		sendScale();
		
	}
        //Accept call creates a new socket for the incoming connection
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        //for each client request creates a thread and assign the client request to it to process
       //so the main thread can entertain next request
        if( pthread_create(&tid[i], NULL, socketThread, &newSocket) != 0 )
           printf("Failed to create thread\n");
        if( i >= 50)
        {
          i = 0;
          while(i < 50)
          {
            pthread_join(tid[i++],NULL);
          }
          i = 0;
        }
    }
  return 0;
}
