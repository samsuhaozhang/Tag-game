/** @file   startmenu.h
    @author Sam Zhang
    @date   8 Oct 2017
    @brief  Displays the end screen

*/
#include "system.h"
#include "tinygl.h"
#include "pacer.h"
#include "led.h"


#define PACER_RATE 500
#define PLAYER_1 '1'
#define PLAYER_2 '2'
#define PLAYER_0 '0'
#define MESSAGE_RATE 10

/**
 * Displays the game over screen
 * if the game is over
 */
void display_game_over (char playerIs, int winner)
{
    tinygl_init (PACER_RATE);
    //tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    led_off();
    if (playerIs == PLAYER_1 && winner == 1) {
        tinygl_text (" WINNER");
    } else if (playerIs == PLAYER_2 && winner == 2) {
        tinygl_text (" WINNER");
    } else {
        tinygl_text (" LOSER");
    }
    
    while (1)
    {
        pacer_wait();
        tinygl_update ();
    }
}
