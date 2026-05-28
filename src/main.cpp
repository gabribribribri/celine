#include <array>
#include <print>

import celine;

int main() {
    LinearApp f {
        {
            { 2, 3, 4 },     //
            { 10, 20, 30 },  //
            { 1, 10, 100 }   //
        }
    };

    Vector<3> v { 1, 1, 1 };
    std::println("{}", f(v));
    std::println("{}", f(1., 1., 1.));


}
