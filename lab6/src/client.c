#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

struct Server {
  char ip[255];
  int port;
};

struct ThreadArgs{
    struct Server server_args;
    int begin;
    int end;
};

uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

void ThreadServer(void *args) {
  struct Server *thread_args = (struct Server *)args;
  // TODO: work continiously, rewrite to make parallel
    struct hostent *hostname = gethostbyname((*thread_args).ip);
    if (hostname == NULL) {
        fprintf(stderr, "gethostbyname failed with %s\n", (*thread_args).ip);
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons((*thread_args).port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
        fprintf(stderr, "Socket creation failed!\n");
        exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
        fprintf(stderr, "Connection failed\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  FILE* server_file = fopen(servers, "r");
  if (server_file == NULL) {
      perror( "cannot read server_file" );
      return 1;
  }


  char str[50];
  int servers_num = 0;
   while (1)
   {
      fgets(str,sizeof(str),server_file);

      if (feof(server_file) != 0)
      {
        break;
      }
      servers_num++;
   }
   char ips[servers_num][50];
   char ports[servers_num][50];
   struct Server *to = malloc(sizeof(struct Server) * servers_num);
   fseek(server_file,0,SEEK_SET);

   int i=0;
   while (1)
   {
      fgets(str,sizeof(str),server_file);

      if (feof(server_file) != 0)
      {
        break;
      }
      strcpy((*(to+i)).ip, strtok(str , ":"));
      (*(to+i)).port = atoi(strtok(NULL, ":"));
      i++;
   }
   fclose(server_file);

   i = 0;
   for (; i<servers_num; i++){
       printf("ip: %s / port: %d\n", (*(to+i)).ip, (*(to+i)).port);
   }

   pthread_t threads[servers_num];
   struct ThreadArgs args[servers_num];
   i = 0;
   for (; i<servers_num; i++){
      args[i].server_args = *(to+i);
      args[i].begin = (k/servers_num)*i + 1;
      args[i].end = (k/servers_num)*(i+1);
      if (pthread_create(&threads[i], NULL, (void *)ThreadServer, (void *)args)) {
      printf("Error: pthread_create failed!\n");
      return 1;
      }
   }


//   unsigned int servers_num = 1;
//   struct Server *to = malloc(sizeof(struct Server) * servers_num);
//   // TODO: delete this and parallel work between servers
//   to[0].port = 20001;
//   memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));

//   // TODO: work continiously, rewrite to make parallel
//   for (int i = 0; i < servers_num; i++) {
//     struct hostent *hostname = gethostbyname(to[i].ip);
//     if (hostname == NULL) {
//       fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
//       exit(1);
//     }

//     struct sockaddr_in server;
//     server.sin_family = AF_INET;
//     server.sin_port = htons(to[i].port);
//     server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

//     int sck = socket(AF_INET, SOCK_STREAM, 0);
//     if (sck < 0) {
//       fprintf(stderr, "Socket creation failed!\n");
//       exit(1);
//     }

//     if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
//       fprintf(stderr, "Connection failed\n");
//       exit(1);
//     }

//     // TODO: for one server
//     // parallel between servers
//     uint64_t begin = 1;
//     uint64_t end = k;

//     char task[sizeof(uint64_t) * 3];
//     memcpy(task, &begin, sizeof(uint64_t));
//     memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
//     memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

//     if (send(sck, task, sizeof(task), 0) < 0) {
//       fprintf(stderr, "Send failed\n");
//       exit(1);
//     }

//     char response[sizeof(uint64_t)];
//     if (recv(sck, response, sizeof(response), 0) < 0) {
//       fprintf(stderr, "Recieve failed\n");
//       exit(1);
//     }

//     // TODO: from one server
//     // unite results
//     uint64_t answer = 0;
//     memcpy(&answer, response, sizeof(uint64_t));
//     printf("answer: %llu\n", answer);

//     close(sck);
//   }
//   free(to);

  return 0;
}

