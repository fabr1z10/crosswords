#include "core/grid.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>


Grid::Grid(int width, int height, const std::vector<int> &data) : _width(width), _height(height),
	_nCells(_width*_height) {
	_grid = std::string(_width * _height, '.');
	_locks.resize(_width * _height, 0);

	_intersections = std::vector<std::pair<int, int>>(_nCells, {-1, -1});

	_acrossGraph = std::make_unique<AdjacencyGraph>();
	_downGraph = std::make_unique<AdjacencyGraph>();
	for (size_t i = 0; i < data.size(); i+=2) {
		setLetter(data[i], data[i+1], '#');
		_blackSquares.push_back({data[i], data[i+1]});
	}

	int n{1};
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			if (getLetter(x, y) == '#') {
				continue;
			}
			bool slotFound{false};
			if (getLetter(x-1, y) == '#' && getLetter(x+1, y) != '#') {
				// new across slot
				// find length
				slotFound = true;
				int len = 1;
				_intersections[y * _width + x].first = n;
				while (getLetter(x + len , y) != '#') {
					_intersections[y * _width + x + len].first = n;
					len ++;
				}
				auto slot = std::make_shared<Slot>(n, Direction::ACROSS, len,x, y);
				_across[n] = slot;
				_defSortedByInt.push_back(slot);
			}
			if (getLetter(x, y-1) == '#' && getLetter(x, y+1) != '#') {
				// new down slot
				slotFound = true;
				int len = 1;
				_intersections[y * _width + x].second = n;
				while (getLetter(x, y+len) != '#') {
					_intersections[(y + len) * _width + x].second = n;
					len++;
				}
				auto slot = std::make_shared<Slot>(n, Direction::DOWN, len, x, y);
				_down[n] = slot;
				_defSortedByInt.push_back(slot);
			}
			if (slotFound) {
				n++;
			}
		}
	}

	std::cout << " --- across\n";
	for (const auto& c : _across) {
		std::cout << c.second->toString() << "\n";
	}

	std::cout << " --- down\n";
	for (const auto& c : _down) {
		std::cout << c.second->toString() << "\n";
	}

	// build bipartite graph
	for (const auto& b : _intersections) {
		if (b.first != -1 && b.second != -1) {
			_acrossToDown[_across.at(b.first).get()].insert(_down.at(b.second).get());
			_downToAcross[_down.at(b.second).get()].insert(_across.at(b.first).get());
			_across[b.first]->intersections++;
			_down[b.second]->intersections++;
		}
	}

	std::cout << " --- build across adjacency graph (a2d)\n";
	for (const auto& oriz : _acrossToDown) {
		for (const auto& b : oriz.second) {
			for (const auto& c : _downToAcross[b]) {
				if (oriz.first != c) {
					_acrossGraph->addEdge(_across.at(oriz.first->number).get(), _across.at(c->number).get());
				}
			}
		}
	}
	std::cout << " --- build down adjacency graph (a2d)\n";
	for (const auto& vert : _downToAcross) {
		for (const auto& b : vert.second) {
			for (const auto& c : _acrossToDown[b]) {
				if (vert.first != c) {
					_downGraph->addEdge(_down.at(vert.first->number).get(), _down.at(c->number).get());
				}
			}
		}
	}


	std::sort(_defSortedByInt.begin(), _defSortedByInt.end(), [](const std::shared_ptr<Slot>& a, const std::shared_ptr<Slot>& b) {
		return a->intersections > b->intersections;
	});
//    std::cout << " --- bip graph (d2a)\n";
//    for (const auto& vert : _downToAcross) {
//        std::cout << vert.first << ": ";
//        for (const auto& b : vert.second) {
//            std::cout << b << ", ";
//        }
//        std::cout << "\n";
//    }


}

void Grid::setLetter(int x, int y, char c) {
	_grid[y * _width + x] = c;

}
char Grid::getLetter(int x, int y) const {
	if (x < 0 || x >= _width || y < 0 || y >= _height) {
		return '#';
	}
	return _grid[y*_width+x];
}

