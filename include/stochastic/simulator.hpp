#pragma once

#include "stochastic/reaction.hpp"
#include <cstddef>
#include <generator>
#include <random>
#include <vector>

namespace stochastic {

struct SimulationState {
  const std::vector<size_t> &quantities;
  const double timestamp;

  SimulationState(const std::vector<size_t> reactant_quantities,
                  double timestamp);
};

class Simulator {
  std::vector<size_t> reactant_quantities;
  std::mt19937 rng;
  std::vector<double> rates;
  std::vector<std::vector<uint8_t>> inputs;
  std::vector<std::vector<uint8_t>> outputs;
  std::vector<std::vector<uint8_t>> reactant_to_reactions;
  std::vector<double> next_reaction_time;

public:
  const double end_time;
  const size_t num_reactions;

  Simulator(double end_time, std::vector<size_t> reactant_quantities,
            const std::vector<Reaction> &reactions);

  std::generator<SimulationState> Simulate();
  std::generator<SimulationState> SimulateFast();
};

} // namespace stochastic
