#include "mainwindow.h"
#include <QMenuBar>
#include "gridselect.h"
#include <QVBoxLayout>
#include "crossword.h"
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include "core/solver.h"
#include <QApplication>

namespace fs = std::filesystem;

void MainWindow::makeStatusBar() {
	auto statusBar = new QStatusBar(this);
	setStatusBar(statusBar);
	_dictLabel = new QLabel("<No dictionary>");
	_gridStatusIcon = new QLabel(this);
	_gridStatusLabel = new QLabel(this);
	statusBar->addPermanentWidget(_dictLabel);
	statusBar->addPermanentWidget(_gridStatusIcon);
	statusBar->addPermanentWidget(_gridStatusLabel);
	setStatusOk();
}

void MainWindow::setStatusOk()
{
	_gridStatusIcon->setPixmap(
			style()->standardIcon(QStyle::SP_DialogApplyButton).pixmap(16, 16)
	);
	_gridStatusLabel->setText("Ready");
}

void MainWindow::setStatusWorking()
{
	_gridStatusIcon->setPixmap(
			style()->standardIcon(QStyle::SP_BrowserReload).pixmap(16, 16)
	);
	_gridStatusLabel->setText("Working...");
}

void MainWindow::generateCrossword() {
	if (_crossword->getGrid() == nullptr) {
		QMessageBox::critical(this, tr("Error"), tr("No crossword grid loaded."));
		return;
	}
	if (_dict == nullptr) {
		QMessageBox::critical(this, tr("Error"), tr("No dictionary loaded."));
	}
	setStatusWorking();
	QApplication::processEvents();
	Solver solver;
	auto grid = _crossword->getGrid();
	solver.solve(*grid.get(), *_dict.get());
	_crossword->setPlayable(true);
	// get clues
	for (const auto& c : grid->getSlots()) {
		auto word = grid->getWord(c.get());
		auto clue = _dict->getClue(word);
		_crossword->addClue(c.get(), clue);
	}

	setStatusOk();
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	// ----- Menu  Bar -----

	QMenu* fileMenu = menuBar()->addMenu("&File");
	QAction* newAction = new QAction("&New", this);
	//new_action.triggered.connect(self.newCrossword)
	connect(newAction, &QAction::triggered, this, &MainWindow::onNew);
	fileMenu->addAction(newAction);

	QAction* openAction = new QAction("Open", this);
	connect(openAction, &QAction::triggered, this,&MainWindow::onOpen);
	fileMenu->addAction(openAction);

	fileMenu->addSeparator();   // ← horizontal line
	QAction* loadDictAction = new QAction(tr("&Load dictionary"), this);
	// 4. Connect action
	connect(loadDictAction, &QAction::triggered,
			this, &MainWindow::onLoadDictionary);
	// 3. Add action to menu
	fileMenu->addAction(loadDictAction);

	QMenu* editMenu = menuBar()->addMenu("&Edit");
	QAction* gen = new QAction("&Generate", this);
	editMenu->addAction(gen);
	connect(gen, &QAction::triggered,
			this, &MainWindow::generateCrossword);

	makeStatusBar();
	auto main = new QWidget();
	auto main_layout = new QVBoxLayout();
	main->setLayout(main_layout);

	_crossword = new Crossword();
	_currentDefinition = new QLabel();
	_crossword->setDefinitionLabel(_currentDefinition);
	_currentDefinition->setMaximumHeight(32);
	main_layout->addWidget(_crossword);
	main_layout->addWidget(_currentDefinition);
	//.cw.definitionLabel = self.currentDefinition
	setCentralWidget(main);

	_dict = std::make_shared<Dictionary>("/home/fabrizio/playground/crossword/dict/ita");

	_dictLabel->setText(QString::fromStdString(" ("  + std::to_string(_dict->getWordCount()) + ")"));
	//self.loadDictionary()
//
//	auto splitter =new QSplitter(this);
//	_tree = new QTreeWidget(splitter);
//	_tree->setHeaderHidden(true);
//	connect(_tree, &QTreeWidget::itemClicked, this, &MainWindow::onTreeItemClicked);
//
//	_rightStack = new QStackedWidget(splitter);
//	_wordsWidget = new WordsWidget();
//	_codeEditor  = new CodeEditor();        // the code editor page
//	_rightStack->addWidget(_codeEditor);			// index 1
//	_rightStack->addWidget(_wordsWidget);		// index 0
//	setCentralWidget(splitter);
}

void MainWindow::onOpen() {
}

void MainWindow::onNew() {
	GridSelect dlg(this);   // parent = this for proper ownership
	//dlg.exec();      // modal dialog
	if (dlg.exec() == QDialog::Accepted) {
		_crossword->setGrid(dlg.getGrid());
		_crossword->repaint();
	}
}

void MainWindow::onLoadDictionary() {
		QString fileName = QFileDialog::getOpenFileName(
				this,
				tr("Open file"),
				QString(),
				tr("All files (*)")
		);

		if (fileName.isEmpty())
			return;

		// Do something with the file
//		QMessageBox::information(
//				this,
//				tr("File selected"),
//				fileName
//		);
		_dict = std::make_shared<Dictionary>(fileName.toStdString());

		_dictLabel->setText(fileName + QString::fromStdString(" ("  + std::to_string(_dict->getWordCount()) + ")"));

		// Example:
		// processFile(fileName);

}

