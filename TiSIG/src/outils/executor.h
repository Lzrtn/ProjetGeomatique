#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <iostream>

class Executor
{
public:
    Executor();
    std::string exec(const char* cmd);
};

#endif // EXECUTOR_H
