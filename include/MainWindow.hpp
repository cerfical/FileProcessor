#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	/** @{ */
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
	/** @} */

private slots:
	/** @{ */
	void loadFile();
	void processFile();
	void saveFile();

	void enableSave();
	/** @} */

private:
	Ui::MainWindow *m_ui = nullptr;
};

#endif
