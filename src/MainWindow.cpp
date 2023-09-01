#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QRegularExpression>
#include <QFileDialog>
#include <QFile>

#include <algorithm>
#include <QMap>


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), m_ui(new Ui::MainWindow()) {
	m_ui->setupUi(this);

	// проинициализировать список доступных политик группировки
	const auto groupPolicy = m_ui->groupPolicy;
	groupPolicy->addItem(tr("Type"), GroupPolicy::ByType);
	groupPolicy->addItem(tr("Name"), GroupPolicy::ByName);
	groupPolicy->addItem(tr("Time"), GroupPolicy::ByTime);
	groupPolicy->addItem(tr("Distance"), GroupPolicy::ByDistance);
	groupPolicy->addItem(tr("None"), GroupPolicy::None);

	// по умолчанию отключить группировку
	groupPolicy->setCurrentIndex(groupPolicy->count() - 1);
}

MainWindow::~MainWindow() {
	delete m_ui;
}


QString MainWindow::stringifyObjectList(const QString& caption, const QList<ObjectInfo>& objects) {
	QString str;

	if(!objects.isEmpty()) {
		str += caption + " {\n";
		for(const auto& obj : objects) {
			str += ' ' + obj.name + ' ';
			str += obj.x + ' ' + obj.y + ' ';
			str += obj.type + ' ';
			str += obj.time + '\n';
		}
		str += "}\n";
	}

	return str;
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


void MainWindow::sortByName(QList<ObjectInfo>& objects) {
	// sort objects by name if in-group sorting is enabled
	if(m_ui->enableSort->isChecked()) {
		std::ranges::sort(objects, [](const ObjectInfo& lhs, const ObjectInfo& rhs) {
			return lhs.name < rhs.name;
		});
	}
}


void MainWindow::groupByType() {
	static const auto otherType = tr("Other");

	// разбить объекты на группы в зависимости от их типов
	QMap<QString, QList<ObjectInfo>> groups;
	for(const auto& obj : m_objects) {
		groups[obj.type].append(obj);
	}

	// распечатать группы объектов, которые имеют достаточный размер
	QString str;
	for(auto it = groups.begin(); it != groups.end(); it++) {
		if(it.key() != otherType ) {
			const auto n = m_ui->typedGroupSize->value();
			if(it.value().size() >= n) {
				sortByName(it.value());
				str += stringifyObjectList(it.key(), it.value());
			} else {
				groups[otherType].append(std::move(it.value()));
			}
		}
	}

	// остальные объекты вынести в отдельную группу
	auto& otherObjects = groups[otherType];

	sortByName(otherObjects);
	str += stringifyObjectList(otherType, otherObjects);

	m_ui->processedText->setPlainText(str);
}

void MainWindow::groupByName() {
	// выполнить группировку по первой букве имени объекта
	QMap<QString, QList<ObjectInfo>> groups;
	for(const auto& obj : m_objects) {
		const auto groupName = obj.name.front();
		if(containsCyrillic(groupName)) {
			groups[groupName].append(obj);
		} else {
			groups["#"].append(obj);
		}
	}

	// вывести на экран разбитый на группы список объектов в алфавитном порядке
	QString str;
	for(auto it = groups.begin(); it != groups.end(); it++) {
		sortByName(it.value());
		str += stringifyObjectList(it.key(), it.value());
	}
	m_ui->processedText->setPlainText(str);
}

void MainWindow::groupByTime() {

}

void MainWindow::groupByDistance() {

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
		m_objects.clear();
		m_ui->loadedText->setPlainText(file.readAll());
		m_ui->loadedFile->setText(filename);
	}
}

void MainWindow::processFile() {
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


void MainWindow::toggleSaveButton() {
	if(!m_ui->processedText->document()->isEmpty()) {
		m_ui->saveButton->setEnabled(true);
	} else {
		m_ui->saveButton->setEnabled(false);
	}
}
