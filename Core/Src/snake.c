/*
 * snake.c
 *
 *  Created on: Dec 12, 2023
 *      Author: QUI
 */

#include "snake.h"
#include <stdlib.h>
#include "led_7seg.h"

uint16_t x_max = 239, y_max = 319;
uint16_t x1 = 120, y1 = 150, x2 = 130, y2 = 160;
uint16_t	headX1, headY1, headX2, headY2;
enum state { goUp, goDown, goLeft, goRight };
enum state firstState = goUp;
enum state prevState = goUp;
uint16_t random_eat(uint16_t minN, uint16_t maxN) {
	return minN + rand()%(maxN + 1 - minN);
}
uint16_t snakeTailX[1000];
uint16_t snakeTailY[1000];
uint16_t x_food;
uint16_t y_food;

uint16_t snakeTailLen = 10;
uint16_t virticalLen = 10;
uint16_t horizonLen = 10;
uint16_t speed = 1;
uint16_t count = 0;
//uint16_t pre_count = 0;
uint16_t CREATE_FOOD = 1;
void setHead(uint16_t X1, uint16_t X2, uint16_t Y1, uint16_t Y2) {
	headX1 = X1;
	headX2 = X2;
	headY1 = Y1;
	headY2 = Y2;
}
void snake_init() {
	lcd_Fill(x1, y1, x2, y2, BLUE);
	setHead(x1, x2, y1, y2);
}
void display() {
	lcd_ShowStr(50,30,"Game Over!!!",WHITE,BLACK,24,0);
	HAL_Delay(1000);
	lcd_ShowStr(50,30,"Game Over!!!",BLUE,YELLOW,24,0);
	HAL_Delay(1000);
}
void fill_white() {

}
void game_over(){
//	lcd_ShowStr(50,30,"Game Over!!!",WHITE,BLACK,24,0);
//	lcd_ShowStr(50,30,"Game Over!!!",WHITE,YELLOW,24,0);
	lcd_ShowStr(25,120,"May choi ngu vl!",WHITE,RED,24,0);
	lcd_ShowStr(10,200,"Diem cua ban la: ",BLUE,WHITE,24,0);
	if(count < 10)
		lcd_ShowIntNum(210,200,count,1,BLUE,WHITE,24);
	else
		lcd_ShowIntNum(210,200,count,2,BLUE,WHITE,24);
	lcd_Fill(x1, y1, x2, y2, WHITE);
	lcd_Fill(x_food, y_food, x_food+5, y_food+5, WHITE);
	display();
}


void turnRight(uint16_t X1, uint16_t X2, uint16_t Y1, uint16_t Y2) {
	if (prevState == goUp)
	Y2 = Y1 + 10;
	else if (prevState == goDown)
	Y1 = Y2 - 10;
	else Y1 = Y2 - 10;
	if (firstState != goRight) {
		horizonLen -= 2;
		X1 -= horizonLen;
		X2 -= horizonLen;
		lcd_Fill(X1,Y1,X2,Y2, WHITE);
	} else {
		if (X2 - X1 >= snakeTailLen){
				lcd_Fill(X1, Y1, X2, Y2, WHITE);
				X1 +=2;
			}
			X2 += 2;
			lcd_Fill(X1, Y1, X2, Y2, BLUE);
	}

}

void turnLeft(uint16_t X1, uint16_t X2, uint16_t Y1, uint16_t Y2) {
	if (prevState == goUp)
	Y2 = Y1 + 10;
	else if (prevState == goDown)
	Y1 = Y2 - 10;
	else Y1 = Y2 - 10;
	//lcd_Fill(x1, y1, x2, y2, WHITE);
	if (X2 - X1 >= snakeTailLen){
		lcd_Fill(X1, Y1, X2, Y2, WHITE);
		X2 -=2;
	}
	X1 -= 2;
	lcd_Fill(X1, Y1, X2, Y2, BLUE);
}

