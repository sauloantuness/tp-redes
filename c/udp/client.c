/************* UDP CLIENT CODE *******************
http://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int clientSocket, portNum, nBytes;

struct sockaddr_in serverAddr;
socklen_t addr_size;

void PDU(){
  printf("Port: %d\n",serverAddr.sin_port);
  printf("Addr: %d\n",serverAddr.sin_addr.s_addr);
}

int main(){
  char buffer[3000];
  strcat(buffer, "GET // HTTP/1.1");

  /*Create UDP socket*/
  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7894);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverAddr;

  while(1){

    nBytes = strlen(buffer) + 1;

    /*Send message to server*/
    sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);

    /*Receive message from server*/
    nBytes = recvfrom(clientSocket,buffer,3000,0,NULL, NULL);

    printf("Received from server: %s\n",buffer);
    PDU();
  }

  return 0;
}
