#ifndef BUG_HPP_INCLUDED
#define BUG_HPP_INCLUDED

#include "Object.hpp"
#include "HasEnergy.hpp"
#include "Intelligence.hpp"

#include <SFML/Graphics.hpp>

#include <array>
#include <cstdlib>

struct Bug : Object, HasEnergy
{
    Intelligence intelligence;

    static constexpr float sensorLatency = .8;
    float nextSensorCheck = sensorLatency * (rand() % 100 / 100.);

    static constexpr int sensorCount = 32;

    bool useFixedIntelligence = true;
    float maxEnergy = 1;
    float sensorRange = .7;
    float leftPush = 0, rightPush = 0;
    float angle = 0;
    static constexpr float reproductionLatency = 10;
    float lastReproduction = reproductionLatency;
    std::vector<nn_real> inputs;


    Bug(class World *world, float x, float y, float e, HasEnergy *source = 0)
      : Object(world, x, y, .04, 1), HasEnergy(e, source),
        // (2 + sensorCount)inputs:
        // sensorCount - color-codes of the light sensors
        // 2 - left and right push of last frame
        // 2 outputs:
        // 2 - left and right push
        intelligence(4, &(std::array<unsigned, 3> {sensorCount + 2, 4, 2})[0])
    {
        inputs.resize(sensorCount + 2);
    }

    void update(float time);
    void render(sf::RenderWindow &window);
    float getSensorAngle(int i);
};

#endif/*BUG_HPP_INCLUDED*/
