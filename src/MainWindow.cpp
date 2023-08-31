#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), m_ui(new Ui::mainWindow()) {
	m_ui->setupUi(this);
}

MainWindow::~MainWindow() {
	delete m_ui;
}
