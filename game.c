#include "game.h"

#include <stdio.h>
#include <stdlib.h>

Sprite brown;
Sprite conyy;

Cash cash1;
Cash cash2;
Cash cash3;
Cash cash4;
Cash cash5;
Cash cash6;

int difficulty;
int frameCount;
int limiter;
// STATE FUNCTIONS

void initGame(void) {
	brown.player = 1;
	brown.x = 110;
	brown.y = 10;
	brown.w = 20;
	brown.h = 33;
	brown.xSpeed = 4;
	brown.ySpeed = 6;
	brown.points = 0;

	conyy.player = 2;
	conyy.x = 110;
	conyy.y = 200;
	conyy.w = 20;
	conyy.h = 33;
	conyy.xSpeed = 6;
	conyy.ySpeed = 4;
	conyy.points = 0;

	initMoney(1, 1, 1, 1, 1, 1);

	frameCount = 0;
	limiter = 100000;
}

void initStart(void) {
	// drawFullScreenImageDMA(welcome);
	drawCenteredString(30, 120, 0, 0, "collect the falling cash!", WHITE);
	drawCenteredString(45, 120, 0, 0, "press 'enter' to play!", WHITE);
	conyy.x = 160 - conyy.h;
	conyy.y = 0;
}

void start(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	if (frameCount == 70000) {
		drawFullScreenImageDMA(welcome);
		drawCenteredString(60, 120, 0, 0, "Too late, Cony is coming ...", RED);
	}
	if (frameCount % 4500 == 0 && frameCount > 70000 && frameCount < 130500 && limiter >= 1) {
		// limiter -= 195;
		limiter /= 2;
	} else if (frameCount == 130500) {
		limiter = 1500;
	}
	if (frameCount % limiter == 0 && frameCount > 130000) {
		drawCenteredString(60, 120, 0, 0, "cony wants you to give me a good grade", WHITE);
		drawCenteredString(80, 120, 0, 0, "press 'enter' to play :)", WHITE);
		// waitForVBlank();
	}
	if (frameCount % limiter == 0 && frameCount > 80000) {
		// undrawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);

		drawRectDMA(conyy.x, conyy.y, conyy.w, conyy.h, RED);

		conyy.y += conyy.ySpeed;
		if (frameCount < 130500) {
			if (conyy.y < 0) {
				conyy.y += abs(conyy.ySpeed);
				conyy.ySpeed = abs(conyy.ySpeed);
			}
			if (conyy.y > 240 - conyy.w) {
				conyy.y -= abs(conyy.ySpeed);
				conyy.ySpeed = -abs(conyy.ySpeed);
			}
			conyy.x += conyy.xSpeed;
			if (conyy.x < 0) {
				conyy.x += abs(conyy.xSpeed);
				conyy.xSpeed = abs(conyy.xSpeed);
			}
			if (conyy.x > 160 - conyy.h) {
				conyy.x -= abs(conyy.xSpeed);
				conyy.xSpeed = -abs(conyy.xSpeed);
			}
		} else {
			if (conyy.y < 100) {
				conyy.y += abs(conyy.ySpeed);
				conyy.ySpeed = abs(conyy.ySpeed);
			}
			if (conyy.y > 140 - conyy.w) {
				conyy.y -= abs(conyy.ySpeed);
				conyy.ySpeed = -abs(conyy.ySpeed);
			}
		}

		drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);
	}

	if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
		*state = INITINSTRUCTIONS;
	}
	frameCount++;
}

void initInstructions(void) {
	// drawFullScreenImageDMA(store);
	drawCenteredString(40, 120, 0, 0, "brown: use arrow keys to move", GRAY);
	drawCenteredString(50, 120, 0, 0, "get 100 points to beat cony!", GRAY);
	drawCenteredString(60, 120, 0, 0, "press 'enter' to continue!", GRAY);
}

void instructions(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
		*state = INITSELECT;
	}
}

