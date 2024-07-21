#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

int main(){
int client_socket = socket(AF_INET,SOCK_STREAM,0);
if(client_socket<0){
perror("failed to create socket");
exit(1);
}
struct sockaddr_in server_address;
memset(&server_address,0,sizeof(server_address));
server_address.sin_family = AF_INET;
server_address.sin_port = htons(12345);
if(inet_pton(AF_INET,"127.0.0.1",&(server_address.sin_addr))<=0){
perror("failed to set server IP address");
exit(1);
}
if(connect(client_socket,(struct sockaddr*)&server_address,sizeof(server_address))<0){
perror("failed to connect to server");
exit(1);
}
printf("connected to time server\n");
char response[MAX_BUFFER_SIZE];
memset(response, 0 ,sizeof(response));
int bytes_received = read(client_socket,response,sizeof(response)-1);
if(bytes_received <0 ){
perror("failed to receive data from server");
exit(1);
}
printf("resonse from server:\n%s\n",response);
close(client_socket);
return 0;
}

