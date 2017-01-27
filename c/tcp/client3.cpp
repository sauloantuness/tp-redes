/*
    C ECHO client example using sockets
    http://www.binarytides.com/server-client-example-c-sockets-linux/
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<bits/stdc++.h>

using namespace std;

void createSegmento(char *segmento){
  char seg[1024];
  strcat(seg,"xxxxxxxxxxxxxxxx");// port source - 16
  strcat(seg,"yyyyyyyyyyyyyyyy");// port destination - 16
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// sequence number - 32
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// acknowlegement - 32
  strcat(seg,"ssss");// tam - 4
  strcat(seg,"wwww");// reserve - 4
  strcat(seg,"rrrr");// flags - 4
  strcat(seg,"dddddddddddddddddddd"); // window - 20
  strcat(seg,"xxxxxxxxxxxxxxxx");// checksum - 16
  strcat(seg,"yyyyyyyyyyyyyyyy");// urgent pointer - 16
  strcat(seg,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");// options - 32
  strcat(segmento, seg);
}

void removeSegmento(char *segment, char *data){
  int length = strlen(segment), i=0, j=0;
  printf("%d\n",length );
  for(i=192, j=0;i<length;i++,j++){
      data[j] = segment[i];
  }
}

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000], segment[3000],data[3000];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        createSegmento(segment);
        strcat(segment, message);
        printf("%s\n", segment);

        //Send some data
        if( send(sock , segment , strlen(segment) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }

        puts("Server reply :");
        printf("%s\n", server_reply);
        removeSegmento(server_reply, data);
        printf("%s\n", data);
    }

    close(sock);
    return 0;
}
