#include "core/dict.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include "core/random.h"
#include <algorithm>

struct SetSizeComparator {
    bool operator()(const std::unordered_set<std::string>& lhs, const std::unordered_set<std::string>& rhs) const {
        if (lhs.size() == rhs.size()) {
            return &lhs < &rhs;
        }
        return lhs.size() < rhs.size();
    }
};
using namespace std;

DictN::DictN(int n) {
    for (size_t i = 0; i < n; ++i) {
        _words.emplace_back();
        for (size_t j = 0; j< 26; ++j) {
            _words[i].emplace_back();
        }
    }
}

void DictN::addWord(const std::string &s) {
    for (int i = 0; i < s.length(); i++) {
        _words[i][static_cast<int>(s[i]) - 97].insert(s);
    }
    _allWords.push_back(s);
}

Dictionary::Dictionary(const std::string& file) {
    for (size_t i = 2; i <= 25; ++i) {
        _subDicts.emplace_back(i);
    }
    fstream fin;
    fin.open(file, ios::in);
    if (!fin.good()) {
        cout << "cannot open " << file;
        exit(1);
    }

    string line;

    // first get the unique words
    std::unordered_set<std::string> words;
    while (getline(fin, line)) {

		auto commaIndex = line.find(',');
		auto word = line.substr(0, commaIndex);
		auto clue = line.substr(commaIndex+1);
        words.insert(word);
		_clues[word].push_back(clue);
    }

    std::cout << "# words: " << words.size() << "\n";
    // Convert unordered_set to vector
    _words = std::vector<std::string>(words.begin(), words.end());

    // Create a random number generator
//    std::random_device rd;  // Seed for the random number generator
//    std::mt19937 gen(rd()); // Mersenne Twister engine
//
//    // Shuffle the vector
//    std::shuffle(_words.begin(), _words.end(), gen);
    for (auto word : _words) {
        //int length = s[0].size();
        _subDicts[word.size()-2].addWord(word);
    }

}

std::vector<std::string> DictN::find(const std::string &pattern) {
    auto it = _cache.find(pattern);
    if (it != _cache.end()) {
        return it->second;
    }

    std::set<std::unordered_set<std::string>, SetSizeComparator> orderedSets;
    // keep a set of sets ordered by size
    // then take inetrsecionts
    for (size_t i = 0 ; i< pattern.size(); ++i) {
        if (pattern[i] != '.') {
            auto c = static_cast<int>(pattern[i]) - 97;
            auto nw = _words[i][c].size();
            if (nw == 0) {
                return {};
            }
            orderedSets.insert(_words[i][c]);
        }
    }

    if (orderedSets.empty()) {
        return _allWords;
    }

    // intersects the n sets
    auto iter = orderedSets.begin();
    std::unordered_set<std::string> result = *iter;
    for (iter = iter++; iter != orderedSets.end(); iter++) {
        result = intersectTwoSets(result, *iter);
    }
    std::vector<std::string> r(result.begin(), result.end());
    _cache[pattern] = r;
    return r;

}

std::unordered_set<std::string> DictN::intersectTwoSets(const std::unordered_set<std::string>& set1,
                                                 const std::unordered_set<std::string>& set2) {
    std::unordered_set<std::string> intersection;
    for (const auto& element : set1) {
        if (set2.find(element) != set2.end()) {
            intersection.insert(element);
        }
    }
    return intersection;
}

std::vector<std::string> Dictionary::find(const std::string &pattern) {
    return _subDicts[pattern.size() - 2].find(pattern);
}

std::string Dictionary::getClue(const std::string& word) const {
	auto it = _clues.find(word);
	if (it == _clues.end()) {
		throw std::runtime_error(" -- Requested clue for a word (" + word + ") not in the dicitonary.");
	}
	return it->second[getRandomNumber(it->second.size()-1)];
}