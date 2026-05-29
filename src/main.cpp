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


    constexpr LinearApp g {
        {
            { 1000., 2000., 3000. },  //
            { 4000., 5000., 6000. },  //
            { 7000., 8000, 9000. }    //
        }
    };


    
    LinearApp exp = f+g;
    for(size_t i = 0; i<3; i++) {
        for(size_t j = 0; j<3; j++) {
            std::println("{}", exp.mat[i][j]);
        }
    }
    std::println("bonsoir");
    std::println("+ :\n", exp);
    std::println("-2 :\n", f - g);
    std::println("-1 :\n", - g);
    std::println("* :\n", 11. * g);
    std::println("/ :\n", g/3.);
    std::println("lots :\n", 6*g - 0.67 * f + 0.00011 * g);
    std::println("base :\n", 6.*X - Y*7. + 0.11*Z + T);

    
}
