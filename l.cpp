#ifndef LINE_HPP
#define LINE_HPP
#include "Point2D.hpp"

struct Line {
public:
    int startPointIndex, endPointIndex;
    
    friend bool operator==(const Line& lhs, const Line& rhs) {
        return lhs.startPointIndex == rhs.startPointIndex && lhs.endPointIndex == rhs.endPointIndex;
    }

    friend bool operator!=(const Line& lhs, const Line& rhs) { 
        return !(lhs == rhs);
    }
};
#endif