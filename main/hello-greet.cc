#include "include/hello-greet.h"
#include <string>


std::string get_greet(const std::string &who) {
  return "Do Not Tell Me Hello " + who;
}
