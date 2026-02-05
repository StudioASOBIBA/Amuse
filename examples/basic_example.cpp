#include <misc/macros.hpp>
#include <misc/types.hpp>

#include <iostream>

using namespace amuse::core;

int main(int argc_, char** argv_)
{
    constexpr Int32 lhs = 10;
    constexpr Int32 rhs = 20;

    std::cout << "Sum of " << lhs << " and " << rhs << " is " << (lhs + rhs) << std::endl;
}