#include "Math/Random.hpp"
#include "Graphics/Vector2.hpp"

Random::Random(uint32_t const seed)
    : m_engine(seed) {}

Random::Random()
    : m_engine(std::random_device{}()) {}

template <typename T>
T Random::get(urd<T> const& dist)
{
    auto& _dist = const_cast<urd<T>&>(dist); /* Bad idea? */
    return _dist(m_engine);
}

template <typename T>
T Random::get(T const& min, T const& max)
{
    return get(urd<T>{min, max});
}

template <>
sf::Vector2f Random::get(sf::Vector2f const& min, sf::Vector2f const& max)
{
    return {
        get<float>(min.x, max.x),
        get<float>(min.y, max.y)
    };
}
