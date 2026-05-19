#include <array>
#include <print>

import celine;

int main() {
    LinearApp<3, 3> f { std::array<std::array<double, 3>, 3> {
        std::array<double, 3> { 2, 3, 4 },
        std::array<double, 3> { 10, 20, 30 },
        std::array<double, 3> {1, 10, 100}
    } };

    Vector<3> v {1, 1, 1};
    std::println("{}", f(v));
}
