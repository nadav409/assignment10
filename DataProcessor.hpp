#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace datatools {

class DataProcessor {
public:
    DataProcessor() = default;
    DataProcessor(std::initializer_list<double> values) : data_(values) {}
    // Fills the data with count numbers in a row starting from startValue, using iota.
    DataProcessor(std::size_t count, double startValue) : data_(count) {
        std::iota(data_.begin(), data_.end(), startValue);
    }

    [[nodiscard]] std::size_t size() const noexcept { return data_.size(); }
    [[nodiscard]] bool empty() const noexcept { return data_.empty(); }

    [[nodiscard]] const std::vector<double>& getData() const noexcept { return data_; }
    [[nodiscard]] std::vector<double>& getData() noexcept { return data_; }

    // Adds up all the values with accumulate.
    [[nodiscard]] double sum() const {
        return std::accumulate(data_.begin(), data_.end(), 0.0);
    }

    // Multiplies all the values with accumulate and multiplies<>, starting from 1.
    [[nodiscard]] double product() const {
        return std::accumulate(data_.begin(), data_.end(), 1.0, std::multiplies<>());
    }

    // Returns the average of the values. Throws if there is no data.
    double average() const {
        if (data_.empty()) {
            throw std::invalid_argument("average of empty DataProcessor");
        }
        return sum() / static_cast<double>(data_.size());
    }

    // Returns the smallest value with min_element. Throws if there is no data.
    double min() const {
        if (data_.empty()) {
            throw std::invalid_argument("min of empty DataProcessor");
        }
        return *std::min_element(data_.begin(), data_.end());
    }

    // Returns the biggest value with max_element. Throws if there is no data.
    double max() const {
        if (data_.empty()) {
            throw std::invalid_argument("max of empty DataProcessor");
        }
        return *std::max_element(data_.begin(), data_.end());
    }

    // Counts how many values pass the predicate, using count_if.
    template <typename Predicate>
    [[nodiscard]] std::size_t countIf(Predicate predicate) const {
        return static_cast<std::size_t>(std::count_if(data_.begin(), data_.end(), predicate));
    }

    // Checks if every value passes the predicate.
    template <typename Predicate>
    [[nodiscard]] bool allOf(Predicate predicate) const {
        return std::all_of(data_.begin(), data_.end(), predicate);
    }

    // Checks if at least one value passes the predicate.
    template <typename Predicate>
    [[nodiscard]] bool anyOf(Predicate predicate) const {
        return std::any_of(data_.begin(), data_.end(), predicate);
    }

    // Checks that no value passes the predicate.
    template <typename Predicate>
    [[nodiscard]] bool noneOf(Predicate predicate) const {
        return std::none_of(data_.begin(), data_.end(), predicate);
    }

    // Returns the first value that passes the predicate, using find_if. Throws if nothing matches.
    template <typename Predicate>
    double findIf(Predicate predicate) const {
        const auto iterator = std::find_if(data_.begin(), data_.end(), predicate);
        if (iterator == data_.end()) {
            throw std::invalid_argument("no matching element found");
        }
        return *iterator;
    }

    // Multiplies matching values of both processors and sums them with inner_product.
    // Throws if the sizes are different.
    double innerProduct(const DataProcessor& other) const {
        if (data_.size() != other.data_.size()) {
            throw std::invalid_argument("innerProduct requires equal sized inputs");
        }
        return std::inner_product(data_.begin(), data_.end(), other.data_.begin(), 0.0);
    }

    // Sorts the data from small to big.
    void sort() {
        std::sort(data_.begin(), data_.end());
    }

    // Sorts the data with the given comparator.
    template <typename Comparator>
    void sort(Comparator comparator) {
        std::sort(data_.begin(), data_.end(), comparator);
    }

    // Puts the smallest count values sorted at the front with partial_sort, the rest stay in any order.
    void partialSort(std::size_t count) {
        count = std::min(count, data_.size());
        std::partial_sort(data_.begin(), data_.begin() + static_cast<std::ptrdiff_t>(count), data_.end());
    }

    // Changes every value in place with transform.
    template <typename Transform>
    void applyTransform(Transform transform) {
        std::transform(data_.begin(), data_.end(), data_.begin(), transform);
    }

    // Returns a new DataProcessor with the transform applied, this one stays the same.
    template <typename Transform>
    [[nodiscard]] DataProcessor transformed(Transform transform) const {
        DataProcessor result;
        result.data_.resize(data_.size());
        std::transform(data_.begin(), data_.end(), result.data_.begin(), transform);
        return result;
    }

