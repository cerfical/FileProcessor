#ifndef WORLDOBJECT_HPP
#define WORLDOBJECT_HPP

#include <QString>
#include <QPointF>

class WorldObject {
public:
	/** @{ */
	friend QDataStream& operator>>(QDataStream& in, WorldObject& obj);

	friend QDataStream& operator<<(QDataStream& out, const WorldObject& obj);
	/** @} */

	/** @{ */
	WorldObject(const QString& name, const QString& type, const QPointF& pos, qreal time)
		: m_name(name), m_type(type), m_position(pos), m_creationTime(time)
	{ }
	/** @} */

	/** @{ */
	const QString& name() const noexcept {
		return m_name;
	}

	const QString& type() const noexcept {
		return m_type;
	}
	/** @} */

	/** @{ */
	const QPointF& position() const noexcept {
		return m_position;
	}

	qreal x() const noexcept {
		return position().y();
	}

	qreal y() const noexcept {
		return position().x();
	}
	/** @} */

	/** @{ */
	qreal creationTime() const noexcept {
		return m_creationTime;
	}
	/** @} */

private:
	QString m_name, m_type;
	QPointF m_position;

	qreal m_creationTime = 0.0;
};

#endif
