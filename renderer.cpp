#include "renderer.hpp"
using namespace std;

Renderer::Renderer(int width, int height, int max_atoms, int max_fps = 60, std::string title = "Collision detection") {
    // initialize window
    this->window.create(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);

    // initialize max atoms
    this->max_atoms = max_atoms;

    // initialize width and height
    this->width = width;
    this->height = height;

    // initialize randomnes
    srand(time(NULL));

    // set framerate using clock object
    this->clock = sf::Clock();
    this->max_fps = max_fps;
}

void Renderer::render() {
    // iteration counter
    int iter = 0;
    // initialize solver
    PhysicsSolver solver(this->atoms, int(this->width), int(this->height), 0.01, 0.95, 8);
    // main loop
    while(this->window.isOpen()) {
        // reset clock
        this->clock.restart();
        // check for events
        sf::Event event;
        while(this->window.pollEvent(event)) {
            // close window if close button is pressed
            if (event.type == sf::Event::Closed) {
                this->window.close();
            }
        }
        
        // clear window
        this->window.clear();


        // until max atoms are reached spawn new atoms and add to vector
        // every 10 iterations try to spawn a new atom
        iter++;
        if (this->atoms.size() < this->max_atoms && iter == 10) {
            iter = 0;

            //float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/width));
            //float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/height));
            
            // preventing spawn on the edges
            //float x = rand() % int(this->width - 20) + 10;
            //float y = rand() % int(this->height - 20) + 10;

            // always spawn on top left corner if position is not already occupied
            int x = 10;
            int y = 10;
            float r = 10;

            if (this->atoms.size() > 0){
                bool spawn = true;
                for (int i = 0; i < this->atoms.size(); i++) {
                    if (std::sqrt((x - this->atoms.at(i)->getX()) * (x - this->atoms.at(i)->getX()) +
                        (y - this->atoms.at(i)->getY()) * (y - this->atoms.at(i)->getY())) < r*2){

                            cout << "posizione occupata" << endl;
                            spawn = false;
                            break;

                    }
                }
                if (spawn){
                    x = 10;
                    y = 10;
                    //float vx = rand() % 100 + 5;
                    //float vy = rand() % 100 + 5;
                    float vx = 0;
                    float vy = 0;
                    float ax = rand() % 10000 + 1;
                    float ay = rand() % 10000 + 1;

                    // create new atom
                    Atom* atom = new Atom(x, y, r, vx, vy, ax, ay);

                    // add atom to vector{
                    this->atoms.push_back(atom);
                }
            }else{
                // random radius
                //float r = rand() % 10 + 10;
                float r = 10;

                // random speed
                float vx = rand() % 1000 + 1;
                float vy = rand() % 1000 + 1;
                //float vx = 0;
                //float vy = 0;

                // random acceleration
                //float ax = rand() % 10 + 1;
                //float ay = rand() % 10 + 1;
                float ax = 0;
                float ay = 0;

                // create new atom
                Atom* atom = new Atom(x, y, r, vx, vy, ax, ay);

                // add atom to vector{
                this->atoms.push_back(atom);
            }
            /*
            // random radius
            //float r = rand() % 10 + 10;
            float r = 10;

            // random speed
            float vx = rand() % 4 + 1;
            float vy = rand() % 4 + 1;
            //float vx = 0;
            //float vy = 0;

            // random acceleration
            //float ax = rand() % 10 + 1;
            //float ay = rand() % 10 + 1;
            float ax = 0;
            float ay = 0;

            // create new atom
            Atom* atom = new Atom(x, y, r, vx, vy, ax, ay);

            // add atom to vector{
            this->atoms.push_back(atom);
            */
        }

        // THIS SECTION CAN BE PARALLELIZED:

        // update atoms positions through solver class
        solver.solve();

        // draw atoms
        for (int i = 0; i < this->atoms.size(); i++) {
            this->window.draw(this->atoms.at(i)->getCircle());
            if (i==0){
                //cout << this->atoms.at(i)->getX() << ", " << this->atoms.at(i)->getY() << ", "<< this->atoms.at(i)->getSpeed().x<< ", "<< this->atoms.at(i)->getSpeed().y<< endl;
            }
        }

        // display window
        this->window.display();

        // calculate time elapsed
        sf::Time elapsed = this->clock.getElapsedTime();


        // limit framerate to max_fps
        if (elapsed.asSeconds() < 1.0f / this->max_fps) {
            sf::sleep(sf::seconds(1.0f / this->max_fps) - elapsed);
        }

        // warn when framerate is below 60
        if (elapsed.asSeconds() > 1.0f / 60.0f) {
            // in red colour
            cout << "\033[1;31m";
            cout << "WARNING: framerate is below 60 FPS" << endl;
            cout << "\033[0m";
        }

        // log estimation of framerate
        cout << "FPS: " << 1.0f / this->clock.getElapsedTime().asSeconds() << endl;

    }
}