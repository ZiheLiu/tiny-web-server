#include <stdio.h>

#include "../csapp.h"
#include "echoservers.h"

int main(int argc, char **argv)
{
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  static pool pool;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  
  listenfd = Open_listenfd(argv[1]);

  init_pool(listenfd, &pool);

  for(;;) {
    /* Wait for listening/connected descriptor(s) to become ready */
    pool.ready_set = pool.read_set;
    pool.nready = Select(pool.maxfd+1, &pool.ready_set, NULL, NULL, NULL);

    /* If listening descriptor ready, add new client to pool */
    if (FD_ISSET(listenfd, &pool.ready_set)) { 
      clientlen = sizeof(struct sockaddr_storage);
      connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
      Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
      printf("Accepted connection from (%s, %s)\n", hostname, port);
      add_client(connfd, &pool);
    }

    /* Echo a text line from each ready connected descriptor */
    check_clients(&pool);
  }
}
