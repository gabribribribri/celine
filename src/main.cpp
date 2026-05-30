#include <print>

import celine;

int main() {
    constexpr Matrix<3, 3> m = { {
        { 6, 6, 6 },  //
        { 7, 7, 7 },  //
        { 8, 8, 8 }   //
    } };

    constexpr LinearApp fm { m };

    constexpr LinearApp<3, 3> f {
        {
            { 2, 3, 4 },     //
            { 10, 20, 30 },  //
            { 1, 10, 100 }   //
        }
    };

    constexpr LinearApp g {
        {
            { 1000., 2000., 3000. },  //
            { 4000., 5000., 6000. },  //
            { 7000., 8000, 9000. }    //
        }
    };

    constexpr LinearForm b = 56 * Z + X * 0.4;

    LinearApp<3, 3> h {
        X + Y + Z,
        b,
        X - Z + 2 * Y,
    };

    std::println("{}", h);

    std::println("{}\n{}", f, g);
    std::println("bonsoir");
    std::println("+ :{}\n", f + g);
    std::println("-2 :{}\n", f - g);
    std::println("-1 :{}\n", -g);
    std::println("* :{}\n", 11. * g);
    std::println("/ :{}\n", g / 3.);
    std::println("lots :{}\n", 6 * g - 0.67 * f + 0.00011 * h);
    std::println("base :{}\n", 6. * X - Y * 7. + 0.11 * Z + T);

    std::println("{}", b);
}
