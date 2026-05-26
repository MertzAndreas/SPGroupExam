#include "stochastic/simulator.hpp"
#include <cfloat>
#include <cstddef>
#include <random>
#include <vector>

namespace stochastic {

Simulator::Simulator(double end_time, std::vector<size_t> reactant_quantities,
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

std::generator<SimulationState> Simulator::Simulate() {

  double current_time = 0;
  while (current_time <= end_time) {
    auto best = std::pair{DBL_MAX, static_cast<size_t>(0)};

    for (size_t i = 0; i < num_reactions; i++) {
      bool has_quantities = true;
      double product = 1.0;

      for (auto id : inputs[i]) {
        if (reactant_quantities[id] <= 0) {
          has_quantities = false;
          break;
        }
        product *= reactant_quantities[id];
      }

      if (!has_quantities)
        continue;

      auto dist = std::exponential_distribution<double>(rates[i] * product);
      double d = dist(rng);

      if (d < best.first) {
        best = {d, i};
      }
    }

    if (best.first == DBL_MAX)
      break;

    const size_t best_idx = best.second;
    current_time += best.first;

    for (auto id : inputs[best_idx]) {
      reactant_quantities[id] -= 1;
    }

    for (auto id : outputs[best_idx]) {
      reactant_quantities[id] += 1;
    }

    co_yield SimulationState{reactant_quantities, current_time};
  }
}

std::generator<SimulationState> Simulator::SimulateFast() {
  auto recompute = [&](size_t r, double t) {
    double product = 1.0;
    for (auto id : inputs[r]) {
      if (reactant_quantities[id] == 0) {
        next_reaction_time[r] = DBL_MAX;
        return;
      }
      product *= reactant_quantities[id];
    }
    next_reaction_time[r] =
        t + std::exponential_distribution<double>(rates[r] * product)(rng);
  };

  double current_time = 0;
  for (size_t i = 0; i < num_reactions; i++)
    recompute(i, current_time);

  while (current_time <= end_time) {
    size_t best_idx = 0;
    for (size_t i = 1; i < num_reactions; i++)
      if (next_reaction_time[i] < next_reaction_time[best_idx])
        best_idx = i;

    if (next_reaction_time[best_idx] == DBL_MAX)
      break;

    current_time = next_reaction_time[best_idx];

    for (auto id : inputs[best_idx])
      reactant_quantities[id] -= 1;
    for (auto id : outputs[best_idx])
      reactant_quantities[id] += 1;

    for (auto id : inputs[best_idx])
      for (auto r : reactant_to_reactions[id])
        recompute(r, current_time);
    for (auto id : outputs[best_idx])
      for (auto r : reactant_to_reactions[id])
        recompute(r, current_time);

    co_yield SimulationState{reactant_quantities, current_time};
  }
}

SimulationState::SimulationState(const std::vector<size_t> reactant_quantities,
                                 double timestamp)
    : timestamp(timestamp), quantities(reactant_quantities) {};
} // namespace stochastic
