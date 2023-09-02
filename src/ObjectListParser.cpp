#include "ObjectListParser.hpp"
#include <QRegularExpression>

bool ObjectListParser::isReal(const QString& str) {
	bool ok = true;
	str.toDouble(&ok);
	return ok;
}

bool ObjectListParser::isInt(const QString& str) {
	bool ok = true;
	str.toLongLong(&ok);
	return ok;
}

bool ObjectListParser::parse(QList<Object>& objects) {
	const static QRegularExpression wsRegex("\\s+");

	bool groupStarted = false, result = true;
	while((m_input.skipWhiteSpace(), !m_input.atEnd())) {
		const auto words = m_input.readLine().split(wsRegex, Qt::SkipEmptyParts);

		// проверить, является ли считанная строка маркером конца группы
		if(words[0] == "}") {
			if(!groupStarted || words.size() > 1) {
				// неправильный синтаксис
				result = false;
			}
			groupStarted = false;
			continue;
		}

		// проверить, является ли считанная строка маркером начала группы
		if(words.value(1, "") == "{") {
			if(groupStarted || words.size() > 2) {
				// неправильный синтаксис
				result = false;
			}
			groupStarted = true;
			continue;
		}

		// строка, описывающая объект, должна состоять ровно из 5 элементов
		result = result && words.size() == 5;

		// считать соответствующие компоненты объекта
		const auto name = words[0];
		const auto x = words.value(1, ""), y = words.value(2, "");
		const auto type = words.value(3, "");
		const auto time = words.value(4, "");

		// проверить правильность указанных координат и времени создания объекта
		result = result && isReal(x) && isReal(y) && isInt(time);
		objects.emplaceBack(name, type, x, y, time);
	}

	// группа должна быть закрытой
	result = result && !groupStarted;
	return result;
}
