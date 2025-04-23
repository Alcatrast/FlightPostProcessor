#pragma once
#ifndef GEPR_H
#define GEPR_H

namespace Model {

    class Point {
    public:
        float X() const;
        float Y() const;
        float Z() const;
        Point(float x, float y, float z);

    private:
        float _x, _y, _z;
    };
}
#endif