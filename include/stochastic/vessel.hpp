#pragma once

#include "stochastic/reaction.hpp"
#include "stochastic/symbol_table.hpp"
#include <climits>
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>
namespace stochastic {
const static std::size_t ENVIRONMENT_ID = ULONG_MAX;

class Vessel {
  std::string name;
  std::vector<Reaction> reactions;
  size_t id_generator = 0;
  // ReactantId -> quantity
  SymbolTable<std::size_t, std::size_t> reactant_quantities;
  SymbolTable<std::size_t, std::string> reactant_names;

public:
  Vessel(std::string simulation_name) : name(simulation_name) {
    reactant_quantities.add(ENVIRONMENT_ID, 0);
    reactant_names.add(ENVIRONMENT_ID, "Ø");
  }
  Reactant environment() const;
  Reactant add(std::string, int quantity);
  void add(Reaction reaction);
  void to_dot(std::ostream &stream);
};
} // namespace stochastic
