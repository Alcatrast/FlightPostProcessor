#ifndef SC_H
#define SC_H

#include <SFML/Graphics.hpp>
#include "GeometricPrimitives.h"
using namespace Model;
namespace View {

    class Scene {
    public:
        static sf::Vector2f projectTo2D(const Point& point, float focalLength, float screenWidth, float screenHeight);
        
    private:
        float _x, _y, _z;
    };
}
#endif