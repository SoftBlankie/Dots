#ifndef POPULATION_H
#define POPULATION_H

#include "dot.h"
#include <iostream>

class Population {
  public:
    Population(std::complex<double> goal, int screen_width, int screen_height, int size);
    void show();
    void update(std::complex<double> goal, int screen_width, int screen_height);
    void calcFitness(std::complex<double> goal);
    bool allDotsDead();
    void naturalSelection(int screen_width, int screen_height);
    void calcFitnessSum();
    Dot selectParent();
    void mutate();
    void setBestDot();

    std::vector<Dot> getDots() const;
  private:
    std::vector<Dot> dots;
    int bestDot;
    double fitnessSum;
    int gen;
    int minStep;
};

Population::Population(std::complex<double> goal, int screen_width, int screen_height, int size) :
  bestDot(0), fitnessSum(0), gen(1), minStep(1000) {
    for (int i = 0; i < size; i++) {
      dots.push_back(Dot(screen_width, screen_height));
    }
}

void Population::show() {
  for (auto& dot : dots) {
    dot.show();
  }
}

void Population::update(std::complex<double> goal, int screen_width, int screen_height) {
  for (auto& dot : dots) {
    if (dot.getBrain().getStep() > minStep) {
      dot.setDead(true);
    } else {
      dot.update(goal, screen_width, screen_height);
    }
  }
}

void Population::calcFitness(std::complex<double> goal) {
  for (auto& dot : dots) {
    dot.calcFitness(goal);
  }
}

bool Population::allDotsDead() {
  for (const auto& dot : dots) {
    if (!dot.getDead() && !dot.getReachedGoal()) {
      return false;
    }
  }
  return true;
}

// Best dots are kept as the top candidates while other dots are children
// derived from the next best candidate closest to its goal
void Population::naturalSelection(int screen_width, int screen_height) {
  std::vector<Dot> newDots;
  setBestDot();
  calcFitnessSum();

  // set best dot unchanged
  newDots.push_back(dots[bestDot].createChild(screen_width, screen_height));
  newDots[0].setIsBest(true);
  // derive other dots from specified parents
  for (int i = 1; i < dots.size(); i++) {
    Dot parent = selectParent();
    newDots.push_back(parent.createChild(screen_width, screen_height));
  }

  dots = newDots;
  gen++;
}

void Population::calcFitnessSum() {
  fitnessSum = 0;
  for (const auto& dot : dots) {
    fitnessSum += dot.getFitness();
  }
}

// Parents are selected depending on the fitness in relation to other dots
Dot Population::selectParent() {
  double random = fmod(rand(), fitnessSum);
  double runningSum = 0;

  for (const auto& dot : dots) {
    runningSum += dot.getFitness();
    if (runningSum > random) {
      return dot;
    }
  }

  return dots[-1];
}

// Bottleneck: O(n^2)
// Dots are mutated in order to offer new gene pools to the species
void Population::mutate() {
  for (int i = 1; i < dots.size(); i++) {
    dots[i].mutate();
  }
}

// Best dot is defined by best fitness or if reached the goal
void Population::setBestDot() {
  double max = 0;
  int maxIndex = 0;
  for (int i = 0; i < dots.size(); i++) {
    if (dots[i].getFitness() > max) {
      max = dots[i].getFitness();
      maxIndex = i;
    }
  }
  bestDot = maxIndex;
  if (dots[bestDot].getReachedGoal()) {
    minStep = dots[bestDot].getBrain().getStep();
  }
  std::cout << "Gen: " << gen << std::endl;
  std::cout << "step: " << minStep << std::endl;
}

std::vector<Dot> Population::getDots() const {
  return dots;
}

#endif
