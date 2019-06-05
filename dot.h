#ifndef DOT_H
#define DOT_H

#include <complex>
#include "brain.h"

class Dot {
  public:
    Dot(int screen_width, int screen_height);
    Dot(int screen_width, int screen_height, Brain brain);
    void show();
    void move();
    void update(std::complex<double> goal, int screen_width, int screen_height);
    void mutate();
    double calcDist(std::complex<double> pos, std::complex<double> goal);
    void calcFitness(std::complex<double> goal);
    Dot createChild(int screen_width, int screen_height);

    void setBrain(Brain brain);
    void setDead(bool dead);
    void setIsBest(bool isBest);

    sf::CircleShape getShape() const;
    std::complex<double> getPos() const;
    Brain getBrain() const;
    bool getDead() const;
    bool getReachedGoal() const;
    double getFitness() const;
  private:
    sf::CircleShape shape;

    std::complex<double> pos;
    std::complex<double> vel;
    std::complex<double> acc;

    Brain brain;
    bool dead;
    bool reachedGoal;
    bool isBest;

    double fitness;
};

Dot::Dot(int screen_width, int screen_height) : brain(1000) {
  pos = std::complex<double>(screen_width/2,screen_height-10);
  show();
  vel = 0;
  acc = 0;

  dead = false;
  reachedGoal = false;
  isBest = false;

  fitness = 0;
}

Dot::Dot(int screen_width, int screen_height, Brain brain) : brain(brain) {
  this->brain.setStep(0);
  pos = std::complex<double>(screen_width/2,screen_height-10);
  show();
  vel = 0;
  acc = 0;

  dead = false;
  reachedGoal = false;
  isBest = false;

  fitness = 0;
}

void Dot::show() {
  if (isBest) {
    shape.setFillColor(sf::Color(0,255,0));
    shape.setRadius(4);
  } else {
    shape.setFillColor(sf::Color(255,255,255));
    shape.setRadius(2);
  }
  shape.setPosition(std::real(pos), std::imag(pos));
}

// Sets acceleration, velocity, and position of dots as complex numbers
void Dot::move() {
  if (brain.getDirections().size() > brain.getStep()) {
    acc = brain.getDirections()[brain.getStep()];
    brain.setStep(brain.getStep()+1);
  } else {
    dead = true;
  }

  if (std::abs(vel+acc) > 5) {
    vel += acc;
    vel /= 5;
  } else {
    vel += acc;
  }
  pos += vel;
}

// Defines boundaries within the window
void Dot::update(std::complex<double> goal, int screen_width, int screen_height) {
  if (!dead && !reachedGoal) {
    move();
    if (std::real(pos) < 2 || std::imag(pos) < 2 ||
        std::real(pos) > screen_width - 2 || std::imag(pos) > screen_height - 2) {
      dead = true;
    } else if (calcDist(pos, goal) < 5) {
      reachedGoal = true;
    }
  }
}

void Dot::mutate() {
  brain.mutate();
}

double Dot::calcDist(std::complex<double> pos, std::complex<double> goal) {
  return sqrt(pow(std::real(pos) - std::real(goal), 2)
          + pow(std::imag(pos) - std::imag(goal), 2));
}

void Dot::calcFitness(std::complex<double> goal) {
  if (reachedGoal) {
    fitness = 1.0/16.0+10000.0/static_cast<double>(pow(brain.getStep(),2));
  } else {
    double distToGoal = calcDist(pos, goal);
    fitness = 1.0/(pow(distToGoal,2));
  }
}

// Child is recreated with the same directions as parent
Dot Dot::createChild(int screen_width, int screen_height) {
  Dot child(screen_width, screen_height, brain);
  return child;
}

void Dot::setBrain(Brain brain) {
  this->brain = brain;
}

void Dot::setDead(bool dead) {
  this->dead = dead;
}

void Dot::setIsBest(bool isBest) {
  this->isBest = isBest;
}

sf::CircleShape Dot::getShape() const {
  return shape;
}

std::complex<double> Dot::getPos() const {
  return pos;
}

Brain Dot::getBrain() const{
  return brain;
}

bool Dot::getDead() const {
  return dead;
}

bool Dot::getReachedGoal() const {
  return reachedGoal;
}

double Dot::getFitness() const {
  return fitness;
}

#endif
