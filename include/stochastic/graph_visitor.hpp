#pragma once

#include "stochastic/symbol_table.hpp"
#include "visitor.hpp"
#include <cstdint>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace stochastic {
class GraphVisitor : public Visitor {
  const SymbolTable<uint8_t, std::string> reactant_id_names;

  uint8_t next_rate_id = 0;
  std::unordered_map<uint8_t, double> rate_nodes;
  std::unordered_map<uint8_t, std::vector<uint8_t>> input_to_rate;
  std::unordered_map<uint8_t, std::vector<uint8_t>> rate_to_output;

  void visit(Reaction &reaction) override;

public:
  GraphVisitor(SymbolTable<uint8_t, std::string> reactant_id_names)
      : reactant_id_names(reactant_id_names) {};

  void to_dot(std::ostream &os) const;
};
} // namespace stochastic
