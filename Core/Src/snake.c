/*
 * snake.c
 *
 *  Created on: Dec 12, 2023
 *      Author: QUI
 */

#include "snake.h"
#include <stdlib.h>
#include "led_7seg.h"
#include "vector.h"
uint8_t currentIndex = 0;
path allPath[30];
int16_t x_max = 234, y_max = 315;
uint8_t flag = 0;
uint8_t flag1 = 0;
uint8_t flag2 = 0;
int16_t x1 = 180, y1 = 110, x2 = 190, y2 = 120;
enum state firstState = goDown;
enum state prevState = goRight;
uint16_t random_eat(uint16_t minN, uint16_t maxN) {
	return minN + rand()%(maxN + 1 - minN);
}
uint16_t x_food;
uint16_t y_food;
uint16_t init_len = 10;
uint16_t snakeTailLen = 10;
uint16_t speed = 2;
uint16_t count = 0;
uint16_t CREATE_FOOD = 1;
obstacle obstacles[9] = {
        {55, 75, 65, 150},    // Wall 1
        {55, 90, 90, 100},    // Wall 2
        {70, 200, 125, 210},  // Wall 3
        {150, 55, 200, 65},    // Wall 4
        {150, 55, 160, 95},   // Wall 5
        {190, 200, 200, 260},    // Wall 6
        {130, 250, 200, 260},
		{90, 150, 140, 160},
		{115, 150, 125, 200}
    };
