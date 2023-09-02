#ifndef OBJECTINFO_HPP
#define OBJECTINFO_HPP

#include "RelativeTime.hpp"
#include "RelativeDistance.hpp"

#include <QString>
#include <QList>


struct ObjectInfo {
	/** @{ */
	RelativeTime relativeTime() const;
	/** @} */

	/** @{ */
	RelativeDistance relativeDistance() const;
	qreal distance() const;
	/** @} */

	QString name;
	QString type;
	QString x, y;
	QString time;
};


QString stringify(const QList<ObjectInfo>& objects, const QString& caption);

QString stringify(const ObjectInfo& obj);


#endif
