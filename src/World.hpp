#ifndef WORLD_HPP_INCLUDED
#define WORLD_HPP_INCLUDED

#include "Object.hpp"
#include "HasEnergy.hpp"

#include "Plant.hpp"
#include "Bug.hpp"

#include <vector>
#include <ctgmath>

class World : public HasEnergy
{
    std::vector<Object *> objects, newObjects, graveyard;
    bool SegmentIntersectRectangle(float a_rectangleMinX, float a_rectangleMinY, float a_rectangleMaxX, float a_rectangleMaxY,
        float a_p1x, float a_p1y, float a_p2x, float a_p2y);

public:

    World();

    void addObject(Object *obj)
    {
        newObjects.push_back(obj);
    }

    Object *getObject(int i)
    {
        return objects.at(i);
    }

    std::vector<Object *> getObjectsIntersectingPoint(float x, float y)
    {
        std::vector<Object *> intersecting;

        for (Object *obj : objects)
        {
            float disX = obj->x - x;
            float disY = obj->y - y;
            float dis2 = disX*disX + disY*disY;
            if (dis2 < obj->r*obj->r)
                intersecting.push_back(obj);
        }

        return intersecting;
    }

    std::vector<Object *> getObjectsIntersectingCircle(float x, float y, float r)
    {
        std::vector<Object *> intersecting;

        for (Object *obj : objects)
        {
            float disX = obj->x - x;
            float disY = obj->y - y;
            float dis = sqrt(disX*disX + disY*disY);
            if (dis < r+obj->r)
                intersecting.push_back(obj);
        }

        return intersecting;
    }

    std::vector<Object *> getObjectsIntersectingSegment(float x1, float y1, float x2, float y2)
    {
        std::vector<Object *> intersecting;

        for (Object *obj : objects)
        {
            if (SegmentIntersectRectangle(
                    obj->x - obj->r, obj->y - obj->r, obj->x + obj->r, obj->y + obj->r,
                    x1, y1, x2, y2))
                intersecting.push_back(obj);
        }

        return intersecting;
    }

    Object *getNearestObject(Object **objects, int objectCount, float x, float y)
    {
        Object *nearest = objects[0];
        float dis2 = (nearest->x-x)*(nearest->x-x)+(nearest->y-y)*(nearest->y-y);
        for (int i = 1; i < objectCount; ++i)
        {
            float dis2_i = (objects[i]->x-x)*(objects[i]->x-x)+(objects[i]->y-y)*(objects[i]->y-y);
            if (dis2_i > dis2)
            {
                nearest = objects[i];
                dis2 = dis2_i;
            }
        }

        return nearest;
    }

    void update(float time);

    void render(class sf::RenderWindow &renderWindow)
    {
        for (Object *obj : objects)
            obj->render(renderWindow);
    }
};

#endif/*WORLD_HPP_INCLUDED*/
