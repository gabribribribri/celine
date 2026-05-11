module;

#include <print>

export module celine:LinearApp;

// keep this here until we have real testing
namespace TestNotBroken {
export int the_funniest_number() {
    return 67;
}
}  // namespace TestNotBroken

// mat[row][col] -> OutDim rows, InDim columns
// f: R^4 -> R^3
// 0 0 0 0
// 0 0 0 0
// 0 0 0 0
export template <std::size_t Row, std::size_t Col>
using Matrix = std::array<std::array<double, Col>, Row>;


export template<std::size_t InDim, std::size_t OutDim>
class LinearApp {
public:

    // something like that
    // template<LinearApp<InDim, 1>... Bzzz>
    // constexpr LinearApp(Bzzz... bzzz) {
        
    // }
};
