#ifndef OBJECTINFO_HPP
#define OBJECTINFO_HPP

#include <QString>

struct ObjectInfo {
	QString name;
	QString type;
	QString x, y;
	QString time;
};

enum GroupPolicy {
	None,
	ByType,
	ByName,
	ByTime,
	ByDistance
};

#endif
