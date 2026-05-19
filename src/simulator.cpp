#include "stochastic/simulator.hpp"
#include <cfloat>
#include <vector>

namespace stochastic {

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
      if (id != ENVIRONMENT_ID) {
        reactant_quantities[id] += 1;
      }
    }

    co_yield SimulationState{reactant_quantities, current_time};
  }
}

} // namespace stochastic
