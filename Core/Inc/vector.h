/*
 * vector.h
 *
 *  Created on: Dec 27, 2023
 *      Author: Thanh Duy
 */

#ifndef INC_VECTOR_H_
#define INC_VECTOR_H_

#include <stdint.h>
#include <stdlib.h>
enum state { goUp, goDown, goLeft, goRight };
typedef struct {
	enum state currentState;
	uint8_t isTail;
    uint32_t length;
    uint32_t x1;
    uint32_t x2;
    uint32_t y1;
    uint32_t y2;

} path;







#endif /* INC_VECTOR_H_ */
