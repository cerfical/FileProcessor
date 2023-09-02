#include "ObjectInfo.hpp"
#include <QDateTime>
#include <QtMath>

RelativeTime ObjectInfo::relativeTime() const {
	const auto date = QDateTime::fromMSecsSinceEpoch(time.toLongLong()).date();
	const auto today = QDate::currentDate();

	if(date == today) {
		return RelativeTime::Today;
	}
	if(date == today.addDays(-1)) {
		return RelativeTime::Yesterday;
	}

	if(date.year() == today.year()) {
		if(date.weekNumber() == today.weekNumber()) {
			return RelativeTime::ThisWeek;
		}
		if(date.month() == today.month()) {
			return RelativeTime::ThisMonth;
		}
		return RelativeTime::ThisYear;
	}

	if(date.year() < today.year()) {
		return RelativeTime::Earlier;
	}
	return RelativeTime::Later;
}


RelativeDistance ObjectInfo::relativeDistance() const {
	const auto d = distance();
	if(d < 100) {
		return RelativeDistance::Under100;
	}

	if(d < 1000) {
		return RelativeDistance::Under1000;
	}

	if(d < 10000) {
		return RelativeDistance::Under10000;
	}

	return RelativeDistance::TooFar;
}

qreal ObjectInfo::distance() const {
	return qSqrt(qPow(x.toDouble(), 2) + qPow(y.toDouble(), 2));
}


QString stringify(const QList<ObjectInfo>& objects, const QString& caption) {
	QString str;
	str += caption + " {\n";
	for(const auto& obj : objects) {
		str += ' ' + stringify(obj) + '\n';
	}
	str += "}\n";
	return str;
}

QString stringify(const ObjectInfo& obj) {
	QString str;

	str += obj.name + ' ';
	str += obj.x + ' ' + obj.y + ' ';
	str += obj.type + ' ';
	str += obj.time;

	return str;
}
