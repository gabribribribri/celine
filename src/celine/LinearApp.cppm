module;

#include <array>
#include <initializer_list>
#include <print>
#include <ranges>
#include <utility>

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
    // shut up clang you can't hold my genius
    constexpr LinearApp(double (&&raw)[OutDim][InDim]) {  // NOLINT
        for (size_t i = 0; i < OutDim; ++i) {
            for (size_t j = 0; j < InDim; ++j) {
                mat[i][j] = raw[i][j];
            }
        }
    }

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

    template <typename... Args>
        requires(sizeof...(Args) == InDim) && (std::same_as<double, Args> && ...)
    constexpr Vector<OutDim> operator()(Args... in_vec) const noexcept {
        Vector<OutDim> out_vec;
        for (std::size_t i = 0; auto& row : mat) {
            out_vec[i++] = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return ((mat[i][Is] * in_vec) + ... + 0.0);
            }(std::make_index_sequence<InDim>());
        }
        return out_vec;
    }
};

export template <std::size_t InDim>
using LinearForm = LinearApp<InDim, 1>;

// Linear Form template specialization
// export template <std::size_t InDim>
// class LinearApp<InDim, 1> {
//     constexpr LinearApp(double (&&row)[InDim]) {  // NOLINT
//         for (size_t j = 0; j < InDim; ++j) {
//             mat[0][j] = raw[j];
//         }
//     }
// };

export template <std::size_t InDim>
LinearApp<InDim, 1>::LinearApp(double (&&row)[InDim]) {
    for (size_t j = 0; j < InDim; ++j) {
        mat[0][j] = raw[j];
    }
}

export template <std::size_t FirstSize, std::size_t SecondSize, size_t RetSize = std::max(FirstSize, SecondSize)>
constexpr LinearForm<RetSize> operator+(LinearForm<FirstSize> const& a, LinearForm<SecondSize> const& b) noexcept {
    return [=]<std::size_t... Is>(std::index_sequence<Is...>) {
        return LinearForm<RetSize> { (
            (Is < FirstSize ? a[0][Is] : 0.0) +
            (Is < SecondSize ? b[0][Is] : 0.0))... };
    };
}

template <size_t FirstSize, size_t SecondSize, size_t RetSize = std::max(FirstSize, SecondSize)>
constexpr Vector<RetSize> sum_arrays(Vector<FirstSize> const& a, Vector<SecondSize> const& b) {
    return [=]<size_t... Is>(std::index_sequence<Is...>) {
        return Vector<RetSize> { (
            (Is < FirstSize ? a[Is] : 0.0) +
            (Is < SecondSize ? b[Is] : 0.0))... };
    }(std::make_index_sequence<RetSize>());
}
