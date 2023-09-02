#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "RelativeTime.hpp"
#include "RelativeDistance.hpp"

#include <QString>
#include <QList>


struct Object {
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


QString stringify(const QList<Object>& objects, const QString& caption);

QString stringify(const Object& obj);


#endif
