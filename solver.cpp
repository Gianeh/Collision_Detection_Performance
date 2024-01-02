#include "solver.hpp"

PhysicsSolver::PhysicsSolver(std::vector<Atom*>& atoms, int width, int height, float dt, float elasticity, int substeps)
    : atoms(atoms), width(width), height(height), dt(dt), elasticity(elasticity), substeps(substeps) {}

void PhysicsSolver::apply_gravity(Atom& atom) {
    // Apply constant gravitational force to the atom
    atom.addG(98100);
}

void PhysicsSolver::verlet_integration(Atom& atom) {
    // Update atom positions using Verlet integration
    sf::Vector2f temp_position = atom.getPosition();
    sf::Vector2f new_position = temp_position + atom.getSpeed() * dt + 0.5f * atom.getAcceleration() * dt * dt;
    atom.setPrevSpeed(atom.getSpeed());
    atom.setPrevPosition(temp_position);
    atom.setPosition(new_position);

    // Update acceleration for Verlet integration
    sf::Vector2f newAcceleration = (atom.getSpeed() - atom.getPrevSpeed()) / dt;
    atom.setAcc(newAcceleration);
}

void PhysicsSolver::handle_collisions_boundary(Atom& atom) {
    // Verify if there's collision with the floor
    if (atom.getY() + atom.getRadius() >= height) {
        // Calculate the overlap
        float overlap = atom.getY() + atom.getRadius() - height;

        // Calculate the impulse
        float impulse = -2.0f * atom.getMass() * atom.getSpeed().y / dt;
        float elasticity_factor = 1.0f + this->elasticity;  // Elasticity factor

        // Update the velocity
        atom.setSpeed(sf::Vector2f(atom.getSpeed().x, -impulse / atom.getMass() * elasticity_factor));

        // Update the position (move away from the boundary)
        atom.setPosition(sf::Vector2f(atom.getX(), height - 2 * overlap));
    }

    // Check left boundary
    if (atom.getX() - atom.getRadius() < 0) {
        // Reflect off the left boundary
        atom.setSpeed(sf::Vector2f(-atom.getSpeed().x, atom.getSpeed().y));
        // Move the atom away from the boundary
        atom.setPosition(sf::Vector2f(atom.getRadius(), atom.getY()));
    }

    // Check right boundary
    else if (atom.getX() + atom.getRadius() > width) {
        // Reflect off the right boundary
        atom.setSpeed(sf::Vector2f(-atom.getSpeed().x, atom.getSpeed().y));
        // Move the atom away from the boundary
        atom.setPosition(sf::Vector2f(width - atom.getRadius(), atom.getY()));
    }

    // Check top boundary
    if (atom.getY() - atom.getRadius() < 0) {
        // Reflect off the top boundary
        atom.setSpeed(sf::Vector2f(atom.getSpeed().x, -atom.getSpeed().y));
        // Move the atom away from the boundary
        atom.setPosition(sf::Vector2f(atom.getX(), atom.getRadius()));
    }
}

void PhysicsSolver::handle_collisions_atoms(Atom& atom, Atom& other) {
    // Calculate the distance between the atoms as absolute value of the difference between their positions
    float distance = std::abs(std::sqrt((atom.getX() - other.getX()) * (atom.getX() - other.getX()) + (atom.getY() - other.getY()) * (atom.getY() - other.getY())));

    // If the atoms are overlapping, resolve the collision
    if (distance <= atom.getRadius() + other.getRadius()) {
        // Calculate the relative velocity
        sf::Vector2f relative_velocity = atom.getSpeed() - other.getSpeed();

        // Calculate the impulse
        float impulse = -2.0f * (atom.getMass() * other.getMass()) / (atom.getMass() + other.getMass()) * relative_velocity.dot(atom.getPosition() - other.getPosition()) / distance;

        // Update the velocities
        atom.setSpeed(atom.getSpeed() + impulse / atom.getMass());
        other.setSpeed(other.getSpeed() - impulse / other.getMass());

        // Update the positions (move the atoms apart)
        atom.setPosition(atom.getPosition() + (atom.getRadius() + other.getRadius() - distance) / 2.0f * relative_velocity.normalized());
        other.setPosition(other.getPosition() - (atom.getRadius() + other.getRadius() - distance) / 2.0f * relative_velocity.normalized());
    }
}

void PhysicsSolver::solve() {
    for (int step = 0; step < substeps; ++step) {
        for (Atom* atom : atoms) {
            // Apply external forces (e.g., gravity)
            apply_gravity(*atom);

            // Update particle positions using Verlet integration
            verlet_integration(*atom);

            // Handle collisions
            handle_collisions(*atom);
        }
    }
}
