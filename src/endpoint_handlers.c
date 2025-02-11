#include "../headers/hashmap.h"
#include "../headers/http_endpoints.h"
#include <stdio.h>

void html_index_handler(int incoming_sd){
  http_send_file(incoming_sd, "html/index.html", "r", "text/html", "keep-alive");
}

void nfl_json_handler(int incoming_sd){
  http_send_file(incoming_sd, "json/nfl.json", "rb", "application/json", "keep-alive");
}

void print_handler(int incoming_sd){
  printf("Hello, hit printing endpoint\n");
}

void favicon_handler(int incoming_sd){
  http_send_file(incoming_sd, "img/B.ico", "rb", "image/x-icon", "close");
}

void javascript_index_handler(int incoming_sd){
  http_send_file(incoming_sd, "javascript/index.js", "r", "application/javascript", "close");
}

void css_styles_handler(int incoming_sd){
  http_send_file(incoming_sd, "css/styles.css", "r", "text/css", "close");
}

void traffic_light_handler(int incoming_sd){
  http_send_file(incoming_sd, "html/traffic_light.html", "r", "text/html", "keep-alive");
}

void test_raw_msg_handler(int incoming_sd){
  http_send_raw_msg(incoming_sd, "test message", "close");
}


hashmap* create_hashmap(){
  hashmap* hmap = init_map();
  add_node(hmap, "/", html_index_handler);
  add_node(hmap, "/json", nfl_json_handler);
  add_node(hmap, "/print", print_handler);
  add_node(hmap, "/favicon.ico", favicon_handler);
  add_node(hmap, "/index.js", javascript_index_handler);
  add_node(hmap, "/styles.css", css_styles_handler);
  add_node(hmap, "/traffic_light", traffic_light_handler);
  add_node(hmap, "/raw_msg", test_raw_msg_handler);
  return hmap;
}
