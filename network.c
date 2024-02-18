#include "HashTable.h"
#include "network.h"


void register_client_memory(struct connection *conn){
    conn->send_region = malloc(sizeof(struct Request));
    conn->recv_region = malloc(sizeof(struct Response));

    conn->send_mr = ibv_reg_mr(s_ctx->pd, conn->send_region, sizeof(struct Request), 
    IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);

    conn->recv_mr = ibv_reg_mr(s_ctx->pd, conn->recv_region, sizeof(struct Response), 
    IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);
}

void register_client_memory(struct connection *conn){
    conn->recv_region = malloc(sizeof(struct Request));
    conn->send_region = malloc(sizeof(struct Response));

    conn->recv_mr = ibv_reg_mr(s_ctx->pd, conn->send_region, sizeof(struct Request), 
    IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);

    conn->send_mr = ibv_reg_mr(s_ctx->pd, conn->recv_region, sizeof(struct Response), 
    IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_WRITE);
}

int on_addr_resolved(struct rdma_cm_id *id){
    struct ibv_qp_init_attr qp_attr;
    struct connection *conn;
    printf("address resolved.\n");
    build_context(id->verbs);
    build_qp_attr(&qp_attr);

    rdma_create_qp(id,s_ctx->pd,&qp_attr);

    id->context = conn = (struct connection *)malloc(sizeof(struct connection));

    conn->id = id;
    conn->qp = id->qp;
    conn->num_completions = 0;

    register_memory(conn);
  //post_receives(conn);

    rdma_resolve_route(id, TIMEOUT_IN_MS);

    return 0;
}

int on_route_resolved(struct rdma_cm_id *id)
{
  struct rdma_conn_param cm_params;

  printf("route resolved.\n");

  memset(&cm_params, 0, sizeof(cm_params));
  TEST_NZ(rdma_connect(id, &cm_params));

  return 0;
}


void post_receives(struct connection *conn){

    struct ibv_recv_wr wr, *bad_wr = NULL;
    struct ibv_sge sge;

    wr.wr_id = (uintptr_t)conn;
    wr.next = NULL;
    wr.sg_list = &sge;
    wr.num_sge = 1;

    sge.addr = (uintptr_t)conn->recv_region;
    sge.length = BUFFER_SIZE;
    sge.lkey = conn->recv_mr->lkey;

    ibv_post_recv(conn->qp, &wr, &bad_wr);  

}

void build_qp_attr(struct ibv_qp_init_attr *qp_attr){
    memset(qp_attr,0,sizeof(*qp_attr));
    qp_attr->send_cq = s_ctx->cq;
    qp_attr->recv_cq = s_ctx->cq;
    qp_attr->qp_type = IBV_QPT_RC;

    qp_attr->cap.max_send_wr = 10;
    qp_attr->cap.max_recv_wr = 10;
    qp_attr->cap.max_send_sge = 1;
    qp_attr->cap.max_recv_sge = 1;    
}




int client_on_connection(void *context){
  struct connection *conn = (struct connection *)context;
  struct ibv_send_wr wr, *bad_wr = NULL;
  struct ibv_sge sge;
  

}

void *poll_cq(void *ctx){
    struct ibv_cq *cq;
    struct ibv_wc wc;

    while(1){
        ibv_get_cq_event(s_ctx->comp_channel,&cq,&ctx);
        ibv_ack_cq_events(cq,1);
        ibv_req_notify_cq(cq,0);

        while(ibv_poll_cq(cq,1,&wc)){
            on_completion(&wc);
        }
    }

    return NULL;
}

void build_context(struct ibv_context *verbs){
    if(s_ctx){
        if(s_ctx->ctx != verbs){
            //die
        }
        return;
    }

    s_ctx = (struct context *)malloc(sizeof(struct context));
    s_ctx->pd = ibv_alloc_pd(s_ctx->ctx);
    s_ctx->comp_channel = ibv_create_comp_channel(s_ctx->ctx);
    s_ctx->cq = ibv_create_cq(s_ctx->ctx, 10, NULL, s_ctx->comp_channel, 0);
    ibv_req_notify_cq(s_ctx->cq, 0);
    pthread_create(&s_ctx->cq_poller_thread, NULL, poll_cq, NULL);

}



