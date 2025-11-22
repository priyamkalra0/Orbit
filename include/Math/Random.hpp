#pragma once

#include <random>
#include <utility>
#include "Math/Vector2.hpp"

template <typename T>
using urd = std::uniform_real_distribution<T>;

class Random
{
public:
    explicit Random(uint32_t const seed)
        : m_engine(seed) {}

    Random()
        : m_engine(std::random_device{}()) {}

    template <typename T>
    T get(urd<T> const& dist)
    {
        auto& _dist = const_cast<urd<T>&>(dist); /* Bad idea? */
        return _dist(m_engine);
    }

    template <typename T>
    T get(T const& min, T const& max)
    {
        return get(urd<T>{min, max});
    }

    template <>
    sf::Vector2f get(sf::Vector2f const& min, sf::Vector2f const& max)
    {
        return {
            get<float>(min.x, max.x),
            get<float>(min.y, max.y)
        };
    }

private:
    std::mt19937 m_engine;
};