void initSelect(void) {
	// fillScreenDMA(BLACK);
	cash1.x = 80;
	cash2.x = 80;
	cash3.x = 80;
	cash1.y = 70 - cash1.w / 2;
	cash2.y = 120 - cash2.w / 2;
	cash3.y = 170 - cash3.w / 2;

	brown.y = 120 - brown.w / 2;
	brown.x = 100;
	drawCenteredString(40, 120, 0, 0, "choose your difficulty!", WHITE);
	drawImageDMA(80, 70 - cash1.w / 2, cash1.w, cash1.h, cash);
	drawImageDMA(80, 120 - cash2.w / 2, cash2.w, cash2.h, cash);
	drawImageDMA(80, 170 - cash3.w / 2, cash3.w, cash3.h, cash);
	drawImageDMA(brown.x, brown.y, brown.w, brown.h, sprite);

	drawCenteredString(70, 70, 0, 0, "easy", WHITE);
	drawCenteredString(70, 120, 0, 0, "medium", WHITE);
	drawCenteredString(70, 170, 0, 0, "hard", WHITE);
}

void select(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	checkAllCollisions(&brown, state);
	moveArrows(currentButtons, previousButtons, state, &brown);
}

void initPlay(void) {
	// fillScreenDMA(BLACK);
	initGame();

	drawImageDMA(brown.x, brown.y, brown.w, brown.h, sprite);
	drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);

	drawRectDMA(5, 120, 1, 150, WHITE);

	drawPoints();

	drawImageDMA(cash1.x, cash1.y, cash1.w, cash1.h, cash);
	drawImageDMA(cash3.x, cash3.y, cash3.w, cash3.h, cash);
	drawImageDMA(cash5.x, cash5.y, cash5.w, cash5.h, cash);
	if (difficulty == 3) {
		drawImageDMA(cash2.x, cash2.y, cash2.w, cash2.h, cash);
	} else {
		drawImageDMA(cash2.x, cash2.y, cash2.w, cash2.h, cash);
		drawImageDMA(cash4.x, cash4.y, cash4.w, cash4.h, cash);
		drawImageDMA(cash6.x, cash6.y, cash6.w, cash6.h, cash);
	}

	waitForVBlank();
}

void play(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	moveArrows(currentButtons, previousButtons, state, &brown);
	if (brown.points >= 100) {
		*state = INITWIN;
	} else if (conyy.points >= 100) {
		*state = INITLOSE;
	}

	if (frameCount % 200 == 0) {
		rainAllMoney();
		moveCony();
		waitForVBlank();
	}
	checkAllCollisions(&brown, state);
	checkAllCollisions(&conyy, state);
	// waitForVBlank();
	frameCount++;
}

void initWin(void) {
	// drawFullScreenImageDMA(end);
	drawCenteredString(70, 165, 0, 0, "Brown wins!", RED);
	drawCenteredString(83, 165, 0, 0, "& saved a forest fire!", GRAY);
}

void win(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
		*state = INITGAME;
	}
}

void initLose(void) {
	// drawFullScreenImageDMA(end);
	drawCenteredString(70, 165, 0, 0, "Cony wins!", RED);
	drawCenteredString(83, 165, 0, 0, "big chungus ftw!", GRAY);
}

void lose(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
		*state = INITGAME;
	}
}

void checkReset(u32 currentButtons, u32 previousButtons, enum gba_state *state) {
	if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
		*state = RESET;
	}
}

void reset(enum gba_state *state) {
	*state = INITGAME;
}

void redraw(enum gba_state *state) {
	// waitForVBlank();
	switch (*state) {
	case START:
		initStart();
		break;
	case SELECT:
		initSelect();
		break;
	case PLAY:
		initPlay();
		break;
	default:
		break;
	}
	// waitForVBlank();
}

// LOGIC FUNCTIONS

void moveArrows(u32 currentButtons, u32 previousButtons, enum gba_state *state, Sprite *brown) {
	if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
		undrawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
		brown->x -= brown->xSpeed;
		if (*state == SELECT) {
			checkInBounds(brown);
		} else if (*state == PLAY) {
			checkInHalf(brown);
		}
		drawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
	}
	if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
		undrawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
		brown->x += brown->xSpeed;
		if (*state == SELECT) {
			checkInBounds(brown);
		} else if (*state == PLAY) {
			checkInHalf(brown);
		}
		drawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
	}
	if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
		undrawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
		brown->y -= brown->ySpeed;
		if (*state == SELECT) {
			checkInBounds(brown);
		} else if (*state == PLAY) {
			checkInHalf(brown);
		}
		drawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
	}
	if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
		undrawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
		brown->y += brown->ySpeed;
		if (*state == SELECT) {
			checkInBounds(brown);
		} else if (*state == PLAY) {
			checkInHalf(brown);
		}
		drawImageDMA(brown->x, brown->y, brown->w, brown->h, sprite);
	}
	UNUSED(state);
}

