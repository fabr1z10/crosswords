#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>


class DictN {
    // a subdictionary of words with n letters
public:
    DictN(int n);
    void addWord(const std::string& s);
    std::vector<std::string> find(const std::string& pattern);

private:
    std::unordered_set<std::string> intersectTwoSets(const std::unordered_set<std::string>& set1,
                                                     const std::unordered_set<std::string>& set2);
    // subset of strings of n characters whose i-th character is fixed
    std::vector<std::vector<std::unordered_set<std::string>>> _words;
    std::vector<std::string> _allWords;
    std::unordered_map<std::string, std::vector<std::string>> _cache;
};

class Dictionary {

public:
    Dictionary(const std::string& file);

    std::vector<std::string> find(const std::string& pattern);

	int getWordCount() const;

	std::string getClue(const std::string&) const;
private:
    std::vector<DictN> _subDicts;
    std::vector<std::string> _words;
	std::unordered_map<std::string, std::vector<std::string>> _clues;
};

inline int Dictionary::getWordCount() const {
	return _words.size();
}