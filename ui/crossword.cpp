#include "crossword.h"
#include <sstream>
#include <QPainter>
#include <QPalette>
#include <QMouseEvent>
#include <iostream>

Crossword::Crossword(QWidget *parent) : QWidget(parent) {
	_bgColor = QColor::fromString("#FBF1C7");
	_currentWordColor = QColor::fromRgb(192, 192, 192);
	_cursorColor = QColor::fromRgb(128, 128, 128);
	_pad = 10;
	_blackPad = 2;
	auto pal = QPalette();
	pal.setColor(this->backgroundRole(), _bgColor);
	setAutoFillBackground(true);
	setPalette(pal);
	setFocusPolicy(Qt::StrongFocus); // IMPORTANT
}

void Crossword::setGrid(std::shared_ptr<Grid> grid) {
	_grid = grid;
	_gridWidth = _grid->getWidth();
	_gridHeight = _grid->getHeight();
	_letters = _grid->getLetters(); // initialize letters
}

void Crossword::moveRight() {
	if (1 + _cursorPos.x() - _highlightedWord->x < _highlightedWord->length) {
		_cursorPos.rx() ++;
	}
}

void Crossword::moveLeft() {
	if (_cursorPos.x() - 1 >= _highlightedWord->x) {
		_cursorPos.rx() --;
	}
}

void Crossword::moveDown() {
	if (1 + _cursorPos.y() - _highlightedWord->y < _highlightedWord->length) {
		_cursorPos.ry()++;
	}
}

void Crossword::moveUp() {
	if (_cursorPos.y() - 1 >= _highlightedWord->y) {
		_cursorPos.ry()--;
	}
}

void Crossword::keyPressEvent(QKeyEvent *event) {
	if (!_playable) return;
	auto key = event->key();
	if (_highlightedWord != nullptr) {
		if (key >= 65 && key <= 90) {
			_letters[_cursorPos.y() * _gridWidth + _cursorPos.x()] = char(key);
			if (_highlightedWord->d == Direction::ACROSS) {
				moveRight();
			} else if (_highlightedWord->d == Direction::DOWN) {
				moveDown();
			}
			repaint();
		} else if (key == Qt::Key_Left) {
			if (_highlightedWord->d == Direction::ACROSS) {
				moveLeft();
				repaint();
			} else {
				if (auto id = _grid->getIntersection(_cursorPos.x(), _cursorPos.y()); id.first != -1) {
					_highlightedWord = _grid->getAcross(id.first);
					moveLeft();
					updateClue();
					repaint();
				}
			}
		} else if (key == Qt::Key_Right) {
			if (_highlightedWord->d == Direction::ACROSS) {
				moveRight();
				repaint();
			} else {
				if (auto id = _grid->getIntersection(_cursorPos.x(), _cursorPos.y()); id.first != -1) {
					_highlightedWord = _grid->getAcross(id.first);
					moveRight();
					updateClue();
					repaint();
				}
			}
		} else if (key == Qt::Key_Up) {
			if (_highlightedWord->d == Direction::DOWN) {
				moveUp();
				repaint();
			} else {
				if (auto id = _grid->getIntersection(_cursorPos.x(), _cursorPos.y()); id.second != -1) {
					_highlightedWord = _grid->getDown(id.second);
					moveUp();
					updateClue();
					repaint();
				}
			}
		} else if (key == Qt::Key_Down) {
			if (_highlightedWord->d == Direction::DOWN) {
				moveDown();
				repaint();
			} else {
				if (auto id = _grid->getIntersection(_cursorPos.x(), _cursorPos.y()); id.second != -1) {
					_highlightedWord = _grid->getDown(id.second);
					moveDown();
					updateClue();
					repaint();
				}
			}
		} else if (key == Qt::Key_Backspace) {
			_letters[_cursorPos.y() * _gridWidth + _cursorPos.x()] = '.';
			repaint();
		}
	}
}

void Crossword::updateClue() {
	std::string clue;
	if (_highlightedWord != nullptr) {
		//qDebug() << "Current slot: " << _highlightedWord->toString() << "; word = " << _grid->getWord(_highlightedWord) << "\n";
		std::stringstream stream;
		stream << _highlightedWord->number << " " << (_highlightedWord->d == Direction::ACROSS ? "orizzontale" : "verticale") <<
			   ": " << _grid->getWord(_highlightedWord);
		auto it = _clues.find(_highlightedWord);

		if (it != _clues.end()) {
			stream << ": " << it->second;
		}
		clue = stream.str();
	}
	if (_definition) {
		_definition->setText(QString::fromStdString(clue));
	}
}

