#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "gba.h"

int main(void) {
	/* TODO: */
	// Manipulate REG_DISPCNT here to set Mode 3. //
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	// Save current and previous state of button input.
	u32 previousButtons = BUTTONS;
	u32 currentButtons = BUTTONS;

	// Load initial application state
	enum gba_state state = INITGAME;

	while (1) {
		currentButtons = BUTTONS;  // Load the current state of the buttons

		checkReset(currentButtons, previousButtons, &state);

		switch (state) {
		// --------------------------------------------------------
		case INITGAME:
			initGame();
			state = INITSTART;
			break;
		// --------------------------------------------------------
		case INITSTART:
			drawFullScreenImageDMA(welcome);
			waitForVBlank();
			initStart();
			state = START;
			break;
		case START:
			start(currentButtons, previousButtons, &state);
			break;
		// --------------------------------------------------------
		case INITINSTRUCTIONS:
			drawFullScreenImageDMA(store);
			waitForVBlank();
			initInstructions();
			state = INSTRUCTIONS;
			break;
		case INSTRUCTIONS:
			instructions(currentButtons, previousButtons, &state);
			break;
		// --------------------------------------------------------
		case INITSELECT:
			fillScreenDMA(BLACK);
			waitForVBlank();
			initSelect();
			state = SELECT;
			break;
		case SELECT:
			select(currentButtons, previousButtons, &state);
			break;
		// --------------------------------------------------------
		case INITPLAY:
			fillScreenDMA(BLACK);
			waitForVBlank();
			initPlay();
			state = PLAY;
			break;
		case PLAY:
			play(currentButtons, previousButtons, &state);
			// waitForVBlank();
			break;
		// --------------------------------------------------------
		case INITWIN:
			drawFullScreenImageDMA(end);
			waitForVBlank();
			initWin();
			state = WIN;
			break;
		case WIN:
			win(currentButtons, previousButtons, &state);
			break;
		// --------------------------------------------------------
		case INITLOSE:
			drawFullScreenImageDMA(end);
			waitForVBlank();
			initLose();
			state = LOSE;
			break;
		case LOSE:
			lose(currentButtons, previousButtons, &state);
			break;
		// --------------------------------------------------------
		case RESET:
			reset(&state);
			break;
		}
		// --------------------------------------------------------
		previousButtons = currentButtons;  // Store the current state of the buttons
	}

	return 0;
}
