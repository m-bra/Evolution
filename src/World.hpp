#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "Object.hpp"
#include <vector>

struct World
{
    std::vector<Object *> objects;

    void update(float time)
    {
        for (Object &obj : objects)
            obj.update(time);
    }

    void render()
    {
        for (Object &obj : objects)
            obj.render();
    }
};

#endif/*WORLD_HPP_INCLUDED*/
