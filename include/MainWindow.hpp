#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

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
	enum GroupPolicy {
		None,
		ByType,
		ByName,
		ByCreationTime,
		ByDistance
	};

	struct ObjectInfo {
		QString name;
		QString type;
		QString x, y;
		QString time;
	};
	/** @} */

	/** @{ */
	static bool containsCyrillic(const QString& str);
	/** @} */

	/** @{ */
	QList<ObjectInfo> parseObjectList();

	void groupByType();
	void groupByName();
	void groupByCreationTime();
	void groupByDistance();
	/** @} */

	QList<ObjectInfo> m_objects;
	Ui::MainWindow *m_ui = nullptr;
};

#endif
