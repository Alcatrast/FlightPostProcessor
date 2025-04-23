#ifndef SC_H
#define SC_H

namespace View {

    class Scene {
        static 
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