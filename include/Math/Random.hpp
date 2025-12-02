#pragma once

#include <random>
#include <utility>
#include "Math/Vector2.hpp"

template <typename T> using uid = std::uniform_int_distribution<T>;
template <typename T> using urd = std::uniform_real_distribution<T>;
using ufd = urd<float>;

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
    T get(uid<T> const& dist)
    {
        auto& _dist = const_cast<uid<T>&>(dist); /* Bad idea? */
        return _dist(m_engine);
    }

    template <typename T>
    T get(T const& min, T const& max)
    {
        return get(urd<T>{min, max});
    }

    template <typename T>
    T get(int32_t const& min, int32_t const& max)
    {
        return get(uid<T>{min, max});
    }

    template <typename T>
    T get(std::pair<T, T> const& bounds)
    {
        return get(bounds.first, bounds.second);
    }

    template <typename T>
    sf::Vector2<T> get(std::pair<urd<T>, urd<T>> const& dist_pair)
    {
        return {
            get(dist_pair.first),
            get(dist_pair.second)
        };
    }

    template <typename T> T get();
    template<> bool get() { return get<int32_t>(0, 1); }

    template<typename T = int8_t> /* template for easy casting to any numerical type */
    T sign() { return get<bool>() ? 1 : -1; }

private:
    std::mt19937 m_engine;
};