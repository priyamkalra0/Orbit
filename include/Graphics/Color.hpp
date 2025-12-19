#pragma once

#include <cstdint>
#include <algorithm>
#include <SFML/Graphics/Color.hpp>

/* constexpr math helpers; WHY THE FUCK ARE THESE NOT CONSTEXPR IN CMATH? */
constexpr static double abs_c(double const x)
{ return (x < 0) ? -x : x; }

constexpr static double fmod_c(double const x, double const y)
{ return x - static_cast<int64_t>(x / y) * y; }

class Color : public sf::Color
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

    constexpr static sf::Color rgb(uint8_t const r, uint8_t const g, uint8_t const b)
    { return {r, g, b}; }

    constexpr static sf::Color rgba(uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a = 255)
    { return {r, g, b, a}; }

    constexpr static sf::Color hwb(double const h, double const w, double const b, double const a = 100.0)
    { return normal_hwb(h, w / 100.0, b / 100.0, a / 100.0); }

    constexpr static sf::Color hsl(double const h, double const s, double const l, double const a = 100.0)
    { return normal_hsl(h, s / 100.0, l / 100.0, a / 100.0); }

    constexpr static sf::Color hsv(double const h, double const s, double const v, double const a = 100.0)
    { return normal_hsv(h, s / 100.0, v / 100.0, a / 100.0); }

    /* Generic factory for any color format */
    template <FormatU8 F = RGBA>
    static sf::Color get(uint8_t, uint8_t, uint8_t, uint8_t a = 255);
    template<> constexpr sf::Color get<RGBA>(uint8_t const r, uint8_t const g, uint8_t const b, uint8_t const a)
    { return rgba(r, g, b, a); }

    template <Format F>
    static sf::Color get(double, double, double, double a = 100.0);

    template<> constexpr sf::Color get<HWB>(double const h, double const w, double const b, double const a)
    { return hwb(h, w, b, a); }

    template<> constexpr sf::Color get<HSL>(double const h, double const s, double const l, double const a)
    { return hsl(h, s, l, a); }

    template<> constexpr sf::Color get<HSV>(double const h, double const s, double const v, double const a)
    { return hsv(h, s, v, a); }

    Color() = delete;

private:
    constexpr static sf::Color normal_hwb(double h, double w, double b, double a)
    {
        /* https://en.wikipedia.org/wiki/HWB_color_model#Conversion */

        double const v { 1 - b };
        double const s {
            (v > 0)
            ? (1 - w / v)
            : 0
        };

        return normal_hsv(h, s, v, a);
    };

    constexpr static sf::Color normal_hsl(double h, double s, double l, double a)
    {
        /* https://en.wikipedia.org/wiki/HSL_and_HSV#Interconversion */

        double const v { l + s * std::min(l, 1 - l) };
        double const S {
            (v > 0)
            ? 2 * (1 - l / v)
            : 0
        };

        return normal_hsv(h, S, v, a);
    };

    constexpr static sf::Color normal_hsv(double h, double s, double v, double a)
    {
        /* https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB */

        double const c { s * v };
        double const H { h / 60.0 };
        double const x { c * (1 - abs_c(fmod_c(H, 2.0) - 1)) };

        double r = 0, g = 0, b = 0;
        if (0 <= H && H < 1) r = c, g = x, b = 0;
        else if (1 <= H && H < 2) r = x, g = c, b = 0;
        else if (2 <= H && H < 3) r = 0, g = c, b = x;
        else if (3 <= H && H < 4) r = 0, g = x, b = c;
        else if (4 <= H && H < 5) r = x, g = 0, b = c;
        else r = c, g = 0, b = x;

        double const m { v - c };

        return rgba(
            static_cast<uint8_t>((r + m) * 255),
            static_cast<uint8_t>((g + m) * 255),
            static_cast<uint8_t>((b + m) * 255),
            static_cast<uint8_t>(a * 255)
        );
    };
};