    // Returns a new DataProcessor with the running sums, using partial_sum.
    [[nodiscard]] DataProcessor partialSums() const {
        DataProcessor result;
        result.data_.resize(data_.size());
        std::partial_sum(data_.begin(), data_.end(), result.data_.begin());
        return result;
    }

    // Returns a new DataProcessor with only the values that pass the predicate, using copy_if.
    template <typename Predicate>
    [[nodiscard]] DataProcessor filter(Predicate predicate) const {
        DataProcessor result;
        std::copy_if(data_.begin(), data_.end(), std::back_inserter(result.data_), predicate);
        return result;
    }

    // Replaces every value that passes the predicate with newValue.
    template <typename Predicate>
    void replaceIf(Predicate predicate, double newValue) {
        std::replace_if(data_.begin(), data_.end(), predicate, newValue);
    }

    // Sets all the values to the same value.
    void fill(double value) {
        std::fill(data_.begin(), data_.end(), value);
    }

    // Resizes the data to count and fills it with values from the generator.
    template <typename Generator>
    void generate(std::size_t count, Generator generator) {
        data_.resize(count);
        std::generate(data_.begin(), data_.end(), generator);
    }

    // Moves the kept values to the front with remove_if, then erase really shrinks the vector.
    // Returns how many values were removed.
    template <typename Predicate>
    [[nodiscard]] int removeIf(Predicate predicate) {
        const auto newEnd = std::remove_if(data_.begin(), data_.end(), predicate);
        const int removed = static_cast<int>(std::distance(newEnd, data_.end()));
        data_.erase(newEnd, data_.end());
        return removed;
    }

    // Removes neighbouring duplicates with unique, then erase cuts off the leftover end.
    void unique() {
        const auto newEnd = std::unique(data_.begin(), data_.end());
        data_.erase(newEnd, data_.end());
    }

    // Reverses the order of the data.
    void reverse() {
        std::reverse(data_.begin(), data_.end());
    }

    // Rotates the data left by shift places with rotate, taking shift % size first.
    void rotate(std::size_t shift) {
        if (data_.empty()) {
            return;
        }
        shift %= data_.size();
        std::rotate(data_.begin(), data_.begin() + static_cast<std::ptrdiff_t>(shift), data_.end());
    }

    // Moves the values that pass the predicate to the front and returns how many there are.
    template <typename Predicate>
    [[nodiscard]] int partition(Predicate predicate) {
        const auto iterator = std::partition(data_.begin(), data_.end(), predicate);
        return static_cast<int>(std::distance(data_.begin(), iterator));
    }

    // Same as partition but with stable_partition, so each group keeps its original order.
    template <typename Predicate>
    [[nodiscard]] int stablePartition(Predicate predicate) {
        const auto iterator = std::stable_partition(data_.begin(), data_.end(), predicate);
        return static_cast<int>(std::distance(data_.begin(), iterator));
    }

    // Puts the value that belongs at index in its sorted spot with nth_element.
    // Smaller values end up before it and bigger ones after it.
    void nthElement(std::size_t index) {
        if (index >= data_.size()) {
            throw std::out_of_range("nthElement index out of range");
        }
        std::nth_element(data_.begin(), data_.begin() + static_cast<std::ptrdiff_t>(index), data_.end());
    }

    // Builds a string of all the values with the delimiter between them.
    [[nodiscard]] std::string toString(const std::string& delimiter = ", ") const {
        std::ostringstream output;
        for (std::size_t index = 0; index < data_.size(); ++index) {
            if (index > 0) {
                output << delimiter;
            }
            output << formatValue(data_[index]);
        }
        return output.str();
    }

private:
    // Turns a number into text. Whole numbers print with no decimal point,
    // and other numbers lose the extra zeros at the end.
    static std::string formatValue(double value) {
        constexpr double epsilon = 1e-9;
        const double rounded = std::round(value);
        if (std::abs(value - rounded) < epsilon) {
            std::ostringstream output;
            output << static_cast<long long>(rounded);
            return output.str();
        }

        std::ostringstream output;
        output.precision(15);
        output << value;
        std::string text = output.str();
        const auto dotPos = text.find('.');
        if (dotPos != std::string::npos) {
            while (!text.empty() && text.back() == '0') {
                text.pop_back();
            }
            if (!text.empty() && text.back() == '.') {
                text.pop_back();
            }
        }
        return text;
    }

    std::vector<double> data_;
};

}  // namespace datatools
