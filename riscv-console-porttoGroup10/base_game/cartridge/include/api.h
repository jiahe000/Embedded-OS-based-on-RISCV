#include <stdint.h>

#ifndef API_H
#define API_H

// retrieval system calls
uint32_t getTicks(void);
uint32_t getController(void);

// timer system calls
uint32_t getTimeStart(void);
uint32_t getTimeEnd(uint64_t start);

// graphic system calls
uint32_t enableTextMode(void);
uint32_t enableGraphicsMode(void);

enum ShapeType { SQUARE, CIRCLE, TRIANGLE, PARALLELOGRAM, NOUGHT, CROSS };
enum BackgroundShapeType { POUND, RECTANGLE };
uint32_t createSmallSprite(enum ShapeType shapeType);
uint32_t setSmallPalette(uint32_t index, uint32_t color);
uint32_t setSmallControl(uint32_t index, uint32_t palette, uint32_t x,
                         uint32_t y, uint32_t z);
uint32_t getSmallSpriteCount();
uint32_t createMediumSprite(enum ShapeType shapeType);
uint32_t setMediumPalette(uint32_t index, uint32_t color);
uint32_t setMediumControl(uint32_t index, uint32_t palette, uint32_t x,
                          uint32_t y, uint32_t z);
uint32_t getMediumSpriteCount();
uint32_t createLargeSprite(enum ShapeType shapeType);
uint32_t setLargePalette(uint32_t index, uint32_t color);
uint32_t setLargeControl(uint32_t index, uint32_t palette, uint32_t x,
                         uint32_t y, uint32_t z);
uint32_t getLargeSpriteCount();
uint32_t createBackground(enum BackgroundShapeType backgroundShapeType);
uint32_t setBackgroundPalette(uint32_t index, uint32_t color);
uint32_t setBackgroundControl(uint32_t index, uint32_t palette, uint32_t x,
                              uint32_t y, uint32_t z);
uint32_t getCMDInterruptCount();
uint32_t getVideoInterruptCount();
uint32_t systemCall(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3,
                    uint32_t arg4, uint32_t call);

#endif