void creatWall(int32_t X1, int32_t Y1, int32_t X2, int32_t Y2) {
    // Replace this with the appropriate function in your graphics library
    lcd_Fill(X1, Y1, X2, Y2, MAGENTA);
}
void snake_init() {
	lcd_Fill(x1, y1, x2, y2, BLUE);
	 //lcd_Fill(55, 75, 56, 150, RED);
	for (int8_t i = 0; i < 9; ++i) {
	       creatWall(obstacles[i].x1, obstacles[i].y1, obstacles[i].x2, obstacles[i].y2);
	    }
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
	lcd_ShowStr(25,120,"NGU!",WHITE,RED,24,0);
	lcd_ShowStr(10,200,"DIEM SO CUA BAN LA: ",BLUE,WHITE,24,0);
	if(count < 10)
		lcd_ShowIntNum(210,200,count,1,BLUE,WHITE,24);
	else
		lcd_ShowIntNum(210,200,count,2,BLUE,WHITE,24);
	lcd_Fill(x1, y1, x2, y2, WHITE);
	lcd_Fill(x_food, y_food, x_food+5, y_food+5, WHITE);
	display();
}
void moveWall() {
	if (flag1 == 0) {
		lcd_Fill(obstacles[7].x2-2, obstacles[7].y1, obstacles[7].x2, obstacles[7].y2, WHITE);
		obstacles[7].x1-=2;
		obstacles[7].x2-=2;
		lcd_Fill(obstacles[7].x1, obstacles[7].y1, obstacles[7].x1+2, obstacles[7].y2, MAGENTA);
		if (obstacles[7].x1 <= 10) flag1 = 1;
	} else {
		lcd_Fill(obstacles[7].x1, obstacles[7].y1, obstacles[7].x1+2, obstacles[7].y2, WHITE);
		obstacles[7].x1+=2;
		obstacles[7].x2+=2;
		lcd_Fill(obstacles[7].x2-2, obstacles[7].y1, obstacles[7].x2, obstacles[7].y2, MAGENTA);
		if (obstacles[7].x2 >= 230) flag1 = 0;
	}

	if (flag2 == 0) {
		lcd_Fill(obstacles[8].x1, obstacles[8].y2-2, obstacles[8].x2, obstacles[8].y2, WHITE);
		obstacles[8].y1-=2;
		obstacles[8].y2-=2;
		lcd_Fill(obstacles[8].x1, obstacles[8].y1, obstacles[8].x2, obstacles[8].y1+2, MAGENTA);
		if (obstacles[8].y1 <= 10) flag2 = 1;
	} else {
		lcd_Fill(obstacles[8].x1, obstacles[8].y1, obstacles[8].x2, obstacles[8].y1+2, WHITE);
		obstacles[8].y1+=2;
		obstacles[8].y2+=2;
		lcd_Fill(obstacles[8].x1, obstacles[8].y2-2, obstacles[8].x2, obstacles[8].y2, MAGENTA);
		if (obstacles[8].y2 >= 310) flag2 = 0;
	}

}
void checkCollision () {
	for (int8_t i = 0; i < 9; ++i) {
		if (x1 >= obstacles[i].x1 && x1 <= obstacles[i].x2) {
			if (y1 >= obstacles[i].y1 && y1 <= obstacles[i].y2) {
				game_over();
				return;
			} else if (y2 >= obstacles[i].y1 && y2 <= obstacles[i].y2) {
				game_over();
				return;
			}
		} else if (x2 >= obstacles[i].x1 && x2 <= obstacles[i].x2) {
			if (y1 >= obstacles[i].y1 && y1 <= obstacles[i].y2) {
				game_over();
				return;
			} else if (y2 >= obstacles[i].y1 && y2 <= obstacles[i].y2) {
				game_over();
				return;
			}
		}
	}
}
void delete_path() {
    // Assuming lcd_Fill is a function to clear the specified area on the LCD with white color
    	if (allPath[0].isTail == 0) {
    		//if (allPath[0].currentState == goRight) lcd_Fill(x1,0, x2, y2, RED);
    		if (allPath[0].length >= snakeTailLen) {
    			if (allPath[0].currentState == goUp) {
					lcd_Fill(allPath[0].x1, allPath[0].y2-speed, allPath[0].x2, allPath[0].y2, WHITE);
					allPath[0].y2 -= speed;
				} else if (allPath[0].currentState == goDown) {
					lcd_Fill(allPath[0].x1, allPath[0].y1, allPath[0].x2, allPath[0].y1 + speed, WHITE);
					allPath[0].y1 += speed;
				} else if (allPath[0].currentState == goLeft) {
					lcd_Fill(allPath[0].x2-speed, allPath[0].y1, allPath[0].x2, allPath[0].y2, WHITE);
					allPath[0].x2 -= speed;
				} else if (allPath[0].currentState == goRight) {
					lcd_Fill(allPath[0].x1, allPath[0].y1, allPath[0].x1+speed, allPath[0].y2, WHITE);
					allPath[0].x1 += speed;
				}

    		}
    	} else if (allPath[0].isTail == 1) {
    		allPath[0].length -= speed;
    		if (allPath[0].currentState == goUp) {
				lcd_Fill(allPath[0].x1, allPath[0].y2-speed, allPath[0].x2, allPath[0].y2, WHITE);
				allPath[0].y2 -= speed;
			} else if (allPath[0].currentState == goDown) {
				lcd_Fill(allPath[0].x1, allPath[0].y1, allPath[0].x2, allPath[0].y1 + speed, WHITE);
				allPath[0].y1 += speed;
			} else if (allPath[0].currentState == goLeft) {
				lcd_Fill(allPath[0].x2-speed, allPath[0].y1, allPath[0].x2, allPath[0].y2, WHITE);
				allPath[0].x2 -= speed;
			} else if (allPath[0].currentState == goRight) {
				lcd_Fill(allPath[0].x1, allPath[0].y1, allPath[0].x1+speed, allPath[0].y2, WHITE);
				allPath[0].x1 += speed;
			}
    		if (flag == 0) {
				if (allPath[0].length == 0 || allPath[0].length < 0) {
						flag = 1;
						delete_path();
						flag = 0;
					for (uint8_t i = 0; i < currentIndex - 1; i++) {
						allPath[i] = allPath[i + 1];
					}
					currentIndex--;
				}
    		}



    	}

}
void right() {

	if (prevState == goRight) {
				if (allPath[currentIndex-1].length <= snakeTailLen) {
					allPath[currentIndex-1].length +=speed;
				}
				allPath[currentIndex-1].x2 = x2;
			} else {
				if (currentIndex == 0) allPath[0].isTail = 0;
				else allPath[currentIndex-1].isTail = 1;
				allPath[currentIndex].isTail = 0;
				if (snakeTailLen == init_len) allPath[currentIndex].length = snakeTailLen;
				else allPath[currentIndex].length = 0;
				allPath[currentIndex].currentState = goRight;
				allPath[currentIndex].x1 = x1;
				allPath[currentIndex].x2 = x2;
				allPath[currentIndex].y1 = y1;
				allPath[currentIndex].y2 = y2;

				currentIndex++;
			}
			prevState = goRight;
			//if (snakeTailLen - allPath[currentIndex-1].length == init_len) allPath[currentIndex-1].length+=speed;
			x1 += speed;
			x2 += speed;


			lcd_Fill(x1, y1, x2, y2, BLUE);
			delete_path();
}
void left() {
	if (prevState == goLeft) {
		if (allPath[currentIndex-1].length <= snakeTailLen) {

							allPath[currentIndex-1].length +=speed;
						}

			allPath[currentIndex-1].x1 = x1;

		} else {
			if (currentIndex == 0) allPath[0].isTail = 0;
			else allPath[currentIndex-1].isTail = 1;
			allPath[currentIndex].isTail = 0;
			if (snakeTailLen == init_len) allPath[currentIndex].length = snakeTailLen;
			else allPath[currentIndex].length = 0;
			allPath[currentIndex].currentState = goLeft;
			allPath[currentIndex].x1 = x1;
			allPath[currentIndex].x2 = x2;
			allPath[currentIndex].y1 = y1;
			allPath[currentIndex].y2 = y2;
			currentIndex++;
		}
		prevState = goLeft;
		//if (snakeTailLen - allPath[currentIndex-1].length == init_len) allPath[currentIndex-1].length+=speed;
		x1 -= speed;
		x2 -= speed;


		lcd_Fill(x1, y1, x2, y2, BLUE);
		delete_path();

}


