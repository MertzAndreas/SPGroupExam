#pragma once

#include "stochastic/environment.hpp"
#include "stochastic/reaction.hpp"
#include <cstddef>
#include <generator>
#include <random>
#include <unordered_map>
#include <vector>

namespace stochastic {

struct SimulationState {
  std::vector<size_t> quantities;
  const double timestamp;

  SimulationState(
      const std::unordered_map<uint8_t, std::size_t> reactant_quantities,
      double timestamp)
      : timestamp(timestamp), quantities(reactant_quantities.size()) {
    for (const auto &[id, val] : reactant_quantities) {
      if (id != stochastic::ENVIRONMENT_ID)
        quantities[id] = val;
    }
  };
};

class Simulator {
  std::unordered_map<uint8_t, std::size_t> reactant_quantities;
  std::mt19937 rng;
  std::vector<double> rates;
  std::vector<std::vector<uint8_t>> inputs;
  std::vector<std::vector<uint8_t>> outputs;

public:
  const double end_time;
  const size_t num_reactions;

  Simulator(double end_time,
            const std::unordered_map<uint8_t, std::size_t> &reactant_quantities,
            const std::vector<Reaction> &reactions)
      : reactant_quantities(reactant_quantities), rng(std::random_device{}()),
        end_time(end_time), num_reactions(reactions.size()),
        rates(reactions.size()), inputs(reactions.size()),
        outputs(reactions.size()) {

    for (size_t i = 0; i < num_reactions; ++i) {
      rates[i] = reactions[i].rate;
      inputs[i] = reactions[i].inputs;
      outputs[i] = reactions[i].outputs;
    }
  };

  std::generator<SimulationState> Simulate();
};

} // namespace stochastic
