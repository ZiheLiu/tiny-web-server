#include "../csapp.h"
#include "sbuf.h"
#include "tiny.h"

#define SBUFSIZE  4
#define INIT_THREAD_N  1
#define THREAD_LIMIT 4096

static int nthreads;
static sbuf_t sbuf;


typedef struct {
  pthread_t tid;
  sem_t mutex;
} ithread;

static ithread threads[THREAD_LIMIT];

void *serve_thread(void *vargp);

void *adjust_threads(void *);

void create_threads(int start, int end);

int main(int argc, char **argv) {
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);

  nthreads = INIT_THREAD_N;
  sbuf_init(&sbuf, SBUFSIZE);
  create_threads(0, nthreads);

  Pthread_create(&tid, NULL, adjust_threads, NULL);

  for(;;) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);

    sbuf_insert(&sbuf, connfd);
  }
}

void *serve_thread(void *vargp) {
  int idx = *(int*)vargp;
  Free(vargp);

  for(;;) {
    P(&(threads[idx].mutex));

    int connfd = sbuf_remove(&sbuf);
    doit(connfd);
    Close(connfd);

    V(&(threads[idx].mutex));
  }
}

void create_threads(int start, int end) {
  int i;
  for (i = start; i < end; i++) {
    Sem_init(&(threads[i].mutex), 0, 1);
    int *arg = (int*)Malloc(sizeof(int));
    *arg = i;
    Pthread_create(&(threads[i].tid), NULL, serve_thread, arg);
  }
}

void *adjust_threads(void *vargp) {
  sbuf_t *sp = &sbuf;

  for(;;) {
    if (sbuf_full(sp)) {
      if (nthreads == THREAD_LIMIT) {
        fprintf(stderr, "too many threads, can't double\n");
        continue;
      }

      int double_n = 2 * nthreads;
      create_threads(nthreads, double_n);
      nthreads = double_n;
      continue;
    }

    if (sbuf_empty(sp)) {
      if (nthreads == 1)
        continue;

      int half_n = nthreads / 2;

      int i;
      for (i = half_n; i < nthreads; i++) {
        P(&(threads[i].mutex));
        Pthread_cancel(threads[i].tid);
        V(&(threads[i].mutex));
      }
      nthreads = half_n;
      continue;
    }
  }
}