bool Grid::areIntersecting(Slot * a, Slot * b) const {
	if (a->d == b->d) {
		return false;
	}
	const auto& m = (a->d == Direction::ACROSS ? _acrossToDown : _downToAcross);
	return m.find(a)->second.count(b) > 0;
}

int Grid::getIndex(Slot* a, Slot* b) const {
	auto intersecting = areIntersecting(a, b);

	if (!intersecting) {
		return -1;
	} else {
		if (a->d == Direction::ACROSS) {
			return (b->x - a->x);
		} else {
			return (b->y - a->y);
		}
	}
}

std::unordered_set<Slot*> Grid::getIntersectingSlots(Slot * slot) {
	auto& g = slot->d == Direction::ACROSS ? _acrossToDown : _downToAcross;
	return g.at(slot);
}

std::string Slot::toString() const {
	std::stringstream stream;
	stream << number << (d == Direction::ACROSS ? " across" : " down") << ", " << length << ", (" << x << ", " << y << ")";
	return stream.str();
}

std::unordered_map<Slot *, int> AdjacencyGraph::getAdjacentSlots(Slot * s) {
	return _edges.at(s);
}


void AdjacencyGraph::addEdge(Slot* a, Slot* b) {

	_edges[a][b]++;
}

void AdjacencyGraph::toString() const {

	for (const auto& c : _edges) {
		for (const auto& d : c.second) {
			std::cout << c.first << " -> " << d.first << "(" << d.second << ")\n";
		}
	}
}

Slot * Grid::peekSlot() {
	return _defSortedByInt.front().get();
}

bool Grid::isComplete(Slot * s) const {
	return (getWord(s).find('.') == std::string::npos);
}

bool Grid::done() const {
	return _grid.find('.') == std::string::npos;
}

std::string Grid::getWord(Slot * s) const {
	int offset = s->y*_width+s->x;
	if (s->d == Direction::ACROSS) {
		return _grid.substr(offset, s->length);
	} else {
		std::stringstream str;
		for (size_t i = 0; i < s->length; ++i) {
			str << _grid[offset];
			offset += _width;
		}
		return str.str();
	}
}

void Grid::setWord(Slot * s, const std::string &word) {
	int offset = s->y*_width+s->x;
	int delta = s->d == Direction::ACROSS ? 1 : _width;

	for (size_t i = 0; i< s->length; ++i) {
		_grid[offset] = word[i];
		if (s->d == Direction::ACROSS) {
			_locks[offset] |= 1;
		} else {
			_locks[offset] |= 2;
		}
		offset += delta;
	}

}

void Grid::clearWord(Slot * s) {
	int offset = s->y*_width+s->x;
	int delta = s->d == Direction::ACROSS ? 1 : _width;
	for (int i = 0; i < s->length; ++i) {
		if (s->d == Direction::ACROSS) {
			_locks[offset] &= 2;
		} else {
			_locks[offset] &= 1;
		}
		if (_locks[offset] == 0) {
			_grid[offset] = '.';
		}
		offset += delta;
	}
}

std::unordered_map<Slot*, int> Grid::getAdjacentSlots(Slot* inSlot) {
	auto& graph = inSlot->d == Direction::ACROSS ? _acrossGraph : _downGraph;
	return graph->getAdjacentSlots(inSlot);

}

void Grid::dump() {
	for (int i = 0 ;i < _height; i++) {
		std::cout << _grid.substr(i * _width, _width) << std::endl;
	}
}

bool Grid::isUnfeasible(Slot * slot, const std::string & str) {
	auto i1 = _unfeasibleSet.find(slot);
	if (i1 == _unfeasibleSet.end()) {
		return false;
	}
	return i1->second.count(str) > 0;
}

const std::vector<std::pair<int, int>> &Grid::getBlackSquares() {
	return _blackSquares;
}

const std::vector<std::shared_ptr<Slot>> &Grid::getDefs() {
	return _defSortedByInt;
}

std::pair<int, int> Grid::getIntersection(int x, int y) {
	return _intersections[y * _width + x];
}

Slot* Grid::getAcross(int id) {
	return _across.at(id).get();
}

Slot* Grid::getDown(int id) {
	return _down.at(id).get();
}