#include "../csapp.h"
#include "sbuf.h"
#include "tiny.h"
#include "echoservers.h"

#define SBUFSIZE  4
#define WORKER_THREAD_N 6
#define THREAD_LIMIT 100




typedef struct {
  pthread_t tid;
  sem_t mutex;
} ithread;

static ithread threads[THREAD_LIMIT];
static sbuf_t sbuf;
pool pools[WORKER_THREAD_N];

void *subreactor_thread();

void *worker_thread(void *vargp);

void create_worker_threads(int start, int end);

int main(int argc, char **argv) {
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);

  for (int i = 0; i < WORKER_THREAD_N; i++) {
    init_pool(listenfd, &pools[i]);
  }

  sbuf_init(&sbuf, SBUFSIZE);

  Pthread_create(&tid, NULL, subreactor_thread, NULL);

  create_worker_threads(0, WORKER_THREAD_N);

  for(;;) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);

    sbuf_insert(&sbuf, connfd);
  }
}

void *subreactor_thread() {
  int next_pool_id = 0;

  for(;;) {
    int connfd = sbuf_remove(&sbuf);
    add_client(connfd, &pools[next_pool_id]);
    next_pool_id = (next_pool_id + 1) % WORKER_THREAD_N;
  }
}

void *worker_thread(void *vargp) {
  int idx = *(int*)vargp;
  Free(vargp);

  for(;;) {
    pools[idx].ready_set = pools[idx].read_set;
    pools[idx].nready = Select(pools[idx].maxfd+1, &pools[idx].ready_set, NULL, NULL, NULL);

    check_clients(&pools[idx]);
  }
}

void create_worker_threads(int start, int end) {
  int i;
  for (i = start; i < end; i++) {
    int *arg = (int*)Malloc(sizeof(int));
    *arg = i;
    Pthread_create(&(threads[i].tid), NULL, worker_thread, arg);
  }
}

