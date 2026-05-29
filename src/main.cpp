#include <array>
#include <print>

import celine;

int main() {
    constexpr LinearApp f {
        {
            { 2, 3, 4 },     //
            { 10, 20, 30 },  //
            { 1, 10, 100 }   //
        }
    };

    Vector<3> v { 1, 1, 1 };
    std::println("{}", f(v));
    std::println("{}", f(1., 1., 1.));

    constexpr LinearApp g {
        {
        {1,2,3,4,5},
        {10, 20, 30, 40, 50},
        {100, 200, 300, 400, 500},
    }
    };

    std::println("{}", g(0., 1., 0., 1., 1.));


    constexpr LinearApp f1 {{{1., 2., 3., 4., 5.}}};
}