void moveCony(void) {
	// if (frameCount % 1000 == 0 && difficulty != 3) {
	if (difficulty != 3) {
		undrawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);

		conyy.y += conyy.ySpeed;
		if (conyy.y < 121) {
			conyy.y += abs(conyy.ySpeed);
			conyy.ySpeed = abs(conyy.ySpeed);
		}
		if (conyy.y > 239 - conyy.w) {
			conyy.y -= abs(conyy.ySpeed);
			conyy.ySpeed = -abs(conyy.ySpeed);
		}

		drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);
		drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);
	}
}

void rainAllMoney(void) {
	// if (frameCount % 600 == 0) {
	rainMoney(&cash1, 1);
	rainMoney(&cash3, 1);
	rainMoney(&cash5, 1);
	if (difficulty == 3) {
		rainMoney(&cash2, 2);
	} else {
		rainMoney(&cash2, 2);
		rainMoney(&cash4, 2);
		rainMoney(&cash6, 2);
	}
	// }
}

void rainMoney(Cash *money, int player) {
	undrawImageDMA(money->x, money->y, money->w, money->h, cash);
	money->x += 5 * difficulty;
	if (difficulty == 3 && player == 2) {
		money->x += 15;
	}
	if (money->x > 160) {
		money->x = 20;
		if (player == 2 && difficulty == 3) {
			money->y = conyy.y + 5;
		} else if (player == 2) {
			money->y = randint(125, 235 - money->w);
		} else {
			money->y = randint(5, 115 - money->w);
		}
	}
	drawImageDMA(money->x, money->y, money->w, money->h, cash);
}

void initMoney(int one, int two, int three, int four, int five, int six) {
	if (one == 1) {
		cash1.x = 20;
		cash1.y = randint(5, 115 - cash1.w);
		cash1.w = 12;
		cash1.h = 11;
	}
	if (two == 1) {
		cash2.x = 20;
		if (difficulty == 3) {
			cash2.y = conyy.y + 5;
		} else {
			cash2.y = randint(125, 235 - cash2.w);
		}
		cash2.w = 12;
		cash2.h = 11;
	}
	if (three == 1) {
		cash3.x = 20;
		cash3.y = randint(5, 115 - cash3.w);
		cash3.w = 12;
		cash3.h = 11;
	}
	if (four == 1) {
		cash4.x = 20;
		if (difficulty == 3) {
			cash4.y = conyy.y + 5;
		} else {
			cash4.y = randint(125, 235 - cash4.w);
		}
		cash4.w = 12;
		cash4.h = 11;
	}
	if (five == 1) {
		cash5.x = 20;
		cash5.y = randint(5, 115 - cash5.w);
		cash5.w = 12;
		cash5.h = 11;
	}
	if (six == 1) {
		cash6.x = 20;
		if (difficulty == 3) {
			cash6.y = conyy.y + 5;
		} else {
			cash6.y = randint(125, 235 - cash6.w);
		}
		cash6.w = 12;
		cash6.h = 11;
	}
}

void checkInBounds(Sprite *brown) {
	if (brown->x < 80) {
		brown->x += brown->xSpeed;
	}
	if (brown->x > 160 - brown->h) {
		brown->x -= brown->xSpeed;
	}
	if (brown->y < 0) {
		brown->y += brown->ySpeed;
	}
	if (brown->y > 240 - brown->w) {
		brown->y -= brown->ySpeed;
	}
}

