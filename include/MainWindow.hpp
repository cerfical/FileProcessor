#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ObjectInfo.hpp"

#include <QMainWindow>
#include <QList>

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

	void toggleSaveButton();
	/** @} */

private:
	/** @{ */
	static QString stringifyObjectList(const QString& caption, const QList<ObjectInfo>& objects);
	static bool containsCyrillic(const QString& str);
	/** @} */

	/** @{ */
	QList<ObjectInfo> parseObjectList();
	void sortByName(QList<ObjectInfo>& objects);

	void groupByType();
	void groupByName();
	void groupByTime();
	void groupByDistance();
	/** @} */

	QList<ObjectInfo> m_objects;
	Ui::MainWindow *m_ui = nullptr;
};

#endif
