#include <SFML/Graphics.hpp>
#include <complex>
#include <time.h>
#include "population.h"

// This program is a C++ recreation of Code Bullet's Dots program created in
// processing. The program simulates how AI's learn through the usage of
// natural selection.
//
// Each generation's population are generated with a set 
// of randomized directions. As each generation unfolds, a representative
// dot is chosen to lead the next generation of dots. Each individual dot's 
// direction is then manipulated to represent variations in their "genes."
//
// Due to the nature of OOP of C++, the program runs
// significantly slower than its processing counterpart.
//
// Challenges :
//  - The conversion of processing's pVectors to C++ 'vectors' denoted as 
//    complex numbers
//  - The optimization of objects according to OOP
//  - Usage of C++'s SFML library
//
// Time Complexity : O(n^2)

void setup(int screen_width, int screen_height, sf::RenderWindow& window) {
  std::string title = "Dots";
  window.create(sf::VideoMode(screen_width,screen_height,32), title);
  window.setFramerateLimit(100);
}

void draw(sf::RenderWindow& window, int screen_width, int screen_height, Population& population, std::complex<double> goal) {
  window.clear(sf::Color::Black);

  sf::CircleShape goalShape(5);
  goalShape.setFillColor(sf::Color(255,0,0));
  goalShape.setPosition(std::real(goal),std::imag(goal));

  window.draw(goalShape);

  if (population.allDotsDead()) {
    population.calcFitness(goal);
    population.naturalSelection(screen_width, screen_height);
    population.mutate();
  } else {
    population.update(goal, screen_width, screen_height);
    population.show();
    for (const auto& dot : population.getDots()) {
      window.draw(dot.getShape());
    }
    window.display();
  }
}

void handleInputEvents(sf::RenderWindow& window) {
  sf::Event event;
  while (window.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Escape) {
          window.close();
        }
        break;
      default:
        break;
    }
  }
}

int main() {
  srand(time(NULL));
  int screen_width = 800;
  int screen_height = 600;
  std::complex<double> goal(400,10);
  // Warning : framedrop higher than 250 
  Population population(goal, screen_width, screen_height, 250); 
  sf::RenderWindow window;
  setup(screen_width,screen_height, window);

  while (window.isOpen()) {
    handleInputEvents(window);
    draw(window, screen_width, screen_height, population, goal);
  }
  return 0;
}
