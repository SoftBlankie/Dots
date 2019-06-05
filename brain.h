#ifndef BRAIN_H
#define BRAIN_H

#include <complex>
#include <math.h>

class Brain {
  public:
    Brain(int size);
    void randomize(int size);
    void mutate();

    void setStep(int step);

    void setDirections(std::vector<std::complex<double>> directions);
    std::vector<std::complex<double>> getDirections() const;
    int getStep() const;
  private:
    std::vector<std::complex<double>> directions;
    int step;
};

Brain::Brain(int size) {
  step = 0;
  randomize(size);
}

void Brain::randomize(int size) {
  for (int i = 0; i < size; i++) {
    double randomAngle = fmod(rand(),2*M_PI);
    double randX = cos(randomAngle);
    double randY = sin(randomAngle);
    directions.push_back(std::complex<double>(randX,randY));
  }
}

// New directions are only introduced depending on the mutation chance.
// This guarantees that some directions will divert from its parent in order
// to represent diversity and progress towards a better path
void Brain::mutate() {
  double mutateRate = 0.01;
  for (auto& direction : directions) {
    double random = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
    if (random < mutateRate) {
      double randomAngle = fmod(rand(),2*M_PI);
      double randX = cos(randomAngle);
      double randY = sin(randomAngle);
      direction = std::complex<double>(randX,randY);
    }
  }
}

void Brain::setDirections(std::vector<std::complex<double>> directions) {
  this->directions = directions;
}

void Brain::setStep(int step) {
  this->step = step;
}

std::vector<std::complex<double>> Brain::getDirections() const {
  return directions;
}

int Brain::getStep() const {
  return step;
}

#endif
