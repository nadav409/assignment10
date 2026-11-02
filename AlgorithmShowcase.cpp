#include "AlgorithmShowcase.hpp"

namespace datatools {

// Sorts first so next_permutation starts from the smallest order, then collects every permutation.
std::vector<std::vector<int>> allPermutations(std::vector<int> values) {
    std::sort(values.begin(), values.end());

    std::vector<std::vector<int>> permutations;
    do {
        permutations.push_back(values);
    } while (std::next_permutation(values.begin(), values.end()));

    return permutations;
}

// Same as above but takes an initializer list like {1, 2, 3}.
std::vector<std::vector<int>> allPermutations(std::initializer_list<int> values) {
    return allPermutations(std::vector<int>(values));
}

// Changes the vector to the next permutation, returns false when it wraps back to the first one.
bool nextPermutation(std::vector<int>& values) {
    return std::next_permutation(values.begin(), values.end());
}

// Changes the vector to the previous permutation, returns false when it wraps back to the last one.
bool prevPermutation(std::vector<int>& values) {
    return std::prev_permutation(values.begin(), values.end());
}

// Reads all the ints from the stream with istream_iterator and copies them into a vector.
std::vector<int> readFromStream(std::istream& input) {
    std::vector<int> values;
    std::copy(std::istream_iterator<int>(input), std::istream_iterator<int>(), std::back_inserter(values));
    return values;
}

// Makes count numbers in a row starting from startValue, using iota.
std::vector<int> generateSequence(std::size_t count, int startValue) {
    std::vector<int> values(count);
    std::iota(values.begin(), values.end(), startValue);
    return values;
}

// Returns the running sums of the values with partial_sum.
std::vector<int> runningTotal(const std::vector<int>& values) {
    std::vector<int> totals(values.size());
    std::partial_sum(values.begin(), values.end(), totals.begin());
    return totals;
}

// Multiplies matching elements and adds them up with inner_product.
// Throws if the sizes are different.
int dotProduct(const std::vector<int>& left, const std::vector<int>& right) {
    if (left.size() != right.size()) {
        throw std::invalid_argument("dotProduct requires equal sized inputs");
    }
    return std::inner_product(left.begin(), left.end(), right.begin(), 0);
}

// Same as above but takes initializer lists.
int dotProduct(std::initializer_list<int> left, std::initializer_list<int> right) {
    return dotProduct(std::vector<int>(left), std::vector<int>(right));
}

// Moves the values that stay to the front with remove, then erase really shrinks the vector.
// Returns how many values were removed.
int eraseValue(std::vector<int>& values, int target) {
    const auto newEnd = std::remove(values.begin(), values.end(), target);
    const int removed = static_cast<int>(std::distance(newEnd, values.end()));
    values.erase(newEnd, values.end());
    return removed;
}

}  // namespace datatools
