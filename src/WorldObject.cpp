#include "WorldObject.hpp"
#include <QDataStream>

QDataStream& operator>>(QDataStream& in, WorldObject& obj) {
	QString name, type;
	qreal x = 0.0, y = 0.0, time = 0.0;

	in >> name;
	in >> x >> y;
	in >> type;
	in >> time;

	obj = WorldObject(name, type, { x, y }, time);
	return in;
}

QDataStream& operator<<(QDataStream& out, const WorldObject& obj) {
	out << obj.name();
	out << obj.x() << obj.y();
	out << obj.type();
	out << obj.creationTime();
	return out;
}
