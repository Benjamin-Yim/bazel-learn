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
void send_document_cb(struct evhttp_request *req, void *argv) {
  // char *s = "";
  const struct evhttp_uri *evhttp_uri = evhttp_request_get_evhttp_uri(req);
  char url[MAX_URL];
  evhttp_uri_join(const_cast<struct evhttp_uri *>(evhttp_uri), url, MAX_URL);
  std::cout << "请求地址:" << url << std::endl;

  evhttp_add_header(req->output_headers, "token", "来自远方的呼唤");
  struct evbuffer *evbuf = evbuffer_new();
  if (!evbuf) {
    return;
  }

  evbuffer_add_printf(evbuf, "来自服务端响应");
  evhttp_send_reply(req, HTTP_OK, "Success", evbuf);
  evbuffer_free(evbuf);
}

void http_cb(struct evhttp_request *req, void *arg) {
  const struct evhttp_uri *evhttp_uri = evhttp_request_get_evhttp_uri(req);

  char url[MAX_URL];
  const char *host = evhttp_request_get_host(req);

  evhttp_uri_join(const_cast<struct evhttp_uri *>(evhttp_uri), url, MAX_URL);

  std::cout << "请求的URL" << url << std::endl;
  struct evbuffer *evbuf = evbuffer_new();
  if (!evbuf) {
    std::cout << "申请缓存空间失败" << std::endl;
    return;
  }

  evbuffer_add_printf(evbuf, "服务端接收到的地址 %s,给你回应了", url);

  evhttp_send_reply(req, HTTP_OK, "Success", evbuf);
  evbuffer_free(evbuf);
}

int main(int argc, char **argv) {
  int http_port = 8080;
  char *http_address = "0.0.0.0";
  struct event_base *base = event_base_new();
  struct evhttp *http_server = evhttp_new(base);

  if (NULL == http_server) {
    std::cout << "新建 HTTP 服务器失败" << std::endl;
    return -1;
  }

  int ebc = evhttp_bind_socket(http_server, http_address, http_port);

  if (ebc < 0) {
    std::cout << "启动服务器失败，绑定地址和端口失败" << std::endl;
    return -1;
  }

  evhttp_set_cb(http_server, "/test", http_cb, NULL);
  evhttp_set_gencb(http_server, send_document_cb, NULL);
  event_base_dispatch(base);
  evhttp_free(http_server);
  return 0;
}