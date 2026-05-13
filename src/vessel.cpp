

#include "stochastic/vessel.hpp"
#include "stochastic/graph_visitor.hpp"
#include "stochastic/reaction.hpp"
#include <climits>
#include <ostream>

namespace stochastic {

void Vessel::add(Reaction reaction) { reactions.push_back(reaction); }

Reactant Vessel::environment() const { return Reactant{ENVIRONMENT_ID}; }

Reactant Vessel::add(std::string name, int quantity) {
  auto id = id_generator++;
  const auto reactant = Reactant{id};
  reactant_quantities.add(reactant.getId(), quantity);
  reactant_names.add(reactant.getId(), name);
  return reactant;
}

void Vessel::to_dot(std::ostream &os) {
  GraphVisitor visitor = GraphVisitor(reactant_names);

  for (auto &reaction : reactions) {
    reaction.accept(visitor);
  }

  visitor.to_dot(os);
}
} // namespace stochastic
