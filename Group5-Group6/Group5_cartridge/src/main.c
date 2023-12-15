#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"
#include "display.h"
#define SMALL_SPRITE_CTRL_OFFSET 16
#define SMALL_SPRITE_SIZE 0x3
#define OFFSET_WIDTH 4
volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile int REFRESH_COUNT = 15000;
volatile int REFRESH_PAUSE_FREEZE = 200000;
volatile int ONE_SECOND = 20;
// volatile char *VIDEO_MEMORY = (volatile char *)(0x500F4800);
// volatile uint32_t *SMALL_SPRITE_CONTROL = (volatile uint32_t *)(0x500E0000);

// thread implementation declaration
typedef uint32_t *TContext;
typedef void (*TEntry)(void *);
TContext InitContext(uint32_t *stacktop, TEntry entry, void *param);
void SwitchContext(TContext *oldcontext, TContext newcontext);
uint32_t ThreadStack[128];
TContext Mainthread;
TContext Otherthread;

int main()
{
    int last_global = 42;
    int x_pos = 0x8C0 / 2;
    uint32_t current_status = 0;
    uint32_t last_status = 0;
    int time_limit = 30; // sec
    int score = 0;
    int width = 0x8;
    int offset[2] = {0,0};
    switchToGraphicsMode();     // api call to switch between graphic and text mode - refer documentation
    setColor(0, 0, 0xFFffb4a8); // api call to change sprite color - refer documentation
    setColor(1, 0, 0xFF3945b0);
    setColor(2, 0, 0xFF2BA7A3);
    setColor(3, 0, 0xFF8B0000);
    setBackgroundColor(0, 0, 0xFFF5DEB3); // api call to change background color - refer documentation
    drawRectangleWithSmallSprite(100, generateSmallSpriteConfig(0, 0, 0, 0, 0));
    drawRectangleWithSmallSprite(1, generateSmallSpriteConfig(0, 0, 0x4, 0x4, 1)); // target 1
    drawRectangleWithSmallSprite(2, generateSmallSpriteConfig(0, 0, 0x4, 0x4, 2)); // target 2
    drawRectangleWithSmallSprite(10, generateSmallSpriteConfig(0, 0, 0, 0, 3));
    drawRectangleWithSmallSprite(11, generateSmallSpriteConfig(0, 0, 0, 0, 3));
    drawRectangleWithSmallSprite(12, generateSmallSpriteConfig(0, 0, 0, 0, 3));
    drawRectangleWithSmallSprite(13, generateSmallSpriteConfig(0, 0, 0, 0, 3));
    drawRectangleWithBackgroundSpriteControl(0, generateBackgroundConfig(0, 0, 0, 0)); // api call to draw a sprite

    int CMD_INTRR = 0;
    int CURRENT_CMD_INTRR;
    int countdown = 1;
    int PAUSE_FREEZE = 1;
    int target_1_flag = 1;
    int target_2_flag = 1;
    int PAUSE_flag = 1; // flag for being able to pause
    int xt_1_pos = 0x0;
    int xt_2_pos = 0x0;
    int time_start = getTicks(); // start timer
    int current_time = getTicks();
    int pause_start = 0;
    int pause_time_cum = 0;
    int last_pause_time_cum = 0;
    while ((current_time - time_start - pause_time_cum) / ONE_SECOND <= time_limit)
    {

        // A flag for target point to decide whether draw a new one
        if (target_1_flag)
        {
            // draw a new target
            xt_1_pos = genRandom(0x8C0);
            while ((xt_1_pos & 0x3F) > 0x3E)
            {
                xt_1_pos = genRandom(0x8C0);
            }

            moveSmallSprite(1, (xt_1_pos & 0x3F) << 3, (xt_1_pos >> 6) << 3);
            // reset flag
            target_1_flag = 0;
        }
        if (target_2_flag)
        {
            // draw a new target
            xt_2_pos = genRandom(0x8C0);
            while (((xt_2_pos & 0x3F) > 0x3E) || (xt_2_pos < 0) || xt_2_pos == xt_1_pos)
            {
                xt_2_pos = genRandom(0x8C0);
            }

            moveSmallSprite(2, (xt_2_pos & 0x3F) << 3, (xt_2_pos >> 6) << 3);
            // reset flag
            target_2_flag = 0;
        }

        if (global != last_global)
        {
            if (!PAUSE_flag)
            {
                // GAME Paused
                pause_time_cum = last_pause_time_cum + getTicks() - pause_start;
            }
            if (!PAUSE_FREEZE)
            {
                CURRENT_CMD_INTRR = getCMDInterruptCount();

                if (CURRENT_CMD_INTRR != CMD_INTRR)
                {
                    // if in graphiscs mode when pressed CMD: Pause game
                    if (PAUSE_flag)
                    {
                        pause_start = current_time;
                        switchToTextMode();
                        char periodStr[100]; //
                        int time_rem = time_limit - (current_time - time_start - pause_time_cum) / ONE_SECOND;
                        snprintf(periodStr, sizeof(periodStr), "----- Game Paused ----- Time remain: %d sec ----- Points: %d ----  Press CMD to resume", time_rem, score);
                        printText(periodStr);
                    }
                    // if in text mode when pressed CMD: resume Game
                    else
                    {
                        last_pause_time_cum = pause_time_cum;
                        switchToGraphicsMode();
                    }
                    CMD_INTRR = CURRENT_CMD_INTRR;
                    PAUSE_flag = 1 - PAUSE_flag;
                    PAUSE_FREEZE = REFRESH_PAUSE_FREEZE;
                }
            }

            controller_status = getStatus();
            if (controller_status == 0x0)
            {
                current_status = last_status;
            }
            else
            {
                current_status = controller_status;
            }

            if (controller_status & 0x1)
            { // move left
                if (x_pos & 0x3F)
                { // 0x3f = 0011 1111
                    x_pos--;
                    offset[0] = -OFFSET_WIDTH;
                }
            }
            if (controller_status & 0x2)
            { // move up
                if (x_pos >= 0x40)
                { // 0x40 = 0100 0000
                    x_pos -= 0x40;
                    offset[1] = -OFFSET_WIDTH;
                }
            }
            if (controller_status & 0x4)
            { // move down
                if (x_pos < 0x880)
                { // 0x880 = 1000 1000 0000
                    x_pos += 0x40;
                    offset[1] = OFFSET_WIDTH;
                }
            }
            if (controller_status & 0x8)
            { // move right
                if ((x_pos & 0x3F) < 0x3E)
                { // 0x3f = 0011 1111
                    x_pos++;
                    offset[0] = OFFSET_WIDTH;
                }
            }
            
            moveSmallSprite(100, ((x_pos & 0x3F) << 3) + offset[0], ((x_pos >> 6) << 3) + offset[1]);
            moveSmallSprite(10, ((x_pos & 0x3F) << 3) - width, ((x_pos >> 6) << 3)-width);
            moveSmallSprite(11, ((x_pos & 0x3F) << 3) - width, ((x_pos >> 6) << 3)+width);
            moveSmallSprite(12, ((x_pos & 0x3F) << 3) + width, ((x_pos >> 6) << 3)-width);
            moveSmallSprite(13, ((x_pos & 0x3F) << 3) + width, ((x_pos >> 6) << 3)+width);
            offset[0] = 0;
            offset[1] = 0;
            // decide if hit the target
            if (((x_pos & 0x3F) < ((xt_1_pos & 0x3F) + SMALL_SPRITE_SIZE)) && (((x_pos & 0x3F) + SMALL_SPRITE_SIZE) > (xt_1_pos & 0x3F)) && ((x_pos >> 6) < ((xt_1_pos >> 6) + SMALL_SPRITE_SIZE)) && (((x_pos >> 6) + SMALL_SPRITE_SIZE) > (xt_1_pos >> 6)))
            {
                target_1_flag = 1;
                score += 1;
            }
            if (((x_pos & 0x3F) < ((xt_2_pos & 0x3F) + SMALL_SPRITE_SIZE)) && (((x_pos & 0x3F) + SMALL_SPRITE_SIZE) > (xt_2_pos & 0x3F)) && ((x_pos >> 6) < ((xt_2_pos >> 6) + SMALL_SPRITE_SIZE)) && (((x_pos >> 6) + SMALL_SPRITE_SIZE) > (xt_2_pos >> 6)))
            {
                target_2_flag = 1;
                score += 1;
            }
            // refresh global
            last_global = global;
        }
        countdown--;
        if (!countdown)
        {
            global++;
            controller_status = getStatus(); // SystemCall(CONTROLLER_PARAMS);
            countdown = REFRESH_COUNT;

            /*Display the video clock period*/
            // display_video_clock_period();
        }
        if (PAUSE_FREEZE)
        {
            PAUSE_FREEZE--;
        }
        current_time = getTicks();
    }

    switchToTextMode();

    time_start = getTicks();
    current_time = getTicks() - time_start;
    int last_time_rem = 0;
    int time_rem = 0;
    while (current_time < (10 * ONE_SECOND))
    {

        time_rem = 10 - (current_time / ONE_SECOND);

        if (time_rem != last_time_rem)
        {
            char periodStr[100];
            snprintf(periodStr, sizeof(periodStr), "GAME OVER!! YOU GOT %d POINTS!! Hit RST or Wait %d s to Restart", score, time_rem);
            printText(periodStr);
            last_time_rem = time_rem;
        }
        current_time = getTicks() - time_start;
    }
    return 0;
}