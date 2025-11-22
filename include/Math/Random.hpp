#pragma once

#include <random>

template <typename T>
using urd = std::uniform_real_distribution<T>;

class Random
{
public:
    explicit Random(uint32_t seed);
    Random();

    template <class T>
    T get(urd<T> const& dist);

    template <class T>
    T get(T const& min, T const& max);

private:
    std::mt19937 m_engine;
};