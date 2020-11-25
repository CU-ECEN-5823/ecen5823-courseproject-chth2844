/*
 * zxgesturesensor.h
 *
 *  Created on: 14-Nov-2020
 *      Author: chira
 */

#include <stdbool.h>
#include "gpio.h"
#include "i2c.h"
#include "log.h"
#include "em_core.h"
/* ZX Sensor register addresses */
#define ZX_STATUS           0x00
#define ZX_DRE              0x01
#define ZX_DRCFG            0x02
#define ZX_GESTURE          0x04
#define ZX_GSPEED           0x05
#define ZX_DCM              0x06
#define ZX_XPOS             0x08
#define ZX_ZPOS             0x0A
#define ZX_LRNG             0x0C
#define ZX_RRNG             0x0E
#define ZX_REGVER           0xFE
#define ZX_MODEL            0xFF

/* ZX Sensor bit names */
#define DRE_RNG             0
#define DRE_CRD             1
#define DRE_SWP             2
#define DRE_HOVER           3
#define DRE_HVG             4
#define DRE_EDGE            5
#define DRCFG_POLARITY      0
#define DRCFG_EDGE          1
#define DRCFG_FORCE         6
#define DRCFG_EN            7

/* Constants */
#define ZX_ERROR            0xFF
#define MAX_X               240
#define MAX_Z               240
#define SET_ALL_DRE         0b00111111

/* Enumeration for possible gestures */

#define RIGHT_SWIPE   0x01
#define LEFT_SWIPE    0x02
#define UP_SWIPE     0x03
#define NO_GESTURE   0xFF


//Function Prototypes
int interpret_gesture();
bool gesture_available();
void enable_gesture_interrupt();
