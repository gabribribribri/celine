#include <array>
#include <initializer_list>
#include <print>
#include <vector>

struct VectorTerm;

// 2. A collection of terms (e.g., 6*x + 7*y)
struct ImageRow {
    std::vector<double> terms;

    constexpr ImageRow() = default;
    constexpr ImageRow(VectorTerm term);
    constexpr ImageRow(std::vector<double>&& row) : terms(std::move(row)) {}

    friend constexpr ImageRow operator+(ImageRow a, ImageRow const& b) {
        size_t i = 0;
        for (double bt : b.terms) {
            if (i >= a.terms.size()) {
                a.terms.push_back(bt);
            } else {
                a.terms.at(i) += bt;
            }
            ++i;
        }
        return a;
    }

    friend constexpr ImageRow operator-(ImageRow a, ImageRow const& b) {}
        

    // friend ImageRow operator-(ImageRow a) {

};

// 1. A single term in a linear combination (e.g., 6.0 * x_1)
struct VectorTerm {
    size_t index;
    double coeff;

    friend constexpr ImageRow operator*(const double coeff, const VectorTerm term) noexcept {
        std::vector<double> out_row(term.index + 1);
        out_row.back() = term.coeff * coeff;
        return out_row;
    }

    friend constexpr ImageRow operator*(const VectorTerm term, const double coeff) noexcept {
        return coeff * term;
    }

};

constexpr ImageRow::ImageRow(VectorTerm term) : terms(term.index+1) {
    terms.back() = term.coeff;
}

// Define global proxies
static constexpr VectorTerm X { .index = 0, .coeff = 1 };
static constexpr VectorTerm Y { .index = 1, .coeff = 1 };
static constexpr VectorTerm Z { .index = 2, .coeff = 1 };
static constexpr VectorTerm T { .index = 3, .coeff = 1 };

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
    constexpr LinearApp(std::initializer_list<ImageRow> exprs) {
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

    constexpr std::array<double, OutDim> operator()(std::array<double, InDim> const& in_vec) const {
        std::array<double, OutDim> out_vec {};
        for (size_t i = 0; i < OutDim; ++i) {
            for (size_t j = 0; j < InDim; ++j) {
                out_vec.at(i) += mat.at(i).at(j) * in_vec.at(j);
            }
        }
        return out_vec;
    }

    constexpr void print_matrix() const {
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

    // f1: R^4 -> R^2
    LinearApp<3, 1> f1 {
        X + 10.0 * Y + 100.0 * Z
    };

    std::array<double, 3> u { { 2.0, 3.0, 4.0 } };
    std::println("f({}) = {}", u, f1(u));

    std::println("Transformation Matrix of f1 :");
    f1.print_matrix();

    // f2: R^4 -> R^2
    LinearApp<4, 2> f2 {
        6. * X + 7. * Y + Z,
        -4. * Y + 2. * Z + T
    };

    std::println("Transformation Matrix of f2 :");
    f2.print_matrix();

    // Input vector (1, 1, 1, 1)
    std::array<double, 4> vec = { 1.0, 1.0, 1.0, 1.0 };
    std::array<double, 2> res = f2(vec);

    std::println("\nf(1, 1, 1, 1) = ({}, {})", res[0], res[1]);
    // Expected: (14, -1)

    return 0;
}
