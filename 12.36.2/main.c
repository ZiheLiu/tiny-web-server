#include "../csapp.h"
#include "sbuf.h"
#include "tiny.h"
#include "echoservers.h"

#define SBUFSIZE  4
#define SUB_REACTOR_N  3
#define WORKER_THREAD_N 3
#define THREAD_LIMIT 100


static sbuf_t sbuf;


typedef struct {
  pthread_t tid;
  pthread_t tid2;
  sem_t mutex;
  pool pool;
} ithread;

static ithread threads[THREAD_LIMIT];

void *subreactor_thread(void *vargp);

void create_subreactor_threads(int start, int end);

void *worker_thread(void *vargp);

void create_worker_threads(int start, int end);

int main(int argc, char **argv) {
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);

  for (int i = 0; i < WORKER_THREAD_N; i++) {

    init_pool(listenfd, &threads[i].pool);
  }

  sbuf_init(&sbuf, SBUFSIZE);
  create_subreactor_threads(0, SUB_REACTOR_N);

  create_worker_threads(0, WORKER_THREAD_N);


  for(;;) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);

    sbuf_insert(&sbuf, connfd);
  }
}

void *subreactor_thread(void *vargp) {
  int idx = *(int*)vargp;
  Free(vargp);

  for(;;) {
    P(&(threads[idx].mutex));

    int connfd = sbuf_remove(&sbuf);
    add_client(connfd, &threads[idx].pool);

    V(&(threads[idx].mutex));
  }
}

void *worker_thread(void *vargp) {
  int idx = *(int*)vargp;
  Free(vargp);

  for(;;) {
    threads[idx].pool.ready_set = threads[idx].pool.read_set;
    threads[idx].pool.nready = Select(threads[idx].pool.maxfd+1, &threads[idx].pool.ready_set, NULL, NULL, NULL);

    check_clients(&threads[idx].pool);
  }
}

void create_subreactor_threads(int start, int end) {
  int i;
  for (i = start; i < end; i++) {
    Sem_init(&(threads[i].mutex), 0, 1);
    int *arg = (int*)Malloc(sizeof(int));
    *arg = i;
    Pthread_create(&(threads[i].tid), NULL, subreactor_thread, arg);
  }
}

void create_worker_threads(int start, int end) {
  int i;
  for (i = start; i < end; i++) {
    int *arg = (int*)Malloc(sizeof(int));
    *arg = i;
    Pthread_create(&(threads[i].tid2), NULL, worker_thread, arg);
  }
}

