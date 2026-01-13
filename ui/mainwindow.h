#pragma once

#include <QMainWindow>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QLabel>
#include "core/dict.h"

class Crossword;

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget* parent = nullptr);
	void activatePanel(int);
	const std::vector<uint8_t>& getVol(int);
	void generateCrossword();
private:
	void setStatusOk();
	void setStatusWorking();
	void makeStatusBar();
	void loadLogic();
	std::string readAGIVersionNumber();
	std::string hex(char);
	void onOpen();
	void onNew();
	void onLoadDictionary();
	void onTreeItemClicked(QTreeWidgetItem* item, int column);
	//void readWords();

	std::string _rootDir;
	std::string _version;
	std::unordered_map<int, std::vector<uint8_t>> _vol;

	QTreeWidget* _tree;
	QStackedWidget* _rightStack;
	Crossword* _crossword;
	std::shared_ptr<Dictionary> _dict;
	QLabel* _currentDefinition;
	QLabel* _dictLabel;
	QLabel* _gridStatusLabel;
	QLabel* _gridStatusIcon;

};


inline const std::vector<uint8_t> &MainWindow::getVol(int id) {
	return _vol[id];
}