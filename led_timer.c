/** @file   led_timer.h
    @author Sam Zhang
    @date   8 Oct 2017
    @brief  led timer for game

*/

#include "system.h"
#include "led.h"
#include "led_timer.h"

#define CURRENT_TIME 5000
#define INCREMENT_TIME 250
#define GAME_LENGTH 20


static uint16_t current_time;
static uint16_t buffer;
static uint16_t loop_num;


/**
 * Initializes the timer
 */
void led_timer_init(void)
{
    current_time = CURRENT_TIME;
    buffer = 0;
    loop_num = 0;
    led_init();
}

/**
 * Keeps track of the game time
 * and changes the LED accordingly.
 */
int led_timer (void) 
{    
    if (buffer > current_time) {
        buffer = 0;
        loop_num++;
        current_time -= INCREMENT_TIME;
        led_toggle();
    }
    buffer++;
    if (loop_num > GAME_LENGTH) {
        return 1;
    } else {
        return 0;
    }
}
