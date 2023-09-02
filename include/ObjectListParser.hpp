#ifndef OBJECTLISTPARSER_HPP
#define OBJECTLISTPARSER_HPP

#include "Object.hpp"

#include <QTextStream>
#include <QList>

class ObjectListParser {
public:
	/** @{ */
	explicit ObjectListParser(const QString& input)
		: m_str(input), m_input(&m_str)
	{ }
	/** @} */

	/** @{ */
	bool parse(QList<Object>& objects);
	/** @} */

private:
	/** @{ */
	static bool isReal(const QString& str);
	static bool isInt(const QString& str);
	/** @} */

	QString m_str;
	QTextStream m_input;
};

#endif
