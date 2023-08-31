#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QFile>

enum GroupPolicy {
	None,
	ByType,
	ByName,
	ByCreationTime,
	ByDistance
};


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), m_ui(new Ui::MainWindow()) {
	m_ui->setupUi(this);

	// проинициализировать список доступных политик группировки
	const auto groupPolicy = m_ui->groupPolicy;
	groupPolicy->addItem(tr("Type"), GroupPolicy::ByType);
	groupPolicy->addItem(tr("Name"), GroupPolicy::ByName);
	groupPolicy->addItem(tr("Creation time"), GroupPolicy::ByCreationTime);
	groupPolicy->addItem(tr("Distance"), GroupPolicy::ByDistance);
	groupPolicy->addItem(tr("None"), GroupPolicy::None);

	// по умолчанию отключить группировку
	groupPolicy->setCurrentIndex(groupPolicy->count() - 1);
}

MainWindow::~MainWindow() {
	delete m_ui;
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


void MainWindow::processFile() {
	// получить выбранную в настоящее время политику группировки
	const auto groupPoilcy = m_ui->groupPolicy->currentData().toInt();

	// выполнить группирующее действие и отобразить результат
	switch(groupPoilcy) {
		case None: {
			m_ui->processedText->setPlainText(m_ui->loadedText->toPlainText());
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


void MainWindow::enableSave() {
	if(!m_ui->processedText->document()->isEmpty()) {
		m_ui->saveButton->setEnabled(true);
	} else {
		m_ui->saveButton->setEnabled(false);
	}
}