void checkInHalf(Sprite *brown) {
	if (brown->x < 81) {
		brown->x += brown->xSpeed;
	}
	if (brown->x > 159 - brown->h) {
		brown->x -= brown->xSpeed;
	}
	if (brown->y < 0) {
		brown->y += brown->ySpeed;
	}
	if (brown->y > 120 - brown->w) {
		brown->y -= brown->ySpeed;
	}
}

int checkCollision(Sprite *sprite, Cash *cashi) {
	if (cashi->x < sprite->x + sprite->w &&
	    cashi->x + cashi->w > sprite->x &&
	    cashi->y < sprite->y + sprite->h &&
	    cashi->y + cashi->h > sprite->y) {
		return 1;
	}
	return 0;
}

void checkAllCollisions(Sprite *player, enum gba_state *state) {
	if (checkCollision(player, &cash1)) {
		undrawImageDMA(cash1.x, cash1.y, cash1.w, cash1.h, cash);
		initMoney(1, 0, 0, 0, 0, 0);
		drawImageDMA(cash1.x, cash1.y, cash1.w, cash1.h, cash);
		drawImageDMA(brown.x, brown.y, brown.w, brown.h, sprite);
		if (*state == SELECT) {
			*state = INITPLAY;
			difficulty = 1;
		} else if (*state == PLAY) {
			player->points++;
			drawPoints();
		}
	}
	if (checkCollision(player, &cash2)) {
		undrawImageDMA(cash2.x, cash2.y, cash2.w, cash2.h, cash);
		initMoney(0, 1, 0, 0, 0, 0);
		drawImageDMA(cash2.x, cash2.y, cash2.w, cash2.h, cash);
		drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);
		if (*state == SELECT) {
			difficulty = 2;
			*state = INITPLAY;
		} else if (*state == PLAY) {
			player->points++;
			drawPoints();
		}
	}
	if (checkCollision(player, &cash3)) {
		undrawImageDMA(cash3.x, cash3.y, cash3.w, cash3.h, cash);
		initMoney(0, 0, 1, 0, 0, 0);
		drawImageDMA(cash3.x, cash3.y, cash3.w, cash3.h, cash);
		drawImageDMA(brown.x, brown.y, brown.w, brown.h, sprite);
		if (*state == SELECT) {
			difficulty = 3;
			*state = INITPLAY;
		} else if (*state == PLAY) {
			player->points++;
			drawPoints();
		}
	}
	if (checkCollision(player, &cash4) && difficulty != 3) {
		undrawImageDMA(cash4.x, cash4.y, cash4.w, cash4.h, cash);
		initMoney(0, 0, 0, 1, 0, 0);
		drawImageDMA(cash4.x, cash4.y, cash4.w, cash4.h, cash);
		drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);
		player->points++;
		drawPoints();
	}
	if (checkCollision(player, &cash5)) {
		undrawImageDMA(cash5.x, cash5.y, cash5.w, cash5.h, cash);
		initMoney(0, 0, 0, 0, 1, 0);
		drawImageDMA(cash5.x, cash5.y, cash5.w, cash5.h, cash);
		drawImageDMA(brown.x, brown.y, brown.w, brown.h, sprite);
		player->points++;
		drawPoints();
	}
	if (checkCollision(player, &cash6) && difficulty != 3) {
		undrawImageDMA(cash6.x, cash6.y, cash6.w, cash6.h, cash);
		initMoney(0, 0, 0, 0, 0, 1);
		drawImageDMA(cash6.x, cash6.y, cash6.w, cash6.h, cash);
		drawImageDMA(conyy.x, conyy.y, conyy.w, conyy.h, cony);
		player->points++;
		drawPoints();
	}
}

void drawPoints(void) {
	char bp[2];
	sprintf(bp, "%d", brown.points);
	char cp[2];
	sprintf(cp, "%d", conyy.points);

	drawRectDMA(5, 45, 22, 11, BLACK);
	drawString(5, 5, "count:", WHITE);
	drawChar(5, 45, bp[0], WHITE);
	drawChar(5, 55, bp[1], WHITE);

	drawRectDMA(5, 165, 22, 11, BLACK);
	drawString(5, 125, "count:", WHITE);
	drawChar(5, 165, cp[0], WHITE);
	drawChar(5, 175, cp[1], WHITE);
}