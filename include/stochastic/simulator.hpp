
#pragma once

#include "stochastic/reaction.hpp"
#include <cstddef>
#include <unordered_map>
#include <vector>

namespace stochastic {
class Simulator {
  double current_time = 0;
  double end_time;
  std::unordered_map<std::size_t, std::size_t> reactant_quantities;
  const std::vector<Reaction> reactions;

  double Delay(Reaction reaction);
  bool has_required_quantity(Reaction reaction, const std::vector<size_t> &ids);

public:
  Simulator(double end_time,
            std::unordered_map<std::size_t, std::size_t> reactant_quantities,
            std::vector<Reaction> reactions)
      : end_time(end_time), reactant_quantities(reactant_quantities),
        reactions(reactions) {};

  void Simulate();
};
} // namespace stochastic
