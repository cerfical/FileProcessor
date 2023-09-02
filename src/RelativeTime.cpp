#include "RelativeTime.hpp"

QString stringify(RelativeTime time) {
	switch(time) {

#define RELATIVE_TIME(t) case RelativeTime::t: return #t;
		RELATIVE_TIME_LIST
#undef RELATIVE_TIME

		default: return "Other";
	}
}
