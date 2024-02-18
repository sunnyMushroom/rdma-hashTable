#include "network.h"

const int BUFFER_SIZE = 1024;
const int TIMEOUT_IN_MS = 500; /* ms */




static void set_OperationRequest(enum reqtype m);


void set_OperationRequest(enum reqtype m){
    c_type = m;
}

int on_event(struct rdma_cm_event *event)
{
  int r = 0;

  if (event->event == RDMA_CM_EVENT_ADDR_RESOLVED)
    r = on_addr_resolved(event->id);
  else if (event->event == RDMA_CM_EVENT_ROUTE_RESOLVED)
    r = on_route_resolved(event->id);
  else if (event->event == RDMA_CM_EVENT_ESTABLISHED)
    r = client_on_connection(event->id->context);
  else if (event->event == RDMA_CM_EVENT_DISCONNECTED)
    r = on_disconnect(event->id);
  else
    die("on_event: unknown event.");

  return r;
}
//<reqtype> <key/value> <ip> <port>
int main(int argc,char **argv){

  struct addrinfo *addr;
  struct rdma_cm_event *event = NULL;
  struct rdma_cm_id *conn = NULL;
  struct rdma_event_channel *ec = NULL;

  if(argc != 5){
    TODO:
  }
  if(strcmp(argv[1],"search") == 0){
    set_OperationRequest(SEARCH);
  }
  else if(strcmp(argv[1],"insert") == 0){
    TODO:
  }
  else if(strcmp(argv[1],"update") == 0){
    TODO:
  }else{
    TODO:
  }

  getaddrinfo(argv[1], argv[2], NULL, &addr);
  ec = rdma_create_event_channel();
  rdma_create_id(ec, &conn, NULL, RDMA_PS_TCP);
  rdma_resolve_addr(conn, NULL, addr->ai_addr, TIMEOUT_IN_MS);
  freeaddrinfo(addr);

  while (rdma_get_cm_event(ec,&event) == 0){
    struct rdma_cm_event event_copy;

    memcpy(&event_copy, event, sizeof(*event));
    rdma_ack_cm_event(event);

    if (on_event(&event_copy))
      break;
  }

  rdma_destroy_event_channel(ec);

  return 0;
  
}