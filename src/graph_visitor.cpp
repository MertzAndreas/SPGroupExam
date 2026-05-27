#include "stochastic/graph_visitor.hpp"
#include "stochastic/environment.hpp"
#include "stochastic/reaction.hpp"
#include <iostream>
#include <ostream>
#include <unordered_set>

namespace stochastic {

void GraphVisitor::visit(Reaction &reaction) {
  const auto rateId = next_rate_id++;
  rate_nodes[rateId] = reaction.rate;

  for (auto i : reaction.inputs)
    input_to_rate[i].push_back(rateId);

  for (auto o : reaction.outputs)
    rate_to_output[rateId].push_back(o);
}

GraphVisitor::GraphVisitor(SymbolTable<uint8_t, std::string> reactant_id_names)
    : reactant_id_names(reactant_id_names) {};

void GraphVisitor::to_dot(std::ostream &os) const {
  std::unordered_set<uint8_t> all_reactant_ids;
  for (const auto &[id, _] : input_to_rate)
    all_reactant_ids.insert(id);
  for (const auto &[_, ids] : rate_to_output)
    all_reactant_ids.insert(ids.begin(), ids.end());

  os << "digraph {\n";

  for (auto id : all_reactant_ids) {
    const auto &name = reactant_id_names.get(id);
    if (id == stochastic::ENVIRONMENT_ID)
      os << std::format(
          "s{}[label=\"{}\",shape=\"box\",fontcolor=\"white\",style=\"filled\","
          "fillcolor=\"black\"];\n",
          id, name);

    else
      os << std::format("s{}[label=\"{}\",shape=\"box\",style=\"filled\","
                        "fillcolor=\"cyan\"];\n",
                        id, name);
  }

  for (const auto &[id, rate] : rate_nodes)
    os << std::format("r{}[label=\"{}\",shape=\"oval\",style=\"filled\","
                      "fillcolor=\"yellow\"];\n",
                      id, rate);

  for (const auto &[species_id, rate_ids] : input_to_rate)
    for (auto rate_id : rate_ids)
      os << std::format("s{} -> r{};\n", species_id, rate_id);

  for (const auto &[rate_id, species_ids] : rate_to_output)
    for (auto species_id : species_ids)
      os << std::format("r{} -> s{};\n", rate_id, species_id);

  os << "}\n";
}
}; // namespace stochastic
