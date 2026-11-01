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

    [[nodiscard]] std::size_t countVowels() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return isVowel(ch);
        }));
    }

    [[nodiscard]] std::size_t countConsonants() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isalpha(ch) != 0 && !isVowel(ch);
        }));
    }

    [[nodiscard]] std::size_t countDigits() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isdigit(ch) != 0;
        }));
    }

    [[nodiscard]] std::size_t countSpaces() const {
        return static_cast<std::size_t>(std::count_if(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isspace(ch) != 0;
        }));
    }

    [[nodiscard]] std::size_t countChar(char target) const {
        return static_cast<std::size_t>(std::count(text_.begin(), text_.end(), target));
    }

    [[nodiscard]] bool isAlphaOnly() const {
        return std::all_of(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isalpha(ch) != 0 || std::isspace(ch) != 0;
        });
    }

    [[nodiscard]] bool hasDigits() const {
        return std::any_of(text_.begin(), text_.end(), [](unsigned char ch) {
            return std::isdigit(ch) != 0;
        });
    }

    [[nodiscard]] std::string toUpper() const {
        return transformText([](unsigned char ch) { return static_cast<char>(std::toupper(ch)); });
    }

    [[nodiscard]] std::string toLower() const {
        return transformText([](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
    }

    [[nodiscard]] std::string reversed() const {
        std::string result = text_;
        std::reverse(result.begin(), result.end());
        return result;
    }

    [[nodiscard]] std::string removeDigits() const {
        return removeIf([](unsigned char ch) { return std::isdigit(ch) != 0; });
    }

    [[nodiscard]] std::string removeVowels() const {
        return removeIf([](unsigned char ch) { return isVowel(ch); });
    }

    [[nodiscard]] std::string replaceChar(char oldChar, char newChar) const {
        std::string result = text_;
        std::replace(result.begin(), result.end(), oldChar, newChar);
        return result;
    }

    [[nodiscard]] std::vector<std::string> getWords() const {
        std::istringstream input(text_);
        std::vector<std::string> words;
        std::string word;
        while (input >> word) {
            words.push_back(word);
        }
        return words;
    }

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

    [[nodiscard]] std::unordered_map<std::string, std::size_t> getWordFrequency() const {
        std::unordered_map<std::string, std::size_t> frequencies;
        for (const auto& word : normalizedWords()) {
            ++frequencies[word];
        }
        return frequencies;
    }

    [[nodiscard]] std::string longestWord() const {
        const auto words = getWords();
        if (words.empty()) {
            return {};
        }
        return *std::max_element(words.begin(), words.end(), [](const auto& left, const auto& right) {
            return left.size() < right.size();
        });
    }

    [[nodiscard]] std::string shortestWord() const {
        const auto words = getWords();
        if (words.empty()) {
            return {};
        }
        return *std::min_element(words.begin(), words.end(), [](const auto& left, const auto& right) {
            return left.size() < right.size();
        });
    }

    [[nodiscard]] std::vector<std::string> wordsSortedByLength() const {
        auto words = getWords();
        std::stable_sort(words.begin(), words.end(), [](const auto& left, const auto& right) {
            return left.size() < right.size();
        });
        return words;
    }

    [[nodiscard]] std::vector<std::string> wordsLongerThan(std::size_t length) const {
        const auto words = getWords();
        std::vector<std::string> result;
        std::copy_if(words.begin(), words.end(), std::back_inserter(result), [length](const std::string& word) {
            return word.size() > length;
        });
        return result;
    }

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
    template <typename Transform>
    [[nodiscard]] std::string transformText(Transform transform) const {
        std::string result = text_;
        std::transform(result.begin(), result.end(), result.begin(), transform);
        return result;
    }

    template <typename Predicate>
    [[nodiscard]] std::string removeIf(Predicate predicate) const {
        std::string result = text_;
        const auto newEnd = std::remove_if(result.begin(), result.end(), predicate);
        result.erase(newEnd, result.end());
        return result;
    }

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
