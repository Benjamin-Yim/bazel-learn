#include "hello-time.h"
#include <ctime>
#include <iostream>

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << "This Current Time：" << std::asctime(std::localtime(&result)) << std::endl;
}