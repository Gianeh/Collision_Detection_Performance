#include "atom.hpp"

Atom::Atom(float x, float y, float r, float vx, float vy, float ax, float ay) {
    position = sf::Vector2f(x, y);
    prevPosition = position;
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

sf::Vector2f Atom::getPrevPosition(){
    return this->prevPosition;
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

float Atom::getMass(){
    float aRadius = this->radius;
    return aRadius*aRadius*3.1416;
}

void Atom::addG(float g){
    this->acceleration += sf::Vector2f(0,g);
}

void Atom::setAcc(float ax,float ay){
    this->acceleration = sf::Vector2f(ax,ay);
}

void Atom::setPosition(sf::Vector2f nPosition){
    this->position = nPosition;
}

void Atom::setPrevPosition(sf::Vector2f pPosition){
    this->prevPosition = pPosition;
}

void Atom::setSpeed(sf::Vector2f nSpeed){
    this->speed = nSpeed;
}

void Atom::addPosition(float px, float py){
    this->position += sf::Vector2f(px,py);  
}
