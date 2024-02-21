#include "playerPaddle.hpp"
#include "constants.hpp"

void PlayerPaddle::Move() {
    int tempPosY = TopLeft.Y + speedY;
    int tempPosYBottom = tempPosY + Height;
    if (tempPosY < 0 || tempPosYBottom > SCR_MAX_Y)
        return;
    
    TopLeft.Y = tempPosY;
}