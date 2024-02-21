#pragma once
#include <nds.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include "constants.hpp"

namespace Menu {
    void drawScore(PrintConsole *, int, int);
    void drawGameMenu(PrintConsole *, int, bool);
    bool menuActions(touchPosition, int &, bool &);
}