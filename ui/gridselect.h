#pragma once

#include <QDialog>
#include "crossword.h"
#include <QListWidget>

struct GridDef {
	GridDef(int width, int height, std::vector<int>& data);
	int width;
	int height;
	std::vector<int> data;
};

class GridSelect : public QDialog {
public:
	GridSelect(QWidget *parent = nullptr);
	std::shared_ptr<Grid> getGrid();
private:
	Crossword* _crossword;
	QListWidget* _list;
private slots:
	void onItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	std::unordered_map<std::string, GridDef> _availableGrids;
};
