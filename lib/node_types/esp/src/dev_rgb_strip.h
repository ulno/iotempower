// rgb_strip.h
// control a multicolor led strip
// DEPRECATED - use rgb_strip_bus instead

#ifndef _RGB_STRIP_H_
#define _RGB_STRIP_H_

// FastLED compatibility for deprecated rgb_strip
#include "rgb_color.h"

#include <device.h>

#define ALL_LEDS -16

// Deprecated RGB_Strip class using custom CRGB compatibility layer
class RGB_Strip : public Device {
    protected:
        int _led_count=0;
        CRGB avg_color;
        CRGB *leds;
        CLEDController *controller;
    public:
        RGB_Strip(const char* name, int _led_count, CLEDController& _controller) 
        : Device(name) {
            this->_led_count = _led_count;
            controller = &_controller;
            set_pollrate_us(10000); // give update chance - TODO: check if necessary
            leds = controller->leds();
        }
        
        RGB_Strip& high() {
            if((uint16_t)avg_color.r+avg_color.g+avg_color.b>0) {
                return *this; // if something is on, do nothing
            }
            return set_color(CRGB::White);
        }
        RGB_Strip& on() { return high(); }
        RGB_Strip& low() {
            return set_color(CRGB::Black);
        }
        RGB_Strip& off() { return low(); }
        RGB_Strip& set_color(CRGB color) {
            return set_color(ALL_LEDS, color, true);
        }
        RGB_Strip& set_color_noshow(CRGB color) {
            return set_color(ALL_LEDS, color, false);
        }

        bool read_color(const Ustring& colorstr, CRGB& color);

        RGB_Strip& set_colorstr(int lednr, const Ustring& color, bool _show=true);
        RGB_Strip& set_colorstr(const Ustring& color, bool _show=true);

        RGB_Strip& set_color( int lednr, CRGB color, bool _show=true) {
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
            value(0).from(avg_color.getLuma()>0?str_on:str_off);
            value(2).from((int)avg_color.getLuma());
            value(4).printf("%02x%02x%02x", avg_color.r, avg_color.g, avg_color.b);
            return *this;
        }

        int led_count() {
            return _led_count;
        }
        void push_front(CRGB color, bool _show=true);
        void push_back(CRGB color, bool _show=true);

        void start() {
            _started = true;
            controller->init(); // re-init, might be important if using onboard-led
            delay(50); // let things settle
            set_color(ALL_LEDS, CRGB::Black);
            delay(100); // let things settle (total 100 is too small)
            set_color(ALL_LEDS, CRGB::Black);
        }
        virtual void process_color(int lednr, CRGB color, bool _show=true) {
            leds[lednr] = color;
            if(_show) show();
        }
        virtual CRGB get_color(int lednr) {
            if(lednr < 0) lednr=0;
            else if(lednr >= led_count()) lednr = led_count()-1;
            return leds[lednr];
        }
        virtual void show() {
            if(!started()) return;
            controller->showLeds(255); // TODO: handle brightness of showLeds?
            // TODO: consider only computing average every 10ms
            // compute average color
            uint32_t avg_r = 0;
            uint32_t avg_g = 0;
            uint32_t avg_b = 0;
            CRGB c;
            int lc = led_count();
            for(int i=lc-1; i>=0; i--) {
                c = leds[i];
                avg_r += c.r;
                avg_g += c.g;
                avg_b += c.b;
            }
            avg_color = CRGB(avg_r/lc, avg_g/lc, avg_b/lc);
        }

        virtual bool measure() { show(); return true; } // give control to library on regular basis
};

#endif // _RGB_STRIP_H_
