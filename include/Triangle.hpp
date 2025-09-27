#include "Renderable.hpp"

enum class TRIANGLES
{
    // By side
    EQUILATERAL, // All equal sides
    ISOSCELES, // Two equal sides
    SCALENE, // No equal sides
    // By angle
    ACUTE, // three angles < 90 degrees
    RIGHT, // One angle = 90 degrees
    OBTUSE // One angle > 90 degrees
};

class Triangle : public Renderable {

    Triangle(TRIANGLES type);
};