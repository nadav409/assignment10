#pragma once

#include <algorithm>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace datatools {

template <typename T>
[[nodiscard]] std::vector<T> setUnion(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_union(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

template <typename T>
[[nodiscard]] std::vector<T> setUnion(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setUnion(std::vector<T>(left), std::vector<T>(right));
}

template <typename T>
[[nodiscard]] std::vector<T> setIntersection(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

template <typename T>
[[nodiscard]] std::vector<T> setIntersection(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setIntersection(std::vector<T>(left), std::vector<T>(right));
}

template <typename T>
[[nodiscard]] std::vector<T> setDifference(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_difference(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

template <typename T>
[[nodiscard]] std::vector<T> setDifference(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setDifference(std::vector<T>(left), std::vector<T>(right));
}

template <typename T>
[[nodiscard]] std::vector<T> setSymmetricDifference(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    std::set_symmetric_difference(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

template <typename T>
[[nodiscard]] std::vector<T> setSymmetricDifference(std::initializer_list<T> left, std::initializer_list<T> right) {
    return setSymmetricDifference(std::vector<T>(left), std::vector<T>(right));
}

template <typename T>
[[nodiscard]] std::vector<T> mergeSorted(std::vector<T> left, std::vector<T> right) {
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    std::vector<T> result;
    result.reserve(left.size() + right.size());
    std::merge(left.begin(), left.end(), right.begin(), right.end(), std::back_inserter(result));
    return result;
}

template <typename T>
[[nodiscard]] std::vector<T> mergeSorted(std::initializer_list<T> left, std::initializer_list<T> right) {
    return mergeSorted(std::vector<T>(left), std::vector<T>(right));
}

template <typename T>
[[nodiscard]] bool includes(std::vector<T> superset, std::vector<T> subset) {
    std::sort(superset.begin(), superset.end());
    std::sort(subset.begin(), subset.end());
    return std::includes(superset.begin(), superset.end(), subset.begin(), subset.end());
}

template <typename T>
[[nodiscard]] bool includes(std::initializer_list<T> superset, std::initializer_list<T> subset) {
    return includes(std::vector<T>(superset), std::vector<T>(subset));
}

inline std::vector<std::string> setUnionStr(std::vector<std::string> left, std::vector<std::string> right) {
    return setUnion(std::move(left), std::move(right));
}

inline std::vector<std::string> setIntersectionStr(std::vector<std::string> left, std::vector<std::string> right) {
    return setIntersection(std::move(left), std::move(right));
}

}  // namespace datatools
