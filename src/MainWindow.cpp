#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QRegularExpression>
#include <QFileDialog>
#include <QFile>

#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), m_ui(new Ui::MainWindow()) {
	m_ui->setupUi(this);

	// проинициализировать список доступных политик группировки
	const auto groupPolicy = m_ui->groupPolicy;
	groupPolicy->addItem(tr("None"), GroupPolicy::None);
	groupPolicy->addItem(tr("Distance"), GroupPolicy::ByDistance);
	groupPolicy->addItem(tr("Time"), GroupPolicy::ByTime);
	groupPolicy->addItem(tr("Type"), GroupPolicy::ByType);
	groupPolicy->addItem(tr("Name"), GroupPolicy::ByName);
}

MainWindow::~MainWindow() {
	delete m_ui;
}


bool MainWindow::containsCyrillic(const QString& str) {
	static QRegularExpression regex("[а-яА-Я]");
	return str.contains(regex);
}


QList<ObjectInfo> MainWindow::parseObjectList() {
	// получить загруженный ранее список объектов в текстовой форме
	auto text = m_ui->loadedText->toPlainText();
	QTextStream in(&text);

	QList<ObjectInfo> objects;
	ObjectInfo obj;

	QString str;
	bool groupStarted = false;

	while((in.skipWhiteSpace(), !in.atEnd())) {
		in >> str;

		// проверить, является ли считанная строка маркером конца группы
		if(str == "}" && groupStarted) {
			groupStarted = false;
			continue;
		}

		obj.name = str; // сохранить имя объекта
		in >> str;

		// проверить, является ли считанная строка маркером начала группы
		if(str == "{" && !groupStarted) {
			groupStarted = true;
			continue;
		}
		obj.x = str; // сохранить x-координату

		// считать описание объекта и сохранить его в списке
		in >> obj.y >> obj.type >> obj.time;
		objects.append(obj);
	}

	return objects;
}

void MainWindow::displayGroups(QMap<QString, QList<ObjectInfo>>& groups, SortPolicy sort) {	
	// преобразовать разбитые на группы объекты в строку и отобразить результат
	QString str;
	for(auto it = groups.begin(); it != groups.end(); it++) {
		if(!it.value().isEmpty()) {
			sortGroup(it.value(), sort);
			str += stringify(it.value(), it.key()) + '\n';
		}
	}
	m_ui->processedText->setPlainText(str);
}

void MainWindow::sortGroup(QList<ObjectInfo>& objects, SortPolicy sort) {
	if(m_ui->sortingEnabled->isChecked()) {
		// в зависимости от ключа сортировки выбрать соотвествующую функцию сравнения
		bool (*compFunc)(const ObjectInfo&, const ObjectInfo&) = nullptr;
		switch(sort) {
			case SortPolicy::ByName: {
				if(m_ui->caseSensitive->isChecked()) {
					compFunc = [](const auto& lhs, const auto& rhs) {
						return lhs.name < rhs.name;
					};
				} else {
					compFunc = [](const auto& lhs, const auto& rhs) {
						return lhs.name.toLower() < rhs.name.toLower();
					};
				}
				break;
			}
			case SortPolicy::ByTime: {
				compFunc = [](const auto& lhs, const auto& rhs) {
					return lhs.time.toLongLong() < rhs.time.toLongLong();
				};
				break;
			}
			case SortPolicy::ByDistance: {
				compFunc = [](const auto& lhs, const auto& rhs) {
					return lhs.distance() < rhs.distance();
				};
				break;
			}
		}
		std::ranges::sort(objects, compFunc);
	}
}


void MainWindow::groupByType() {
	const auto isCaseSensitive = m_ui->caseSensitive->isChecked();;
	const auto n = m_ui->typedGroupSize->value();

	// разбить объекты на группы в зависимости от типа
	QMap<QString, QList<ObjectInfo>> groups;
	for(const auto& obj : m_objects) {
		groups[isCaseSensitive ? obj.type : obj.type.toLower()].append(obj);
	}

	// вынести объекты из групп, имеющих слишком маленький размер, в отдельную группу
	auto& others = groups[isCaseSensitive ? tr("Other") : tr("Other").toLower()];
	for(auto it = groups.begin(); it != groups.end(); it++) {
		// если группа не является группой Разное и при этом имеет недостаточный размер, либо имя группы пустое
		if((&others != &it.value() && it.value().size() < n) || it.key().isEmpty()) {
			others.append(it.value());
			it.value().clear();
		}
	}

	displayGroups(groups, SortPolicy::ByName);
}

