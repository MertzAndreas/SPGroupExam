#pragma once

#include "stochastic/environment.hpp"
#include "stochastic/reaction.hpp"
#include "stochastic/simulator.hpp"
#include "stochastic/symbol_table.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
namespace stochastic {

class Vessel {
  std::string name;
  std::vector<Reaction> reactions;
  uint8_t id_generator = 0;
  // ReactantId -> quantity

  std::unordered_map<uint8_t, std::size_t> reactant_quantities;
  SymbolTable<uint8_t, std::string> reactant_names;

public:
  Vessel(std::string simulation_name) : name(simulation_name), reactions() {
    reactant_quantities.insert({stochastic::ENVIRONMENT_ID, 0});
    reactant_names.add(stochastic::ENVIRONMENT_ID, "Ø");
  }
  Simulator create_simulator(double end_time) const;
  Reactant environment() const;
  Reactant add(std::string, int quantity);
  uint8_t get_reactant_by_name(std::string str);
  void add(Reaction reaction);
  void to_dot(std::ostream &stream);
  void draw_simulation_chart(Simulator &sim, int data_points = 1000) const;
};
} // namespace stochastic
