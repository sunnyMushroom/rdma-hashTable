#include <netdb.h>
#include <unistd.h>
#include <rdma/rdma_cma.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

static struct context *s_ctx = NULL;

int main(int argc,char **argv){
  struct sockaddr_in addr;
  struct rdma_cm_event *event = NULL;
  struct rdma_cm_id *listener = NULL;
  struct rdma_event_channel *ec = NULL;
  uint16_t port = 0;

  memset(&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port=htons(22222);

  ec=rdma_create_event_channel();
  rdma_create_id(ec,&listener,NULL,RDMA_PS_TCP);
  rdma_bind_addr(listener,(struct sockaddr *)&addr);
  rdma_listen(listener,10);//排队长度10
  port = ntohs(rdma_get_src_port(listener));
  printf("listening on port %d.\n", port);

}

int on_event(struct rdma_cm_event *event){

  int r = 0;

  if(event->event == RDMA_CM_EVENT_CONNECT_REQUEST){
    r = on_connect_request(event->id);
  }
  else if (event->event == RDMA_CM_EVENT_ESTABLISHED){
    r = on_connection(event->id->context);
  }
  else if(event->event == RDMA_CM_EVENT_DISCONNECTED){
    r = on_disconnect(event->id);
  }

  //else
  
}
