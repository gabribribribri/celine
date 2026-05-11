module;

#include <print>

export module celine:LinearApp;

// mat[row][col] -> OutDim rows, InDim columns
// f: R^4 -> R^3
// 0 0 0 0
// 0 0 0 0
// 0 0 0 0
export template <std::size_t Row, std::size_t Col>
using Matrix = std::array<std::array<double, Col>, Row>;

export class LinearApp {
public:
    static int something() noexcept {
        std::println("Woaw ! This is working !");
        return 42;
    };
};
