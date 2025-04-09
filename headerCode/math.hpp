#pragma once
#include <iostream>

using namespace std;

struct Vector2f{
    Vector2f()
    :x(0.0f), y(0.0f)
    {}

    Vector2f(float p_x, float p_y)
    :x(p_x), y(p_y)
    {}

    void print(){
        cout << x << ", " << y << endl;
    }

    float x, y;
};
