#include "include/hello-greet.h"
#include <iostream>

int main(int argc, char **argv) {
  std::cout << "Hello World" << std::endl;
  int a = 34242;
  std::cout << "Hello World" << std::endl;
  std::cout << "Hello World:" << a << std::endl;
  std::cout << "Hello World" << std::endl;
  std::string who = "World";
  std::cout << get_greet(who) << std::endl;
  return 0;
}
