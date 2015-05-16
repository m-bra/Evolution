#include "Bug.hpp"

#include "World.hpp"

#include <ctgmath>
#include <algorithm>

float Bug::getSensorAngle(int i)
{
    return  angle + (i / float(sensorCount - 1) - .5) * M_PI;
}

void Bug::update(float time)
{
    if (energy < 0)
        setDead();

    if (isDead())
        return;

    // for now it is assumable that the bug lives and has energy

    nextSensorCheck-= time;
    if (nextSensorCheck < 0)
    {
        nextSensorCheck = sensorLatency;

        // update sensors
        for (int i = 0; i < sensorCount; ++i)
        {
            float sensorAngle = getSensorAngle(i);

            float sensorX1 = x + cos(sensorAngle) * r * 1.01;
            float sensorY1 = y + sin(sensorAngle) * r * 1.01;
            float sensorX2 = x + cos(sensorAngle) * sensorRange;
            float sensorY2 = y + sin(sensorAngle) * sensorRange;

            std::vector<Object *> intersecting = world->getObjectsIntersectingSegment(sensorX1, sensorY1, sensorX2, sensorY2);

            if (intersecting.empty())
                inputs[i] = 0;
            else {
                Object *obj;
                if (intersecting.size() == 1)
                    obj = intersecting[0];
                else
                    obj = world->getNearestObject(intersecting.data(), intersecting.size(), x, y);

                inputs[i] = (obj->x - x) * (obj->x - x) + (obj->y - y) * (obj->y - y);
            }
        }

        inputs[sensorCount] = leftPush;
        inputs[sensorCount + 1] = rightPush;

        // calculate movement
        if (useFixedIntelligence)
        {
            leftPush/= 3;
            rightPush/= 3;

            int pushCount = 0;
            for (int i = 0; i < sensorCount; ++i)
            {
                if (inputs[i] > .01)
                {
                    ++pushCount;

                    float sensorElong = (i - (sensorCount - 1) / 2.) / ((sensorCount - 1) / 2.);
                    // damping of objects which are not in the center of view
                    float centerDamping = .4 + .6 * (1 - fabs(sensorElong));

                    leftPush+= .05 * centerDamping * (1 + 2 * sensorElong);
                    rightPush+= .05 * centerDamping * (1 - 2  * sensorElong);
                }
            }

            if (pushCount > 0)
            {
                leftPush/= (float) pushCount;
                rightPush/= (float) pushCount;
                //leftPush+= (rand() % 200 / 100. - 1) * .002;
        //rightPush+= (rand() % 200 / 100. - 1) * .002;
            }
            else
            {
                leftPush+= (rand() % 200 / 100. - 1) * .01;
                rightPush+= (rand() % 200 / 100. - 1) * .04;
            }

            // adjust sensor range
            if (pushCount < (sensorCount - 1) / 4.)
                sensorRange+= time;
            else if (pushCount > (sensorCount - 1) / 4. * 3)
                sensorRange-= time;

            sensorRange = std::min(3.f, std::max(0.f, sensorRange));
        }
        else
        {
            std::vector<nn_real> outputs(2);
            intelligence.calculate(&inputs[0], &outputs[0]);
            leftPush = outputs[0];
            rightPush = outputs[1];
        }
    }

    // apply movement
    angle+= (leftPush - rightPush);
    x+= cos(angle) * (leftPush + rightPush) * time;
    y+= sin(angle) * (leftPush + rightPush) * time;
    giveEnergy(world, (leftPush + rightPush) * time / 2);

    // check for collisions
    std::vector<Object *> intersecting = world->getObjectsIntersectingCircle(x, y, r);
    for (Object *obj : intersecting)
    {
        Plant *plant = dynamic_cast<Plant *>(obj);
        if (plant)
        {
            plant->setDead();
            takeAllEnergy(plant);
        }

        Bug *bug = dynamic_cast<Bug *>(obj);
        if (bug)
        {
            if (this->energy > this->maxEnergy && bug->energy < bug->maxEnergy)
                giveEnergy(bug, maxEnergy - energy);
            if (this->energy < this->maxEnergy && this->energy > bug->energy * 2)
                takeAllEnergy(bug);
        }
    }

    // reproduce
    lastReproduction+= time;
    if (lastReproduction > reproductionLatency && energy > maxEnergy * 1.6)
    {
        lastReproduction = 0;
        Bug *child = new Bug(world, x + cos(angle - M_PI) * r * 3, y + sin(angle - M_PI) * r * 3, energy / 2, this);
        child->angle = angle - M_PI;
        world->addObject(child);
    }
}

void Bug::render(sf::RenderWindow &window)
{
    static sf::CircleShape mainCircle;
    float clampedEnergy = std::min(std::max(energy, 0.f), 1.f);
    mainCircle.setFillColor(sf::Color(255, 255 - clampedEnergy * 255, 255 - clampedEnergy * 255));
    mainCircle.setRadius(r);
    mainCircle.setPosition(x - r, y - r);
    window.draw(mainCircle);

    for (int i = 0; i < sensorCount; ++i)
    {
        static sf::CircleShape sensorCircle;
        sf::Color sensorColor = inputs[i] ? sf::Color::Green : sf::Color::Black;
        float sensorRadius = r / 10;

        sensorCircle.setFillColor(sensorColor);
        sensorCircle.setRadius(sensorRadius);
        float sensorAngle = getSensorAngle(i);
        sensorCircle.setPosition(x + cos(sensorAngle) * r - sensorRadius, y + sin(sensorAngle) * r - sensorRadius);
        window.draw(sensorCircle);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            sf::Vertex vertices[] =
            {
                 sf::Vertex(
                     sf::Vector2f(
                         x + cos(sensorAngle) * r - sensorRadius,
                         y + sin(sensorAngle) * r - sensorRadius),
                    inputs[i] ? sf::Color::Green : sf::Color::Red),
                 sf::Vertex(
                     sf::Vector2f(
                         x + cos(sensorAngle) * (r + sensorRange) - sensorRadius,
                         y + sin(sensorAngle) * (r + sensorRange) - sensorRadius),
                    inputs[i] ? sf::Color::Green : sf::Color::Red),
            };
            window.draw(vertices, 2, sf::Lines);
        }
    }
}
