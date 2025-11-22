#include "Graphics/Color.hpp"

#include <random>

sf::Color Color::normal_hwb(double const h, double const w, double const b, double const a)
{
    /* https://en.wikipedia.org/wiki/HWB_color_model#Conversion */

    double const v = 1.0 - b;
    double const s = 1.0 - w / v;

    return normal_hsv(h, s, v, a);
}

sf::Color Color::normal_hsl(double const h, double const s, double const l, double const a)
{
    /* https://en.wikipedia.org/wiki/HSL_and_HSV#Interconversion */

    double const v = l + s * std::min(l, 1 - l);
    double const S = (v > 0.0) ? 2 * (1 - l / v) : 0;

    return normal_hsv(h, S, v, a);
}

sf::Color Color::normal_hsv(double const h, double const s, double const v, double const a)
{
    /* https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB */

    double const c = s * v;
    double const H = h / 60.0;
    double const x = c * (1 - std::abs(fmod(H, 2) - 1));

    double r, g, b;
    if (0 <= H && H < 1) r = c, g = x, b = 0;
    else if (1 <= H && H < 2) r = x, g = c, b = 0;
    else if (2 <= H && H < 3) r = 0, g = c, b = x;
    else if (3 <= H && H < 4) r = 0, g = x, b = c;
    else if (4 <= H && h < 5) r = x, g = 0, b = c;
    else r = c, g = 0, b = x;

    double const m = v - c;

    return rgba(
        static_cast<uint8_t>((r + m) * 255),
        static_cast<uint8_t>((g + m) * 255),
        static_cast<uint8_t>((b + m) * 255),
        static_cast<uint32_t>(a)
    );
};