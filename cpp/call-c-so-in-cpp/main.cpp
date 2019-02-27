#include <iostream>
#include <string>
#include "cat.h"

using std::cout;
using std::endl;
using std::string;

int main()
{
  string name = "Felix";
  cout<< "Meet my cat, " << name << "!" <<endl;

  cat_speak(name.c_str());
  return 0;
}
