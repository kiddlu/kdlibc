#include <iostream>
#include <string>

#include "cat.h"

using namespace std;

Cat::Cat(const string & name):_name(name){}

void Cat::speak()
{
  cout << "Meow! I'm " << _name << endl;
}
