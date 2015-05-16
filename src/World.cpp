#include "World.hpp"

World::World() : HasEnergy(25)
{
    for (int i = 0; i < 4; ++i)
    {
        Plant *plant = new Plant(this, rand() % 200 / 100.f - 1, rand() % 200 / 100.f - 1, .2, this);
        objects.push_back(plant);
    }

    for (int i = 0; i < 4; ++i)
    {
        Bug *bug = new Bug(this, rand() % 200 / 100.f - 1, rand() % 200 / 100.f - 1 , 1, this);
        objects.push_back(bug);
    }
}

void World::update(float time)
{
    // stupid O(n2)
    /*
    for (Object *first : objects)
        for (Object *second : objects)
            if (first != second)
            {
                float disX = second->x - first->x;
                float disY = second->y - first->y;
                float dis = sqrt(disX*disX + disY*disY);
                if (dis <= first->r + second->r)
                    first->onCollideWith(second, time);
            }*/

    for (Object *obj : objects)
        obj->update(time);

    for (Object *obj : newObjects)
        objects.push_back(obj);
    newObjects.clear();

    //TODO: prevIter necessary or just dont jump when erasing?
    //auto prevIter = 0;
    auto iter = objects.begin();
    while (iter != objects.end())
    {
        if ((*iter)->isDead())
        {
            HasEnergy *hasEnergy = dynamic_cast<HasEnergy *>(*iter);
            if (hasEnergy)
                takeAllEnergy(hasEnergy);
            graveyard.push_back(*iter);
            objects.erase(iter);
            //iter = prevIter ? prevIter + 1 : objects.begin();
        }
        else
        {
            ++iter;
            //prevIter = prevIter ? prevIter + 1 : objects.begin();
        }
    }

    // clean graveyard
    for (Object *obj : graveyard)
    {
        for (Object *obj2 : objects)
            obj2->onObjectDeleted(obj);
        delete obj;
    }
    graveyard.clear();
}

// http://stackoverflow.com/a/100165/2979958
bool World::SegmentIntersectRectangle(float a_rectangleMinX, float a_rectangleMinY, float a_rectangleMaxX, float a_rectangleMaxY,
    float a_p1x, float a_p1y, float a_p2x, float a_p2y)
{
    double minX = a_p1x;
    double maxX = a_p2x;

    if(a_p1x > a_p2x)
    {
        minX = a_p2x;
        maxX = a_p1x;
    }

    if(maxX > a_rectangleMaxX)
        maxX = a_rectangleMaxX;

    if(minX < a_rectangleMinX)
        minX = a_rectangleMinX;

    if(minX > maxX)
        return false;

    double minY = a_p1y;
    double maxY = a_p2y;

    double dx = a_p2x - a_p1x;

    if(fabs(dx) > 0.0000001)
    {
        double a = (a_p2y - a_p1y) / dx;
        double b = a_p1y - a * a_p1x;
        minY = a * minX + b;
        maxY = a * maxX + b;
    }

    if(minY > maxY)
    {
        double tmp = maxY;
        maxY = minY;
        minY = tmp;
    }

    if(maxY > a_rectangleMaxY)
        maxY = a_rectangleMaxY;

    if(minY < a_rectangleMinY)
        minY = a_rectangleMinY;

    if(minY > maxY) // If Y-projections do not intersect return false
        return false;
    return true;
}
