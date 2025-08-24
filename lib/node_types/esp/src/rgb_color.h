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

    // Color constants - compatible with FastLED
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
};

// Color constant definitions
const CRGB CRGB::Black(0, 0, 0);
const CRGB CRGB::White(255, 255, 255);
const CRGB CRGB::Red(255, 0, 0);
const CRGB CRGB::Green(0, 255, 0);
const CRGB CRGB::Blue(0, 0, 255);
const CRGB CRGB::Yellow(255, 255, 0);
const CRGB CRGB::Cyan(0, 255, 255);
const CRGB CRGB::Magenta(255, 0, 255);
const CRGB CRGB::Purple(128, 0, 128);
const CRGB CRGB::Orange(255, 165, 0);
const CRGB CRGB::Pink(255, 192, 203);
const CRGB CRGB::DeepPink(255, 20, 147);
const CRGB CRGB::Brown(165, 42, 42);
const CRGB CRGB::Gold(255, 215, 0);
const CRGB CRGB::Grey(128, 128, 128);
const CRGB CRGB::Gray(128, 128, 128);
const CRGB CRGB::LightGrey(211, 211, 211);
const CRGB CRGB::LightGray(211, 211, 211);
const CRGB CRGB::LightBlue(173, 216, 230);
const CRGB CRGB::LightGreen(144, 238, 144);

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
    uint8_t h, s, v;
    
    inline CHSV() : h(0), s(0), v(0) {}
    inline CHSV(uint8_t hue, uint8_t saturation, uint8_t value) : h(hue), s(saturation), v(value) {}
    
    // Convert HSV to RGB - simplified conversion
    CRGB toRGB() const {
        uint8_t region, remainder, p, q, t;
        uint8_t hue = h, sat = s, val = v;
        
        if (sat == 0) {
            return CRGB(val, val, val);
        }
        
        region = hue / 43;
        remainder = (hue - (region * 43)) * 6;
        
        p = (val * (255 - sat)) >> 8;
        q = (val * (255 - ((sat * remainder) >> 8))) >> 8;
        t = (val * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;
        
        switch (region) {
            case 0:  return CRGB(val, t, p);
            case 1:  return CRGB(q, val, p);
            case 2:  return CRGB(p, val, t);
            case 3:  return CRGB(p, q, val);
            case 4:  return CRGB(t, p, val);
            default: return CRGB(val, p, q);
        }
    }
};

#endif // _RGB_COLOR_H_