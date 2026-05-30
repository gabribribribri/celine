module;

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <format>
#include <print>
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
public:
    // c'est définitivement comme ça la meilleure manière de stocker mat !
    Matrix<OutDim, InDim> mat;

    constexpr LinearApp() noexcept : mat {} {};

    // ugly but here for inline construction
    constexpr LinearApp(double const (&raw)[OutDim][InDim]) noexcept {  // NOLINT
        for (size_t i = 0; i < OutDim; ++i) {
            for (size_t j = 0; j < InDim; ++j) {
                mat[i][j] = raw[i][j];
            }
        }
        // std::copy(raw.begin(), raw.end(), mat.begin());
    }

    constexpr LinearApp(Matrix<OutDim, InDim> const& other_mat) : mat { other_mat } {}

    static constexpr LinearApp with_last(double val) noexcept
        requires(InDim >= 1 && OutDim >= 1)
    {
        LinearApp app {};
        app.mat.back().back() = val;
        return app;
    }

    template <std::size_t... LinesInDims>
        requires((LinesInDims <= InDim) && ...) && (sizeof...(LinesInDims) == OutDim)
    constexpr LinearApp(LinearApp<LinesInDims, 1> const&... lines) noexcept : mat {} {  // NOLINT
        // I know state mutating fold expressions in lambdas is an anti-pattern
        // I just don't care because the alternatives are damn ugly
        int i = 0;
        ([&, this] {
            for (size_t j = 0; j < LinesInDims; ++j) {
                mat[i][j] = lines.mat[0][j];
            }
            i++;
        }(),
         ...);
    }

    constexpr Vector<OutDim> operator()(Vector<InDim> const& in_vec) const noexcept {
        Vector<OutDim> out_vec;
        for (size_t i = 0; i < OutDim; ++i) {
            double term = 0.;
            for (size_t j = 0; j < InDim; ++j) {
                term += mat[i][j] * in_vec[j];
            }
            out_vec.at(i) = term;
        }

        return out_vec;
    }

    template <typename... Args>
        requires(sizeof...(Args) == InDim) && (std::convertible_to<double, Args> && ...)
    constexpr Vector<OutDim> operator()(Args... in_vec) const noexcept {
        return this->operator()(Vector<InDim> { static_cast<double>(in_vec)... });
    }

    friend constexpr LinearApp operator+(LinearApp const& lhs, LinearApp const& rhs) noexcept {
        LinearApp new_app = lhs;
        for (std::size_t i = 0; i < OutDim; ++i) {
            for (std::size_t j = 0; j < InDim; ++j) {
                new_app.mat[i][j] += rhs.mat[i][j];
            }
        }
        return new_app;
    }

    friend constexpr LinearApp operator-(LinearApp const& app) noexcept {
        LinearApp new_app = app;
        for (auto& vec : new_app.mat) {
            for (auto& val : vec) {
                val *= -1;
            }
        }
        return new_app;
    }

    friend constexpr LinearApp operator-(LinearApp const& lhs, LinearApp const& rhs) noexcept {
        return lhs + (-rhs);
    }

    friend constexpr LinearApp operator*(double coef, LinearApp const& app) noexcept {
        LinearApp new_app;
        for (std::size_t i = 0; i < OutDim; ++i) {
            for (std::size_t j = 0; j < InDim; ++j) {
                new_app.mat[i][j] = coef * app.mat[i][j];
            }
        }
        return new_app;
    }

    friend constexpr LinearApp operator*(LinearApp const& app, double coef) noexcept {
        return coef * app;
    }

    friend constexpr LinearApp operator/(LinearApp const& app, double coef) noexcept {
        return (1 / coef) * app;
    }
};

export template <std::size_t FirstSize, std::size_t SecondSize, size_t RetSize = std::max(FirstSize, SecondSize)>
constexpr LinearApp<RetSize, 1> operator+(LinearApp<FirstSize, 1> const& lhs, LinearApp<SecondSize, 1> const& rhs) noexcept {
    return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return LinearApp<RetSize, 1> { { { (
            (Is < FirstSize ? lhs.mat[0][Is] : 0.0) +
            (Is < SecondSize ? rhs.mat[0][Is] : 0.0))... } } };
    }(std::make_index_sequence<RetSize>());
}

export template <std::size_t FirstSize, std::size_t SecondSize, size_t RetSize = std::max(FirstSize, SecondSize)>
constexpr LinearApp<RetSize, 1> operator-(LinearApp<FirstSize, 1> const& lhs, LinearApp<SecondSize, 1> const& rhs) noexcept {
    return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
        return LinearApp<RetSize, 1> { { { (
            (Is < FirstSize ? lhs.mat[0][Is] : 0.0) -
            (Is < SecondSize ? rhs.mat[0][Is] : 0.0))... } } };
    }(std::make_index_sequence<RetSize>());
}

export inline constexpr LinearApp X = LinearApp<1, 1>::with_last(1.0);
export inline constexpr LinearApp Y = LinearApp<2, 1>::with_last(1.0);
export inline constexpr LinearApp Z = LinearApp<3, 1>::with_last(1.0);
export inline constexpr LinearApp T = LinearApp<4, 1>::with_last(1.0);

template <std::size_t InDim, size_t OutDim>
struct std::formatter<LinearApp<InDim, OutDim>> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    constexpr auto format(LinearApp<InDim, OutDim> const& app, std::format_context& ctx) const {
        std::string result = std::format("LinearApp : {} -> {}\n", InDim, OutDim);
        for (size_t i = 0; i < OutDim; ++i) {
            if (OutDim == 1) {
                result += "[  ";
            } else if (i == 0) {
                result += "┏  ";
            } else if (i + 1 == OutDim) {
                result += "┗  ";
            } else {
                result += "┃  ";
            }
            for (size_t j = 0; j < InDim; ++j) {
                result += std::format("{}", app.mat[i][j]) + (j == InDim - 1 ? "" : "\t");
            }

            if (OutDim == 1) {
                result += "\t]\n";
            } else if (i == 0) {
                result += "\t┓\n";
            } else if (i + 1 == OutDim) {
                result += "\t┛\n";
            } else {
                result += "\t┃\n";
            }
        }
        return std::format_to(ctx.out(), "{}", result);
    }
};
