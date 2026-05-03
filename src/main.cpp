#include <array>
#include <initializer_list>
#include <print>
#include <vector>

// 2. A collection of terms (e.g., 6*x + 7*y)
struct ImageRow {
    std::vector<double> terms;

    ImageRow() = default;
    ImageRow(std::vector<double>&& row) : terms(std::move(row)) {}

    friend ImageRow operator+(ImageRow a, ImageRow const& b) {
        for (double t : b.terms) {
            a.terms.push_back(t);
        }
        return a;
    }

    friend ImageRow operator-(ImageRow a, ImageRow const& b) {
        for (double t : b.terms) {
            a.terms.push_back(-t);
        }
        return a;
    }

    friend ImageRow operator-(ImageRow a) {
        for (double& term : a.terms) {
            term *= -1;
        }
        return a;
    }

    friend constexpr ImageRow operator*(double scalar, ImageRow a) noexcept {
        for (double& t : a.terms) {
            t *= scalar;
        }
        return a;
    }
};

// 1. A single term in a linear combination (e.g., 6.0 * x_1)
struct Term {
    size_t index;
    double coeff;

    friend constexpr ImageRow operator*(const double coeff, const Term term) noexcept {
        std::vector<double> out_row(term.index);
        out_row.back() = term.coeff * coeff;
        return std::move(out_row);
    }

    friend constexpr ImageRow operator+(ImageRow row, const Term term) noexcept {
        if (row.terms.size() < term.index) {
            row.terms.resize(term.index);
        }
        row.terms.at(term.index) += term.coeff;
        return row;
    }
};

// Define global proxies
static constexpr Term X { .index = 0, .coeff = 1 };
static constexpr Term Y { .index = 1, .coeff = 1 };
static constexpr Term Z { .index = 2, .coeff = 1 };
static constexpr Term T { .index = 3, .coeff = 1 };

// 4. The Main Class
template <size_t InDim, size_t OutDim>
class LinearApp {
private:
    // mat[row][col] -> OutDim rows, InDim columns
    // f: R^4 -> R^3
    // 0 0 0 0
    // 0 0 0 0
    // 0 0 0 0
    std::array<std::array<double, InDim>, OutDim> mat {};

public:
    // { 6*X + 7*Y, ... } syntax
    LinearApp(std::initializer_list<ImageRow> exprs) {
        // TODO use enumerate instead of ugly floaty variables
        size_t row = 0;
        for (ImageRow const& e : exprs) {
            if (row >= OutDim) {
                break;
            }
            size_t col = 0;
            for (const auto& term : e.terms) {
                if (col >= InDim) {
                    break;
                }
                mat.at(row).at(col) += term;
                col++;
            }
            row++;
        }
    }

    std::array<double, OutDim> operator()(std::array<double, InDim> const& in_vec) const {
        std::array<double, OutDim> out_vec {};
        for (size_t i = 0; i < OutDim; ++i) {
            for (size_t j = 0; j < InDim; ++j) {
                out_vec.at(i) += mat.at(i).at(j) * in_vec.at(j);
            }
        }
        return out_vec;
    }

    void print_matrix() const {
        for (size_t i = 0; i < OutDim; ++i) {
            std::print("[ ");
            for (size_t j = 0; j < InDim; ++j) {
                std::print("{}{}", mat.at(i).at(j), (j == InDim - 1 ? "" : "\t"));
            }
            std::println(" ]");
        }
    }
};

// --- Test Case ---

int main() {
    // f: R^4 -> R^2
    LinearApp<4, 2> f {
        6. * X + 7. * Y + 1. *Z,
        -4. * Y + 2. * Z + 1. * T
    };

    std::println("Transformation Matrix :");
    f.print_matrix();

    // Input vector (1, 1, 1, 1)
    std::array<double, 4> vec = { 1.0, 1.0, 1.0, 1.0 };
    std::array<double, 2> res = f(vec);

    std::println("\nf(1, 1, 1, 1) = ({}, {})", res[0], res[1]);
    // Expected: (14, -1)

    return 0;
}
