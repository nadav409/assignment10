#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <istream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace datatools {

class TextAnalyzer {
public:
    TextAnalyzer() = default;
    explicit TextAnalyzer(std::string text) : text_(std::move(text)) {}

    // Counts the vowels in the text with count_if.
    [[nodiscard]] std::size_t countVowels() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return isVowel(ch);
        }));
    }

    // Counts the letters that are not vowels.
    [[nodiscard]] std::size_t countConsonants() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isalpha(ch) != 0 && !isVowel(ch);
        }));
    }

    // Counts the digit characters in the text.
    [[nodiscard]] std::size_t countDigits() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isdigit(ch) != 0;
        }));
    }

    // Counts the whitespace characters in the text.
    [[nodiscard]] std::size_t countSpaces() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isspace(ch) != 0;
        }));
    }

    // Counts how many times target shows up in the text.
    [[nodiscard]] std::size_t countChar(char target) const {
        return static_cast<std::size_t>(std::count(text_.begin(), text_.end(), target));
    }

    // Checks that the text has only letters and spaces, using all_of.
    [[nodiscard]] bool isAlphaOnly() const {
        return std::all_of(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isalpha(ch) != 0 || std::isspace(ch) != 0;
        });
    }

    // Checks if the text has at least one digit, using any_of.
    [[nodiscard]] bool hasDigits() const {
        return std::any_of(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isdigit(ch) != 0;
        });
    }

    // Returns the text in upper case.
    [[nodiscard]] std::string toUpper() const {
        return transformText([](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    }

    // Returns the text in lower case.
    [[nodiscard]] std::string toLower() const {
        return transformText([](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    }

    // Returns a reversed copy of the text.
    [[nodiscard]] std::string reversed() const {
        std::string result = text_;
        std::reverse(result.begin(), result.end());
        return result;
    }

    // Returns the text without any digits.
    [[nodiscard]] std::string removeDigits() const {
        return removeIf([](unsigned char ch) { return std::isdigit(ch) != 0; });
    }

    // Returns the text without any vowels.
    [[nodiscard]] std::string removeVowels() const {
        return removeIf([](unsigned char ch) { return isVowel(ch); });
    }

    // Returns a copy of the text with every oldChar changed to newChar.
    [[nodiscard]] std::string replaceChar(char oldChar, char newChar) const {
        std::string result = text_;
        std::replace(result.begin(), result.end(), oldChar, newChar);
        return result;
    }

    // Splits the text into words by reading from an istringstream, so any whitespace separates words.
    [[nodiscard]] std::vector<std::string> getWords() const {
        std::istringstream input(text_);
        std::vector<std::string> words;
        std::string word;
        while (input >> word) {
            words.push_back(word);
        }
        return words;
    }

    // Returns each lower case word once, in the order it first shows up.
    // The seen map keeps track of the words we already added.
    [[nodiscard]] std::vector<std::string> getUniqueWords() const {
        std::vector<std::string> uniqueWords;
        std::unordered_map<std::string, bool> seen;
        for (const auto& word : normalizedWords()) {
            if (!seen.contains(word)) {
                seen.emplace(word, true);
                uniqueWords.push_back(word);
            }
        }
        return uniqueWords;
    }

    // Counts how many times each lower case word appears.
    [[nodiscard]] std::unordered_map<std::string, std::size_t> getWordFrequency() const {
        std::unordered_map<std::string, std::size_t> frequencies;
        for (const auto& word : normalizedWords()) {
            ++frequencies[word];
        }
        return frequencies;
    }

    // Returns the longest word with max_element comparing sizes, or an empty string if there are no words.
    [[nodiscard]] std::string longestWord() const {
        const auto words = getWords();
        if (words.empty()) {
            return {};
        }
        return *std::max_element(words.begin(), words.end(), [](const auto& left, const auto& right) {
            return left.size() < right.size();
        });
    }

    // Returns the shortest word with min_element comparing sizes, or an empty string if there are no words.
    [[nodiscard]] std::string shortestWord() const {
        const auto words = getWords();
        if (words.empty()) {
            return {};
        }
        return *std::min_element(words.begin(), words.end(), [](const auto& left, const auto& right) {
            return left.size() < right.size();
        });
    }

    // Sorts the words by length with stable_sort, so words with the same length keep their order.
    [[nodiscard]] std::vector<std::string> wordsSortedByLength() const {
        auto words = getWords();
        std::stable_sort(words.begin(), words.end(), [](const auto& left, const auto& right) {
            return left.size() < right.size();
        });
        return words;
    }

    // Returns the words that have more than length characters, using copy_if.
    [[nodiscard]] std::vector<std::string> wordsLongerThan(std::size_t length) const {
        const auto words = getWords();
        std::vector<std::string> result;
        std::copy_if(words.begin(), words.end(), std::back_inserter(result), [length](const std::string& word) {
            return word.size() > length;
        });
        return result;
    }

    // Joins the words back together with the delimiter between them.
    [[nodiscard]] std::string joinWords(const std::string& delimiter) const {
        const auto words = getWords();
        std::ostringstream output;
        for (std::size_t index = 0; index < words.size(); ++index) {
            if (index > 0) {
                output << delimiter;
            }
            output << words[index];
        }
        return output.str();
    }

private:
    // Returns a copy of the text with transform applied to every character.
    template <typename Transform>
    [[nodiscard]] std::string transformText(Transform transform) const {
        std::string result = text_;
        std::transform(result.begin(), result.end(), result.begin(), transform);
        return result;
    }

    // Removes the characters that pass the predicate with remove_if, then erase really shrinks the string.
    template <typename Predicate>
    [[nodiscard]] std::string removeIf(Predicate predicate) const {
        std::string result = text_;
        const auto newEnd = std::remove_if(result.begin(), result.end(), predicate);
        result.erase(newEnd, result.end());
        return result;
    }

    // Returns all the words in lower case, used by getUniqueWords and getWordFrequency.
    [[nodiscard]] std::vector<std::string> normalizedWords() const {
        auto words = getWords();
        std::transform(words.begin(), words.end(), words.begin(), [](const std::string& word) {
            std::string lower = word;
            std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char ch) {
                return static_cast<char>(std::tolower(ch));
            });
            return lower;
        });
        return words;
    }

    // Checks if the char is a vowel, for both upper and lower case.
    static bool isVowel(unsigned char ch) {
        switch (std::tolower(ch)) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u': return true;
            default: return false;
        }
    }

    std::string text_;
};

}  // namespace datatools
