#pragma once

#include "box.hpp"

class PlayerPaddle : public Box
{
public:
    inline PlayerPaddle(int w, int h, int sX, int sY, int tlX, int tlY, int clr) : Box(w, h, sX, sY, tlX, tlY, clr) {};
    inline ~PlayerPaddle(){};
    void Move() override;
    int score = 0;
};