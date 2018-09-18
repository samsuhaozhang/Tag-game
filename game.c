/** @file   game.c
    @author Sam Zhang
    @date   8/10/2017
    @brief  Main file for tag game

*/

#include "system.h"
#include "pio.h"
#include "selectMenu.h"
#include "people_start.h"
#include "pacer.h"
#include "tinygl.h"
#include "led_timer.h"
#include "fonts/font5x7_1.h"
#include "led.h"
#include "navswitch.h"
#include "endScreen.h"

#define PACER_RATE 500
#define PLAYER_1 '1'
#define PLAYER_2 '2'
#define PLAYER_0 '0'
#define MESSAGE_RATE 10

/**
 * Main game loop that calls all other functions
 * 
 */
int main (void)
{
    int winner;
    system_init ();
    selectMenu_init();

    pacer_init(PACER_RATE);

    while(1)
    {
        selectMenu_run();
        if (returnPlayerIs() != PLAYER_0)
        {
            break;
        }
    }
    tinygl_clear ();
    led_timer_init();
    people_init();
    
    while (1)
    {
        people_start ();
        if (return_game_time() == 1) {
            winner = 1;
            break;
        } else if (is_game_over () == 1){
            winner = 2;
            break;
        }
    }
    char playerIs = returnPlayerIs();
    display_game_over(playerIs, winner);
    
    return 0;
}

