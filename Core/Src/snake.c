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

uint16_t random_eat(uint16_t minN, uint16_t maxN) {
	return minN + rand()%(maxN + 1 - minN);
}

uint16_t x_food;
uint16_t y_food;


uint16_t count = 0;
//uint16_t pre_count = 0;
uint16_t CREATE_FOOD = 1;

void snake_init() {
	lcd_Fill(x1, y1, x2, y2, BLUE);
}
void display() {
	lcd_ShowStr(50,30,"Game Over!!!",WHITE,BLACK,24,0);
	HAL_Delay(1000);
	lcd_ShowStr(50,30,"Game Over!!!",BLUE,YELLOW,24,0);
	HAL_Delay(1000);
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

void right() {
	lcd_Fill(x1, y1, x2, y2, WHITE);
	x1 += 1;
	x2 += 1;
	lcd_Fill(x1, y1, x2, y2, BLUE);
}

void left() {
	lcd_Fill(x1, y1, x2, y2, WHITE);
	x1 -= 1;
	x2 -= 1;
	lcd_Fill(x1, y1, x2, y2, BLUE);
}

void up() {
	lcd_Fill(x1, y1, x2, y2, WHITE);
	y1 -= 1;
	y2 -= 1;
	lcd_Fill(x1, y1, x2, y2, BLUE);
}

void down() {
	lcd_Fill(x1, y1, x2, y2, WHITE);
	y1 += 1;
	y2 += 1;
	lcd_Fill(x1, y1, x2, y2, BLUE);
}

void eat_food_success() {
	CREATE_FOOD = 1; //QUAY LAI HÀM TẠO FRUIT
	count++; //tăng số điểm
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

enum state { goUp, goDown, goLeft, goRight };
enum state firstState = goUp;

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
		firstState = goUp;
	}
	else if (button_count[14] == 1) {
		firstState = goDown;
	}
	else if (button_count[11] == 1) {
		firstState = goRight;
	}
	else if (button_count[9] == 1) {
		firstState = goLeft;
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


