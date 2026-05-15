

#include "stochastic/vessel.hpp"
#include "stochastic/chart.hpp"
#include "stochastic/graph_visitor.hpp"
#include "stochastic/reaction.hpp"
#include "stochastic/simulator.hpp"
#include <ostream>

namespace stochastic {

void Vessel::add(Reaction reaction) { reactions.push_back(reaction); }

Reactant Vessel::environment() const { return Reactant{ENVIRONMENT_ID}; }

Reactant Vessel::add(std::string name, int quantity) {
  auto id = id_generator++;
  const auto reactant = Reactant{id};
  reactant_quantities.insert({reactant.getId(), quantity});
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

void Vessel::simulate(double end_time) {
  Simulator sim{end_time, reactant_quantities, reactions, reactant_names};
  auto series = sim.Simulate();
  Chart chart{series, name};
  chart.ViewChart(end_time);
}
} // namespace stochastic
