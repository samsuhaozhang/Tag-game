/** @file   startmenu.h
    @author Sam Zhang
    @date   8 Oct 2017
    @brief  game role people remote

*/

#include "navswitch.h"
#include "ir_uart.h"
#include "selectMenu.h"
#include "tinygl.h"
#include "system.h"
#include "fonts/font5x7_1.h"
#include "utils/pacer.h"

#define PACER_RATE 100
#define MESSAGE_RATE 10
#define PLAYER_1 '1'
#define PLAYER_2 '2'
#define PLAYER_0 '0'


static char current_option = PLAYER_1;
static char player_selected = PLAYER_0;
static char player_is = PLAYER_0;

/**
 * displays a character using the tinygl module.
 */
void display_character (char character)
{
    char buffer[2];

    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

/**
 * Sends the selected character to
 * the other fun kit
 */
void start_sender (void)
{
    if (navswitch_down_p (NAVSWITCH_EAST)) {
        current_option = PLAYER_2;
        display_character (current_option);
    } if (navswitch_down_p (NAVSWITCH_WEST)) {
        current_option = PLAYER_1;
        display_character (current_option);
    } else if (navswitch_down_p(NAVSWITCH_PUSH)) {
        ir_uart_putc(current_option);
        player_is = current_option;
    }
    tinygl_update();
    navswitch_update();
}

/**
 * Receives a character from a
 * fun kit.
 */
void start_receiver (void)
{
    if (ir_uart_read_ready_p ())
    {
        player_selected = ir_uart_getc ();
        if (player_selected == PLAYER_1){
            player_is = PLAYER_2;
        } else {
            player_is = PLAYER_1;
        }
    }
}

/**
 * initializes the select menu
 */
void selectMenu_init (void)
{
    ir_uart_init();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    display_character (current_option);
}

/**
 * getter for the player_is variable
 */
char returnPlayerIs (void)
{
    return player_is;
}

/**
 * calls update navswitch, start sender, start receiver
 * and tinygl update functions which runs the select menu
 */
void selectMenu_run(void) 
{
    navswitch_update ();
    start_sender ();
    start_receiver ();
    tinygl_update ();
}
