#pragma once

#include "stochastic/chart.hpp"
#include "stochastic/reaction.hpp"
#include "stochastic/symbol_table.hpp"
#include <cstddef>
#include <unordered_map>
#include <utility>
#include <vector>

namespace stochastic {
class Simulator {
  double current_time = 0;
  double end_time;
  const std::vector<Reaction> reactions;
  const SymbolTable<uint8_t, std::string> reactant_id_names;
  std::unordered_map<uint8_t, std::size_t> reactant_quantities;
  std::unordered_map<uint8_t, std::vector<std::pair<double, double>>>
      quantity_over_time;

  double Delay(Reaction reaction);
  bool has_required_quantity(Reaction reaction,
                             const std::vector<uint8_t> &ids);
  const std::vector<Series> to_series() const;

public:
  Simulator(double end_time,
            std::unordered_map<uint8_t, std::size_t> reactant_quantities,
            std::vector<Reaction> reactions,
            const SymbolTable<uint8_t, std::string> &reactant_id_names)
      : end_time(end_time), reactant_quantities(reactant_quantities),
        reactions(reactions), reactant_id_names(reactant_id_names) {};

  std::vector<Series> Simulate();
};
} // namespace stochastic
