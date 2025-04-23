#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#define _USE_MATH_DEFINES // For M_PI in Windows
#include <math.h> //For M_PI in Windows, and for older compilers
#include "GeometricPrimitives.h"
#include "Scene.h"
using namespace Model;
using namespace View;


int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML 3D Line");
    window.setFramerateLimit(60);

    float focalLength = 500.0f;
    float cameraDistance = 500.0f;

    std::vector<Point> points = {
        Point(0.0f, 0.0f, 100.0f),
        Point(50.0f, 30.0f, 150.0f),
        Point(100.0f, -20.0f, 200.0f),
        Point(150.0f, 40.0f, 250.0f),
        Point(200.0f, 0.0f, 300.0f)
    };

    const int gridSize = 20;
    const int planeSize = 200;

    float rotationX = 135.0f; // вращение вверх и вниз (но сохран€€ принадлежность плоскости)
    float rotationZ = 0.0f; // вращение вокруг оси Z (вращение самой плоскости)
    bool isRotating = false;
    sf::Vector2i lastMousePos;

#ifndef M_PI
    constexpr double M_PI = 3.14159265358979323846;
#endif

    // —начала вращаем вокруг Z, потом поднимаем/опускаем камеру (вращаем вокруг X)
    auto rotatePoint = [&](const Point& p) {
        float radZ = rotationZ * M_PI / 180.0f;

        // Rotate around Z axis
        float x1 = p.X() * cos(radZ) - p.Y() * sin(radZ);
        float y1 = p.X() * sin(radZ) + p.Y() * cos(radZ);
        float z1 = p.Z();

        float radX = rotationX * M_PI / 180.0f;

        // ¬ращаем вокруг X, но только Y и Z (эмулируем вращение камеры вверх/вниз)
        float y2 = y1 * cos(radX) - z1 * sin(radX);
        float z2 = y1 * sin(radX) + z1 * cos(radX);
        float x2 = x1;

        return Point(x2, y2, z2 + cameraDistance);
        };

    std::vector<sf::Vertex> gridVertices;
    for (int x = -planeSize; x <= planeSize; x += gridSize) {
        gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(static_cast<float>(x), -planeSize, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));
        gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(static_cast<float>(x), planeSize, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));

        gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(-planeSize, static_cast<float>(x), points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));
        gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(planeSize, static_cast<float>(x), points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled) {
                float zoomDelta = 0.0f;
#if SFML_VERSION_MAJOR < 2 || (SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR < 5)
                zoomDelta = event.mouseWheelScroll.wheel;
#else
                zoomDelta = event.mouseWheelScroll.delta;
#endif

                cameraDistance -= zoomDelta * 50.0f;
                cameraDistance = std::max(cameraDistance, 100.0f);
                cameraDistance = std::min(cameraDistance, 1000.0f);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                isRotating = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                isRotating = false;
            }
        }

        if (isRotating) {
            sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
            sf::Vector2i mouseDelta = currentMousePos - lastMousePos;

            // √оризонтальное движение вращает вокруг Z
            float rotationSpeedZ = mouseDelta.x * 0.1f;
            rotationZ += rotationSpeedZ;

            // ¬ертикальное движение вращает "камеру" вверх/вниз
            float rotationSpeedX = mouseDelta.y * 0.1f;
            rotationX += rotationSpeedX;

            lastMousePos = currentMousePos;
        }

        window.clear();

        gridVertices.clear();
        for (int x = -planeSize; x <= planeSize; x += gridSize) {
            gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(static_cast<float>(x), -planeSize, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));
            gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(static_cast<float>(x), planeSize, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));

            gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(-planeSize, static_cast<float>(x), points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));
            gridVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(planeSize, static_cast<float>(x), points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color(100, 100, 100)));
        }

        window.draw(gridVertices.data(), gridVertices.size(), sf::Lines);

        if (points.size() > 1) {
            std::vector<sf::Vertex> lineVertices;
            for (const auto& point : points) {
                lineVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(point), focalLength, screenWidth, screenHeight), sf::Color::White));
            }
            window.draw(lineVertices.data(), lineVertices.size(), sf::LineStrip);
        }

        std::vector<sf::Vertex> axesVertices;

        axesVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(0, 0, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color::Red));
        axesVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(50, 0, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color::Red));

        axesVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(0, 0, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color::Green));
        axesVertices.push_back(sf::Vertex(Scene::projectTo2D(rotatePoint(Point(0, 50, points[0].Z())), focalLength, screenWidth, screenHeight), sf::Color::Green));

        window.draw(axesVertices.data(), axesVertices.size(), sf::Lines);

        window.display();
    }

    return 0;
}