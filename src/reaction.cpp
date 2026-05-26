

#include "stochastic/reaction.hpp"

namespace stochastic {

Reaction::Reaction(const ReactionBuilder reaction_builder,
                   const ReactantGroup reactant_group)
    : inputs(reaction_builder.reactant_ids), rate(reaction_builder.rate),
      outputs(reactant_group.reactant_ids) {}
void Reaction::accept(Visitor &visitor) { visitor.visit(*this); };

ReactantGroup operator+(const ReactantGroup &left, const ReactantGroup &right) {
  return ReactantGroup(left, right);
}

ReactionBuilder operator>>(const ReactantGroup &left, const double &rate) {
  return ReactionBuilder{left, rate};
}

Reaction operator>>=(const ReactionBuilder &left, const ReactantGroup &right) {
  return Reaction{left, right};
}

ReactantGroup::ReactantGroup(const Reactant &r) : reactant_ids{r.id} {};
ReactantGroup::ReactantGroup(const ReactantGroup &g1, const ReactantGroup &g2) {
  reactant_ids = g1.reactant_ids;
  reactant_ids.insert(reactant_ids.end(), g2.reactant_ids.begin(),
                      g2.reactant_ids.end());
}

Reactant::Reactant(uint8_t id) : id(id) {}
ReactionBuilder::ReactionBuilder(ReactantGroup reactant_group, double rate)
    : reactant_ids(reactant_group.reactant_ids), rate(rate) {};
} // namespace stochastic
