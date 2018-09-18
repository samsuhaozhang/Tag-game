/** @file   people-start.h
    @author Sam Zhang
    @date   8 Oct 2017
    @brief  main game function

*/

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "task.h"
#include "ir_uart.h"
#include "pacer.h"
#include "selectMenu.h"
#include "led_timer.h"

#define DISPLAY_TASK_RATE 200

#define IR_UART_TX_TASK_RATE 20

#define IR_UART_RX_TASK_RATE 20

#define NAVSWITCH_TASK_RATE 20

#define GAME_OVER 1

typedef enum {XPOS, YPOS} tx_message_t;

typedef enum {SEND_XPOS, SEND_YPOS} tx_state_t;


static tinygl_point_t this_pos;
static tinygl_point_t other_pos;
static tinygl_point_t centre_point;
int game_over = 0;
int game_time = 0;

/**
 * updates the tinygl
 */
static void display_task (void)
{
    tinygl_update ();
}

/**
 * sends the x and y position messages to another fun kit
 */
static void ir_uart_tx_task (void)
{
    static uint8_t state = SEND_XPOS;

    /* Alternately, send x position and y position messages.  */
    switch (state)
    {
    case SEND_XPOS:
        ir_uart_putc ((XPOS << 4) | (this_pos.x & 0x0f));
        state = SEND_YPOS;
        break;

    case SEND_YPOS:
        ir_uart_putc ((YPOS << 4) | (this_pos.y & 0x0f));
        state = SEND_XPOS;
        break;
    }
}

/**
 * revieves and decodes the x and y positions from another fun kit
 */
static void ir_uart_rx_task (void)
{
    uint8_t c;

    if (!ir_uart_read_ready_p ())    
        return;

    tinygl_draw_point (other_pos, 0);

    c = ir_uart_getc ();

    /* Decode the received message.  */
    switch (c >> 4)
    {
    case XPOS:
        other_pos.x = c & 0x0f;
        break;

    case YPOS:
        other_pos.y = c & 0x0f;
        break;
    }
    
    tinygl_draw_point (other_pos, 1);
}

/**
 * moves the players LED when the navswitch is pressed
 * Checks to make sure they are not moving off screen
 */
static void navswitch_task (void)
{
    tinygl_point_t newpos;

    navswitch_update ();
    
    newpos = this_pos;

    if (navswitch_push_event_p (NAVSWITCH_NORTH) && newpos.y - 1 > centre_point.y - 4) {
        newpos.y--;
        
    }
    if (navswitch_push_event_p (NAVSWITCH_SOUTH) && newpos.y + 1 < centre_point.y + 4) {
        newpos.y++;
    }
    if (navswitch_push_event_p (NAVSWITCH_EAST) && newpos.x + 1 < centre_point.x + 3) {
        newpos.x++;
    }
    if (navswitch_push_event_p (NAVSWITCH_WEST) && newpos.x - 1 > centre_point.x - 3) {
        newpos.x--;
    }
    /* Note, there is no checking that have moved off screen.  */

    if (this_pos.x != newpos.x || this_pos.y != newpos.y)
    {
        tinygl_draw_point (this_pos, 0);
        this_pos = newpos;
        tinygl_draw_point (this_pos, 1);
        ir_uart_tx_task ();
        ir_uart_tx_task ();
    }
}

/**
 * Initialises the board with the two players
 */
void people_init (void)
{
    system_init ();
    tinygl_init (DISPLAY_TASK_RATE);
    ir_uart_init ();
    if (returnPlayerIs () == '1'){
        this_pos = tinygl_point (3/TINYGL_WIDTH, 2/TINYGL_HEIGHT);
        other_pos = tinygl_point (20/TINYGL_WIDTH, 42/TINYGL_HEIGHT);
    } else {
        this_pos = tinygl_point (20/TINYGL_WIDTH, 42/TINYGL_HEIGHT);
        other_pos = tinygl_point (3/TINYGL_WIDTH, 2/TINYGL_HEIGHT);
    }
}

/**
 * Checks whether the two LEDs are on the same coordinate.
 */
static void position_task (void)
{
    if (this_pos.x == other_pos.x && this_pos.y == other_pos.y){
        game_over = GAME_OVER;
    } 
}

/**
 * Returns the game over variable
 */
int is_game_over (void)
{
    return game_over;
}

/**
 * Returns the game time variable
 */
int return_game_time (void)
{
    return game_time;
}

/**
 * Runs the main game and calls the functions
 * above
 */
void people_start (void)
{   
    
    centre_point = tinygl_point (TINYGL_WIDTH/2, TINYGL_HEIGHT/2);
    tinygl_draw_point (this_pos, 1);
    
    while(1)
    {
        display_task();
        ir_uart_rx_task();
        navswitch_task();
        ir_uart_rx_task();
        game_time = led_timer ();
        position_task ();
        if (game_over == GAME_OVER) {
            break;
        } else if (game_time == GAME_OVER){
            break;
        }
    }


}
