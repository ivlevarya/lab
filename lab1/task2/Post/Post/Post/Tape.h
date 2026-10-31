#pragma once

#pragma once

#include <vector>

class Command;
class PostMachine;

class Tape {
private:
    int carriage = 0;
    std::vector<int> cells{ 0 };

    friend class Command;
    friend class PostMachine;
};