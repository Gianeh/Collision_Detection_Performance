// Multithreaded Verlet Physics Simulator
#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>

#include <thread>
#include <mutex>

#define title "Improved Particle Simulation"
#define substeps 4

struct Particle {
    sf::Vector2f position;
    sf::Vector2f prevPosition;
    sf::Vector2f velocity;
    float mass;
    float radius;
    float invMass; // Inverse of mass for efficient computation
    float restitution; // Coefficient of restitution for collision response
    sf::Color color;

    Particle(double x, double y, double vx, double vy, double m, double r) 
        : position(x, y), prevPosition(x, y), velocity(vx, vy), mass(m), radius(r),
          color(sf::Color(rand() % 255, rand() % 255, rand() % 255)) {
        invMass = (mass > 0) ? 1.0 / mass : 0;
        restitution = 0.85; // Adjust this value as needed
    }
};

class VerletPhysicsSimulator {
public:

    VerletPhysicsSimulator(int w, int h, int maxParticles, unsigned int th) : width(w), height(h), maxParticles(maxParticles), threads_num(th) {
        particles.reserve(maxParticles);
    }

    void addParticle(double x, double y, double vx, double vy, double mass, double radius) {
        if (canSpawnParticle(x, y, radius)) {
            particles.emplace_back(x, y, vx, vy, mass, radius);
        }
    }

    void update() {
        // measure update time
            auto update_start = std::chrono::high_resolution_clock::now();

        // Calcola il numero di thread disponibili sul sistema
        unsigned int numThreads;
        if (this->threads_num <= std::thread::hardware_concurrency()) {
            numThreads = this->threads_num;
        } else {
            numThreads = std::thread::hardware_concurrency();
        }
        std::cout << "using " << numThreads << " threads" << std::endl;
        std::vector<std::thread> threads(numThreads);

        // measure expended microseconds for thread assignments
            auto assign_start = std::chrono::high_resolution_clock::now();

        int blockSize = particles.size() / numThreads;  // number of particles assigned to a thread
        for (unsigned int i = 0; i < numThreads; ++i) {
            int startIdx = i * blockSize;
            int endIdx = (i + 1) * blockSize;
            if (i == numThreads - 1) {
                endIdx = particles.size();  // Assicurati che l'ultimo thread gestisca le rimanenti particelle
            }

            auto task_assignment_start = std::chrono::high_resolution_clock::now();

            // Crea un thread per aggiornare una porzione delle particelle
            threads[i] = std::thread([this, startIdx, endIdx]() {
                for (int j = startIdx; j < endIdx; ++j) {
                    // Apply gravity to ith particle
                    this->applyGravity(this->particles[j]);
                    // Esegui l'integrazione di Verlet per ogni particella nel blocco
                    sf::Vector2f tempPos = this->particles[j].position;
                    this->particles[j].position += this->particles[j].velocity * this->timeStep;
                    this->particles[j].prevPosition = tempPos;

                    // Gestisci la collisione con i bordi per ogni particella nel blocco
                    this->handleBoundaryCollision(this->particles[j]);
                }
            });

            auto task_assignment_stop = std::chrono::high_resolution_clock::now();

            auto task_assignment_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(task_assignment_stop - task_assignment_start);

            std::cout << "task assignment time: " << task_assignment_elapsed.count() << std::endl;

        }

                auto assign_stop = std::chrono::high_resolution_clock::now();

                auto assign_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(assign_stop - assign_start);

                std::cout << "thread assignment time: " << assign_elapsed.count() << std::endl;


        // measure expended microseconds for thread joining
                auto join_start = std::chrono::high_resolution_clock::now();

        // Aspetta che tutti i thread abbiano completato
        for (auto& t : threads) {
            t.join();
        }

                auto join_stop = std::chrono::high_resolution_clock::now();

                auto join_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(join_stop - join_start);

                std::cout << "thread joining time: " << join_elapsed.count() << std::endl;

        // Dopo che tutti i thread hanno completato l'aggiornamento delle posizioni e la gestione delle collisioni con i bordi,
        // gestisci le collisioni tra particelle in modo sequenziale o implementa una strategia parallela se necessario
        //handleParticleCollisions();

                auto update_stop = std::chrono::high_resolution_clock::now();

                auto update_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(update_stop - update_start);

                std::cout << "update time: " << update_elapsed.count() << std::endl;
                
    }


