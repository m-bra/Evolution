#ifndef OBJECT_HPP_INCLUDED
#define OBJECT_HPP_INCLUDED

struct Object
{
    // collision shape (r = radius, idiot!)
    float x, y, r;
    class World *world = 0;

    virtual void update(float time) {}
    virtual void onCollideWith(Object *other) {}
    virtual void render() {}
}

#endif/*OBJECT_HPP_INCLUDED*/
