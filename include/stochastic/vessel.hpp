#pragma once

#include "stochastic/reaction.hpp"
#include <string>
namespace stochastic {
class Vessel {
  std::string name;

public:
  Vessel(std::string simulation_name) : name(simulation_name) {}
  Reactant environment() const;
  Reactant add(std::string, int quantity);
  void add(Reaction reaction);
};
} // namespace stochastic
