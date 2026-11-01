#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "AlgorithmShowcase.hpp"
#include "DataProcessor.hpp"
#include "SetOperations.hpp"
#include "TextAnalyzer.hpp"

#include <sstream>
#include <vector>

using namespace datatools;

TEST_CASE("Student - DataProcessor empty sum and product") {
    DataProcessor dp;
    CHECK(dp.sum() == doctest::Approx(0.0));
    CHECK(dp.product() == doctest::Approx(1.0));
}

TEST_CASE("Student - DataProcessor toString trims integers") {
    DataProcessor dp({5.0, 2.5, 3.0});
    CHECK(dp.toString() == "5, 2.5, 3");
}

TEST_CASE("Student - DataProcessor findIf finds first match") {
    DataProcessor dp({1.0, 4.0, 2.0, 4.0});
    CHECK(dp.findIf([](double value) { return value == 4.0; }) == doctest::Approx(4.0));
}

TEST_CASE("Student - DataProcessor generate overwrites data") {
    DataProcessor dp({1.0, 2.0, 3.0});
    int counter = 0;
    dp.generate(4, [&counter]() { return static_cast<double>(10 + counter++); });
    CHECK(dp.getData() == std::vector<double>{10.0, 11.0, 12.0, 13.0});
}

TEST_CASE("Student - DataProcessor rotate larger than size") {
    DataProcessor dp({1.0, 2.0, 3.0});
    dp.rotate(8);
    CHECK(dp.getData() == std::vector<double>{3.0, 1.0, 2.0});
}

TEST_CASE("Student - DataProcessor nthElement places pivot") {
    DataProcessor dp({9.0, 1.0, 7.0, 3.0, 5.0});
    dp.nthElement(2);
    CHECK(dp.getData()[2] == doctest::Approx(5.0));
}

TEST_CASE("Student - DataProcessor stablePartition preserves order") {
    DataProcessor dp({4.0, 1.0, 6.0, 3.0, 2.0, 5.0});
    const int count = dp.stablePartition([](double value) { return static_cast<int>(value) % 2 == 0; });
    CHECK(count == 3);
    CHECK(dp.getData().front() == doctest::Approx(4.0));
    CHECK(dp.getData()[1] == doctest::Approx(6.0));
    CHECK(dp.getData()[2] == doctest::Approx(2.0));
}

TEST_CASE("Student - TextAnalyzer counts punctuation safely") {
    TextAnalyzer analyzer("A!b? c.");
    CHECK(analyzer.countVowels() == 1);
    CHECK(analyzer.countConsonants() == 2);
    CHECK(analyzer.countSpaces() == 1);
}

TEST_CASE("Student - TextAnalyzer isAlphaOnly allows spaces") {
    TextAnalyzer analyzer("Hello World");
    CHECK(analyzer.isAlphaOnly());
    CHECK_FALSE(TextAnalyzer("Hello-World").isAlphaOnly());
}

TEST_CASE("Student - TextAnalyzer lowercase frequency normalizes words") {
    TextAnalyzer analyzer("The the THE");
    const auto frequencies = analyzer.getWordFrequency();
    CHECK(frequencies.at("the") == 3);
}

TEST_CASE("Student - TextAnalyzer unique words keep first encounters") {
    TextAnalyzer analyzer("One two one THREE two");
    CHECK(analyzer.getUniqueWords() == std::vector<std::string>{"one", "two", "three"});
}

TEST_CASE("Student - TextAnalyzer joinWords and reverse") {
    TextAnalyzer analyzer("ab cd ef");
    CHECK(analyzer.joinWords("|") == "ab|cd|ef");
    CHECK(analyzer.reversed() == "fe dc ba");
}

TEST_CASE("Student - TextAnalyzer longest and shortest empty") {
    TextAnalyzer analyzer("");
    CHECK(analyzer.longestWord().empty());
    CHECK(analyzer.shortestWord().empty());
}

TEST_CASE("Student - SetOperations union sorts unsorted input") {
    CHECK(setUnion({5, 1, 3}, {4, 3, 2}) == std::vector<int>{1, 2, 3, 4, 5});
}

TEST_CASE("Student - SetOperations intersection on duplicates") {
    CHECK(setIntersection({1, 1, 2, 4}, {1, 3, 4, 4}) == std::vector<int>{1, 4});
}

TEST_CASE("Student - SetOperations difference empty result") {
    CHECK(setDifference({1, 2}, {2, 1}).empty());
}

TEST_CASE("Student - SetOperations symmetric difference disjoint") {
    CHECK(setSymmetricDifference({1, 2}, {3, 4}) == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("Student - SetOperations includes subset") {
    CHECK(includes({7, 1, 5, 3}, {1, 3, 5}));
    CHECK_FALSE(includes({1, 2, 3}, {1, 4}));
}

TEST_CASE("Student - AlgorithmShowcase permutations of duplicate values") {
    const auto permutations = allPermutations({2, 1, 1});
    CHECK(permutations.size() == 3);
    CHECK(permutations.front() == std::vector<int>{1, 1, 2});
}

TEST_CASE("Student - AlgorithmShowcase prevPermutation wraps correctly") {
    std::vector<int> values{1, 2, 3};
    CHECK_FALSE(prevPermutation(values));
    CHECK(values == std::vector<int>{3, 2, 1});
}

TEST_CASE("Student - AlgorithmShowcase stream input and filtering") {
    std::istringstream input("9 8 7 6");
    CHECK(readFromStream(input) == std::vector<int>{9, 8, 7, 6});

    std::ostringstream output;
    printFiltered({1, 2, 3, 4, 5}, output, [](int value) { return value > 2; }, " ");
    CHECK(output.str() == "3 4 5 ");
}

TEST_CASE("Student - AlgorithmShowcase numeric helpers") {
    CHECK(generateSequence(4, -2) == std::vector<int>{-2, -1, 0, 1});
    CHECK(runningTotal({2, 2, 2}) == std::vector<int>{2, 4, 6});
    CHECK(dotProduct({1, 0, -1}, {2, 3, 4}) == -2);
}

TEST_CASE("Student - AlgorithmShowcase erase helpers") {
    std::vector<int> values{1, 2, 2, 3, 2, 4};
    CHECK(eraseValue(values, 2) == 3);
    CHECK(values == std::vector<int>{1, 3, 4});

    CHECK(eraseIf(values, [](int value) { return value % 2 == 1; }) == 2);
    CHECK(values == std::vector<int>{4});
}

TEST_CASE("Student - AlgorithmShowcase uniqueInPlace on sorted input") {
    std::vector<int> values{1, 1, 2, 2, 2, 3};
    uniqueInPlace(values);
    CHECK(values == std::vector<int>{1, 2, 3});
}