void turnDown(uint16_t X1, uint16_t X2, uint16_t Y1, uint16_t Y2) {

	X1 = X2 - 10;
	if (firstState != goDown) {
		virticalLen -= 2;
		Y1 -= virticalLen;
		Y2 -= virticalLen;
		lcd_Fill(X1,Y1,X2,Y2, WHITE);
	} else {
		if (Y2 - Y1 >= snakeTailLen){
				lcd_Fill(X1, Y1, X2, Y2, WHITE);
				Y2 -=2;
			}
			Y1 -= 2;
			lcd_Fill(X1, Y1, X2, Y2, BLUE);
	}
}
void turnUp(uint16_t X1, uint16_t X2, uint16_t Y1, uint16_t Y2) {
	if (prevState == goRight)
		X1 = X2 - 10;
		else if (prevState == goLeft)
		X2 = X1 + 10;
		else X1 = X2 - 10;
	if (firstState != goUp) {
		virticalLen -= 2;
		Y1 += virticalLen;
		Y2 += virticalLen;
		lcd_Fill(X1,Y1,X2,Y2, WHITE);
	} else {
		if (Y2 - Y1 >= snakeTailLen){
				lcd_Fill(X1, Y1, X2, Y2, WHITE);
				Y1 -=2;
			}
			Y2 -= 2;
			lcd_Fill(X1, Y1, X2, Y2, BLUE);
	}
}
void right() {
	switch (prevState) {
				case goUp:
					turnRight(x1,x2,headY1,headY2);
					if (virticalLen >= 0)
					turnUp(headX1,headX2,y1,y2);
					//deleteDown(headX1,headX2,headY1+snakeTailLen,headY2+snakeTailLen);
					x2 += 2;
					if (x2 - x1 >= snakeTailLen+2)
						x1 += 2;
					break;
				case goDown:
					turnRight(x1,x2,headY1,headY2);
					if (virticalLen >= 0)
					turnDown(headX1,headX2,y1,y2);
					//deleteDown(headX1,headX2,headY1+snakeTailLen,headY2+snakeTailLen);
					x2 += 2;
					if (x2 - x1 >= snakeTailLen+2)
						x1 += 2;
					break;
				case goLeft:
					turnRight(x1,x2,headY1,headY2);
					x2 += 2;
					if (x2 - x1 >= snakeTailLen+2)
						x1 += 2;
					break;
				default:
					turnRight(x1,x2,headY1,headY2);
					x2 += 2;
					if (x2 - x1 >= snakeTailLen +2)
						x1 += 2;
					break;
			}

}
void left() {
	switch (prevState) {
			case goUp:
				turnLeft(x1,x2,headY1,headY2);
				if (virticalLen >= 0)
				turnUp(headX1,headX2,y1,y2);
				//deleteDown(headX1,headX2,headY1+snakeTailLen,headY2+snakeTailLen);
				x1 -= 2;
				if (x2 - x1 >= snakeTailLen + 2)
					x2 -= 2;
				break;
			case goDown:
				turnLeft(x1,x2,headY1,headY2);
				if (virticalLen >= 0)
				turnDown(headX1,headX2,y1,y2);
				//deleteDown(headX1,headX2,headY1+snakeTailLen,headY2+snakeTailLen);
				x1 -= 2;
				if (x2 - x1 >= snakeTailLen +2)
					x2 -= 2;
				break;
			case goRight:
				turnLeft(x1,x2,headY1,headY2);
				x1 -= 2;
				if (x2 - x1 >= snakeTailLen + 2)
					x2 -= 2;
				break;
			default:
				turnLeft(x1,x2,headY1,headY2);
				x1 -= 2;
				if (x2 - x1 >= snakeTailLen + 2)
					x2 -= 2;
				break;
		}

}

void up() {
	switch (prevState) {
				case goUp:
					break;
				case goDown:
					break;
				case goRight:
					turnUp(headX1, headX2, y1, y2);
					if (horizonLen >= 0)
						turnRight(x1, x2, headY1, headY2);
					y1 -= 2;
					if (y2 - y1 >= snakeTailLen +2)
						y2 -= 2;
					break;
				default:
					break;
			}

}

