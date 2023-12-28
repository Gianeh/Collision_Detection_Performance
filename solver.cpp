#include "solver.hpp"


    PhysicsSolver::PhysicsSolver(std::vector<Atom*>& atoms, int width, int height, float dt, int substeps)
        : atoms(atoms), width(width), height(height), dt(dt), substeps(substeps) {}

    void PhysicsSolver::apply_gravity(Atom& atom) {
        atom.addG(9.8);
    }

    void PhysicsSolver::verlet_integration(Atom& atom) {
        sf::Vector2f temp_position = atom.getPosition();
        sf::Vector2f new_position = temp_position + atom.getSpeed() * dt + 0.5f * atom.getAcceleration() * dt * dt;
        atom.setPrevPosition(temp_position);
        atom.setPosition(new_position);
    }

    void PhysicsSolver::handle_collisions(Atom& atom) {
    float elasticity = 0.99; // Adjust this value for desired elasticity
    if (atom.getX() - atom.getRadius() < 0) {
        //std::cout << "sono entrato qui <0 x" << std::endl;
            atom.setSpeed(sf::Vector2f(-atom.getSpeed().x*elasticity,atom.getSpeed().y));
        } else if (atom.getX() + atom.getRadius() > width) {
            //std::cout << "sono entrato qui > width x" << std::endl;
            atom.setSpeed(sf::Vector2f(-atom.getSpeed().x*elasticity, atom.getSpeed().y));
        }

        if (atom.getY() - atom.getRadius() < 0) {
            //std::cout << "sono entrato qui <0 y" << std::endl;
            atom.setSpeed(sf::Vector2f(atom.getSpeed().x, -atom.getSpeed().y*elasticity));
        } else if (atom.getY() + atom.getRadius() > height){
            //std::cout << "sono entrato qui > height y" << std::endl;
            atom.setSpeed(sf::Vector2f(atom.getSpeed().x, -atom.getSpeed().y*elasticity));
        }
    // Collision between atoms
        for (Atom* other : atoms) {
            if (other != &atom) {
                float distance = std::sqrt((atom.getX() - other->getX()) * (atom.getX() - other->getX()) +
                                        (atom.getY() - other->getY()) * (atom.getY() - other->getY()));

                float min_distance = atom.getRadius() + other->getRadius();
                if (distance < min_distance) {
                    // Handle collision response (elastic bounce)
                    sf::Vector2f normal = sf::Vector2f(atom.getX() - other->getX(), atom.getY() - other->getY());
                    normal = normal / distance; // Normalize

                    sf::Vector2f relative_velocity = atom.getSpeed() - other->getSpeed();
                    float relative_speed = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

                    if (relative_speed < 0) {
                        // Calculate impulse
                        float impulse = (1.0f + elasticity) * relative_speed / (1 / atom.getMass() + 1 / other->getMass());

                        // Apply impulse to atoms
                        atom.setSpeed(atom.getSpeed() - impulse / atom.getMass() * normal);
                        other->setSpeed(other->getSpeed() + impulse / other->getMass() * normal);
                    }
                }
            }
        }
    // ...
    }

    void PhysicsSolver::solve() {
        for (int step = 0; step < substeps; ++step) {
            for (Atom* atom : atoms) {
                atom->setAcc(0.0, 0.0); // Reset acceleration at the beginning of each substep

                // Apply external forces (e.g., gravity)
                atom->addG(9.81);

                // Update particle positions using Verlet integration
                verlet_integration(*atom);

                // Handle collisions
                handle_collisions(*atom);
            }
        }
    }