#pragma once

#include "stochastic/chart.hpp"
#include "stochastic/reaction.hpp"
#include "stochastic/symbol_table.hpp"
#include "stochastic/vessel.hpp"
#include <cstddef>
#include <generator>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

namespace stochastic {

struct SimulationState {
  std::vector<size_t> quantities;
  double timestamp;

  SimulationState(
      const std::unordered_map<uint8_t, std::size_t> reactant_quantities,
      double timestamp)
      : timestamp(timestamp) {
    quantities.reserve(reactant_quantities.size());

    std::cout << reactant_quantities.size() << std::endl;

    for (const auto &[id, val] : reactant_quantities) {
      if (id != stochastic::ENVIRONMENT_ID)
        quantities[id] = val;
    }
  };
};

class Simulator {
  double end_time;
  const std::vector<Reaction> reactions;
  const SymbolTable<uint8_t, std::string> reactant_id_names;
  std::unordered_map<uint8_t, std::size_t> reactant_quantities;
  std::unordered_map<uint8_t, std::vector<std::pair<double, double>>>
      quantity_over_time;

  std::vector<Series> series_data;

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

  std::generator<SimulationState> Simulate();
  const std::vector<Series> &get_simualtion_data() const;
};

} // namespace stochastic
