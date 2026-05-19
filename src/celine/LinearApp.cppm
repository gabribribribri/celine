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

export template <std::size_t Dim>
using Vector = std::array<double, Dim>;

export template <std::size_t InDim, std::size_t OutDim>
class LinearApp {
private:
    // c'est définitivement comme ça la meilleure manière de stocker mat !
    Matrix<OutDim, InDim> mat;

public:
    constexpr LinearApp(Matrix<OutDim, InDim> matrix) : mat(matrix) {}

    constexpr Vector<OutDim> operator()(Vector<InDim> const& in_vec) const noexcept {
        Vector<OutDim> out_vec;
        for (size_t i = 0; i < OutDim; ++i) {
            double term = 0.;
            for (size_t j = 0; j < InDim; ++j) {
                term += mat.at(i).at(j) * in_vec.at(j);
            }
            out_vec.at(i) = term;
        }

        return out_vec;
    }

    // something like that
    // template<LinearApp<InDim, 1>... Bzzz>
    // constexpr LinearApp(Bzzz... bzzz) {
    // }
};
