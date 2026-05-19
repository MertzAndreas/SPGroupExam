

#include "stochastic/reaction.hpp"

namespace stochastic {

Reaction::Reaction(const ReactionBuilder reaction_builder,
                   const ReactantGroup reactant_group)
    : inputs(reaction_builder.reactant_ids), rate(reaction_builder.rate),
      outputs(reactant_group.reactant_ids) {};

ReactantGroup operator+(const ReactantGroup &left, const ReactantGroup &right) {
  return ReactantGroup(left, right);
}

ReactionBuilder operator>>(const ReactantGroup &left, const double &rate) {
  return ReactionBuilder{left, rate};
}

Reaction operator>>=(const ReactionBuilder &left, const ReactantGroup &right) {
  return Reaction{left, right};
}

ReactantGroup::ReactantGroup(const ReactantGroup &g1, const ReactantGroup &g2) {
  reactant_ids = g1.reactant_ids;
  reactant_ids.insert(reactant_ids.end(), g2.reactant_ids.begin(),
                      g2.reactant_ids.end());
}

} // namespace stochastic
