#include <netdb.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFFER_SIZE = 1024;
const int TIMEOUT_IN_MS = 500; /* ms */

enum reqtype c_type = SEARCH;

struct context {
  struct ibv_context *ctx;
  struct ibv_pd *pd;
  struct ibv_cq *cq;
  struct ibv_comp_channel *comp_channel;

  pthread_t cq_poller_thread;
};

enum reqtype{
    INSERT,
    SEARCH,
    DELETE,
    UPDATE
};

enum restype{
    SUCCESS,
    KEY_NOT_FOUND,//search update delete
    KEY_ALREADY_EXISTS,//insert
    INVALID_ARGUMENT,
};


struct Request
{
    enum Reqtype;
    int key;
};

struct Response{
    enum restype;
    int value;
};


static struct context *s_ctx = NULL;

struct connection {
  struct rdma_cm_id *id;
  struct ibv_qp *qp;

  struct ibv_mr *recv_mr;
  struct ibv_mr *send_mr;

  struct Response *res;
  struct Request *req;

  char *recv_region;
  char *send_region;

  int num_completions;

};

static void build_context(struct ibv_context *verbs);
static void build_qp_attr(struct ibv_qp_init_attr *qp_attr);
static void *poll_cq(void *ctx);
void post_receives(struct connection *conn);
static void register_client_memory();
static void register_server_memory();


static int on_addr_resolved(struct rdma_cm_id *id);
static int on_route_resolved(struct rdma_cm_id *id);
static int server_on_connection(struct rdma_cm_id);
static int client_on_connection(void *context);
static void on_completion(struct ibv_wc *wc);//client
static int on_disconnect(struct rdma_cm_id *id);







