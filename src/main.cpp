#include <Arduino.h>
#include <tft_io.hpp>
#include <st7789.hpp>
#include <gfx_cpp14.hpp>
using namespace arduino;
using namespace gfx;

#define HSPI_MISO -1
#define HSPI_MOSI 35
#define HSPI_CLK 36
#define LCD_HOST HSPI
#define LCD_CS 34
#define LCD_RST 38
#define LCD_DC 37
#define LCD_BL 33

using bus_t = tft_spi_ex<LCD_HOST,LCD_CS,HSPI_MOSI,HSPI_MISO,HSPI_CLK>;

using lcd_t = st7789<135,240,LCD_DC,LCD_RST,LCD_BL,bus_t,1,true>;

using color_t = color<typename lcd_t::pixel_type>;

lcd_t lcd;

void alpha_demo() {
#ifdef LCD_EPAPER
    draw::suspend(lcd);
#endif

    draw::filled_rectangle(lcd, (srect16)lcd.bounds(), color_t::black);

    for (int y = 0; y < lcd.dimensions().height; y += 16) {
        for (int x = 0; x < lcd.dimensions().width; x += 16) {
            if (0 != ((x + y) % 32)) {
                draw::filled_rectangle(lcd,
                                       srect16(spoint16(x, y), ssize16(16, 16)),
                                       color_t::white);
            }
        }
    }
    randomSeed(millis());

    rgba_pixel<32> px;
    spoint16 tpa[3];
    const uint16_t sw =
        min(lcd.dimensions().width, lcd.dimensions().height) / 4;
    for (int i = 0; i < 30; ++i) {
        px.channel<channel_name::R>((rand() % 256));
        px.channel<channel_name::G>((rand() % 256));
        px.channel<channel_name::B>((rand() % 256));
        px.channel<channel_name::A>(50 + rand() % 156);
        srect16 sr(0, 0, rand() % sw + sw, rand() % sw + sw);
        sr.offset_inplace(rand() % (lcd.dimensions().width - sr.width()),
                          rand() % (lcd.dimensions().height - sr.height()));
        switch (rand() % 4) {
            case 0:
                draw::filled_rectangle(lcd, sr, px);
                break;
            case 1:
                draw::filled_rounded_rectangle(lcd, sr, .1, px);
                break;
            case 2:
                draw::filled_ellipse(lcd, sr, px);
                break;
            case 3:
                tpa[0] = {int16_t(((sr.x2 - sr.x1) / 2) + sr.x1), sr.y1};
                tpa[1] = {sr.x2, sr.y2};
                tpa[2] = {sr.x1, sr.y2};
                spath16 path(3, tpa);
                draw::filled_polygon(lcd, path, px);
                break;
        }
    }
#ifdef LCD_EPAPER
    draw::resume(lcd);
#endif

    delay(2000);
}

void setup() {
    Serial.begin(115200);
    lcd.fill(lcd.bounds(),color_t::purple);    
}

void loop() {
    alpha_demo();
}