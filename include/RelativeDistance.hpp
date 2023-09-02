#ifndef RELATIVEDISTANCE_HPP
#define RELATIVEDISTANCE_HPP

#define RELATIVE_DISTANCE_LIST    \
	RELATIVE_DISTANCE(Under100)   \
	RELATIVE_DISTANCE(Under1000)  \
	RELATIVE_DISTANCE(Under10000) \
	RELATIVE_DISTANCE(TooFar)


#include <QString>


enum class RelativeDistance {

#define RELATIVE_DISTANCE(d) d,
	RELATIVE_DISTANCE_LIST
#undef RELATIVE_DISTANCE

};


QString stringify(RelativeDistance distance);


#endif
