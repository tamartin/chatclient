// Taylor Martin

// Starter code from http://www.linuxhowtos.org/C_C++/socket.html as suggested
// by Alex during class

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>  

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
  
    portno = 49153;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname("pilot");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    n = read(sockfd,buffer,255);
    printf("%s\n",buffer);
    bzero(buffer,256);
    printf("Enter your name: ");
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
      error("ERROR writing to socket");
    bzero(buffer,256);
    sleep(1);
    n = read(sockfd,buffer,255);
    printf("%s\n",buffer);
    bzero(buffer,256);
    while (1){
      printf(">> ");
      fgets(buffer,255,stdin);
      if (strlen(buffer) > 1){
	n = write(sockfd,buffer,strlen(buffer));
      }
      bzero(buffer,256);

      int message;
      ioctl(sockfd,FIONREAD,&message);
      if (message != 0){
	n = read(sockfd,buffer,255);
	printf("%s\n",buffer);
	bzero(buffer,256);
      }
    }
    close(sockfd);
    return 0;
}
