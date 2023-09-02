#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Object.hpp"

#include <QMainWindow>
#include <QMap>


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
	void saveFile();

	void processText();
	void parseText();

	void toggleSaveButton();
	/** @} */

private:
	/** @{ */
	enum class SortPolicy {
		ByName,
		ByDistance,
		ByTime
	};

	enum GroupPolicy {
		None,
		ByType,
		ByName,
		ByTime,
		ByDistance
	};
	/** @} */

	/** @{ */
	static bool containsCyrillic(const QString& str);
	/** @} */

	/** @{ */
	void displayGroups(QMap<QString, QList<Object>>& groups, SortPolicy sort);
	void sortGroup(QList<Object>& objects, SortPolicy sort);

	void groupByType();
	void groupByName();
	void groupByTime();
	void groupByDistance();
	/** @} */

	QList<Object> m_objects;
	Ui::MainWindow *m_ui = nullptr;
};

#endif
