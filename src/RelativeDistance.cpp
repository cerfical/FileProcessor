#include "RelativeDistance.hpp"

QString stringify(RelativeDistance distance) {
	switch(distance) {

#define RELATIVE_DISTANCE(d) case RelativeDistance::d: return #d;
		RELATIVE_DISTANCE_LIST
#undef RELATIVE_DISTANCE

		default: return "Other";
	}
}
