#pragma once

#include "stochastic/reaction.hpp"
#include "stochastic/symbol_table.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
namespace stochastic {
const static uint8_t ENVIRONMENT_ID = UINT8_MAX;

class Vessel {
  std::string name;
  std::vector<Reaction> reactions;
  uint8_t id_generator = 0;
  // ReactantId -> quantity

  std::unordered_map<uint8_t, std::size_t> reactant_quantities;
  SymbolTable<uint8_t, std::string> reactant_names;

public:
  Vessel(std::string simulation_name) : name(simulation_name), reactions() {
    reactant_quantities.insert({ENVIRONMENT_ID, 0});
    reactant_names.add(ENVIRONMENT_ID, "Ø");
  }
  Reactant environment() const;
  Reactant add(std::string, int quantity);
  void add(Reaction reaction);
  void to_dot(std::ostream &stream);
  void simulate(double end_time);
};
} // namespace stochastic
