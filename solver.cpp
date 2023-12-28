#include "solver.hpp"


    PhysicsSolver::PhysicsSolver(std::vector<Atom>& atoms, int width, int height, float dt = 0.1, int substeps = 8)
        : atoms(atoms), width(width), height(height), dt(dt), substeps(substeps) {}

    void PhysicsSolver::apply_gravity(Atom& atom) {
        atom.addG(9.8);
    }

    void PhysicsSolver::verlet_integration(Atom& atom) {
        sf::Vector2f temp_position = atom.getPosition();
        sf::Vector2f new_position = 2.0f * atom.getPosition() - atom.getPrevPosition() + atom.getAcceleration() * dt * dt;
        atom.setPrevPosition(temp_position);
        atom.setPosition(new_position);
    }

    void PhysicsSolver::handle_collisions(Atom& atom) {
        // Collision with walls
        if (atom.getX() - atom.getRadius() < 0) {
            atom.setSpeed(sf::Vector2f(-atom.getSpeed().x, atom.getSpeed().y));
        } else if (atom.getX() + atom.getRadius() > width) {
            atom.setSpeed(sf::Vector2f(-atom.getSpeed().x, atom.getSpeed().y));
        }

        if (atom.getY() - atom.getRadius() < 0) {
            atom.setSpeed(sf::Vector2f(atom.getSpeed().x, -atom.getSpeed().y));
        } else if (atom.getY() + atom.getRadius() > height) {
            atom.setSpeed(sf::Vector2f(atom.getSpeed().x, -atom.getSpeed().y));
        }

        // Collision between atoms
        for (Atom& other : atoms) {
            if (&atom != &other) {
                float distance = std::sqrt((atom.getX() - other.getX()) * (atom.getX() - other.getX()) +
                                           (atom.getY() - other.getY()) * (atom.getY() - other.getY()));

                float min_distance = atom.getRadius() + other.getRadius();
                if (distance < min_distance) {
                    // Handle collision response (simple bounce)
                    sf::Vector2f normal = sf::Vector2f(atom.getX() - other.getX(), atom.getY() - other.getY());
                    normal = normal / distance; // Normalize

                    sf::Vector2f relative_velocity = atom.getSpeed() - other.getSpeed();
                    float relative_speed = relative_velocity.x * normal.x + relative_velocity.y * normal.y;

                    if (relative_speed < 0) {
                        sf::Vector2f impulse = (2.0f * relative_speed / (atom.getMass() + other.getMass())) * normal;

                        atom.setSpeed(atom.getSpeed() - impulse / atom.getMass());
                        other.setSpeed(other.getSpeed() + impulse / other.getMass());
                    }
                }
            }
        }
    }

    void PhysicsSolver::solve() {
        for (int step = 0; step < substeps; ++step) {
            for (Atom& atom : atoms) {
                atom.setAcc(0.0, 0.0); // Reset acceleration at the beginning of each substep

                // Apply external forces (e.g., gravity)
                apply_gravity(atom);

                // Update particle positions using Verlet integration
                verlet_integration(atom);

                // Handle collisions
                handle_collisions(atom);
            }
        }
    }