
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "api.h"

#define PLAYER_1_SPRITE_DATA_IDX 1
#define PLAYER_1_SPRITE_PALETTE_IDX 1
#define PLAYER_1_SPRITE_CONTROL_IDX 1

#define TARGET_SPRITE_DATA_IDX 3
#define TARGET_SPRITE_PALETTE_IDX 3
#define TARGET_SPRITE_CONTROL_IDX 3

#define BG_CONTROL_IDX 0
#define BG_PIXEL_DATA_IDX 0
#define BG_PALETTE_IDX 0
#define BG_IMAGE_IDX 0

#define COL_MAGIC_LIMIT 460
#define ROW_MAGIC_LIMIT 230
#define MEDIUM_MAGIC_SIZE 30
#define LARGE_MAGIC_SIZE 60

// #define CONTROLLER (*((volatile uint32_t *)0x40000018)) 

// Define colors
uint32_t RED = 0xFFFF0000;
uint32_t BLUE = 0xFF3377FF;
uint32_t GREEN = 0xFF33DD33;
uint32_t DARK_GREEN = 0xFF006600;
uint32_t WHITE = 0xFFFFFFFF;
uint32_t YELLOW = 0xFFFFFF00;
uint32_t ORANGE = 0xFFFF7F50;
uint32_t DEEP_PINK = 0xFFFF1493;
uint32_t LIGHT_PINK = 0xFFFFB6C1;
uint32_t HOT_PINK = 0xFFFF69B4;


typedef struct POS_TYPE {
  int16_t x, y, z;
} POS_TYPE;

volatile int global = 42;
volatile uint32_t controller_status = 0;
volatile int count = 0;
volatile int REFRESH_COUNT = 10000;

#define MODE_CONTROL (*((volatile uint32_t *)0x500F6780)) // mode control register
#define TEXT_MODE 0x0
#define GRAPHICS_MODE 0x1
volatile char *VIDEO_MEMORY = (volatile char *)(0x50000000 + 0xF4800);
volatile char *TEXT_DATA = (volatile char *)(0x500F4800);

// Function prototypes
void set_player_sprite_1(POS_TYPE *pos);
void set_target_sprite_1(POS_TYPE *pos);
void set_target_sprite_2(POS_TYPE *pos);
void set_background();
void control_movement_1(POS_TYPE *player_pos);
// uint32_t display_time_remain(uint32_t start_time);
void wait_for_any_key_input();
// void wait_for_i_key_input();
void show_start_screen();
uint32_t game_loop();
void show_end_screen(uint32_t score);
uint32_t score_update_1(uint32_t score, const POS_TYPE *player_pos, POS_TYPE *target_pos1, POS_TYPE *target_pos2);
uint32_t my_rand(int range);
uint32_t my_rand(int range);
void custom_delay(int milliseconds);

int main() {
  show_start_screen();
  wait_for_any_key_input();
  set_background();
  while (1) {
    uint32_t score = 0;
    score = game_loop();
    show_end_screen(score);
    custom_delay(1000);
    wait_for_any_key_input();
    // wait_for_i_key_input();
  }
  return 0;
}

uint32_t game_loop() {
  int last_global = 42;
  int countdown = 1;
  int32_t score = 0;
  uint32_t time_limit =  30000;
  POS_TYPE player_1_pos = {.x = 0, .y = 0, .z = 0};
  POS_TYPE target_pos1 = {.x = my_rand(COL_MAGIC_LIMIT), .y = my_rand(ROW_MAGIC_LIMIT), .z = 3};
  POS_TYPE target_pos2 = {.x = my_rand(COL_MAGIC_LIMIT-2), .y = my_rand(ROW_MAGIC_LIMIT-2), .z = 3};

  set_player_sprite_1(&player_1_pos);
  set_target_sprite_1(&target_pos1);
  set_target_sprite_2(&target_pos2);

  uint32_t start_time = getTimeStart();

  createLargeSprite(SQUARE);
  createLargeSprite(SQUARE);
  createLargeSprite(SQUARE);
  createLargeSprite(SQUARE);
  createMediumSprite(SQUARE);
  createMediumSprite(SQUARE);
  createMediumSprite(SQUARE);
  createMediumSprite(SQUARE);
  createSmallSprite(SQUARE);
  createSmallSprite(SQUARE);
  createSmallSprite(SQUARE);
  createSmallSprite(SQUARE);

  while (getTimeEnd(start_time) < time_limit)
  {
    controller_status = getController();
    if (global != last_global) {
      if (controller_status)
      {
        control_movement_1(&player_1_pos);
        set_player_sprite_1(&player_1_pos);
        score = score_update_1(score, &player_1_pos, &target_pos1, &target_pos2);
      }
      last_global = global;
    }
    countdown--;
    if (!countdown)
    {
      global++;
      controller_status = getController();
      countdown = REFRESH_COUNT;
    }

  }

  return score;
}