void MainWindow::groupByName() {
	// выполнить группировку по первой букве имени объекта:
	QMap<QString, QList<ObjectInfo>> groups;
	for(const auto& obj : m_objects) {
		QString groupName;
		if(!obj.name.isEmpty() && containsCyrillic(obj.name.front())) {
			// если имя не пустое и начинается с кириллической буквы, используем ее для имени группы
			groupName = obj.name.front();
			if(!m_ui->caseSensitive->isChecked()) {
				groupName = groupName.toLower();
			}
		} else {
			// иначе используем символ решетки
			groupName = "#";
		}

		groups[groupName].append(obj);
	}

	displayGroups(groups, SortPolicy::ByName);
}

void MainWindow::groupByTime() {
	// сгруппировать объекты по времени их создания
	QMap<QString, QList<ObjectInfo>> groups;
	for(const auto& obj : m_objects) {
		groups[stringify(obj.relativeTime())].append(obj);
	}

	displayGroups(groups, SortPolicy::ByTime);
}

void MainWindow::groupByDistance() {
	// сгруппировать объекты по их относительному расстоянию от начала координат
	QMap<QString, QList<ObjectInfo>> groups;
	for(const auto& obj : m_objects) {
		groups[stringify(obj.relativeDistance())].append(obj);
	}

	displayGroups(groups, SortPolicy::ByDistance);
}


void MainWindow::loadFile() {
	// запросить у пользователя имя входного файла
	const auto prevFilename = m_ui->loadedFile->text();
	const auto filename = QFileDialog::getOpenFileName(
		this, // родительский виджет
		"", // использовать заголовок по умолчанию
		!prevFilename.isEmpty() ? prevFilename : QDir::homePath(), // начальная директория
		"", // без фильтров
		nullptr, // без фильтра по умолчанию
		QFileDialog::DontUseNativeDialog // использовать QWidget реализацию диалога
	);

	// отобразить содержание выбранного файла на экране
	QFile file(filename);
	if(file.open(QIODevice::ReadOnly)) {
		m_ui->loadedText->setPlainText(file.readAll());
		m_ui->loadedFile->setText(filename);
	}
}

void MainWindow::saveFile() {
	// запросить у пользователя имя выходного файла
	const auto prevFilename = m_ui->savedFile->text();
	const auto filename = QFileDialog::getSaveFileName(
		this, // родительский виджет
		"", // использовать заголовок по умолчанию
		!prevFilename.isEmpty() ? prevFilename : QDir::homePath(), // начальная директория
		"", // без фильтров
		nullptr, // без фильтра по умолчанию
		QFileDialog::DontUseNativeDialog // использовать QWidget реализацию диалога
	);

	// сохранить обработанный файл на диск
	QFile file(filename);
	if(file.open(QIODevice::WriteOnly)) {
		QTextStream(&file) << m_ui->processedText->toPlainText();
		m_ui->savedFile->setText(filename);
	}
}


void MainWindow::processText() {
	// выполнить группирующее действие и отобразить результат
	const auto groupPoilcy = m_ui->groupPolicy->currentData().toInt();
	if(groupPoilcy == None) {
		m_ui->processedText->setPlainText(m_ui->loadedText->toPlainText());
		return;
	}

	if(m_objects.isEmpty()) {
		m_objects = parseObjectList();
	}
	switch(groupPoilcy) {
		case ByType: {
			groupByType();
			break;
		}
		case ByName: {
			groupByName();
			break;
		}
		case ByTime: {
			groupByTime();
			break;
		}
		case ByDistance: {
			groupByDistance();
			break;
		}
	}
}

void MainWindow::parseText() {
	m_objects.clear();
	processText();
}


void MainWindow::toggleSaveButton() {
	if(!m_ui->processedText->document()->isEmpty()) {
		m_ui->saveButton->setEnabled(true);
	} else {
		m_ui->saveButton->setEnabled(false);
	}
}
