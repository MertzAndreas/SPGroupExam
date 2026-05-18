

#include "stochastic/simulator.hpp"
#include "stochastic/chart.hpp"
#include "stochastic/reaction.hpp"
#include "stochastic/vessel.hpp"
#include <cfloat>
#include <cstdint>
#include <generator>
#include <random>
#include <unordered_map>
#include <vector>

namespace stochastic {
double Simulator::Delay(Reaction reaction) {
  const auto rate = reaction.reaction_builder.rate;
  const auto ids = reaction.reaction_builder.reactant_group.getIds();
  if (!has_required_quantity(reaction, ids))
    return DBL_MAX;

  auto product = 1;
  for (auto id : ids) {
    product *= reactant_quantities.at(id);
  }
  auto term = rate * product;

  std::exponential_distribution<> d(term);
  static std::mt19937 rng(std::random_device{}());
  return d(rng);
}

bool Simulator::has_required_quantity(Reaction,
                                      const std::vector<uint8_t> &ids) {
  for (auto id : ids) {
    if (reactant_quantities.at(id) <= 0)
      return false;
  }

  return true;
}

const std::vector<Series> Simulator::to_series() const {
  auto series = std::vector<Series>{};
  for (const auto &[id, vec] : quantity_over_time) {
    if (id == stochastic::ENVIRONMENT_ID)
      continue;
    const auto &name = reactant_id_names.get(id);
    series.push_back(Series{vec, name});
  }

  return series;
}

std::generator<SimulationState> Simulator::Simulate() {
  double current_time = 0;
  while (current_time <= end_time) {
    for (const auto &[key, value] : reactant_quantities)
      quantity_over_time[key].push_back({current_time, value});

    auto min_delay = DBL_MAX;
    auto min_index = 0;

    for (size_t i = 0; i < reactions.size(); i++) {
      auto delay = Delay(reactions[i]);
      if (delay < min_delay) {
        min_delay = delay;
        min_index = i;
      }
    }

    const auto &reaction = reactions[min_index];
    const auto &input_ids = reaction.reaction_builder.reactant_group.getIds();
    const auto &output_ids = reaction.reactant_group.getIds();
    current_time += min_delay;

    if (!has_required_quantity(reaction, input_ids))
      continue;

    for (auto id : input_ids)
      reactant_quantities.at(id) -= 1;

    for (auto id : output_ids)
      if (id != stochastic::ENVIRONMENT_ID)
        reactant_quantities.at(id) += 1;

    co_yield SimulationState{reactant_quantities, current_time};
  }

  series_data = to_series();
}

const std::vector<Series> &Simulator::get_simualtion_data() const {
  return series_data;
}

} // namespace stochastic
