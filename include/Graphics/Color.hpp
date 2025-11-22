#pragma once

#include <SFML/Graphics/Color.hpp>

class Color
{
public:
    /* Extends color support to 4 formats:
     * RGB[A]
     * HWB
     * HSL
     * HSV
     */

    static sf::Color rgb(uint8_t const r, uint8_t const g, uint8_t const b)
    { return {r, g, b}; }

    static sf::Color rgba(uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255.0)
    { return {r, g, b, a}; }

    static sf::Color hwb(double const h, double const w, double const b, double const a = 255.0)
    { return normal_hwb(h, w / 100.0, b / 100.0, a); }

    static sf::Color hsl(double const h, double const s, double const l, double const a = 255.0)
    { return normal_hsl(h, s / 100.0, l / 100.0, a); }

    static sf::Color hsv(double const h, double const s, double const v, double const a = 255.0)
    { return normal_hsv(h, s / 100.0, v / 100.0, a); }


private:
    static sf::Color normal_hwb(double h, double w, double b, double a);
    static sf::Color normal_hsl(double h, double s, double l, double a);
    static sf::Color normal_hsv(double h, double s, double v, double a);
};