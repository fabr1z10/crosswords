#include "gridselect.h"
#include <QHBoxLayout>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include <QDialogButtonBox>
#include "config.h"


GridDef::GridDef(int width, int height, std::vector<int> &d) : width(width), height(height), data(d) {
}

GridSelect::GridSelect(QWidget *parent) : QDialog(parent) {
	// Constructor implementation (if needed)
	setMinimumSize(640, 400);
	setWindowTitle("Scegli uno schema");
	auto layout = new QHBoxLayout();
	_crossword = new Crossword(this);
	_list = new QListWidget();
	_crossword->setPlayable(false);
	//self.li.currentItemChanged.connect(self.item_changed)
	connect(_list, &QListWidget::currentItemChanged, this, &GridSelect::onItemChanged);
	// initialize available grids from YAML
	try {
		auto schemaFile = ConfigManager::instance().get().paths.schema_file;
		YAML::Node config = YAML::LoadFile(schemaFile);
		for (const auto& items : config) {
			auto name = items.first.as<std::string>();
			auto size = items.second["size"].as<std::vector<int>>();
			auto blackCells = items.second["black"].as<std::vector<int>>();

			_availableGrids.insert(std::make_pair(name, GridDef({size[0], size[1], blackCells})));
			_list->addItem(QString::fromStdString(name));
		}
		layout->addWidget(_crossword, 2);
		_list->setCurrentRow(0);
		auto rhs = new QWidget();
		auto vl = new QVBoxLayout();
		rhs->setLayout(vl);
		vl->addWidget(_list);
		auto okbtn = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
		connect(okbtn, &QDialogButtonBox::accepted, this, &GridSelect::accept);
		connect(okbtn, &QDialogButtonBox::rejected, this, &GridSelect::reject);

		vl->addWidget(okbtn);
		layout->addWidget(rhs, 1);
		setLayout(layout);
	} catch (YAML::BadFile& bf) {
		std::cerr << bf.msg << bf.what() << "\n";
		exit(1);
	}
}

void GridSelect::onItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
	auto& def = _availableGrids.at(current->text().toStdString());
	auto grid = std::make_shared<Grid>(def.width, def.height, def.data);
	_crossword->setGrid(grid);
	_crossword->repaint();
	//std::cout << "selected: " << current->text().toStdString();
//	self.preview_widget.defineGrid(grid['size'][0], grid['size'][1], grid['black'])
//	self.preview_widget.repaint()
}

std::shared_ptr<Grid> GridSelect::getGrid() {
	return _crossword->getGrid();
}