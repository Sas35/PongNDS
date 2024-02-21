#include <nds.h>
#include <gl2d.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
//Sound
#include <maxmod9.h>
#include "soundbank.h"
#include "soundbank_bin.h"

#include "box.hpp"
#include "playerPaddle.hpp"
#include "controls.hpp"
#include "constants.hpp"
#include "menu.hpp"
#include "ai.hpp"

using namespace std;

volatile int frame = 0;

void Vblank()
{
    frame++;
}

void CheckInputs(PlayerPaddle &player1, PlayerPaddle &player2, const Box ball, const int paddleVel, const bool ai) {
    // Button inputs
    scanKeys();
    if (leftUp()) {
        player1.speedY = -paddleVel;
    }
    else if (leftDown()) {
        player1.speedY = paddleVel;
    }
    else {
        player1.speedY = 0;
    }

    if (ai) {
        player2.speedY = paddleVel * Ai::aiMove((player2.TopLeft.Y + player2.Height) / 2, (ball.TopLeft.Y + ball.Height) / 2);
    }
    else {
        if (rightUp()) {
            player2.speedY = -paddleVel;
        }
        else if (rightDown()) {
            player2.speedY = paddleVel;
        }
        else {
            player2.speedY = 0;
        }
    }
}

void ResetBall(Box &ball) {
    ball.TopLeft.X = (SCR_MAX_X - ball.Width) / 2;
    ball.TopLeft.Y = (SCR_MAX_Y - ball.Height) / 2;

    ball.speedX *= rand()%2 ? 1 : -1;
    ball.speedY *= rand()%2 ? 1 : -1;
    
    mmEffect( SFX_POINT );
}

int main(void) {
    touchPosition touchXY;
    irqSet(IRQ_VBLANK, Vblank);

	videoSetMode(MODE_5_3D);
	videoSetModeSub(MODE_0_2D);

    int ballSpeed = 1;
    int paddleVel = 2;
    bool ai = true;
    bool menuChange = true;
    int changeFrm = 0;

    Box ball(10, 10, ballSpeed, ballSpeed, (SCR_MAX_X - 10)/ 2, (SCR_MAX_Y - 10)/ 2, 888888);
    PlayerPaddle player1(5, 40, 0, 0, 10, (SCR_MAX_Y - 40)/ 2, 232323);
    PlayerPaddle player2(5, 40, 0, 0, SCR_MAX_X - 20, (SCR_MAX_Y  - 40)/ 2, 555555);

    vector<Box *> boxes;
    boxes.push_back(&ball);
    boxes.push_back(&player1);
    boxes.push_back(&player2);

    // Init consoles
	PrintConsole scoreConsole;
	consoleInit(&scoreConsole, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
    // Draws right at the top of the bottom screen
	consoleSetWindow(&scoreConsole, CONSOLE_T_WIDTH / 6, 0, CONSOLE_T_WIDTH / 1.5, 1);

	PrintConsole menuConsole;
	consoleInit(&menuConsole, 1, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	// Draws in the middle of the bottom screen
	consoleSetWindow(&menuConsole, 0, CONSOLE_T_HEIGHT / 2, CONSOLE_T_WIDTH, 4);

    // Sound
    //---------------------------------------------------------
	// init maxmod with default settings
	//---------------------------------------------------------
	mmInitDefaultMem( (mm_addr)soundbank_bin );
	mmLoadEffect( SFX_BOUNCE );
	mmLoadEffect( SFX_POINT );


    // initialize gl2d
	glScreen2D();

    while (true) {
        glBegin2D();

        Menu::drawScore(&scoreConsole, player1.score, player2.score);

        CheckInputs(player1, player2, ball, paddleVel, ai);
        touchRead(&touchXY);

        if (menuChange) {
			Menu::drawGameMenu(&menuConsole, ballSpeed, ai);
            ball.speedX = ball.speedX > 0 ? ballSpeed : -ballSpeed;
            ball.speedY = ball.speedY > 0 ? ballSpeed : -ballSpeed;
			menuChange = false;

			changeFrm = frame;
		}

		if (frame >= changeFrm + 20) {
			changeFrm = 0;
		}

		// If we don't have a menu change, and it's been enough frames, we can wait for an input
		if (!menuChange && changeFrm == 0) {
            menuChange = Menu::menuActions(touchXY, ballSpeed, ai);
        }

        //Ball movement/collisions
        if (ball.TopLeft.X <= 10) {
            ++player2.score;
            ResetBall(ball);
        }
        else if (ball.GetBottomRight().X >= SCR_MAX_X) {
            ++player1.score;
            ResetBall(ball);
        }
        
        if (ball.TopLeft.Y <= 0) {
            ball.speedY = ballSpeed;
            mmEffect( SFX_BOUNCE );
        }
        else if (ball.GetBottomRight().Y >= SCR_MAX_Y) {
            ball.speedY = -ballSpeed;
            mmEffect( SFX_BOUNCE );
        }

        if (ball.CheckCollision(player1) || ball.CheckCollision(player2)) {
            ball.speedX = -ball.speedX;
            mmEffect( SFX_BOUNCE );
        }


        for(auto& currentBox: boxes) {
            currentBox->Move();
            glBoxFilled(currentBox->TopLeft.X, currentBox->TopLeft.Y, currentBox->GetBottomRight().X, currentBox->GetBottomRight().Y, currentBox->color);
        }

        glEnd2D();
        glFlush(0);

        swiWaitForVBlank();
    }
}