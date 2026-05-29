module;

#include <algorithm>
#include <array>
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

    constexpr LinearApp() noexcept = default;

    // shut up clang you can't hold my genius
    constexpr LinearApp(double (&&raw)[OutDim][InDim]) noexcept {  // NOLINT
        for (size_t i = 0; i < OutDim; ++i) {
            for (size_t j = 0; j < InDim; ++j) {
                mat[i][j] = raw[i][j];
            }
        }
    }

    static constexpr LinearApp with_last(double val) noexcept {
        LinearApp app {};
        app.mat.back().back() = val;
        return app;
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

    friend constexpr LinearApp operator+(LinearApp const& lhs, LinearApp const& rhs) noexcept {
        LinearApp new_app = lhs;
        for (std::size_t i = 0; auto& vec : rhs.mat) {
            for (std::size_t j = 0; auto val : vec) {
                new_app.mat[i][j++] += val;
            }
            i++;
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
        for (std::size_t i = 0; auto& vec : app.mat) {
            for (std::size_t j = 0; auto val : vec) {
                new_app.mat[i++][j++] = coef * val;
            }
        }
        return new_app;
    }

    friend constexpr LinearApp operator*(LinearApp const& app, double coef) noexcept {
        return coef * app;
    }

    friend constexpr LinearApp operator/(LinearApp const& app, double coef) noexcept {
        return (1/coef) * app;
    }
};

export template <std::size_t FirstSize, std::size_t SecondSize, size_t RetSize = std::max(FirstSize, SecondSize)>
constexpr LinearApp<RetSize, 1> operator+(LinearApp<FirstSize, 1> const& lhs, LinearApp<SecondSize, 1> const& rhs) noexcept {
    return [=]<std::size_t... Is>(std::index_sequence<Is...>) {
        return LinearApp<RetSize, 1> { { { (
            (Is < FirstSize ? lhs.mat[0][Is] : 0.0) +
            (Is < SecondSize ? rhs.mat[0][Is] : 0.0))... } } };
    }(std::make_index_sequence<RetSize>());
}

export template <std::size_t FirstSize, std::size_t SecondSize, size_t RetSize = std::max(FirstSize, SecondSize)>
constexpr LinearApp<RetSize, 1> operator-(LinearApp<FirstSize, 1> const& lhs, LinearApp<SecondSize, 1> const& rhs) noexcept {
    return [=]<std::size_t... Is>(std::index_sequence<Is...>) {
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

    auto format(LinearApp<InDim, OutDim> const& app, std::format_context& ctx) const {
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
