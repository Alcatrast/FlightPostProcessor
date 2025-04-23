#include "GeometricPrimitives.h"
namespace Model {

        float Point::X() const { return _x; }
        float Point::Y() const { return _y; }
        float Point::Z() const { return _z; }
        Point::Point(float x, float y, float z) : _x(x), _y(y), _z(z) {}
}