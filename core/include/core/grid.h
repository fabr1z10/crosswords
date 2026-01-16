#pragma once


#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>

enum Direction { ACROSS, DOWN};

struct Slot {
	Slot(int n, Direction d, int length, int x, int y)
			: number(n), d(d), length(length), x(x), y(y), intersections(0) {}
	int number;
	Direction d;
	int length;
	int x, y;
	int intersections;
	std::string toString() const;

};

class AdjacencyGraph {
public:
	AdjacencyGraph() = default;
	void addNode(int);
	void addEdge(Slot*, Slot*);
	void toString() const;
	int getBest(int);
	std::unordered_map<Slot*, int> getAdjacentSlots(Slot*);

private:
	std::vector<int> nodes;
	std::unordered_map<Slot*, std::unordered_map<Slot*, int>> _edges;
};

class Grid {
public:
	Grid(int w, int h, const std::vector<int>& data);
	void setLetter(int x, int y, char c);
	char getLetter(int x, int y) const;
	Slot* peekSlot();
	std::shared_ptr<Slot> popSlot();
	std::string getWord(Slot*) const;
	bool isComplete(Slot*) const;
	bool done() const;
	void setWord(Slot*, const std::string& word);
	void clearWord(Slot*);
	// find intersecting slots for a given slot
	std::unordered_set<Slot*> getIntersectingSlots(Slot*);
	/* If a and b are intersecting, returns the index of the character
	 * of a shared with b; returns -1 otherwise.
	 */

	bool areIntersecting(Slot*, Slot*) const;
	int getIndex(Slot* a, Slot* b) const;
	std::unordered_map<Slot*, int> getAdjacentSlots(Slot*);
	void dump();
	void addUnfeasibility(Slot*, const std::string&);
	bool isUnfeasible(Slot*, const std::string&);
	const std::vector<std::shared_ptr<Slot>>& getSlots() const {
		return _defSortedByInt;
	}
	const std::vector<std::shared_ptr<Slot>>& getDefs();
	const std::vector<std::pair<int, int>>& getBlackSquares();
	int getWidth() const ;
	int getHeight() const;
	std::pair<int, int> getIntersection(int x, int y);
	Slot* getAcross(int);
	Slot* getDown(int);
	std::string getLetters() const;
private:
	int _width;
	int _height;
	int _nCells;
	std::string _grid;
	std::vector<int> _locks;
	std::vector<std::pair<int, int>> _blackSquares;

	std::unordered_map<int, std::shared_ptr<Slot>> _across;
	std::unordered_map<int, std::shared_ptr<Slot>> _down;
	std::vector<std::pair<int, int>> _intersections;
	std::unordered_map<Slot*, std::unordered_set<Slot*>> _acrossToDown;
	std::unordered_map<Slot*, std::unordered_set<Slot*>> _downToAcross;
	std::unique_ptr<AdjacencyGraph> _acrossGraph;
	std::unique_ptr<AdjacencyGraph> _downGraph;
	std::vector<std::shared_ptr<Slot>> _defSortedByInt;
	std::unordered_map<Slot*, std::unordered_set<std::string>> _unfeasibleSet;
};

inline void Grid::addUnfeasibility(Slot * slot, const std::string & s) {
	_unfeasibleSet[slot].insert(s);
}

inline std::string Grid::getLetters() const {
	return _grid;
}

inline int Grid::getWidth() const {
	return _width;
}
inline int Grid::getHeight() const {
	return _height;
}