// dev_rgb_base.h
// rgb_base class (base for doing rgb-strips and leds)

#ifndef _RGB_BASE_H_
#define _RGB_BASE_H_

// For rgb_strip_bus: use NeoPixelBus RgbColor
// For rgb_strip (deprecated): still use FastLED CRGB
#include <NeoPixelBus.h>
#include "neopixel_color_utils.h"

#include <device.h>

#define ALL_LEDS -16

class RGB_Base : public Device {
    protected:
        int _led_count=0;
        RgbColor avg_color;
    public:
        RGB_Base(const char* name, int led_count);
        RGB_Base& high() {
            if((uint16_t)avg_color.R+avg_color.G+avg_color.B>0) {
                return *this; // if something is on, do nothing
            }
            return set_color(NeoColorConsts::White); // else switch to white TODO: make on-brightness configurable
        }
        RGB_Base& on() { return high(); }
        RGB_Base& low() {
            return set_color(NeoColorConsts::Black);
        }
        RGB_Base& off() { return low(); }
        RGB_Base& set_color(RgbColor color) {
            return set_color(ALL_LEDS, color, true);
        }
        RGB_Base& set_color_noshow(RgbColor color) {
            return set_color(ALL_LEDS, color, false);
        }

        bool read_color(const Ustring& colorstr, RgbColor& color); // TODO: make static

        RGB_Base& set_colorstr(int lednr, const Ustring& color, bool _show=true);
        RGB_Base& set_colorstr(const Ustring& color, bool _show=true);

// TODO: implement setting a bar (percentage or number of leds at once)

        RGB_Base& set_color( int lednr, RgbColor color, bool _show=true) {
            if(!started()) return *this;
            if(lednr<0) {
                if(lednr==ALL_LEDS) {
                    for(int nr=0; nr<led_count(); nr++) 
                        process_color(nr, color, false);
                    if(_show) show();
                } else { // push front
                    push_front(color, _show);
                }
            } else if (lednr >= led_count()) {
                push_back(color, _show);
            } else {
                process_color(lednr, color, _show);
            }
            // update values with last color set
            // TODO: make this reporting skipable
            // TODO: seems like also on and off should be ignored for home-assistant
            value(0).from(getLuma(avg_color)>0?str_on:str_off);
            value(2).from((int)getLuma(avg_color));
            value(4).printf("%02x%02x%02x", avg_color.R, avg_color.G, avg_color.B);
            return *this;
        }

        int led_count() {
            return _led_count;
        }
        void push_front(RgbColor color, bool _show=true);
        void push_back(RgbColor color, bool _show=true);

        // these 4 need be re-implemented
        virtual void start() {
            // keep _started at false
        }

        virtual void process_color(int lednr, RgbColor color, bool _show=true) {
            avg_color = color; // in strip, real average is computed
            // show not evaluated in single led case (but in rgb_strip)
        }

        virtual RgbColor get_color(int lednr) {
            return avg_color;
        }

        virtual void show() {
            // nothing by default
        }

       virtual bool measure() { show(); return true; } // give control to library on regular basis
};

#endif // _RGB_BASE_H_