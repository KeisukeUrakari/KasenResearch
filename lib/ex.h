#pragma once

#include <iostream>

class Hoge {
  public:
    Hoge() {
        std::cout << "const" << std::endl;
    }

    ~Hoge() {
        std::cout << "dest" << std::endl;
    }
};

void func();