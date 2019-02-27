#if 0
#include <cstdio>

int add(int x, int y)
{
    std::printf("This C++ Func");
    return(x+y);
}
#else
#include <iostream>

int add(int x, int y)
{
    std::cout << "This C++ Func" << std::endl;
    return(x+y);
}
#endif
