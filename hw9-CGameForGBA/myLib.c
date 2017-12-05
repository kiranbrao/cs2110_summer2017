#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
	/*	for(int c = 0; c<width; c++) {
			setPixel(row+r, col+c, color);
		}
	*/
	DMA[3].src = &color;
	DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawGrid(int gridsize, unsigned short color) {
	for(int r = 0; r < 160; r += gridsize) {
		for(int c = 0; c <=  (184 - gridsize); c++) {
			setPixel(r, c, color);
		}
		//DMA[3].src = &color;
		//DMA[3].dst = &videoBuffer[OFFSET(r, 0, 240)];
		//DMA[3].cnt = 240 | DMA_ON | DMA_SOURCE_FIXED;
	}
	for(int c = 0; c < 184; c+= gridsize) {
		for(int r = 0; r <= (160 - gridsize); r++) {
			setPixel(r, c, color);
		}
	}
}

void drawBorder() {
	int gridsize = GRIDSIZE;
	int colors[] = {DARKGREEN, BROWN, DARKGREEN, BROWN, DARKGREEN, BROWN, DARKGREEN};
	for (int c = 185 - gridsize; c < 240; c++) {
		setPixel(0, c, colors[0]);
		setPixel((160 - gridsize), c, colors[0]);
	}
	for (int c = 186 - gridsize; c < 239; c++) {
		setPixel(1, c, colors[1]);
		setPixel((159 - gridsize), c, colors[1]);
	}
	for (int c = 187 - gridsize; c < 238; c++) {
		setPixel(2, c, colors[2]);
		setPixel((158 - gridsize), c, colors[2]);
	}
	for (int c = 188 - gridsize; c < 237; c++) {
		setPixel(3, c, colors[3]);
		setPixel((157 - gridsize), c, colors[3]);
	}
	for (int c = 189 - gridsize; c < 236; c++) {
		setPixel(4, c, colors[4]);
		setPixel((156 - gridsize), c, colors[4]);
	}
	for (int c = 190 - gridsize; c < 235; c++) {
		setPixel(5, c, colors[5]);
		setPixel((155 - gridsize), c, colors[5]);
	}
	for (int c = 191 - gridsize; c < 234; c++) {
		setPixel(6, c, colors[6]);
		setPixel((154 - gridsize), c, colors[6]);
	}
	for (int r = 0; r < 160 - gridsize; r++) {
		setPixel(r, 185 - gridsize, colors[0]);
		setPixel(r, 239, colors[0]);
	}
	for (int r = 1; r < 159 - gridsize; r++) {
		setPixel(r, 186 - gridsize, colors[1]);
		setPixel(r, 238, colors[1]);
	}
	for (int r = 2; r < 158 - gridsize; r++) {
		setPixel(r, 187 - gridsize, colors[2]);
		setPixel(r, 237, colors[2]);
	}
	for (int r = 3; r < 157 - gridsize; r++) {
		setPixel(r, 188 - gridsize, colors[3]);
		setPixel(r, 236, colors[3]);
	}
	for (int r = 4; r < 156 - gridsize; r++) {
		setPixel(r, 189 - gridsize, colors[4]);
		setPixel(r, 235, colors[4]);
	}
	for (int r = 5; r < 155 - gridsize; r++) {
		setPixel(r, 190 - gridsize, colors[5]);
		setPixel(r, 234, colors[5]);
	}
	for (int r = 6; r < 154 - gridsize; r++) {
		setPixel(r, 191 - gridsize, colors[6]);
		setPixel(r, 233, colors[6]);
	}
}

unsigned short* getGridArray(int gridsize) {
	int i = 0;
	static unsigned short gridArray[NUMCELLS];
	for(char r = 1; r < (160 - gridsize); r += gridsize) {
		for(char c = 1; c < (184 - gridsize); c += gridsize) {
			gridArray[i] = (r << 8) + c;
			i++;
		}
	}
	return gridArray;
}

unsigned short getRandomGridPoint(unsigned short* gridArray, SEGMENT head, SEGMENT* body, SEGMENT oldtail, int bodysize) {
	//printf("%d", (rand() % NUMCELLS));
	//static int counter = 0;
	/*if ((score % 2) == 0) {
		return gridArray[200];
	} else {
		return gridArray[20];
	}*/
	//counter++;
	int numcells = NUMCELLS;
	int random = (rand() % (numcells-1));
	int allClear = 0;
	while (!allClear) {
		allClear = 1;
		int row = gridArray[random] >> 8;
		int col = gridArray[random] & (0x00FF);
		if (((head.row)==row) && (head.col==col)) {
			random = (rand() % (numcells-1));
			allClear = 0;
			break;
		}
		if (((oldtail.row)==row) && (oldtail.col==col)) {
			random = (rand() % (numcells-1));
			allClear = 0;
			break;
		}
		for (int i = 0; i < bodysize; i++) {
			if ((row == body[i].row) && (col == body[i].col)) {
				random = (rand() % (numcells-1));
				allClear = 0;
				break;
			}
		}
	}
	return gridArray[random];
}

void clearscreen(volatile unsigned short color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240 * 160) | DMA_SOURCE_FIXED | DMA_ON;
}


void drawImage3(int r, int c, int width, int height, const unsigned short* image){
	/*for (int i = 0, a = 0; i < width; i++, a++) {
        for (int j = 0, b = 0; j < height; j++, b++) {grid
            videoBuffer[(j*240) + i] = image[((b-c)*(width)) + (a-r)];
        }
    }*/
    for (int i = 0; i < height; i++) {
  		DMA[3].src = image + (i * width);
  		DMA[3].dst = videoBuffer + (c + ((r+i) * 240));
  		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
    }
}


int checkFoodEaten(SEGMENT* head, FOOD* food) {
	if ((head->row == food->row) && (head->col == food->col)) {
		return 1;
	} else {
		return 0;
	}
}

int checkCollision(SEGMENT head, SEGMENT* body, int bodysize) {

	for (int i=1; i < bodysize; i++) {
		if ((head.row == body[i].row) && (head.col == body[i].col)) {
			return 1;
		}
	}
	return 0;
}

void delay(int n) {
	volatile int x = 0;
	for(int i=0; i<n*8000; i++) {
		x++;
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160)
		;
	while(SCANLINECOUNTER<160)
		;
}