uint32_t score_update_1(uint32_t score, const POS_TYPE *player_pos, POS_TYPE *target_pos1, POS_TYPE *target_pos2) {
  // Collision detection remains the same
  if ((player_pos->x + LARGE_MAGIC_SIZE >= target_pos1->x) && (player_pos->x <= target_pos1->x + LARGE_MAGIC_SIZE) &&
      (player_pos->y + LARGE_MAGIC_SIZE >= target_pos1->y) && (player_pos->y <= target_pos1->y + LARGE_MAGIC_SIZE))
  {
    // Update score and position of target point if eaten
    score++;
    target_pos1->x = my_rand(COL_MAGIC_LIMIT);
    target_pos1->y = my_rand(ROW_MAGIC_LIMIT);
    set_target_sprite_1(target_pos1); // Draw new target
  }
  if ((player_pos->x + LARGE_MAGIC_SIZE >= target_pos2->x) && (player_pos->x <= target_pos2->x + LARGE_MAGIC_SIZE) &&
      (player_pos->y + LARGE_MAGIC_SIZE >= target_pos2->y) && (player_pos->y <= target_pos2->y + LARGE_MAGIC_SIZE))
  {
    /* Update score and position of target point if eaten */
    score++;
    target_pos2->x = my_rand(COL_MAGIC_LIMIT-2);
    target_pos2->y = my_rand(ROW_MAGIC_LIMIT-2);
    set_target_sprite_2(target_pos2); // Draw new target
  }
  return score;
}

uint32_t my_rand(int range) {
  uint32_t next = getTimeStart();
  return next % range;
}

void control_movement_1(POS_TYPE *player_pos) {
  controller_status = getController();
  // Movement remains the same
  if (controller_status & 0x1) { // 'a' -> LEFT'
    player_pos->x -= LARGE_MAGIC_SIZE / 30;
    if (player_pos->x <= 0) {
      player_pos->x = COL_MAGIC_LIMIT;
    }
  }
  if (controller_status & 0x2) { // 'w' -> UP
    player_pos->y -= LARGE_MAGIC_SIZE / 30;
    if (player_pos->y <= 0) {
      player_pos->y = ROW_MAGIC_LIMIT;
    }
  }
  if (controller_status & 0x4) { // 'x' -> DOWN
    player_pos->y += LARGE_MAGIC_SIZE / 30;
    if (player_pos->y >= ROW_MAGIC_LIMIT) {
      player_pos->y = 0;
    }
  }
  if (controller_status & 0x8) { // d' -> RIGHT
    player_pos->x += LARGE_MAGIC_SIZE / 30;
    if (player_pos->x >= COL_MAGIC_LIMIT) {
      player_pos->x = 0;
    }
  }
}

void set_player_sprite_1(POS_TYPE *pos) {
  enableGraphicsMode();
  // createMediumSprite(SQUARE);
  setLargePalette(PLAYER_1_SPRITE_PALETTE_IDX, WHITE);
  setLargeControl(PLAYER_1_SPRITE_DATA_IDX, PLAYER_1_SPRITE_PALETTE_IDX, pos->x, pos->y, pos->z);
}

void set_target_sprite_1(POS_TYPE *pos) {
  enableGraphicsMode();
  // createMediumSprite(SQUARE);
  setMediumPalette(TARGET_SPRITE_PALETTE_IDX, HOT_PINK);
  setMediumControl(TARGET_SPRITE_DATA_IDX, TARGET_SPRITE_PALETTE_IDX, pos->x, pos->y, pos->z);
}

void set_target_sprite_2(POS_TYPE *pos) {
  enableGraphicsMode();
  // createMediumSprite(SQUARE);
  setSmallPalette(TARGET_SPRITE_PALETTE_IDX, YELLOW);
  setSmallControl(TARGET_SPRITE_DATA_IDX, TARGET_SPRITE_PALETTE_IDX, pos->x, pos->y, pos->z);
}

void set_background() {
  enableGraphicsMode();
  createBackground(RECTANGLE);
  setBackgroundPalette(0, LIGHT_PINK);
  setBackgroundControl(0, 0, 0, 0, 0);
}

void show_start_screen() {
  enableTextMode();
  display_text(" Welcome! Game rule: try to eat as many pink squares and yellow squares as possible in 20 secs. Move with AXWD. Good luck : ) Press ANY of AXWD to continue ", -1);
}

void show_end_screen(uint32_t score) {
  enableTextMode();
  // simple_display_text("Game is over! YOU (player 1) GOT %d POINTS. Press i to continue                                                                        ",score, -1);
  display_text(" Game is over!                                                                                                                                              ", -1);
}

void wait_for_any_key_input() {
  while (getController() == 0x0)
    ;
}

void wait_for_i_key_input() {
  while (getController() & 0x20)
    ;
}

void display_text(char *new_text, uint32_t start_idx) {
  uint32_t offset = 0;
  while (new_text[offset] != '\0') { 
    TEXT_DATA[start_idx + offset] = new_text[offset++];
  }
}

void custom_delay(int milliseconds)
{
  uint32_t end_time = getTimeStart() + milliseconds;
  while (getTimeStart() < end_time)
    ;
}


