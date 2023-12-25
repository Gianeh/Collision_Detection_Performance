#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Atom{

    private:

        sf::Vector2f position;
        sf::Vector2f speed;
        sf::Vector2f acceleration;
        float radius;

        // circle body in sfml
        sf::CircleShape circle;

    public:

        Atom(float x, float y, float r, float vx, float vy, float ax, float ay);

        // Getters
        sf::Vector2f getPosition();
        sf::Vector2f getSpeed();
        sf::Vector2f getAcceleration();
        sf::CircleShape getCircle();
        float getX();
        float getY();
        float getRadius();
};