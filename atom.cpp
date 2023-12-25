#include "atom.hpp"

Atom::Atom(float x, float y, float r, float vx, float vy, float ax, float ay) {
    position = sf::Vector2f(x, y);
    speed = sf::Vector2f(vx, vy);
    acceleration = sf::Vector2f(ax, ay);
    radius = r;
    // circle body
    circle = sf::CircleShape(radius);
    circle.setFillColor(sf::Color::White);
    // set position of circle
    circle.setPosition(position - sf::Vector2f(radius, radius));
}

sf::Vector2f Atom::getPosition() {
    return this->position;
}

sf::Vector2f Atom::getSpeed() {
    return this->speed;
}

sf::Vector2f Atom::getAcceleration() {
    return this->acceleration;
}

sf::CircleShape Atom::getCircle() {
    return this->circle;
}

float Atom::getX() {
    return this->position.x;
}

float Atom::getY() {
    return this->position.y;
}

float Atom::getRadius() {
    return this->radius;
}
