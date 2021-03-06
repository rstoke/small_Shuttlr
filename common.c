#include "common_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * \brief Creating socket
 *
 * \param prop Protocoto to use, port_num Port number
 * \return file descriptor
 */

//TODO
//int create_socket(int prop, int *port_num)    //TODO verify usefullness prop
//{
  /* fonction de creation et d'attachement */
  /* d'une nouvelle socket */
  /* renvoie le numero de descripteur */
  /* et modifie le parametre port_num */
  /*
  int fd = 0;
  struct sockaddr_in serv_addr;

  //creation of the socket
  fprintf(stdout,"Trying to connect with port_num %d\n",*port_num);
  fd=socket(AF_INET,SOCK_STREAM,0);
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_port=htons(*port_num);
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=INADDR_ANY;
  if (fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
   }


  // attachement of the socket
  if(bind(fd,(struct sockaddr*) &serv_addr, sizeof(serv_addr)) <0) {
     perror("bind");
     exit(EXIT_FAILURE);
   }
  return fd;*/
//}

//Rest of Joseph's code
 /*while(connect(fd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) == -1)
   printf("Socket %d connected in TCP/IP mode \n", fd);
 {
   perror("connect");
 { //Listenning socket
   struct sockaddr_in addr_listen;
   fd = socket(AF_INET, SOCK_STREAM, 0);
   fprintf(stdout,"La socket %d est maintenant ouverte en mode TCP/IP\n", fd);
   addr_listen.sin_family = AF_INET;
   addr_listen.sin_port = htons(*port_num);
   addr_listen.sin_addr.s_addr = INADDR_ANY;
   while (bind(fd, (struct sockaddr *) &addr_listen, sizeof(addr_listen)) == -1)
   {
   perror("bind");
   }
   fprintf(stdout,"bind success\n");
   if (listen(fd,listen_backlog) == -1)
   {
   perror("listen");
   return -1;
   }
   fp*/

//Backup original
//{
  /* fonction de creation et d'attachement */
  /* d'une nouvelle socket */
  /* renvoie le numero de descripteur */
  /* et modifie le parametre port_num */
  /*
   int fd = 0;
   struct sockaddr_in serv_addr;
   memset(&serv_addr,0,sizeof(serv_addr));


   serv_addr.sin_port=htons(0);
   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr=INADDR_ANY;

   fd=socket(AF_INET,SOCK_STREAM,0);
   bind(fd, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr_in));
   *port_num=serv_addr.sin_port;
   return fd;
   */
//}



/* Vous pouvez ecrire ici toutes les fonctions */
/* qui pourraient etre utilisees par le lanceur */
/* et le processus intermediaire. N'oubliez pas */
/* de declarer le prototype de ces nouvelles */
/* fonctions dans common_impl.h */



char** create_pool_hosts(char *filename, int *num_procs)
{
  //Init
  FILE* myfile = fopen(filename, "r");
  if(myfile == NULL)
  {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }
  int no_lines = 0;
  char c = 0;
  char **pool = NULL;

  //Count number of lines
  while( (c = fgetc(myfile)) != EOF) {
    if (c == '\n') {
      no_lines++;
    }
  }

  //Allocate memory
  pool = malloc(no_lines * sizeof(char *));  //possible improvement init the array with NULLs
  for (int i = 0; i < no_lines; i++) {
    pool[i] = malloc(MAX_LEN_HOSTNAME * sizeof(char));
    memset(pool[i], '\0', MAX_LEN_HOSTNAME);
  }

  //Cursor back to init
  rewind(myfile);

  //Extract each line
  for (int i = 0; i < no_lines; i++) {
    fgets(pool[i], MAX_LEN_HOSTNAME, myfile);
    pool[i][strlen(pool[i])-1] = '\0';  //remove the '\n' character captured
  }

  fclose(myfile);

  //Output
  *num_procs = no_lines;
  return pool;
}

void destroy_pool_hosts(char **pool, int num_procs) {
  for (int i = 0; i < num_procs; i++) {
    free(pool[i]); //free each string
  }
  free(pool);  //free array
}

/* utilities */
//TODO TMP replace by Joseph
int create_socket() {
  int option = 1;
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   //Sockets config: Blocking  //TODO Possible to add SO_REUSEADDR with setsockopt() during dev phase testing...etc
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  if (sockfd < 0) {
    ERROR_EXIT("Error - socket opening");
  }

  return sockfd;
}

void do_send(int sockfd, char *buffer, int buffer_size) {
  int progress = 0; //total sent
  int sent = 0; //each try
  do {
    if ( (sent = send(sockfd, buffer+sent, buffer_size-sent, 0)) < 0 ) {
      ERROR_EXIT("Error - send");
    }
    progress += sent;
  } while(progress != buffer_size);
}

int do_recv(int sockfd, char *buffer, int buffer_size) {
  int progress = 0; //total sent
  int read = 0; //each try
  do {
    if ( (read = recv(sockfd, buffer+read, buffer_size-read, 0)) < 0 ) {
      ERROR_EXIT("Error - recv");
    }
    progress += read;
  } while(progress != buffer_size);

  return progress;
}
