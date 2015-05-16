#ifndef PLANT_HPP_INCLUDED
#define PLANT_HPP_INCLUDED

#include "Object.hpp"
#include "HasEnergy.hpp"

#include <SFML/Graphics.hpp>

struct Plant : Object, HasEnergy
{
private:
    static float constexpr radiusPerEnergy = .25;

    sf::Sprite sprite;
    unsigned long colorCode;

    float energyPerSecond, maxEnergy, spreadFactor = 4, energyPass = .5;
    int maxChildren = 5;

    Plant *parent = 0, *ancestor = 0;
    int childrenCount = 0;

    static constexpr float parentCheckLatency = .8;
    float nextParentCheck = parentCheckLatency * (rand() % 100 / 100.);

    float initialAngle;

public:
    Plant(class World *world, float x, float y, float e, HasEnergy *source)
        : Object(world, x, y, e * radiusPerEnergy, .5), HasEnergy(e, source),
        energyPerSecond(.005 + .01 * (rand() % 100 / 100.)), maxEnergy(.3 + .1 * (rand() % 100 / 100.)),
        colorCode(rand())
    {
    }

    void onObjectDeleted(Object *other);
    void update(float time);
    void render(sf::RenderWindow &window);
};

#endif//PLANT_HPP_INCLUDED
