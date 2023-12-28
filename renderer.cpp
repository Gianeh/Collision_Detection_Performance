#include "renderer.hpp"
using namespace std;

Renderer::Renderer(int width, int height, int max_atoms, std::string title = "Collision detection") {
    // initialize window
    this->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

    // initialize max atoms
    this->max_atoms = max_atoms;

    // initialize width and height
    this->width = width;
    this->height = height;

    // initialize randomnes
    srand(time(NULL));
}

void Renderer::render() {
    // clear window
    this->window.clear();


    // until max atoms are reached spawn new atoms and add to vector
    if (this->atoms.size() < this->max_atoms) {

        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/width));
        float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/height));
        // random radius
        float r = rand() % 10 + 10;
        //float r = 10;

        // random speed
        float vx = rand() % 10 + 1;
        float vy = rand() % 10 + 1;
        //float vx = 0;
        //float vy = 0;

        // random acceleration
        float ax = rand() % 10 + 1;
        float ay = rand() % 10 + 1;
        //float ax = 0;
        //float ay = 0;

        // create new atom
        Atom* atom = new Atom(x, y, r, vx, vy, ax, ay);

        // add atom to vector{
        this->atoms.push_back(atom);
    }

    // THIS SECTION CAN BE PARALLELIZED:

    // update atoms positions through solver class

    //solver.solve(this->atoms);

    // draw atoms
    for (int i = 0; i < this->atoms.size(); i++) {
        this->window.draw(this->atoms.at(i)->getCircle());
        cout << this->atoms.at(i)->getX() << ", " << this->atoms.at(i)->getY() << ", "<< this->atoms.at(i)->getRadius() << endl;
    }

    // display window
    this->window.display();
}