#include "include/hello-greet.h"
// #include <event.h>
#include <event2/util.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <event2/buffer.h>
#include <event2/buffer_compat.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <sys/queue.h>
int MAX_URL = 8192;
//  bazel build  //main:hello-world  --linkopt='-L/usr/local/lib/'
//  --copt='-levent' --linkopt='-levent' 编译命令
void response_cb(struct evhttp_request *req, void *argv) {
  evbuffer *buff = evhttp_request_get_input_buffer(req);
  size_t len = evbuffer_get_length(buff);
  char data[1024];
  bzero(&data, sizeof(data));
  char *discoverData;
  while (1) {
    int n = evbuffer_remove(buff, data, 1024);
    if (n <= 0) {
      break;
    }
    

    std::cout << "接收到的数据" << data << std::endl;
  }

  rapidjson::Document d;
  d.Parse(data);
  rapidjson::Value &s = d["serverIp"];

  std::cout << "获取到的 serverIp：" << s.GetString() << std::endl;

  event_base_loopbreak((event_base *)argv);
}

int main(int argc, char **argv) {
  char *server_addr = "10.101.20.13";
  int port = 8081;
  struct event_base *base = event_base_new();
  struct evhttp_connection *connect =
      evhttp_connection_base_new(base, NULL, server_addr, port);
  struct evhttp_request *req = evhttp_request_new(response_cb, base);

  evhttp_add_header(evhttp_request_get_output_headers(req), "host",
                    "localhost");
  evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type",
                    "application/x-www-form-urlencoded");

  struct evbuffer *output_buffer = evhttp_request_get_output_buffer(req);
  char *discoverContext =
      "requestData=%7B%0A%09%22Incompatible%22%3A%20false%2C%0A%09%22Protocols%"
      "22%3A%20%5B%22Http%22%5D%2C%0A%09%22ServiceGroups%22%3A%20%5B%7B%0A%09%"
      "09%22Name%22%3A%20%22uat.dsf.tcbase.dsf.demo.java.service1%22%2C%0A%09%"
      "09%22Services%22%3A%20%5B%7B%0A%09%09%09%22Apis%22%3A%20%5B%22exception%"
      "22%5D%2C%0A%09%09%09%22Name%22%3A%20%22linan%22%0A%09%09%7D%5D%2C%0A%09%"
      "09%22Tags%22%3A%20%7B%0A%09%09%09%22Value%22%3A%20%22%22%2C%0A%09%09%09%"
      "22static%22%3A%20false%0A%09%09%7D%2C%0A%09%09%22Version%22%3A%20%2245."
      "0.0.0%22%0A%09%7D%5D%2C%0A%09%22header%22%3A%20%7B%0A%09%09%22CHost%22%"
      "3A%20%22localhost%22%2C%0A%09%09%22CIP%22%3A%20%2210.72.100.103%22%2C%"
      "0A%09%09%22CName%22%3A%20%22dsf.train.michelin.backend%22%2C%0A%09%09%"
      "22CPort%22%3A%20%227419%22%2C%0A%09%09%22CVersion%22%3A%20%222.3.15%22%"
      "2C%0A%09%09%22ClientOs%22%3A%20%22linux%22%2C%0A%09%09%22Content-Type%"
      "22%3A%20%22application%2Fjson%3B%20charset%3DUTF-8%22%2C%0A%09%09%"
      "22PLevel%22%3A%20%225%22%2C%0A%09%09%22PVersion%22%3A%20%222.3.16%22%2C%"
      "0A%09%09%22RTime%22%3A%20%222019-11-27%2014%3A25%3A08.252%22%2C%0A%09%"
      "09%22Rpc-Cient-Info%22%3A%20%22%7B%5C%22type%5C%22%22%2C%0A%09%09%22SID%"
      "22%3A%20%229003%22%2C%0A%09%09%22SName%22%3A%20%"
      "22ServiceContractDiscovery%22%2C%0A%09%09%22SerialNumber%22%3A%200%2C%"
      "0A%09%09%22StatusCode%22%3A%200%2C%0A%09%09%22Timeout%22%3A%200%0A%09%"
      "7D%0A%7D&undefined=";
  evbuffer_add(output_buffer, discoverContext, sizeof(discoverContext));

  evhttp_make_request(connect, req, EVHTTP_REQ_POST,
                      "/central/service/discovery");
  event_base_dispatch(base);
  evhttp_connection_free(connect);

  return 0;
}