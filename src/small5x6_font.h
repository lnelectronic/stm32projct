#ifndef Small5x6PL_font_h
#define Small5x6PL_font_h

#include "Arduino.h"

const uint8_t Small5x6PL[] PROGMEM =
{
251, 8, 32, '~'+1+18,   // -width, height, firstChar, lastChar
        0x03, 0x00, 0x00, 0x00, 0x00, 0x00,      //  
        0x01, 0x5E, 0x00, 0x00, 0x00, 0x00,      // !
        0x03, 0x06, 0x00, 0x06, 0x00, 0x00,      // "
        0x05, 0x28, 0x7C, 0x28, 0x7C, 0x28,      // #
        0x05, 0x48, 0x54, 0xFE, 0x54, 0x24,      // $
        0x05, 0x22, 0x10, 0x08, 0x04, 0x22,      // %
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00,      // &
        0x03, 0x04, 0x0A, 0x04, 0x00, 0x00,      // '
        0x02, 0x3C, 0x42, 0x00, 0x00, 0x00,      // (
        0x02, 0x42, 0x3C, 0x00, 0x00, 0x00,      // )
        0x05, 0x28, 0x10, 0x7C, 0x10, 0x28,      // *
        0x05, 0x10, 0x10, 0x7C, 0x10, 0x10,      // +
        0x02, 0x80, 0x40, 0x00, 0x00, 0x00,      // ,
        0x04, 0x10, 0x10, 0x10, 0x10, 0x00,      // -
        0x01, 0x40, 0x00, 0x00, 0x00, 0x00,      // .
        0x03, 0x60, 0x18, 0x06, 0x00, 0x00,      // /
        0x05, 0x3C, 0x42, 0x42, 0x42, 0x3C,      // 0
        0x03, 0x44, 0x7E, 0x40, 0x00, 0x00,      // 1
        0x05, 0x64, 0x52, 0x52, 0x52, 0x4C,      // 2
        0x05, 0x24, 0x42, 0x4A, 0x4A, 0x34,      // 3
        0x05, 0x30, 0x28, 0x24, 0x7E, 0x20,      // 4
        0x05, 0x4E, 0x4A, 0x4A, 0x4A, 0x32,      // 5
        0x05, 0x3C, 0x4A, 0x4A, 0x4A, 0x30,      // 6
        0x05, 0x02, 0x42, 0x22, 0x12, 0x0E,      // 7
        0x05, 0x34, 0x4A, 0x4A, 0x4A, 0x34,      // 8
        0x05, 0x0C, 0x52, 0x52, 0x52, 0x3C,      // 9
        0x01, 0x50, 0x00, 0x00, 0x00, 0x00,      // :
        0x02, 0x80, 0x50, 0x00, 0x00, 0x00,      // ;
        0x03, 0x10, 0x28, 0x44, 0x00, 0x00,      // <
        0x03, 0x28, 0x28, 0x28, 0x00, 0x00,      // =
        0x03, 0x44, 0x28, 0x10, 0x00, 0x00,      // >
        0x04, 0x04, 0x02, 0x52, 0x0C, 0x00,      // ?
        0x05, 0x3C, 0x42, 0x5A, 0x5A, 0x0C,      // @
        0x05, 0x7C, 0x12, 0x12, 0x12, 0x7C,      // A
        0x05, 0x7E, 0x4A, 0x4A, 0x4A, 0x34,      // B
        0x05, 0x3C, 0x42, 0x42, 0x42, 0x24,      // C
        0x05, 0x7E, 0x42, 0x42, 0x42, 0x3C,      // D
        0x05, 0x7E, 0x4A, 0x4A, 0x42, 0x42,      // E
        0x05, 0x7E, 0x0A, 0x0A, 0x02, 0x02,      // F
        0x05, 0x3C, 0x42, 0x42, 0x52, 0x34,      // G
        0x05, 0x7E, 0x08, 0x08, 0x08, 0x7E,      // H
        0x03, 0x42, 0x7E, 0x42, 0x00, 0x00,      // I
        0x05, 0x22, 0x42, 0x42, 0x42, 0x3E,      // J
        0x05, 0x7E, 0x08, 0x08, 0x14, 0x62,      // K
        0x05, 0x7E, 0x40, 0x40, 0x40, 0x40,      // L
        0x05, 0x7E, 0x04, 0x38, 0x04, 0x7E,      // M
        0x05, 0x7E, 0x04, 0x08, 0x10, 0x7E,      // N
        0x05, 0x3C, 0x42, 0x42, 0x42, 0x3C,      // O
        0x05, 0x7E, 0x12, 0x12, 0x12, 0x0C,      // P
        0x05, 0x3C, 0x42, 0x42, 0x42, 0xBC,      // Q
        0x05, 0x7E, 0x12, 0x12, 0x12, 0x6C,      // R
        0x05, 0x24, 0x4A, 0x4A, 0x4A, 0x30,      // S
        0x05, 0x02, 0x02, 0x7E, 0x02, 0x02,      // T
        0x05, 0x3E, 0x40, 0x40, 0x40, 0x3E,      // U
        0x05, 0x06, 0x18, 0x60, 0x18, 0x06,      // V
        0x05, 0x1E, 0x60, 0x18, 0x60, 0x1E,      // W
        0x05, 0x62, 0x14, 0x08, 0x14, 0x62,      // X
        0x05, 0x06, 0x08, 0x70, 0x08, 0x06,      // Y
        0x05, 0x42, 0x62, 0x52, 0x4A, 0x46,      // Z
        0x02, 0x7E, 0x42, 0x00, 0x00, 0x00,      // [
        0x05, 0x02, 0x04, 0x08, 0x10, 0x20,      // BackSlash
        0x02, 0x42, 0x7E, 0x00, 0x00, 0x00,      // ]
        0x03, 0x04, 0x02, 0x04, 0x00, 0x00,      // ^
        0x04, 0x80, 0x80, 0x80, 0x80, 0x00,      // _
        0x02, 0x02, 0x04, 0x00, 0x00, 0x00,      // `
        0x05, 0x20, 0x54, 0x54, 0x54, 0x78,      // a
        0x05, 0x7E, 0x48, 0x48, 0x48, 0x30,      // b
        0x04, 0x38, 0x44, 0x44, 0x44, 0x00,      // c
        0x05, 0x30, 0x48, 0x48, 0x48, 0x7E,      // d
        0x05, 0x38, 0x54, 0x54, 0x54, 0x18,      // e
        0x04, 0x08, 0x7C, 0x0A, 0x02, 0x00,      // f
        0x05, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,      // g
        0x05, 0x7E, 0x08, 0x08, 0x08, 0x70,      // h
        0x03, 0x48, 0x7A, 0x40, 0x00, 0x00,      // i
        0x03, 0x80, 0x80, 0x7A, 0x00, 0x00,      // j
        0x04, 0x7E, 0x10, 0x28, 0x40, 0x00,      // k
        0x03, 0x3E, 0x40, 0x40, 0x00, 0x00,      // l
        0x05, 0x78, 0x04, 0x18, 0x04, 0x78,      // m
        0x05, 0x7C, 0x04, 0x04, 0x04, 0x78,      // n
        0x05, 0x38, 0x44, 0x44, 0x44, 0x38,      // o
        0x05, 0xFC, 0x24, 0x24, 0x24, 0x18,      // p
        0x05, 0x18, 0x24, 0x24, 0x24, 0xFC,      // q
        0x05, 0x7C, 0x08, 0x04, 0x04, 0x08,      // r
        0x05, 0x48, 0x54, 0x54, 0x54, 0x24,      // s
        0x04, 0x04, 0x3E, 0x44, 0x40, 0x00,      // t
        0x05, 0x3C, 0x40, 0x40, 0x40, 0x3C,      // u
        0x05, 0x1C, 0x20, 0x40, 0x20, 0x1C,      // v
        0x05, 0x1C, 0x60, 0x18, 0x60, 0x1C,      // w
        0x05, 0x44, 0x28, 0x10, 0x28, 0x44,      // x
        0x05, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,      // y
        0x05, 0x44, 0x64, 0x54, 0x4C, 0x44,      // z
        0x03, 0x10, 0x28, 0x44, 0x00, 0x00,      // {
        0x01, 0x7E, 0x00, 0x00, 0x00, 0x00,      // |
        0x03, 0x44, 0x28, 0x10, 0x00, 0x00,      // }
        0x04, 0x04, 0x02, 0x04, 0x02, 0x00,      // ~
        0x05, 0xFE, 0x82, 0x82, 0x82, 0xFE,      // 
        0x05, 0x7C, 0x12, 0x12, 0x92, 0x7C,      // €
        0x05, 0x3C, 0x42, 0x46, 0x43, 0x24,      // 
        0x05, 0x7E, 0x4A, 0x4A, 0xC2, 0x82,      // ‚
        0x05, 0x7E, 0x48, 0x44, 0x40, 0x40,      // ƒ
        0x05, 0x7E, 0x08, 0x12, 0x21, 0x7E,      // „
        0x05, 0x3C, 0x42, 0x46, 0x43, 0x3C,      // …
        0x05, 0x24, 0x4A, 0x4E, 0x4B, 0x30,      // †
        0x05, 0x42, 0x66, 0x53, 0x4A, 0x46,      // ‡
        0x05, 0x42, 0x62, 0x53, 0x4A, 0x46,      // ˆ
        0x05, 0x20, 0x54, 0x54, 0x54, 0xF8,      // ‰
        0x04, 0x38, 0x44, 0x4C, 0x46, 0x00,      // Š
        0x05, 0x38, 0x54, 0x54, 0xD4, 0x98,      // ‹
        0x04, 0x10, 0x3E, 0x44, 0x40, 0x00,      // Œ
        0x05, 0x7C, 0x04, 0x06, 0x05, 0x78,      // 
        0x05, 0x38, 0x44, 0x4C, 0x46, 0x38,      // Ž
        0x05, 0x48, 0x54, 0x56, 0x54, 0x20,      // 
        0x05, 0x44, 0x6C, 0x56, 0x4C, 0x44,      // 
        0x05, 0x44, 0x64, 0x56, 0x4C, 0x44,      // ‘
        0x05, 0x7C, 0xAA, 0xC2, 0xAA, 0x7C,      // ’
        0x05, 0x7C, 0xCA, 0xA2, 0xCA, 0x7C,      // “
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00,      // ”
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00,      // •
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00       // –
};

#endif