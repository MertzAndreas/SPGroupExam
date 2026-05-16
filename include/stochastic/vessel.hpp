#pragma once

#include "stochastic/reaction.hpp"
#include "stochastic/symbol_table.hpp"
#include <cstddef>
#include <string>
#include <vector>
namespace stochastic {
class Vessel {
  std::string name;
  std::vector<Reaction> reactions;
  size_t id_generator;
  // ReactantId -> quantity
  SymbolTable<std::size_t, std::size_t> symbol_table;

public:
  Vessel(std::string simulation_name) : name(simulation_name) {}
  Reactant environment() const;
  Reactant add(std::string, int quantity);
  void add(Reaction reaction);
};
} // namespace stochastic
