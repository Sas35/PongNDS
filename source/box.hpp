#pragma once

struct Point {
    int X;
    int Y;
};


class Box {
public:
    inline Box() {};
    inline Box(int w, int h, int sX, int sY, int tlX, int tlY, int clr) :
        Width(w), Height(h), speedX(sX), speedY(sY), TopLeft(Point{tlX, tlY}), color(clr) {};
    inline ~Box() {};

    inline Point GetBottomRight() {
        return Point{TopLeft.X + Width, TopLeft.Y + Height};
    }

    virtual void Move() { TopLeft.X += speedX; TopLeft.Y += speedY; };
    bool CheckCollision(Box box1);

    int Width = 10;
    int Height = 10;
    int speedX = 0;
    int speedY = 0;
    Point TopLeft;
    int color;
};