

#include "stochastic/vessel.hpp"
#include "stochastic/reaction.hpp"
#include <climits>

namespace stochastic {

void Vessel::add(Reaction reaction) { reactions.push_back(reaction); }

Reactant Vessel::environment() const { return Reactant{ULONG_MAX}; }

Reactant Vessel::add(std::string, int quantity) {
  const auto reactant = Reactant{id_generator++};
  symbol_table.add(reactant.getId(), quantity);
  return reactant;
}

} // namespace stochastic
