#pragma once

#include <algorithm>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace datatools {

// Sorts both inputs first because set_union works on sorted ranges, then returns the values from both.
template <typename T>
[[nodiscard]] std::vector<T> setUnion(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_union(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

// Same as above but takes initializer lists.
template <typename T>
[[nodiscard]] std::vector<T> setUnion(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setUnion(std::vector<T>(left), std::vector<T>(right));
}

// Sorts both inputs and returns the values that are in both, using set_intersection.
template <typename T>
[[nodiscard]] std::vector<T> setIntersection(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

// Same as above but takes initializer lists.
template <typename T>
[[nodiscard]] std::vector<T> setIntersection(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setIntersection(std::vector<T>(left), std::vector<T>(right));
}

// Sorts both inputs and returns the values in left that are not in right, using set_difference.
template <typename T>
[[nodiscard]] std::vector<T> setDifference(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_difference(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

// Same as above but takes initializer lists.
template <typename T>
[[nodiscard]] std::vector<T> setDifference(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setDifference(std::vector<T>(left), std::vector<T>(right));
}

// Sorts both inputs and returns the values that are in only one of them, using set_symmetric_difference.
template <typename T>
[[nodiscard]] std::vector<T> setSymmetricDifference(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_symmetric_difference(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

// Same as above but takes initializer lists.
template <typename T>
[[nodiscard]] std::vector<T> setSymmetricDifference(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setSymmetricDifference(std::vector<T>(left), std::vector<T>(right));
}

// Sorts both inputs and merges them into one sorted vector with merge, keeping duplicates.
template <typename T>
[[nodiscard]] std::vector<T> mergeSorted(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    result.reserve(left.size() + right.size());
    std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

// Same as above but takes initializer lists.
template <typename T>
[[nodiscard]] std::vector<T> mergeSorted(std::initializer_list<T> left, std::initializer_list<T> right) {
    return mergeSorted(std::vector<T>(left), std::vector<T>(right));
}

// Sorts both inputs and checks if every value of subset is also in superset, using includes.
template <typename T>
[[nodiscard]] bool includes(std::vector<T> superset, std::vector<T> subset) {
    std::sort(superset.begin(), superset.end());
    std::sort(subset.begin(), subset.end());
    return std::includes(superset.begin(), superset.end(), subset.begin(), subset.end());
}

// Same as above but takes initializer lists.
template <typename T>
[[nodiscard]] bool includes(std::initializer_list<T> superset, std::initializer_list<T> subset) {
    return includes(std::vector<T>(superset), std::vector<T>(subset));
}

// Union for string vectors, just calls the template version.
inline std::vector<std::string> setUnionStr(std::vector<std::string> left, std::vector<std::string> right) {
    return setUnion(std::move(left), std::move(right));
}

// Intersection for string vectors, just calls the template version.
inline std::vector<std::string> setIntersectionStr(std::vector<std::string> left, std::vector<std::string> right) {
    return setIntersection(std::move(left), std::move(right));
}

}  // namespace datatools
