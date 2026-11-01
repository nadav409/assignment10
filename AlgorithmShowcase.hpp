#pragma once

#include <algorithm>
#include <initializer_list>
#include <istream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace datatools {

[[nodiscard]] std::vector<std::vector<int>> allPermutations(std::vector<int> values);
[[nodiscard]] std::vector<std::vector<int>> allPermutations(std::initializer_list<int> values);
bool nextPermutation(std::vector<int>& values);
[[nodiscard]] bool prevPermutation(std::vector<int>& values);

template <typename T>
void printWithIterator(const std::vector<T>& values, std::ostream& output, const std::string& delimiter = " ") {
    std::copy(values.begin(), values.end(), std::ostream_iterator<T>(output, delimiter.c_str()));
}

template <typename T>
void printWithIterator(std::initializer_list<T> values, std::ostream& output, const std::string& delimiter = " ") {
    printWithIterator(std::vector<T>(values), output, delimiter);
}

inline void printDoublesWithIterator(const std::vector<double>& values, std::ostream& output, const std::string& delimiter = " ") {
    printWithIterator(values, output, delimiter);
}

[[nodiscard]] std::vector<int> readFromStream(std::istream& input);

template <typename Predicate>
void printFiltered(const std::vector<int>& values, std::ostream& output, Predicate predicate, const std::string& delimiter = " ") {
    std::copy_if(values.begin(), values.end(), std::ostream_iterator<int>(output, delimiter.c_str()), predicate);
}

template <typename Predicate>
void printFiltered(std::initializer_list<int> values, std::ostream& output, Predicate predicate, const std::string& delimiter = " ") {
    printFiltered(std::vector<int>(values), output, predicate, delimiter);
}

[[nodiscard]] std::vector<int> generateSequence(std::size_t count, int startValue);
[[nodiscard]] std::vector<int> runningTotal(const std::vector<int>& values);
int dotProduct(const std::vector<int>& left, const std::vector<int>& right);
int dotProduct(std::initializer_list<int> left, std::initializer_list<int> right);
[[nodiscard]] int eraseValue(std::vector<int>& values, int target);

template <typename Predicate>
[[nodiscard]] int eraseIf(std::vector<int>& values, Predicate predicate) {
    const auto newEnd = std::remove_if(values.begin(), values.end(), predicate);
    const int removed = static_cast<int>(std::distance(newEnd, values.end()));
    values.erase(newEnd, values.end());
    return removed;
}

inline void uniqueInPlace(std::vector<int>& values) {
    const auto newEnd = std::unique(values.begin(), values.end());
    values.erase(newEnd, values.end());
}

}  // namespace datatools