void down() {
	switch (prevState) {
			case goUp:

				break;
			case goDown:

				break;
			case goRight:

				break;
			default:

				break;
		}
	x1 = x2 - 10;
		y1 = y2 - snakeTailLen;
		lcd_Fill(x1, y1, x2, y2, WHITE);
		y1 += 2;
		y2 += 2;
		lcd_Fill(x1, y1, x2, y2, BLUE);
}

void eat_food_success() {
	CREATE_FOOD = 1; //QUAY LAI HÀM TẠO FRUIT
	count++; //tăng số điểm
	snakeTailLen += 10;

	lcd_Fill(x_food, y_food, x_food+5, y_food+5, WHITE);
}

void food() {
	if(CREATE_FOOD == 1) { // Tạo mồi nhậu lần đầu tiên
		x_food = random_eat(0,x_max-5);
		y_food = random_eat(0,y_max-5);
		lcd_Fill(x_food, y_food, x_food+5, y_food+5, RED);
		CREATE_FOOD = 0; // tránh tạo liên tục mồi nhậu

	}
	else {
		if((x2 == x_food) && (
				(y1 >= y_food && y1 <= y_food+5)
			||	(y2 >= y_food && y2 <= y_food+5)
			||  (y1 <= y_food && y2 >= y_food+5)
			)
			) {
			eat_food_success();
		}
		if((x1 == x_food+5) && (
				(y1 >= y_food && y1 <= y_food+5)
			||	(y2 >= y_food && y2 <= y_food+5)
			||  (y1 <= y_food && y2 >= y_food+5)
			)
			) {
			eat_food_success();
		}
		if((y2 == y_food) && (
				(x1 >= x_food && x1 <= x_food+5)
			||	(x2 >= x_food && x2 <= x_food+5)
			||  (x1 <= x_food && x2 >= x_food+5)
			)
			) {
			eat_food_success();
		}
		if((y1 == y_food+5) && (
				(x1 >= x_food && x1 <= x_food+5)
			||	(x2 >= x_food && x2 <= x_food+5)
			||  (x1 <= x_food && x2 >= x_food+5)
			)
			) {
			eat_food_success();
		}
	}
}



void move() {
	if(x1 == 0 || x1 == x_max || x2 == 0 || x2 == x_max
	|| y1 == 0 || y1 == y_max || y2 == 0 || y2 == y_max) { //đụng tường
			game_over();
			return;
	}
	led7_SetDigit(0, 0, 0);
	led7_SetDigit(0, 1, 0);
	led7_SetDigit(count/10, 2, 0);
	led7_SetDigit(count%10, 3, 0); //hiển thị điểm số
//	count++;

	food();
	//snake move with button
	if (button_count[6] == 1) {
		prevState = firstState;
		setHead(x1, x2, y1, y2);
		firstState = goUp;
		virticalLen = snakeTailLen;
	}
	else if (button_count[14] == 1) {
		prevState = firstState;
		setHead(x1, x2, y1, y2);
		firstState = goDown;
		virticalLen = snakeTailLen;
	}
	else if (button_count[11] == 1) {
		prevState = firstState;
		setHead(x1, x2, y1, y2);
		firstState = goRight;
		horizonLen = snakeTailLen;
	}
	else if (button_count[9] == 1) {
		prevState = firstState;
		setHead(x1, x2, y1, y2);
		firstState = goLeft;
		horizonLen = snakeTailLen;
	}
	switch (firstState) {
		case goUp:
			up();
			break;
		case goDown:
			down();
			break;
		case goRight:
			right();
			break;
		default:
			left();
			break;
	}

}

void wall() {
	lcd_DrawRectangle(0, 0, x_max, y_max, RED); //TẠO TƯỜNG
}

void start_game() {
	lcd_ShowStr(0,120,"Nhan nut 10 de bat dau",WHITE,RED,24,0);
	if (button_count[10] == 1)
		move();
}


