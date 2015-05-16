#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <iostream>

struct Object
{
    // collision shape (r = radius, idiot!)
    float x, y, r;
    float code = 1;
    class World *world = 0;

    Object(class World *world, float x, float y, float r, float code = 0) : world(world), x(x), y(y), r(r) {}
    virtual ~Object() {}

    virtual void update(float time) {}
    virtual void onCollideWith(Object *other, float time)
    {
        std::cout << "Object::onCollideWith() should not be reached\n";
        exit(EXIT_FAILURE); // should not be reached
        float dx = x - other->x;
        float dy = y - other->y;
        x+= time * dx * .5;
        y+= time * dy * .5;
    }
    virtual void render(sf::RenderWindow &renderWindow) {}
    // notify object that another object is dead and pointers to it are not guaranteed to be valid
    virtual void onObjectDeleted(Object *other) {}
    virtual void onDead() {}

    void setDead()
    {
        dead = true;
        onDead();
    }

    bool isDead()
    {
        return dead;
    }

private:
    bool dead = false;
};

#endif/*OBJECT_HPP_INCLUDED*/
