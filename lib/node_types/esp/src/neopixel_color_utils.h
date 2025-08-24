// neopixel_color_utils.h
// RgbColor utilities and constants for compatibility with CRGB interface

#ifndef _NEOPIXEL_COLOR_UTILS_H_
#define _NEOPIXEL_COLOR_UTILS_H_

#include <NeoPixelBus.h>

// Type alias for backward compatibility - CRGB is now RgbColor
typedef RgbColor CRGB;

// Color constants using RgbColor
namespace NeoColorConsts {
    const RgbColor Black(0, 0, 0);
    const RgbColor White(255, 255, 255);
    const RgbColor Red(255, 0, 0);
    const RgbColor Green(0, 255, 0);
    const RgbColor Blue(0, 0, 255);
    const RgbColor Yellow(255, 255, 0);
    const RgbColor Cyan(0, 255, 255);
    const RgbColor Magenta(255, 0, 255);
    const RgbColor Purple(128, 0, 128);
    const RgbColor Orange(255, 165, 0);
    const RgbColor Pink(255, 192, 203);
    const RgbColor DeepPink(255, 20, 147);
    const RgbColor Brown(165, 42, 42);
    const RgbColor Gold(255, 215, 0);
    const RgbColor Grey(128, 128, 128);
    const RgbColor Gray(128, 128, 128);
    const RgbColor LightGrey(211, 211, 211);
    const RgbColor LightGray(211, 211, 211);
    const RgbColor LightBlue(173, 216, 230);
    const RgbColor LightGreen(144, 238, 144);
}

// Compatibility namespace for CRGB-style access
namespace CRGB {
    const RgbColor& Black = NeoColorConsts::Black;
    const RgbColor& White = NeoColorConsts::White;
    const RgbColor& Red = NeoColorConsts::Red;
    const RgbColor& Green = NeoColorConsts::Green;
    const RgbColor& Blue = NeoColorConsts::Blue;
    const RgbColor& Yellow = NeoColorConsts::Yellow;
    const RgbColor& Cyan = NeoColorConsts::Cyan;
    const RgbColor& Magenta = NeoColorConsts::Magenta;
    const RgbColor& Purple = NeoColorConsts::Purple;
    const RgbColor& Orange = NeoColorConsts::Orange;
    const RgbColor& Pink = NeoColorConsts::Pink;
    const RgbColor& DeepPink = NeoColorConsts::DeepPink;
    const RgbColor& Brown = NeoColorConsts::Brown;
    const RgbColor& Gold = NeoColorConsts::Gold;
    const RgbColor& Grey = NeoColorConsts::Grey;
    const RgbColor& Gray = NeoColorConsts::Gray;
    const RgbColor& LightGrey = NeoColorConsts::LightGrey;
    const RgbColor& LightGray = NeoColorConsts::LightGray;
    const RgbColor& LightBlue = NeoColorConsts::LightBlue;
    const RgbColor& LightGreen = NeoColorConsts::LightGreen;
}

// Utility functions for RgbColor
inline uint8_t getLuma(const RgbColor& color) {
    // Use same formula as FastLED
    return (color.R * 54 + color.G * 183 + color.B * 19) >> 8;
}

inline RgbColor blend(const RgbColor& color1, const RgbColor& color2, uint8_t scale) {
    uint8_t inv_scale = 255 - scale;
    return RgbColor(
        (color1.R * inv_scale + color2.R * scale) >> 8,
        (color1.G * inv_scale + color2.G * scale) >> 8,
        (color1.B * inv_scale + color2.B * scale) >> 8
    );
}

// HSV to RGB conversion struct for compatibility
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
    operator RgbColor() const {
        uint8_t region, remainder, p, q, t;
        uint8_t hue_val = h, sat_val = s, val_val = v;
        
        if (sat_val == 0) {
            return RgbColor(val_val, val_val, val_val);
        }
        
        region = hue_val / 43;
        remainder = (hue_val - (region * 43)) * 6;
        
        p = (val_val * (255 - sat_val)) >> 8;
        q = (val_val * (255 - ((sat_val * remainder) >> 8))) >> 8;
        t = (val_val * (255 - ((sat_val * (255 - remainder)) >> 8))) >> 8;
        
        switch (region) {
            case 0:  return RgbColor(val_val, t, p);
            case 1:  return RgbColor(q, val_val, p);
            case 2:  return RgbColor(p, val_val, t);
            case 3:  return RgbColor(p, q, val_val);
            case 4:  return RgbColor(t, p, val_val);
            default: return RgbColor(val_val, p, q);
        }
    }
};

#endif // _NEOPIXEL_COLOR_UTILS_H_