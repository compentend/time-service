#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024
#define LOG_FILE "log.txt"

void logRequest(const char* ip_address,const char* request_time){
FILE* log_file = fopen(LOG_FILE,"a");
if(log_file==NULL){
perror("failed to open log file");
return;
}
fprintf(log_file,"%s\t%s\n",ip_address,request_time);
fclose(log_file);
}
int main(){
int server_socket = socket(AF_INET,SOCK_STREAM,0);
if(server_socket<0){
perror("failed to creat socket");
exit(1);
}
struct sockaddr_in server_address;
memset(&server_address,0,sizeof(server_address));
server_address.sin_family = AF_INET;
server_address.sin_addr.s_addr = htonl(INADDR_ANY);
server_address.sin_port = htons(12345);
if(bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address))<0){
perror("failed to bind address");
exit(1);
}
if(listen(server_socket,5)<0){
perror("failed to listen");
exit(1);
}
printf("time server start");
while(1){
struct sockaddr_in client_address;
socklen_t client_address_length = sizeof(client_address);
int client_socket = accept(server_socket,(struct sockaddr*)&client_address,&client_address_length);
if(client_socket < 0){
perror("failed to accept connetion");
exit(1);
}
char ip_address[INET_ADDRSTRLEN];
inet_ntop(AF_INET,&(client_address.sin_addr),ip_address,INET_ADDRSTRLEN);
time_t current_time = time(NULL);
struct tm* time_info = localtime(&current_time);
char request_time[MAX_BUFFER_SIZE];
strftime(request_time,MAX_BUFFER_SIZE,"%Y-%m-%d %H:%M:%S",time_info);
char response[MAX_BUFFER_SIZE];
snprintf(response,MAX_BUFFER_SIZE,"server time:%s\n",request_time);
write(client_socket,response,strlen(response));
logRequest(ip_address,request_time);
close(client_socket);
}
close(server_socket);
return 0;
}
