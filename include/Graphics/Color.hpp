#pragma once

#include <cstdint>
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

    // Note: These enums are seperated to allow the ::get<RGBA> template
    // to have a UInt8 signature, the formats are meant to be directly
    // accessed using Color:: anyway so it shouldn't matter
    enum FormatU8 { RGBA };
    enum Format { HWB, HSL, HSV };

    static sf::Color rgb(uint8_t const r, uint8_t const g, uint8_t const b)
    { return {r, g, b}; }

    static sf::Color rgba(uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255)
    { return {r, g, b, a}; }

    static sf::Color hwb(double const h, double const w, double const b, double const a = 255.0)
    { return normal_hwb(h, w / 100.0, b / 100.0, a); }

    static sf::Color hsl(double const h, double const s, double const l, double const a = 255.0)
    { return normal_hsl(h, s / 100.0, l / 100.0, a); }

    static sf::Color hsv(double const h, double const s, double const v, double const a = 255.0)
    { return normal_hsv(h, s / 100.0, v / 100.0, a); }

    /* Generic factory for any color format */
    template <FormatU8 F = RGBA>
    static sf::Color get(uint8_t, uint8_t, uint8_t, uint8_t a = 255);
    template<> sf::Color get<RGBA>(uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
    { return rgba(r, g, b, a); }

    template <Format F>
    static sf::Color get(double, double, double, double a = 255.0);

    template<> sf::Color get<HWB>(double const h, double const w, double const b, double const a)
    { return hwb(h, w, b, a); }

    template<> sf::Color get<HSL>(double const h, double const s, double const l, double const a)
    { return hsl(h, s, l, a); }

    template<> sf::Color get<HSV>(double const h, double const s, double const v, double const a)
    { return hsv(h, s, v, a); }

    Color() = delete;

private:
    static sf::Color normal_hwb(double h, double w, double b, double a);
    static sf::Color normal_hsl(double h, double s, double l, double a);
    static sf::Color normal_hsv(double h, double s, double v, double a);
};