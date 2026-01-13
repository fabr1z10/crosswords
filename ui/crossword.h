#pragma once

#include <QWidget>
#include "core/grid.h"
#include <QVector2D>
#include <QLabel>

class Crossword : public QWidget {
	Q_OBJECT
public:
	Crossword(QWidget* parent = nullptr);

	std::shared_ptr<Grid> getGrid() const;

	void setGrid(std::shared_ptr<Grid>);

	void setPlayable(bool);

	void setDefinitionLabel(QLabel*);

	void addClue(Slot*, const std::string& clue);
protected:
	void mousePressEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent *event) override;
private:
	float _pad;
	float _blackPad;
	int _gridWidth = 0;
	int _gridHeight = 0;
	QColor _bgColor;
	QColor _currentWordColor;
	QColor _cursorColor;
	bool _playable = true;
	Slot* _highlightedWord = nullptr;
	QVector2D _cursorPos;
	QVector2D _topLeft;
	QVector2D _bottomRight;
	float _cellSize;
	std::shared_ptr<Grid> _grid;
	QLabel* _definition = nullptr;
	std::unordered_map<Slot*, std::string> _clues;

};

inline std::shared_ptr<Grid> Crossword::getGrid() const {
	return _grid;
}

inline void Crossword::setPlayable(bool value) {
	_playable = value;
}

inline void Crossword::setDefinitionLabel(QLabel * label) {
	_definition = label;
}