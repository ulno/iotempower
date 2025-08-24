// rgb_color.h
// RGB color type and utilities - replacement for FastLED CRGB
// Designed to be compatible with NeoPixelBus RgbColor

#ifndef _RGB_COLOR_H_
#define _RGB_COLOR_H_

#include <Arduino.h>

// RGB color class compatible with FastLED's CRGB interface
struct CRGB {
    union {
        struct {
            uint8_t r, g, b;
        };
        struct {
            uint8_t red, green, blue;
        };
        uint8_t raw[3];
    };

    // Constructors
    inline CRGB() : r(0), g(0), b(0) {}
    inline CRGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
    inline CRGB(uint32_t colorcode) : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b(colorcode & 0xFF) {}

    // Compatibility with NeoPixelBus RgbColor
    inline CRGB(const CRGB& other) : r(other.r), g(other.g), b(other.b) {}
    inline CRGB& operator=(const CRGB& other) {
        r = other.r;
        g = other.g; 
        b = other.b;
        return *this;
    }

    // Utility functions - compatible with FastLED
    uint8_t getLuma() const {
        // Use same formula as FastLED
        return (r * 54 + g * 183 + b * 19) >> 8;
    }

    // Array access operator
    inline uint8_t& operator[](uint8_t index) {
        return raw[index];
    }
    inline const uint8_t& operator[](uint8_t index) const {
        return raw[index];
    }

    // Color constants - using static const approach for better compatibility
    static const CRGB Black;
    static const CRGB White;
    static const CRGB Red;
    static const CRGB Green;
    static const CRGB Blue;
    static const CRGB Yellow;
    static const CRGB Cyan;
    static const CRGB Magenta;
    static const CRGB Purple;
    static const CRGB Orange;
    static const CRGB Pink;
    static const CRGB DeepPink;
    static const CRGB Brown;
    static const CRGB Gold;
    static const CRGB Grey;
    static const CRGB Gray;
    static const CRGB LightGrey;
    static const CRGB LightGray;
    static const CRGB LightBlue;
    static const CRGB LightGreen;
};

// FastLED-compatible color constants using macros for simplicity
#define CRGB_BLACK    CRGB(0, 0, 0)
#define CRGB_WHITE    CRGB(255, 255, 255) 
#define CRGB_RED      CRGB(255, 0, 0)
#define CRGB_GREEN    CRGB(0, 255, 0)
#define CRGB_BLUE     CRGB(0, 0, 255)
#define CRGB_YELLOW   CRGB(255, 255, 0)
#define CRGB_CYAN     CRGB(0, 255, 255)
#define CRGB_MAGENTA  CRGB(255, 0, 255)
#define CRGB_PURPLE   CRGB(128, 0, 128)
#define CRGB_ORANGE   CRGB(255, 165, 0)
#define CRGB_PINK     CRGB(255, 192, 203)
#define CRGB_DEEPPINK CRGB(255, 20, 147)
#define CRGB_BROWN    CRGB(165, 42, 42)
#define CRGB_GOLD     CRGB(255, 215, 0)
#define CRGB_GREY     CRGB(128, 128, 128)
#define CRGB_GRAY     CRGB(128, 128, 128)
#define CRGB_LIGHTGREY CRGB(211, 211, 211)
#define CRGB_LIGHTGRAY CRGB(211, 211, 211)
#define CRGB_LIGHTBLUE CRGB(173, 216, 230)
#define CRGB_LIGHTGREEN CRGB(144, 238, 144)

// Define the color constants outside the class
const CRGB CRGB::Black = CRGB_BLACK;
const CRGB CRGB::White = CRGB_WHITE;
const CRGB CRGB::Red = CRGB_RED;
const CRGB CRGB::Green = CRGB_GREEN;
const CRGB CRGB::Blue = CRGB_BLUE;
const CRGB CRGB::Yellow = CRGB_YELLOW;
const CRGB CRGB::Cyan = CRGB_CYAN;
const CRGB CRGB::Magenta = CRGB_MAGENTA;
const CRGB CRGB::Purple = CRGB_PURPLE;
const CRGB CRGB::Orange = CRGB_ORANGE;
const CRGB CRGB::Pink = CRGB_PINK;
const CRGB CRGB::DeepPink = CRGB_DEEPPINK;
const CRGB CRGB::Brown = CRGB_BROWN;
const CRGB CRGB::Gold = CRGB_GOLD;
const CRGB CRGB::Grey = CRGB_GREY;
const CRGB CRGB::Gray = CRGB_GRAY;
const CRGB CRGB::LightGrey = CRGB_LIGHTGREY;
const CRGB CRGB::LightGray = CRGB_LIGHTGRAY;
const CRGB CRGB::LightBlue = CRGB_LIGHTBLUE;
const CRGB CRGB::LightGreen = CRGB_LIGHTGREEN;

// Utility functions - compatible with FastLED
inline CRGB blend(const CRGB& color1, const CRGB& color2, uint8_t scale) {
    uint8_t inv_scale = 255 - scale;
    return CRGB(
        (color1.r * inv_scale + color2.r * scale) >> 8,
        (color1.g * inv_scale + color2.g * scale) >> 8,
        (color1.b * inv_scale + color2.b * scale) >> 8
    );
}

// Type aliases for gradients (used in rgb_matrix)
typedef int16_t saccum87;
typedef int32_t accum88;

// HSV color struct - compatible with FastLED CHSV
struct CHSV {
    union {
        struct {
            uint8_t h, s, v;
        };
        struct {
            uint8_t hue, sat, val;
        };
    };
    
    inline CHSV() : h(0), s(0), v(0) {}
    inline CHSV(uint8_t hue, uint8_t saturation, uint8_t value) : h(hue), s(saturation), v(value) {}
    
    // Convert HSV to RGB - simplified conversion compatible with FastLED
    operator CRGB() const {
        uint8_t region, remainder, p, q, t;
        uint8_t hue_val = h, sat_val = s, val_val = v;
        
        if (sat_val == 0) {
            return CRGB(val_val, val_val, val_val);
        }
        
        region = hue_val / 43;
        remainder = (hue_val - (region * 43)) * 6;
        
        p = (val_val * (255 - sat_val)) >> 8;
        q = (val_val * (255 - ((sat_val * remainder) >> 8))) >> 8;
        t = (val_val * (255 - ((sat_val * (255 - remainder)) >> 8))) >> 8;
        
        switch (region) {
            case 0:  return CRGB(val_val, t, p);
            case 1:  return CRGB(q, val_val, p);
            case 2:  return CRGB(p, val_val, t);
            case 3:  return CRGB(p, q, val_val);
            case 4:  return CRGB(t, p, val_val);
            default: return CRGB(val_val, p, q);
        }
    }
};

#endif // _RGB_COLOR_H_