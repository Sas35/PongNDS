#include "box.hpp"

bool Box::CheckCollision(Box box1) {
    if (box1.TopLeft.X > GetBottomRight().X || TopLeft.X > box1.GetBottomRight().X ) return false;
    if (box1.TopLeft.Y > GetBottomRight().Y || TopLeft.Y > box1.GetBottomRight().Y ) return false;

    return true;
}