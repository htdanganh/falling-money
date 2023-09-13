#ifndef GAME_H
#define GAME_H

#include "gba.h"
#include "images/cash.h"
#include "images/cony.h"
#include "images/end.h"
#include "images/sprite.h"
#include "images/store.h"
#include "images/welcome.h"

enum gba_state {
	INITGAME,
	INITSTART,
	START,
	INITINSTRUCTIONS,
	INSTRUCTIONS,
	INITSELECT,
	SELECT,
	INITPLAY,
	PLAY,
	INITWIN,
	WIN,
	INITLOSE,
	LOSE,
	RESET,
};

typedef struct Sprite {
	int player;
	int x;
	int y;
	int w;
	int h;
	int points;
	int size;
	int xSpeed;
	int ySpeed;
} Sprite;

typedef struct Cash {
	int x;
	int y;
	int w;
	int h;
	int speed;
	char test;
} Cash;

void initGame(void);

void initStart(void);
void start(u32 currentButtons, u32 previousButtons, enum gba_state *state);

void initInstructions(void);
void instructions(u32 currentButtons, u32 previousButtons, enum gba_state *state);

void initSelect(void);
void select(u32 currentButtons, u32 previousButtons, enum gba_state *state);

void initPlay(void);
void play(u32 currentButtons, u32 previousButtons, enum gba_state *state);

void initWin(void);
void win(u32 currentButtons, u32 previousButtons, enum gba_state *state);

void initLose(void);
void lose(u32 currentButtons, u32 previousButtons, enum gba_state *state);

void checkReset(u32 currentButtons, u32 previousButtons, enum gba_state *state);
void reset(enum gba_state *state);

void redraw(enum gba_state *state);

void moveArrows(u32 currentButtons, u32 previousButtons, enum gba_state *state, Sprite *sprite);
void moveCony(void);

void rainMoney(Cash *money, int player);
void rainAllMoney(void);
void initMoney(int one, int two, int three, int four, int five, int six);

void checkInBounds(Sprite *brown);
void checkInHalf(Sprite *brown);

int checkCollision(Sprite *sprite, Cash *cash);
void checkAllCollisions(Sprite *sprite, enum gba_state *state);

void drawPoints(void);

#endif
