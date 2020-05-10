#include "include/hello-greet.h"
// #include <event.h>
#include <event2/util.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include <event2/buffer.h>
#include <event2/buffer_compat.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <sys/queue.h>

int MAX_URL = 8192;

void response_cb(struct evhttp_request *req, void *argv) {
  evbuffer *buff = evhttp_request_get_input_buffer(req);
  size_t len = evbuffer_get_length(buff);
  char data[1024];
  bzero(&data, sizeof(data));
  while (1) {
    int n = evbuffer_remove(buff, data, 1024);
    if (n <= 0) {
      break;
    }
    std::cout << "接收到的数据" << data << std::endl;
  }
  event_base_loopbreak((event_base *)argv);
}

int main(int argc, char **argv) {
  char *server_addr = "0.0.0.0";
  int port = 8080;
  struct event_base *base = event_base_new();
  struct evhttp_connection *connect =
      evhttp_connection_base_new(base, NULL, server_addr, port);
  struct evhttp_request *req = evhttp_request_new(response_cb, base);
  evhttp_add_header(evhttp_request_get_output_headers(req), "host",
                    "localhost");
  evhttp_make_request(connect, req, EVHTTP_REQ_POST, "/test");
  event_base_dispatch(base);
  evhttp_connection_free(connect);

  return 0;
}