    void applyGravity(Particle& particle) {
        particle.velocity.y += gravity * timeStep;
    }

    void handleBoundaryCollision(Particle& particle) {
        // Corrected boundary collision handling with proper type casting
        if (particle.position.x - particle.radius < 0 || particle.position.x + particle.radius > width) {
            particle.velocity.x *= -bounceFactor;
            particle.position.x = std::max(particle.radius, std::min<float>(particle.position.x, width - particle.radius));
        }
        if (particle.position.y - particle.radius < 0 || particle.position.y + particle.radius > height) {
            particle.velocity.y *= -bounceFactor;
            particle.position.y = std::max(particle.radius, std::min<float>(particle.position.y, height - particle.radius));
        }
    }

    void handleParticleCollisions() {
        for (std::size_t i = 0; i < particles.size(); ++i) {
            for (std::size_t j = i + 1; j < particles.size(); ++j) {
                Particle &p1 = particles[i];
                Particle &p2 = particles[j];
                sf::Vector2f delta = p2.position - p1.position;
                float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y); // Use float instead of double
                float minDist = p1.radius + p2.radius; // Use float instead of double

                if (dist < minDist) {
                    sf::Vector2f normal = delta / dist; // No need for type casting, both delta and dist are floats
                    sf::Vector2f relativeVelocity = p2.velocity - p1.velocity;
                    float velocityAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y; // Use float

                    if (velocityAlongNormal > 0) continue; // They are moving apart

                    // Collision response
                    float e = std::min(p1.restitution, p2.restitution); // Use float
                    float j = (-(1 + e) * velocityAlongNormal) / (p1.invMass + p2.invMass); // Use float
                    sf::Vector2f impulse = j * normal; // No type conflict

                    p1.velocity -= p1.invMass * impulse;
                    p2.velocity += p2.invMass * impulse;

                    // Positional correction for sinking
                    const float percent = 0.8f; // Use float
                    const float slop = 0.1f; // Use float
                    //sf::Vector2f correction = std::max(dist - minDist - slop, 0.0f) / (p1.invMass + p2.invMass) * percent * normal; // Use float
                    sf::Vector2f correction = std::max(- dist + minDist + slop, 0.0f) / (p1.invMass + p2.invMass) * percent * normal; // Use float

                    /*
                    if (dist - minDist - slop < 0)
                        std::cout << "dist: " << dist << " minDist: " << minDist << " slop: " << slop << std::endl;

                    if (std::hypot(correction.x, correction.y) > 0 )
                        std::cout << "correction: " << std::hypot(correction.x, correction.y) << std::endl;
                    */

                    p1.position -= p1.invMass * correction;
                    p2.position += p2.invMass * correction;
                }
            }
        }
    }


    bool canSpawnParticle(double x, double y, double radius) {
        for (const auto& particle : particles) {
            double dx = x - particle.position.x;
            double dy = y - particle.position.y;
            double distance = std::hypot(dx, dy);
            if (distance < radius + particle.radius) {
                return false;
            }
        }
        return true;
    }

    const std::vector<Particle>& getParticles() const {
        return particles;
    }

private:

    unsigned int threads_num;

    std::vector<Particle> particles;

    int width;
    int height;

    int maxParticles;
    float gravity = 9.81f;  // Use float instead of double
    float timeStep = 0.16666667f / substeps;  // Use float instead of double
    float bounceFactor = 0.90f;  // Use float instead of double
};
