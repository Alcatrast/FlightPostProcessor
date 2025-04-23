
#include <SFML/Graphics.hpp>
#include "GeometricPrimitives.h"
#include"Scene.h"
using namespace Model;
namespace View {

         sf::Vector2f Scene::projectTo2D(const Point& point, float focalLength, float screenWidth, float screenHeight) {
        float x = point.X() * focalLength / (point.Z() + focalLength);
        float y = point.Y() * focalLength / (point.Z() + focalLength);
        return sf::Vector2f(x + screenWidth / 2.0f, y + screenHeight / 2.0f);
    }

}
