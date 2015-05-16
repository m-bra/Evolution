#include "World.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <ctgmath>

float const timeSpeed = 2;

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "Plants'n'Bugs");
    window.setFramerateLimit(30);
    bool running = true;

    sf::View view(sf::Vector2f(0, 0), sf::Vector2f(2, 2));
    window.setView(view);

    World world;

    sf::Clock clock;

    bool dragging;
    int lastMouseX, lastMouseY;
    int oldWinX = window.getSize().x, oldWinY = window.getSize().y;
    Object *following = 0;

    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                running = false;
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    dragging = true;
                    lastMouseX = sf::Mouse::getPosition(window).x, lastMouseY = sf::Mouse::getPosition(window).y;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Right)
                    dragging = false;
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f coords = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    std::vector<Object *> intersecting = world.getObjectsIntersectingPoint(coords.x, coords.y);
                    following = intersecting.size() ? intersecting[0] : 0;
                }

                break;
            case sf::Event::MouseMoved:
                if (dragging)
                {
                    view.move(((float)lastMouseX - event.mouseMove.x) * view.getSize().x / window.getSize().x, ((float)lastMouseY - event.mouseMove.y) * view.getSize().y / window.getSize().y);
                    lastMouseX = event.mouseMove.x;
                    lastMouseY = event.mouseMove.y;
                }
                break;
            case sf::Event::MouseWheelMoved:
                view.setSize(view.getSize() * (float)pow(2, event.mouseWheel.delta / 2.));\
                break;
            case sf::Event::Resized:
                view.setSize(view.getSize().x / (float)oldWinX * window.getSize().x, view.getSize().y / (float)oldWinY *  window.getSize().y);
                oldWinX = window.getSize().x, oldWinY = window.getSize().y;
                break;
            default: break;
            }
        }

        world.update(clock.restart().asSeconds() * timeSpeed);

        window.clear();

        if (following)
            view.setCenter(sf::Vector2f(following->x, following->y));

        window.setView(view);
        world.render(window);

        window.display();
    }
    return 0;
}
