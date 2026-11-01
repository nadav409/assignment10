#include "AlgorithmShowcase.hpp"

namespace datatools {

std::vector<std::vector<int>> allPermutations(std::vector<int> values) {
    std::sort(values.begin(), values.end());

    std::vector<std::vector<int>> permutations;
    do {
        permutations.push_back(values);
    } while (std::next_permutation(values.begin(), values.end()));

    return permutations;
}

std::vector<std::vector<int>> allPermutations(std::initializer_list<int> values) {
    return allPermutations(std::vector<int>(values));
}

bool nextPermutation(std::vector<int>& values) {
    return std::next_permutation(values.begin(), values.end());
}

bool prevPermutation(std::vector<int>& values) {
    return std::prev_permutation(values.begin(), values.end());
}

std::vector<int> readFromStream(std::istream& input) {
    std::vector<int> values;
    std::copy(std::istream_iterator<int>(input), std::istream_iterator<int>(), std::back_inserter(values));
    return values;
}

std::vector<int> generateSequence(std::size_t count, int startValue) {
    std::vector<int> values(count);
    std::iota(values.begin(), values.end(), startValue);
    return values;
}

std::vector<int> runningTotal(const std::vector<int>& values) {
    std::vector<int> totals(values.size());
    std::partial_sum(values.begin(), values.end(), totals.begin());
    return totals;
}

int dotProduct(const std::vector<int>& left, const std::vector<int>& right) {
    if (left.size() != right.size()) {
        throw std::invalid_argument("dotProduct requires equal sized inputs");
    }
    return std::inner_product(left.begin(), left.end(), right.begin(), 0);
}

int dotProduct(std::initializer_list<int> left, std::initializer_list<int> right) {
    return dotProduct(std::vector<int>(left), std::vector<int>(right));
}

int eraseValue(std::vector<int>& values, int target) {
    const auto newEnd = std::remove(values.begin(), values.end(), target);
    const int removed = static_cast<int>(std::distance(newEnd, values.end()));
    values.erase(newEnd, values.end());
    return removed;
}

}  // namespace datatools