void Crossword::mousePressEvent(QMouseEvent *event) {
	if (_grid == nullptr || !_playable) {
		return;
	}
	auto pos = event->pos();
	if (pos.x() < _topLeft.x() or pos.y() < _topLeft.y() or pos.x() > _bottomRight.x() or pos.y() > _bottomRight.y()) {
		return;
	}
	int col = int((pos.x() - _topLeft.x()) / _cellSize);
	int row = int((pos.y() - _topLeft.y()) / _cellSize);
	_cursorPos = QPoint(col, row);
	//qDebug() << "x="<<  col << "y=" << row << "\n";
	//acrossClue = _grid->get.grid.getAcross(row, col)
	auto intersection = _grid->getIntersection(col, row);
	if (intersection.first == -1 && intersection.second == -1) {
		_highlightedWord = nullptr;
	} else if (intersection.second == -1) {
		_highlightedWord = _grid->getAcross(intersection.first);
	} else if (intersection.first == -1) {
		_highlightedWord = _grid->getDown(intersection.second);
	} else {
		// clicked on a cell with both across and down
		Slot* aSlot = _grid->getAcross(intersection.first);
		Slot* dSlot = _grid->getDown(intersection.second);
		if (_highlightedWord == nullptr || _highlightedWord != aSlot) {
			_highlightedWord = aSlot;
		} else {
			_highlightedWord = dSlot;
		}
	}
	updateClue();
	repaint();
}


void Crossword::paintEvent(QPaintEvent *event) {
	if (_gridWidth == 0) return;

	Q_UNUSED(event);

	QPainter painter(this);

	auto rect = this->rect();
	auto w1 = (rect.width() - 2 * _pad) / _gridWidth;
	auto w2 = (rect.height() - 2 * _pad) / _gridHeight;
	auto cell_size = std::min(w1, w2);
	auto grid_width = _gridWidth * cell_size;
	auto grid_height = _gridHeight * cell_size;

	// now compute top left corner
	QVector2D tl;
	if (w2 <= w1) {
		tl = QVector2D((int) ((rect.width() - grid_width) / 2), _pad);
	} else {
		tl = QVector2D(_pad, (int) ((rect.height() - grid_height) / 2));
	}

	QVector2D br = tl + QVector2D(grid_width, grid_height);
	_topLeft = tl;
	_bottomRight = br;
	_cellSize = cell_size;
	// draw highlighted word (if any)
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	if (_highlightedWord) {
		brush.setColor(_currentWordColor);
	}
	painter.setBrush(brush);
	if (_highlightedWord != nullptr) {
		float hww = 0.f;
		float hwh = 0.f;
		if (_highlightedWord->d == Direction::ACROSS) {
			hww = _highlightedWord->length * cell_size;
			hwh = cell_size;
		} else {
			hww = cell_size;
			hwh = _highlightedWord->length * cell_size;
		}
		painter.drawRect(
				tl.x() + _highlightedWord->x * cell_size,
				tl.y() + _highlightedWord->y * cell_size,
				hww,
				hwh);

		brush.setColor(_cursorColor);
		painter.setBrush(brush);
		painter.drawRect(tl.x() + _cursorPos.x() * cell_size, tl.y() + _cursorPos.y() * cell_size,
			cell_size, cell_size);
	}
	QList<QLineF> verticalLines;
	QList<QLineF> horizontalLines;
	for (int i = 0; i <= _gridWidth; ++i) {
		float x = tl.x() + i * cell_size;
		verticalLines.push_back(QLineF(x, tl.y(), x, tl.y() + grid_height));
	}
	for (int i = 0; i <= _gridHeight; ++i) {
		float y = tl.y() + i * cell_size;
		horizontalLines.push_back(QLineF(tl.x(), y, tl.x() + grid_width, y));
	}
	painter.drawLines(verticalLines);
	painter.drawLines(horizontalLines);
	// plot black squares
	QBrush blackBrush;
	blackBrush.setColor(QColor::fromRgb(0, 0, 0));
	blackBrush.setStyle(Qt::SolidPattern);
	painter.setBrush(blackBrush);
	for (const auto& b : _grid->getBlackSquares()) {
		painter.drawRect(
			tl.x() + b.first * cell_size + _blackPad,
			tl.y() + b.second * cell_size + _blackPad,
			cell_size - 2 * _blackPad,
			cell_size - 2 * _blackPad);
	}

	// place clue numbers
	painter.setFont(QFont("times", cell_size * 0.25));
	for (const auto& d : _grid->getDefs()) {
		painter.drawText(
			tl.x() + d->x * cell_size + _blackPad,
			tl.y() + d->y * cell_size + _blackPad + cell_size * 0.25,
			QString::fromStdString(std::to_string(d->number)));
	}


	// place letters
	painter.setFont(QFont("arial", cell_size * 0.65));
	for (int i = 0; i < _letters.size(); ++i) {
		if (_letters[i] == '.' || _letters[i] == '#') continue;
		int ix = i % _gridWidth;
		int iy = (int) (i / _gridWidth);

		auto r = QRect(
			_topLeft.x() + ix * cell_size,
			_topLeft.y() + iy * cell_size,
			cell_size,
			cell_size);

		painter.drawText(r, Qt::AlignCenter, QString(_letters[i]));


	}
	//for pos, letter in self.letters.items():
	//r = QRect(self.tl.x() + pos[0] * cell_size, self.tl.y() + pos[1] * cell_size, cell_size, cell_size)
	//ainter.drawText(qr, Qt.AlignCenter, letter)


}

void Crossword::addClue(Slot * slot, const std::string &clue) {
	_clues[slot] = clue;
}

std::string Crossword::getClue(Slot* slot) const {
	return _clues.at(slot);
}