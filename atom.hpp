#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class Atom{

    private:

        sf::Vector2f position;
        sf::Vector2f speed;
        sf::Vector2f acceleration;
        sf::Vector2f prevPosition;
        float radius;

        // circle body in sfml
        sf::CircleShape circle;
        sf::Color color;

    public:

        Atom(float x, float y, float r, float vx, float vy, float ax, float ay);

        // Getters
        sf::Vector2f getPosition();
        sf::Vector2f getPrevPosition();
        sf::Vector2f getSpeed();
        sf::Vector2f getAcceleration();
        sf::CircleShape getCircle();
        float getX();
        float getY();
        float getRadius();
        float getMass();
        void addG(float g);
        void setAcc(float ax, float ay);
        void setPosition(sf::Vector2f nPosition);
        void setPrevPosition(sf::Vector2f pPosition);
        void addPosition(float px, float py);
        void setSpeed(sf::Vector2f nSpeed);
};