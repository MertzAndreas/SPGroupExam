

#include "stochastic/simulator.hpp"
#include "stochastic/reaction.hpp"
#include <cfloat>
#include <climits>
#include <cstddef>
#include <random>
#include <unordered_map>
#include <vector>

namespace stochastic {
double Simulator::Delay(Reaction reaction) {
  const auto rate = reaction.reaction_builder.rate;
  const auto ids = reaction.reaction_builder.reactant_group.getIds();

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
                                      const std::vector<size_t> &ids) {
  for (auto id : ids) {
    if (reactant_quantities.at(id) <= 0)
      return false;
  }

  return true;
}

void Simulator::Simulate() {
  while (current_time <= end_time) {
    auto min_delay = DBL_MAX;
    auto min_id = ULONG_MAX;

    for (auto reaction : reactions) {
      auto delay = Delay(reaction);
      if (delay < min_delay) {
        min_delay = delay;
        min_id = reaction.id;
      }
    }

    const auto &reaction = reactions[min_id];
    const auto &input_ids = reaction.reaction_builder.reactant_group.getIds();
    const auto &output_ids = reaction.reactant_group.getIds();
    current_time += min_delay;

    if (!has_required_quantity(reaction, input_ids))
      continue;

    for (auto id : input_ids)
      reactant_quantities.at(id) -= 1;

    for (auto id : output_ids)
      reactant_quantities.at(id) += 1;
  }
}
} // namespace stochastic
