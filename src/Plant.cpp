#include "Plant.hpp"
#include "World.hpp"

#include <cstdlib>
#include <ctgmath>
#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>

template <typename T>
T clamp(T val, T min, T max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

void Plant::onObjectDeleted(Object *other)
{
    if (other == parent)
        parent = ancestor = 0;
}

void Plant::update(float time)
{
    nextParentCheck-= time;
    if (nextParentCheck < 0)
    {
        nextParentCheck = parentCheckLatency;

        Plant *currentParent = parent;
        if (!parent)
            ancestor = 0;
        else
            while (true)
            {
                if (!currentParent->parent)
                {
                    ancestor = currentParent;
                    break;
                }
                currentParent = currentParent->parent;
            }
    }

    // absorb energy from world
    if (energy < maxEnergy)
    {
        takeEnergy(world, energyPerSecond * time);
    }

    // if too much energy, explode to another child!
    if (energy > maxEnergy && (ancestor ? ancestor : this)->childrenCount < (ancestor ? ancestor : this)->maxChildren)
    {
        float newEnergy = energy * this->energyPass;
        float newRadius = newEnergy * radiusPerEnergy;

        float parentAngle = initialAngle - M_PI;

        // try to find a place for the newborn :)
        float nx, ny, angle;
        bool found = false;

        // if parent, opposite direction +-90°
        if (parent)
            angle = (parentAngle - M_PI) + ((rand() % 2001 / 1000.f)-1) * M_PI * .25;
        else
            angle = (rand() % 1001 / 1000.f) * M_PI * 2;

        float ndis = r*spreadFactor;
        nx = x + sin(angle) * ndis;
        ny = y + cos(angle) * ndis;

        // now check if this position is free of other objects
        std::vector<Object *> intersecting = world->getObjectsIntersectingCircle(nx, ny, ndis - r);
        if (intersecting.empty())
        {
            found = true;
        }
        else
        {
            // some enemy destroying shit
        }

        if (found)
        {
            Plant *child = new Plant(world, nx, ny, newEnergy, this);
            child->parent = this;
            child->ancestor = ancestor ? ancestor : this;
            child->initialAngle = angle;
            child->energyPerSecond = energyPerSecond;
            child->maxEnergy = clamp<float>((rand() % 201 / 100. - 1) * .04 + maxEnergy, .08, .3);
            child->spreadFactor = clamp<float>((rand() % 301 / 100. - 1.5) + spreadFactor, 3, 8);
            world->addObject(child);

            ++(ancestor ? ancestor : this)->childrenCount;
        }
    }

/*
    if (parent)
    {
        float const dx = parent->x - x;
        float const dy = parent->y - y;
        float const dis = sqrt(dx*dx + dy*dy);

        // keep initial angle!
        x = parent->x + sin(initialAngle) * dis;
        y = parent->y + cos(initialAngle) * dis;

        // go away from parent, we're adults now! :3
        if (false)
        {
            float const unitsPerSecond = .004;
            x-= dx / dis * time * unitsPerSecond;
            y-= dy / dis * time * unitsPerSecond;
        }
    }*/

    r = energy * radiusPerEnergy;
}

void Plant::render(sf::RenderWindow &window)
{
    static sf::CircleShape circle;
    static sf::Vertex line[2];

    unsigned long colorCode = (ancestor ? ancestor : this)->colorCode;
    sf::Color color = sf::Color(colorCode % 256, (colorCode / 256) % 256, (colorCode / 256 / 256) % 256);

    if (parent)
    {
        line[0].position.x = x;
        line[0].position.y = y;
        line[1].position.x = parent->x;
        line[1].position.y = parent->y;
        line[0].color = color;
        line[1].color = color;
        window.draw(line, 2, sf::Lines);
    }

    circle.setFillColor(color);
    circle.setPosition(x - r, y - r);
    circle.setRadius(r);
    window.draw(circle);
}
