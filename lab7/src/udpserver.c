#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// #define SERV_PORT 20001
// #define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char **argv) {
  int sockfd, n;
  int sock2 = 0;
  char mesg[atoi(argv[2])], ipadr[16];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if (argc != 3) {
    printf("usage: serv_port, bufsize\n");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket problem");
    exit(1);
  }

  memset(&servaddr, 0, SLEN);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(atoi(argv[1]));

  if (bind(sockfd, (SADDR *)&servaddr, SLEN) < 0) {
    perror("bind problem");
    exit(1);
  }
  printf("SERVER starts...\n");

  while (1) {
      if (sock2 != 0){
          sockfd = sock2;
      }
    unsigned int len = SLEN;

    if ((n = recvfrom(sockfd, mesg, atoi(argv[2]), 0, (SADDR *)&cliaddr, &len)) < 0) {
      perror("recvfrom");
      exit(1);
    }
    mesg[n] = 0;

    printf("REQUEST %s      FROM %s : %d\n", mesg,
           inet_ntop(AF_INET, (void *)&cliaddr.sin_addr.s_addr, ipadr, 16),
           ntohs(cliaddr.sin_port));

    if (sendto(sockfd, mesg, n, 0, (SADDR *)&cliaddr, len) < 0) {
      perror("sendto");
      exit(1);
    }

    // падение сервера = сокет не может принимать данные
    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    sleep(5);

    if ((sock2 = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket problem");
        exit(1);
    }
    if (bind(sock2, (SADDR *)&servaddr, SLEN) < 0) {
        perror("bind problem");
        exit(1);
    }
  }
}
