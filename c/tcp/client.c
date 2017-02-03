// http://www.programminglogic.com/example-of-client-server-program-in-c-using-sockets-and-tcp/
// http://www-usr.inf.ufsm.br/~giovani/sockets/sockets.txt
/****************** CLIENT CODE ****************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


int main(){
	int welcomeSocket, newSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;

	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7894);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	/*---- Listen on the socket, with 5 max connection requests queued ----*/
	if(listen(welcomeSocket,5)==0)
	  printf("Listening\n");
	else
	  printf("Error\n");

	/*---- Accept call creates a new socket for the incoming connection ----*/
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

	recv(newSocket, buffer, 1024, 0);

	printf("Data received: %s",buffer);   

	
	// -------------------------------

	// int clientSocket;
	// char buffer[1024];
	// struct sockaddr_in serverAddr;
	// socklen_t addr_size;

	// clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	// serverAddr.sin_family = AF_INET;
	// serverAddr.sin_port = htons(7893);
	// serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	// addr_size = sizeof serverAddr;
	// connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);

	// recv(clientSocket, buffer, 1024, 0);


	// // -------------------------------

	// send(newSocket,buffer,13,0);



	// CLIENTE



  return 0;
}