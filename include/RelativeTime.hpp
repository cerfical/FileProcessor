#ifndef RELATIVETIME_HPP
#define RELATIVETIME_HPP

#define RELATIVE_TIME_LIST   \
	RELATIVE_TIME(Today)     \
	RELATIVE_TIME(Yesterday) \
	RELATIVE_TIME(ThisWeek)  \
	RELATIVE_TIME(ThisMonth) \
	RELATIVE_TIME(ThisYear)  \
	RELATIVE_TIME(Earlier)   \
	RELATIVE_TIME(Later)


#include <QString>


enum class RelativeTime {

#define RELATIVE_TIME(t) t,
	RELATIVE_TIME_LIST
#undef RELATIVE_TIME

};


QString stringify(RelativeTime time);


#endif
