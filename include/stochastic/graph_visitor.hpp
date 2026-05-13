#pragma once

#include "stochastic/symbol_table.hpp"
#include "visitor.hpp"
#include <cstddef>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace stochastic {
class GraphVisitor : public Visitor {
  const SymbolTable<std::size_t, std::string> reactant_id_names;

  std::size_t next_rate_id = 0;
  std::unordered_map<size_t, double> rate_nodes;
  std::unordered_map<size_t, std::vector<size_t>> input_to_rate;
  std::unordered_map<size_t, std::vector<size_t>> rate_to_output;

  void visit(Reaction &reaction) override;

public:
  GraphVisitor(SymbolTable<std::size_t, std::string> reactant_id_names)
      : reactant_id_names(reactant_id_names) {};

  void to_dot(std::ostream &os) const;
};
} // namespace stochastic
