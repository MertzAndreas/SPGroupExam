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
                  double timestamp)
      : timestamp(timestamp), quantities(reactant_quantities) {};
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
            const std::vector<Reaction> &reactions)
      : reactant_quantities(reactant_quantities), rng(std::random_device{}()),
        end_time(end_time), num_reactions(reactions.size()),
        rates(reactions.size()), inputs(reactions.size()),
        outputs(reactions.size()),
        reactant_to_reactions(reactant_quantities.size()),
        next_reaction_time(reactions.size()) {

    for (size_t i = 0; i < num_reactions; ++i) {
      rates[i] = reactions[i].rate;
      inputs[i] = reactions[i].inputs;
      outputs[i] = reactions[i].outputs;

      for (auto reactant_id : reactions[i].inputs) {
        reactant_to_reactions[reactant_id].push_back(i);
      }

      for (auto reactant_id : reactions[i].outputs) {
        reactant_to_reactions[reactant_id].push_back(i);
      }
    }
  };

  std::generator<SimulationState> Simulate();
  std::generator<SimulationState> SimulateFast();
};

} // namespace stochastic
