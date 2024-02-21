#include "ai.hpp"

namespace Ai{
    int aiMove(int paddleMidPos, int ballY) {
        return paddleMidPos > ballY ? -1 : 1;
    }
}