

#include "stochastic/vessel.hpp"
#include "stochastic/chart.hpp"
#include "stochastic/environment.hpp"
#include "stochastic/graph_visitor.hpp"
#include "stochastic/reaction.hpp"
#include "stochastic/simulator.hpp"
#include <cstdint>
#include <ostream>
#include <stdexcept>

namespace stochastic {

void Vessel::add(Reaction reaction) { reactions.push_back(reaction); }

Reactant Vessel::environment() const {
  return Reactant{stochastic::ENVIRONMENT_ID};
}

Reactant Vessel::add(std::string name, int quantity) {
  auto id = id_generator++;
  const auto reactant = Reactant{id};
  reactant_quantities.push_back(quantity);
  reactant_names.add(reactant.id, name);
  return reactant;
}

uint8_t Vessel::get_reactant_by_name(std::string str) {
  for (const auto [key, val] : reactant_names) {
    if (str == val)
      return key;
  }
  throw std::runtime_error("Requested name was not in the symbol table");
}

void Vessel::to_dot(std::ostream &os) {
  GraphVisitor visitor = GraphVisitor(reactant_names);

  for (auto &reaction : reactions) {
    reaction.accept(visitor);
  }

  visitor.to_dot(os);
}

Vessel::Vessel(std::string simulation_name)
    : name(simulation_name), reactions() {
  reactant_quantities.push_back(0);
  reactant_names.add(stochastic::ENVIRONMENT_ID, "Ø");
}
Simulator Vessel::create_simulator(double end_time) const {
  return Simulator{end_time, reactant_quantities, reactions};
}

void Vessel::draw_simulation_chart(Simulator &sim, int data_points) const {
  std::unordered_map<size_t, std::vector<double>> values;
  std::vector<double> timestamps;

  auto next_observation = 0.0;
  const auto observation_interval = sim.end_time / data_points;
  const auto quantity_size = reactant_quantities.size();

  for (const auto &state : sim.Simulate()) {
    if (state.timestamp >= next_observation) {
      timestamps.push_back(state.timestamp);
      for (size_t id = 0; id < quantity_size; id++) {
        if (id == stochastic::ENVIRONMENT_ID)
          continue;
        values[id].push_back(static_cast<double>(state.quantities[id]));
      }
      next_observation += observation_interval;
    }
  }

  std::vector<Series> series;
  for (const auto &[id, name] : reactant_names) {
    if (id == stochastic::ENVIRONMENT_ID)
      continue;
    series.emplace_back(name, std::move(values[id]));
  }

  Chart{std::move(series), std::move(timestamps), name}.ViewChart(sim.end_time);
}
} // namespace stochastic
