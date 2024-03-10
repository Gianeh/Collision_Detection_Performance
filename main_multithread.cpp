
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <chrono>

#include <thread>
#include <mutex>

#include "multithread.hpp"


#define width 2200
#define height 1300

#define BenchTime 1000

int main() {
    int maxparticle;
    std::cout << "Enter the number of particles: ";
    std::cin >> maxparticle;
    unsigned int numThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;
    VerletPhysicsSimulator simulator(width, height, maxparticle, numThreads);
    int frames = 0;

    // Add particles
    for (int k = 0; k < maxparticle; ++k) {

        double radius = 2.0;
        // random radius
        // double radius = static_cast<double>(rand() % 10 + 5);
        // double mass = 1.0;
        // based on radius mass
        double mass = std::pow(radius, 2) * 3.14;
        float vx = static_cast<float>(rand() % 20 - 10);
        float vy = static_cast<float>(rand() % 20 - 10);

        bool placed = false;
        while (!placed) {
            int x = rand() % int(width - radius * 2) + radius;
            int y = rand() % int(height - radius * 2) + radius;
            if (simulator.canSpawnParticle(x, y, radius)) {
                simulator.addParticle(x, y, vx, vy, mass, radius);
                placed = true;
            }
        }
    }

    sf::RenderWindow window(sf::VideoMode(width, height), title, sf::Style::Close | sf::Style::Titlebar);
    sf::Clock clock;

    int start = std::time(nullptr);

    // Benchmark time
    int btime = 0;
    std::vector <double> frames_per_second;

    while (window.isOpen()) {
        clock.restart();
        btime += 1;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // add particle on mouse click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            int x = sf::Mouse::getPosition(window).x;
            int y = sf::Mouse::getPosition(window).y;
            
            if (x >= 0 && y >= 0 && x <= width && y <= height){

                double radius = static_cast<double>(rand() % 100 + 10);
                double mass = std::pow(radius, 2) * 3.14;

                if (simulator.canSpawnParticle(x, y, radius)) {
                    simulator.addParticle(x, y, 0, 0, mass, radius);
                }
            }
        }

        auto update_start = std::chrono::high_resolution_clock::now();

        for (int step = 0; step < substeps; ++step) {
            simulator.update();
        }

        auto update_stop = std::chrono::high_resolution_clock::now();

        auto upate_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(update_stop - update_start);

        std::cout << "external update time: " << upate_elapsed.count() << std::endl;


        window.clear();
        
        for (const auto &particle : simulator.getParticles()) {
            sf::CircleShape circle(particle.radius);
            circle.setFillColor(particle.color);
            circle.setPosition(particle.position.x - particle.radius, particle.position.y - particle.radius);
            window.draw(circle);
        }
         //this section of code but parallelized on numThreads threads if there are enough threads available the max otherwise
        
        /*
        auto render_start = std::chrono::high_resolution_clock::now();

        if (numThreads <= std::thread::hardware_concurrency()) {
            numThreads = numThreads;
        } else {
            numThreads = std::thread::hardware_concurrency();
        }
        std::vector<std::thread> threads(numThreads);
        std::vector<std::vector<sf::CircleShape>> circlesPerThread(numThreads);

        int blockSize = simulator.getParticles().size() / numThreads;
        for (unsigned int i = 0; i < numThreads; ++i) {
            int startIdx = i * blockSize;
            int endIdx = (i + 1) * blockSize;
            if (i == numThreads - 1) {
                endIdx = simulator.getParticles().size();  // Assicurati che l'ultimo thread gestisca le rimanenti particelle
            }

            threads[i] = std::thread([&window, &simulator, &circlesPerThread, i, startIdx, endIdx] {
                sf::CircleShape circle;
                for (int j = startIdx; j < endIdx; ++j) {
                    const auto &particle = simulator.getParticles()[j];
                    circle.setRadius(particle.radius);
                    circle.setFillColor(particle.color);
                    circle.setPosition(particle.position.x - particle.radius, particle.position.y - particle.radius);
                    circlesPerThread[i].push_back(circle);
                }
            });
            
        }

        for (auto& t : threads) {
            t.join();
        }

        // Combine the results from all threads into a single vector
        std::vector<sf::CircleShape> circles;
        for (const auto &threadCircles : circlesPerThread) {
            circles.insert(circles.end(), threadCircles.begin(), threadCircles.end());
        }

        auto render_stop = std::chrono::high_resolution_clock::now();
        std::cout << "\n\nrender time: " << std::chrono::duration_cast<std::chrono::microseconds>(render_stop - render_start).count() << std::endl;


        auto draw_start = std::chrono::high_resolution_clock::now();

        for (const auto &circle : circles) {
            window.draw(circle);
        }

        auto draw_stop = std::chrono::high_resolution_clock::now();
        std::cout << "\n\ndraw time: " << std::chrono::duration_cast<std::chrono::microseconds>(draw_stop - draw_start).count() << std::endl;

        */

        window.display();

        sf::Time elapsed = clock.getElapsedTime();
        sf::sleep(sf::seconds(1.0f / 60.0f - elapsed.asSeconds()));

        // log fps
        std::cout << "\033[1;31mfps: \033[0m" << 1.0f / clock.getElapsedTime().asSeconds() << std::endl;
        frames_per_second.push_back(1.0f / clock.getElapsedTime().asSeconds());
        // log the number of threads used
        std::cout << "used threads: " << numThreads << std::endl;

        frames++;

        if (frames == 60){
            std::cout << "elapsed time: " << (std::time(nullptr) - start) << std::endl;
            frames = 0;
            start = std::time(nullptr);
        }

        if (btime == BenchTime)
            break;
    }
    // log the average fps
    double sum = 0;
    for (int i = 0; i < frames_per_second.size(); i++){
        sum += frames_per_second[i];
    }
    std::cout << "average fps: " << sum / frames_per_second.size() << std::endl;

    return 0;
}
