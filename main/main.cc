#include "include/hello-greet.h"
//  依赖方式
#include "lib/hello-time.h"

#include <iostream>

int main(int argc, char **argv) {
  

  print_localtime();
  std::cout << "Hello World" << std::endl;
  int a = 34242;
  std::cout << "Hello World" << std::endl;
  std::cout << "Hello World:" << a << std::endl;
  std::cout << "Hello World" << std::endl;
  std::string who = "World";
  std::cout << get_greet(who) << std::endl;
  return 0;
}
