/**
 * @file test.cpp
 * @brief Unit tests for Text Analysis & Data Processing Tool using doctest
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "DataProcessor.hpp"
#include "TextAnalyzer.hpp"
#include "SetOperations.hpp"
#include "AlgorithmShowcase.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <fstream>

using namespace datatools;

// ==================== DATAPROCESSOR TESTS ====================

TEST_CASE("DataProcessor - Default Constructor") {
    DataProcessor dp;
    CHECK(dp.size() == 0);
    CHECK(dp.empty());
}

TEST_CASE("DataProcessor - Vector Constructor") {
    DataProcessor dp({1.0, 2.0, 3.0});
    CHECK(dp.size() == 3);
    CHECK(dp.getData()[0] == doctest::Approx(1.0));
}

TEST_CASE("DataProcessor - Iota Constructor") {
    DataProcessor dp(5, 10.0);
    CHECK(dp.size() == 5);
    CHECK(dp.getData()[0] == doctest::Approx(10.0));
    CHECK(dp.getData()[4] == doctest::Approx(14.0));
}

TEST_CASE("DataProcessor - sum") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0, 5.0});
    CHECK(dp.sum() == doctest::Approx(15.0));
}

TEST_CASE("DataProcessor - sum empty") {
    DataProcessor dp;
    CHECK(dp.sum() == doctest::Approx(0.0));
}

TEST_CASE("DataProcessor - product") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0});
    CHECK(dp.product() == doctest::Approx(24.0));
}

TEST_CASE("DataProcessor - average") {
    DataProcessor dp({10.0, 20.0, 30.0});
    CHECK(dp.average() == doctest::Approx(20.0));
}

TEST_CASE("DataProcessor - average empty throws") {
    DataProcessor dp;
    CHECK_THROWS_AS(dp.average(), std::invalid_argument);
}

TEST_CASE("DataProcessor - min max") {
    DataProcessor dp({5.0, 1.0, 9.0, 3.0});
    CHECK(dp.min() == doctest::Approx(1.0));
    CHECK(dp.max() == doctest::Approx(9.0));
}

TEST_CASE("DataProcessor - min max empty throws") {
    DataProcessor dp;
    CHECK_THROWS_AS(dp.min(), std::invalid_argument);
    CHECK_THROWS_AS(dp.max(), std::invalid_argument);
}

TEST_CASE("DataProcessor - countIf") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0, 5.0});
    CHECK(dp.countIf([](double x) { return x > 3; }) == 2);
    CHECK(dp.countIf([](double x) { return x > 10; }) == 0);
}

TEST_CASE("DataProcessor - allOf anyOf noneOf") {
    DataProcessor dp({2.0, 4.0, 6.0});
    CHECK(dp.allOf([](double x) { return static_cast<int>(x) % 2 == 0; }) == true);
    CHECK(dp.anyOf([](double x) { return x > 5; }) == true);
    CHECK(dp.noneOf([](double x) { return x < 0; }) == true);
}

TEST_CASE("DataProcessor - findIf") {
    DataProcessor dp({1.0, 3.0, 5.0, 7.0});
    CHECK(dp.findIf([](double x) { return x > 4; }) == doctest::Approx(5.0));
}

TEST_CASE("DataProcessor - findIf not found throws") {
    DataProcessor dp({1.0, 2.0});
    CHECK_THROWS_AS(dp.findIf([](double x) { return x > 10; }), std::invalid_argument);
}

TEST_CASE("DataProcessor - innerProduct") {
    DataProcessor a({1.0, 2.0, 3.0});
    DataProcessor b({4.0, 5.0, 6.0});
    CHECK(a.innerProduct(b) == doctest::Approx(32.0));
}

TEST_CASE("DataProcessor - innerProduct size mismatch throws") {
    DataProcessor a({1.0, 2.0});
    DataProcessor b({1.0});
    CHECK_THROWS_AS(a.innerProduct(b), std::invalid_argument);
}

TEST_CASE("DataProcessor - sort ascending") {
    DataProcessor dp({5.0, 3.0, 1.0, 4.0, 2.0});
    dp.sort();
    auto d = dp.getData();
    CHECK(d[0] == doctest::Approx(1.0));
    CHECK(d[4] == doctest::Approx(5.0));
}

TEST_CASE("DataProcessor - sort custom comparator") {
    DataProcessor dp({1.0, 3.0, 2.0});
    dp.sort([](double a, double b) { return a > b; });
    auto d = dp.getData();
    CHECK(d[0] == doctest::Approx(3.0));
    CHECK(d[2] == doctest::Approx(1.0));
}

TEST_CASE("DataProcessor - partialSort") {
    DataProcessor dp({5.0, 3.0, 1.0, 4.0, 2.0});
    dp.partialSort(3);
    auto d = dp.getData();
    CHECK(d[0] == doctest::Approx(1.0));
    CHECK(d[1] == doctest::Approx(2.0));
    CHECK(d[2] == doctest::Approx(3.0));
}

TEST_CASE("DataProcessor - applyTransform") {
    DataProcessor dp({1.0, 2.0, 3.0});
    dp.applyTransform([](double x) { return x * 10; });
    auto d = dp.getData();
    CHECK(d[0] == doctest::Approx(10.0));
    CHECK(d[1] == doctest::Approx(20.0));
    CHECK(d[2] == doctest::Approx(30.0));
}

TEST_CASE("DataProcessor - transformed (new)") {
    DataProcessor dp({1.0, 2.0, 3.0});
    DataProcessor result = dp.transformed([](double x) { return x + 1; });
    CHECK(dp.getData()[0] == doctest::Approx(1.0));
    CHECK(result.getData()[0] == doctest::Approx(2.0));
}

TEST_CASE("DataProcessor - partialSums") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0});
    DataProcessor ps = dp.partialSums();
    auto d = ps.getData();
    CHECK(d[0] == doctest::Approx(1.0));
    CHECK(d[1] == doctest::Approx(3.0));
    CHECK(d[2] == doctest::Approx(6.0));
    CHECK(d[3] == doctest::Approx(10.0));
}

TEST_CASE("DataProcessor - filter") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0, 5.0});
    DataProcessor result = dp.filter([](double x) { return x > 3; });
    CHECK(result.size() == 2);
    CHECK(result.getData()[0] == doctest::Approx(4.0));
    CHECK(result.getData()[1] == doctest::Approx(5.0));
}

TEST_CASE("DataProcessor - replaceIf") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0, 5.0});
    dp.replaceIf([](double x) { return x > 3; }, 0.0);
    CHECK(dp.getData()[3] == doctest::Approx(0.0));
    CHECK(dp.getData()[4] == doctest::Approx(0.0));
    CHECK(dp.getData()[2] == doctest::Approx(3.0));
}

TEST_CASE("DataProcessor - fill") {
    DataProcessor dp({1.0, 2.0, 3.0});
    dp.fill(7.0);
    CHECK(dp.getData()[0] == doctest::Approx(7.0));
    CHECK(dp.getData()[2] == doctest::Approx(7.0));
}

TEST_CASE("DataProcessor - generate") {
    DataProcessor dp;
    int n = 0;
    dp.generate(4, [&n]() { return static_cast<double>(++n); });
    CHECK(dp.size() == 4);
    CHECK(dp.getData()[0] == doctest::Approx(1.0));
    CHECK(dp.getData()[3] == doctest::Approx(4.0));
}

TEST_CASE("DataProcessor - removeIf") {
    DataProcessor dp({1.0, -2.0, 3.0, -4.0, 5.0});
    int count = dp.removeIf([](double x) { return x < 0; });
    CHECK(count == 2);
    CHECK(dp.size() == 3);
}

TEST_CASE("DataProcessor - unique") {
    DataProcessor dp({1.0, 1.0, 2.0, 3.0, 3.0});
    dp.unique();
    CHECK(dp.size() == 3);
}

TEST_CASE("DataProcessor - reverse") {
    DataProcessor dp({1.0, 2.0, 3.0});
    dp.reverse();
    CHECK(dp.getData()[0] == doctest::Approx(3.0));
    CHECK(dp.getData()[2] == doctest::Approx(1.0));
}

TEST_CASE("DataProcessor - rotate") {
    DataProcessor dp({1.0, 2.0, 3.0, 4.0, 5.0});
    dp.rotate(2);
    CHECK(dp.getData()[0] == doctest::Approx(3.0));
    CHECK(dp.getData()[4] == doctest::Approx(2.0));
}

TEST_CASE("DataProcessor - partition") {
    DataProcessor dp({5.0, 1.0, 4.0, 2.0, 3.0});
    int count = dp.partition([](double x) { return x <= 3; });
    CHECK(count == 3);
}

TEST_CASE("DataProcessor - stablePartition") {
    DataProcessor dp({5.0, 1.0, 4.0, 2.0, 3.0, 6.0});
    int count = dp.stablePartition([](double x) { return x <= 3; });
    CHECK(count == 3);
    auto d = dp.getData();
    CHECK(d[0] == doctest::Approx(1.0));
    CHECK(d[1] == doctest::Approx(2.0));
    CHECK(d[2] == doctest::Approx(3.0));
}

TEST_CASE("DataProcessor - toString") {
    DataProcessor dp({1.0, 2.0, 3.0});
    std::string s = dp.toString(", ");
    CHECK(s.find("1") != std::string::npos);
    CHECK(s.find("2") != std::string::npos);
    CHECK(s.find("3") != std::string::npos);
}

TEST_CASE("DataProcessor - nthElement") {
    DataProcessor dp({5.0, 3.0, 1.0, 4.0, 2.0});
    dp.nthElement(2);
    CHECK(dp.getData()[2] == doctest::Approx(3.0));
}

// ==================== TEXTANALYZER TESTS ====================

TEST_CASE("TextAnalyzer - countVowels") {
    TextAnalyzer ta("Hello World");
    CHECK(ta.countVowels() == 3);
}

TEST_CASE("TextAnalyzer - countConsonants") {
    TextAnalyzer ta("Hello World");
    CHECK(ta.countConsonants() == 7);
}

TEST_CASE("TextAnalyzer - countDigits") {
    TextAnalyzer ta("abc123def");
    CHECK(ta.countDigits() == 3);
}

TEST_CASE("TextAnalyzer - countSpaces") {
    TextAnalyzer ta("a b c d");
    CHECK(ta.countSpaces() == 3);
}

TEST_CASE("TextAnalyzer - countChar") {
    TextAnalyzer ta("hello");
    CHECK(ta.countChar('l') == 2);
    CHECK(ta.countChar('z') == 0);
}

TEST_CASE("TextAnalyzer - isAlphaOnly") {
    TextAnalyzer ta1("Hello World");
    CHECK(ta1.isAlphaOnly() == true);

    TextAnalyzer ta2("Hello 123");
    CHECK(ta2.isAlphaOnly() == false);
}

TEST_CASE("TextAnalyzer - hasDigits") {
    TextAnalyzer ta1("abc123");
    CHECK(ta1.hasDigits() == true);

    TextAnalyzer ta2("abcdef");
    CHECK(ta2.hasDigits() == false);
}

TEST_CASE("TextAnalyzer - toUpper") {
    TextAnalyzer ta("Hello World");
    CHECK(ta.toUpper() == "HELLO WORLD");
}

TEST_CASE("TextAnalyzer - toLower") {
    TextAnalyzer ta("Hello World");
    CHECK(ta.toLower() == "hello world");
}

TEST_CASE("TextAnalyzer - reversed") {
    TextAnalyzer ta("Hello");
    CHECK(ta.reversed() == "olleH");
}

TEST_CASE("TextAnalyzer - removeDigits") {
    TextAnalyzer ta("abc123def456");
    CHECK(ta.removeDigits() == "abcdef");
}

TEST_CASE("TextAnalyzer - removeVowels") {
    TextAnalyzer ta("Hello World");
    CHECK(ta.removeVowels() == "Hll Wrld");
}

TEST_CASE("TextAnalyzer - replaceChar") {
    TextAnalyzer ta("hello");
    CHECK(ta.replaceChar('l', 'r') == "herro");
}

TEST_CASE("TextAnalyzer - getWords") {
    TextAnalyzer ta("Hello World Foo");
    auto words = ta.getWords();
    CHECK(words.size() == 3);
    CHECK(words[0] == "Hello");
    CHECK(words[1] == "World");
    CHECK(words[2] == "Foo");
}

TEST_CASE("TextAnalyzer - getWords empty") {
    TextAnalyzer ta("");
    auto words = ta.getWords();
    CHECK(words.empty());
}

TEST_CASE("TextAnalyzer - getUniqueWords") {
    TextAnalyzer ta("the cat and the dog");
    auto unique = ta.getUniqueWords();
    CHECK(unique.size() == 4);
}

TEST_CASE("TextAnalyzer - getWordFrequency") {
    TextAnalyzer ta("the cat and the dog the");
    auto freq = ta.getWordFrequency();
    CHECK(freq["the"] == 3);
    CHECK(freq["cat"] == 1);
}

TEST_CASE("TextAnalyzer - longestWord") {
    TextAnalyzer ta("I am wonderful");
    CHECK(ta.longestWord() == "wonderful");
}

TEST_CASE("TextAnalyzer - longestWord empty") {
    TextAnalyzer ta("");
    CHECK(ta.longestWord() == "");
}

TEST_CASE("TextAnalyzer - shortestWord") {
    TextAnalyzer ta("I am wonderful");
    CHECK(ta.shortestWord() == "I");
}

TEST_CASE("TextAnalyzer - wordsSortedByLength") {
    TextAnalyzer ta("I am the best");
    auto sorted = ta.wordsSortedByLength();
    CHECK(sorted[0] == "I");
    CHECK(sorted[3] == "best");
}

TEST_CASE("TextAnalyzer - wordsLongerThan") {
    TextAnalyzer ta("I am the best today");
    auto long_words = ta.wordsLongerThan(2);
    CHECK(long_words.size() == 3);
}

TEST_CASE("TextAnalyzer - joinWords") {
    TextAnalyzer ta("Hello World");
    CHECK(ta.joinWords("-") == "Hello-World");
}

TEST_CASE("TextAnalyzer - removeDigits with spaces") {
    TextAnalyzer ta("abc 123 def");
    CHECK(ta.removeDigits() == "abc  def");
}

TEST_CASE("TextAnalyzer - removeDigits trailing space") {
    TextAnalyzer ta("hello 123");
    CHECK(ta.removeDigits() == "hello ");
}

// ==================== SET OPERATIONS TESTS ====================

TEST_CASE("setUnion") {
    auto result = setUnion({1, 2, 3}, {2, 3, 4});
    CHECK(result == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("setUnion - disjoint") {
    auto result = setUnion({1, 2}, {3, 4});
    CHECK(result == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("setUnion - empty") {
    auto result = setUnion({}, {1, 2});
    CHECK(result == std::vector<int>{1, 2});
}

TEST_CASE("setIntersection") {
    auto result = setIntersection({1, 2, 3, 4}, {2, 4, 6});
    CHECK(result == std::vector<int>{2, 4});
}

TEST_CASE("setIntersection - no overlap") {
    auto result = setIntersection({1, 2}, {3, 4});
    CHECK(result.empty());
}

TEST_CASE("setDifference") {
    auto result = setDifference({1, 2, 3, 4, 5}, {2, 4});
    CHECK(result == std::vector<int>{1, 3, 5});
}

TEST_CASE("setSymmetricDifference") {
    auto result = setSymmetricDifference({1, 2, 3}, {2, 3, 4});
    CHECK(result == std::vector<int>{1, 4});
}

TEST_CASE("mergeSorted") {
    auto result = mergeSorted({1, 3, 5}, {2, 4, 6});
    CHECK(result == std::vector<int>{1, 2, 3, 4, 5, 6});
}

TEST_CASE("mergeSorted - with duplicates") {
    auto result = mergeSorted({1, 3}, {1, 3});
    CHECK(result == std::vector<int>{1, 1, 3, 3});
}

TEST_CASE("includes - true") {
    CHECK(includes({1, 2, 3, 4, 5}, {2, 3}) == true);
}

TEST_CASE("includes - false") {
    CHECK(includes({1, 2, 3}, {2, 4}) == false);
}

TEST_CASE("includes - empty subset") {
    CHECK(includes({1, 2, 3}, {}) == true);
}

TEST_CASE("setUnionStr") {
    auto result = setUnionStr({"apple", "banana"}, {"banana", "cherry"});
    CHECK(result == std::vector<std::string>{"apple", "banana", "cherry"});
}

TEST_CASE("setIntersectionStr") {
    auto result = setIntersectionStr({"apple", "banana", "cherry"}, {"banana", "cherry", "date"});
    CHECK(result == std::vector<std::string>{"banana", "cherry"});
}

// ==================== ALGORITHM SHOWCASE TESTS ====================

TEST_CASE("allPermutations") {
    auto perms = allPermutations({1, 2, 3});
    CHECK(perms.size() == 6);
}

TEST_CASE("allPermutations - single element") {
    auto perms = allPermutations({1});
    CHECK(perms.size() == 1);
}

TEST_CASE("allPermutations - empty") {
    auto perms = allPermutations({});
    CHECK(perms.size() == 1);
}

TEST_CASE("allPermutations - unsorted input") {
    auto perms = allPermutations({3, 1, 2});
    CHECK(perms.size() == 6);
}

TEST_CASE("nextPermutation") {
    std::vector<int> v = {1, 2, 3};
    bool result = nextPermutation(v);
    CHECK(result == true);
    CHECK(v == std::vector<int>{1, 3, 2});
}

TEST_CASE("nextPermutation - last perm wraps") {
    std::vector<int> v = {3, 2, 1};
    bool result = nextPermutation(v);
    CHECK(result == false);
    CHECK(v == std::vector<int>{1, 2, 3});
}

TEST_CASE("prevPermutation") {
    std::vector<int> v = {1, 3, 2};
    bool result = prevPermutation(v);
    CHECK(result == true);
    CHECK(v == std::vector<int>{1, 2, 3});
}

TEST_CASE("printWithIterator") {
    std::ostringstream oss;
    printWithIterator({1, 2, 3}, oss, " ");
    CHECK(oss.str() == "1 2 3 ");
}

TEST_CASE("printDoublesWithIterator") {
    std::ostringstream oss;
    printDoublesWithIterator({1.5, 2.5}, oss, " ");
    CHECK(oss.str().find("1.5") != std::string::npos);
    CHECK(oss.str().find("2.5") != std::string::npos);
}

TEST_CASE("readFromStream") {
    std::istringstream iss("10 20 30");
    auto result = readFromStream(iss);
    CHECK(result == std::vector<int>{10, 20, 30});
}

TEST_CASE("readFromStream - empty") {
    std::istringstream iss("");
    auto result = readFromStream(iss);
    CHECK(result.empty());
}

TEST_CASE("printFiltered") {
    std::ostringstream oss;
    printFiltered({1, 2, 3, 4, 5}, oss, [](int x) { return x % 2 == 0; }, " ");
    CHECK(oss.str() == "2 4 ");
}

TEST_CASE("generateSequence") {
    auto seq = generateSequence(5, 10);
    CHECK(seq == std::vector<int>{10, 11, 12, 13, 14});
}

TEST_CASE("generateSequence - zero count") {
    auto seq = generateSequence(0, 5);
    CHECK(seq.empty());
}

TEST_CASE("runningTotal") {
    auto result = runningTotal({1, 2, 3, 4, 5});
    CHECK(result == std::vector<int>{1, 3, 6, 10, 15});
}

TEST_CASE("runningTotal - empty") {
    auto result = runningTotal({});
    CHECK(result.empty());
}

TEST_CASE("dotProduct") {
    CHECK(dotProduct({1, 2, 3}, {4, 5, 6}) == 32);
}

TEST_CASE("dotProduct - size mismatch throws") {
    CHECK_THROWS_AS(dotProduct({1, 2}, {1}), std::invalid_argument);
}

TEST_CASE("eraseValue") {
    std::vector<int> v = {1, 2, 3, 2, 4, 2};
    int count = eraseValue(v, 2);
    CHECK(count == 3);
    CHECK(v == std::vector<int>{1, 3, 4});
}

TEST_CASE("eraseValue - not found") {
    std::vector<int> v = {1, 2, 3};
    int count = eraseValue(v, 5);
    CHECK(count == 0);
    CHECK(v.size() == 3);
}

TEST_CASE("eraseIf") {
    std::vector<int> v = {1, 2, 3, 4, 5};
    int count = eraseIf(v, [](int x) { return x % 2 == 0; });
    CHECK(count == 2);
    CHECK(v == std::vector<int>{1, 3, 5});
}

TEST_CASE("uniqueInPlace") {
    std::vector<int> v = {1, 1, 2, 3, 3, 4};
    uniqueInPlace(v);
    CHECK(v == std::vector<int>{1, 2, 3, 4});
}

TEST_CASE("uniqueInPlace - no duplicates") {
    std::vector<int> v = {1, 2, 3};
    uniqueInPlace(v);
    CHECK(v == std::vector<int>{1, 2, 3});
}

TEST_CASE("uniqueInPlace - empty") {
    std::vector<int> v = {};
    uniqueInPlace(v);
    CHECK(v.empty());
}

// ==================== STUDENT TEST FILE CHECK ====================

TEST_CASE("StudentTest - File exists") {
    std::ifstream f("StudentTest.cpp");
    CHECK_MESSAGE(f.good(), "StudentTest.cpp not found. You must create StudentTest.cpp with at least 20 test cases.");
}