void up() {

	if (prevState == goUp) {
		if (allPath[currentIndex-1].length <= snakeTailLen) {

					allPath[currentIndex-1].length += speed;
				}

		allPath[currentIndex-1].y1 -= y1 ;


	} else {
		if (currentIndex == 0) allPath[0].isTail = 0;
		else allPath[currentIndex-1].isTail = 1;
		allPath[currentIndex].isTail = 0;
		if (snakeTailLen == init_len) allPath[currentIndex].length = snakeTailLen;
		else allPath[currentIndex].length = 0;
		allPath[currentIndex].currentState = goUp;
		allPath[currentIndex].x1 = x1;
		allPath[currentIndex].x2 = x2;
		allPath[currentIndex].y1 = y1;
		allPath[currentIndex].y2 = y2;
		currentIndex++;
	}
	prevState = goUp;
	//if (snakeTailLen - allPath[currentIndex-1].length == init_len) allPath[currentIndex-1].length+=speed;

	y1 -= speed;
	y2 -= speed;


	lcd_Fill(x1, y1, x2, y2, BLUE);
	delete_path();
}


void down() {

	if (prevState == goDown) {
		if (allPath[currentIndex-1].length <= snakeTailLen) {

			allPath[currentIndex-1].length += speed;
		}

		allPath[currentIndex-1].y2 = y2;
	} else {
		if (currentIndex == 0) allPath[0].isTail = 0;
		else allPath[currentIndex-1].isTail = 1;
		allPath[currentIndex].isTail = 0;
		if (snakeTailLen == init_len) allPath[currentIndex].length = snakeTailLen;
		else allPath[currentIndex].length = 0;
		allPath[currentIndex].currentState = goDown;
		allPath[currentIndex].x1 = x1;
		allPath[currentIndex].x2 = x2;
		allPath[currentIndex].y1 = y1;
		allPath[currentIndex].y2 = y2;
		currentIndex++;
	}
	prevState = goDown;
	//if (snakeTailLen - allPath[currentIndex-1].length == init_len) allPath[currentIndex-1].length+=speed;
		y1 += speed;
		y2 += speed;

		lcd_Fill(x1, y1, x2, y2, BLUE);
		delete_path();
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
	if(x1 <= 5 || x1 >= x_max || x2 <= 5 || x2 >= x_max
	|| y1 <= 0 || y1 >= y_max || y2 <= 0 || y2 >= y_max) { //đụng tường
			game_over();
			return;
	}
	led7_SetDigit(0, 0, 0);
	led7_SetDigit(0, 1, 0);
	led7_SetDigit(count/10, 2, 0);
	led7_SetDigit(count%10, 3, 0); //hiển thị điểm số
//	count++;

	food();
	moveWall();
	checkCollision();
	//snake move with button
	if (button_count[6] == 1) {
		if (firstState != goDown) {
			prevState = firstState;
			firstState = goUp;
		}

	}
	else if (button_count[14] == 1) {
		if (firstState != goUp) {
			prevState = firstState;
			firstState = goDown;
		}

	}
	else if (button_count[11] == 1) {
		if (firstState != goLeft) {
			prevState = firstState;
			firstState = goRight;
		}

	}
	else if (button_count[9] == 1) {
		if (firstState != goRight) {
			prevState = firstState;
			firstState = goLeft;
		}

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
	lcd_DrawRectangle(5, 5, x_max, y_max, RED); //TẠO TƯỜNG
}

void start_game() {
	lcd_ShowStr(0,120,"Nhan nut 10 de bat dau",WHITE,RED,24,0);
	if (button_count[10] == 1)
		move();
}


