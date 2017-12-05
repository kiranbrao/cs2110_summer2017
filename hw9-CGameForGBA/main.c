#include "myLib.h"
#include "text.h"
#include <stdio.h>
#include "snekstart.h"
#include "sneklost.h"
#include "snek.h"

//#define NBALLS 1
int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;
	int numcells = NUMCELLS;
	
	SEGMENT head;
	SEGMENT oldtail = head;
	SEGMENT body[numcells];
	FOOD food;
	int size = GRIDSIZE - 1;
	int bodysize = 0;
	int colors[] = { RED, BLUE, YELLOW, CYAN, MAGENTA, WHITE, ORANGE, PURPLE, TEAL, PINK};
	int ncolors = sizeof(colors) / sizeof(colors[0]);
	unsigned short* gridArray;
	gridArray = getGridArray(GRIDSIZE);
	unsigned short randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
	head.row = randomGridPoint >> 8;
	head.col = (randomGridPoint & (0x00FF));
	//head.row = gridArray[50] >> 8;
	//head.col = gridArray[50] & (0x00FF);
	head.color = GREEN;
	head.rd = 0;
	head.cd = 0;
	head.size = size;
	randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
	food.row = randomGridPoint >> 8;
	food.col = randomGridPoint & (0x00FF);
	//food.row = gridArray[100] >> 8;
	//food.col = gridArray[100] & (0x00FF);
	food.rd = 0;
	food.cd = 0;
	food.size = size;
	food.color = MAGENTA;
	oldtail = head;
	unsigned int slower = 0;
	int iseaten = 0;
	int score = 0;
	char scoreStr[5];
	char oldScoreStr[5];
	//char string4[5];
	//int pressedA = 0;
	//int pressedStart = 0;
	//int pressedSelect = 0;
	enum gameState gameState = START;
	int speedmod = SPEEDMOD;
	int oldspeedmod = speedmod;
	int speedmult = 0;
	int initialzeGame = 1;

	while(1) { // Game Loop
		switch(gameState) {
			case START:
				waitForVblank();
				drawImage3(0,0,SNEKSTART_WIDTH,SNEKSTART_HEIGHT,snekstart);
				gameState = START_NODRAW;
				break;
			case START_NODRAW:
					if(KEY_DOWN_NOW(BUTTON_A)) {
							//pressedA = 0;
							gameState = GAME;
					}
						break;
			case GAME:
				if (initialzeGame) {
					clearscreen(BLACK);
					drawRect(0,0,(160 - GRIDSIZE), (184 - GRIDSIZE), DARKGREEN);
					drawGrid(GRIDSIZE, BROWN);
					drawBorder();
					drawRect(food.row, food.col, food.size, food.size, food.color);
					initialzeGame = 0;
					drawImage3(15, 191, SNEK_WIDTH, SNEK_HEIGHT, snek);
					drawString(88, 196, "NOMS", GREEN);
					drawString(99, 193, "EATEN", GREEN);
					sprintf(scoreStr, "%d", score);
					drawString(115, 205, scoreStr, WHITE);
					for (int i = 0; i < 5; i++) {
						oldScoreStr[i] = scoreStr[i];
					}
				}
				if(KEY_DOWN_NOW(BUTTON_UP)) {
					if(bodysize == 1) {
						if (((head.row - GRIDSIZE) != oldtail.row) && (head.col != oldtail.col)){
							head.rd = -1 * GRIDSIZE;
							head.cd = 0;
						}
					} else if (bodysize > 1) {
						if (((head.row - GRIDSIZE) != body[1].row) && (head.col != body[1].col)){
							head.rd = -1 * GRIDSIZE;
							head.cd = 0;
						}
					} else {
						head.rd = -1 * GRIDSIZE;
						head.cd = 0;
					}
				}
				if(KEY_DOWN_NOW(BUTTON_DOWN)) {
					if(bodysize == 1) {
						if (((head.row + GRIDSIZE) != oldtail.row) && (head.col != oldtail.col)) {
							head.rd = 1 * GRIDSIZE;
							head.cd = 0;
						}
					} else if (bodysize > 1) {
						if (((head.row + GRIDSIZE) != body[1].row) && (head.col != body[1].col)){
							head.rd = 1 * GRIDSIZE;
							head.cd = 0;
						}
					} else {
						head.rd = 1 * GRIDSIZE;
						head.cd = 0;
					}

				}
				if(KEY_DOWN_NOW(BUTTON_LEFT)) {
					if(bodysize == 1) {
						if (((head.col - GRIDSIZE) != oldtail.col) && (head.row != oldtail.row)) {
							head.cd = -1 * GRIDSIZE;
							head.rd = 0;
						} 
					} else if (bodysize > 1) {
						if (((head.col - GRIDSIZE) != body[1].col) && (head.row != body[1].row)){
							head.cd = -1 * GRIDSIZE;
							head.rd = 0;
						}
					} else {
						head.cd = -1 * GRIDSIZE;
						head.rd = 0;
					}
				}
				if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
					if(bodysize == 1) {
						if (((head.col + GRIDSIZE) != oldtail.col) && (head.row != oldtail.row)) {
							head.cd = 1 * GRIDSIZE;
							head.rd = 0;
						}
					} else if (bodysize > 1) {
						if (((head.col + GRIDSIZE) != body[1].col) && (head.row != body[1].row)){
							head.cd = 1 * GRIDSIZE;
							head.rd = 0;
						}
					} else {
						head.cd = 1 * GRIDSIZE;
						head.rd = 0;
					}
				}
				/*if(KEY_DOWN_NOW(BUTTON_B) && !pressedStart) {
					gameState = PAUSE;
					break;
				}*/
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					//pressedSelect = 1;
					gameState = RESTART;
					break;
				}
						
				iseaten = checkFoodEaten(&head, &food);

				int oldspeedmult = speedmult;
				speedmult = bodysize/5;
				if (oldspeedmult != speedmult) {
					if ((oldspeedmod - ((speedmult * 400))) > 0) {
						speedmod = (oldspeedmod - ((speedmult * 400)));
					} else {
						speedmod = 1;
					}
				}

				if (slower%speedmod == 0) {
					struct segment *hp = &head;
					hp->row = hp->row + hp->rd;
					(*hp).col += hp->cd;
					if(hp->row < 1) {
						hp->row = 160 - GRIDSIZE - size;
					}
					if(hp->row > 160- GRIDSIZE - size) {
						hp->row = 1;
					}
					if(hp->col < 1) {
						hp->col = 184 - GRIDSIZE - size;
					}
					if(hp->col > 184 - GRIDSIZE - size) {
						hp->col = 1;
					} 

					waitForVblank();

					if (iseaten) {
						score++;
						struct food *fp = &food;
						unsigned short oldColor = fp->color;
						randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
						fp->row = randomGridPoint >> 8;
						fp->col = randomGridPoint & 0x00FF;
						fp->color = colors[rand() % ncolors];
						bodysize++;
						if (bodysize == 1) {
							body[0] = head; 
							body[0].color = oldColor;
						} else {
							body[bodysize-1] = body[bodysize-2];
							body[bodysize-1].color = oldColor;
						}
						sprintf(scoreStr, "%d", score);
						drawString(115, 205, oldScoreStr, BLACK);
						for (int i = 0; i < 5; i++) {
							oldScoreStr[i] = scoreStr[i];
						}
						drawString(115, 205, scoreStr, WHITE);

						drawRect(fp->row, fp->col, fp->size, fp->size, fp->color);
					}

					if (checkCollision(head, body, bodysize)) {
						gameState = GAMEOVER;
						break;
					}

					struct segment *otp = &oldtail;
					drawRect(otp->row, otp->col, otp->size, otp->size, DARKGREEN);

					for (int i = 0; i < bodysize; i++) {
						struct segment *bp = &body[i];
						drawRect(bp->row, bp->col, bp->size, bp->size, bp->color);
					}

					drawRect(hp->row, hp->col, size, size, hp->color);
					if (bodysize > 0) {
						oldtail = body[bodysize-1];
						for (int i = bodysize; i >= 0; i--) {
							if (i == 0) {
								unsigned short tempColor = body[i].color;
								body[i] = head;
								body[i].color = tempColor;
							} else {
								unsigned short tempColor = body[i].color;
								body[i] = body[i-1];
								body[i].color = tempColor;
							}
						}
					} else {
						oldtail = head;
					}
				}
				slower++;
				if (slower == 3200000000){
					slower = 1;
				}
				break;
			/*case PAUSE:
				if(KEY_DOWN_NOW(BUTTON_B) && !pressedStart) {
					pressedStart = 0;
					gameState = GAME;
				}
				break;*/
			case GAMEOVER:
				waitForVblank();
				drawImage3(0,0,SNEKLOST_WIDTH,SNEKLOST_HEIGHT,sneklost);
				sprintf(scoreStr, "%d", score);
				drawString(77, 146, scoreStr, WHITE);
				randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
				gameState = GAMEOVER_NODRAW;
				head.row = randomGridPoint >> 8;
				head.col = (randomGridPoint & (0x00FF));
				head.rd = 0;
				head.cd = 0;
				randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
				food.row = randomGridPoint >> 8;
				food.col = randomGridPoint & (0x00FF);
				bodysize = 0;
				oldtail = head;
				score = 0;
				speedmod = SPEEDMOD;
				initialzeGame = 1;
				break;
			case GAMEOVER_NODRAW:
				if(KEY_DOWN_NOW(BUTTON_A)) {
					//pressedA = 0;
					gameState = GAME;
				}
				if(KEY_DOWN_NOW(BUTTON_SELECT)) {
					//pressedSelect = 1;
					gameState = START;
					break;
				}
				break;
			case RESTART:
				waitForVblank();
				gameState = START;
				randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
				head.row = randomGridPoint >> 8;
				head.col = (randomGridPoint & (0x00FF));
				head.rd = 0;
				head.cd = 0;
				randomGridPoint = getRandomGridPoint(gridArray, head, body, oldtail, bodysize);
				food.row = randomGridPoint >> 8;
				food.col = randomGridPoint & (0x00FF);
				bodysize = 0;
				oldtail = head;
				score = 0;
				speedmod = SPEEDMOD;
				initialzeGame = 1;
				break;
		}
			
		/*if (KEY_DOWN_NOW(BUTTON_B)) {
			pressedStart = 1;
		} else {
			pressedStart = 0;
		} */

		/*if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			pressedSelect = 1;
		} else {
			pressedSelect = 0;
		} */
	}
}



		




