#pragma once

#include "stochastic/visitor.hpp"
#include <cstdint>
#include <vector>

namespace stochastic {

struct ReactionBuilder;
struct Reaction;

struct Reactant {
  uint8_t id;
  explicit Reactant(uint8_t id) : id(id) {}
};

struct ReactantGroup {
  std::vector<uint8_t> reactant_ids;

  ReactantGroup(const Reactant &r) : reactant_ids{r.id} {};
  ReactantGroup(const ReactantGroup &g1, const ReactantGroup &g2);

  friend ReactantGroup operator+(const ReactantGroup &left,
                                 const ReactantGroup &right);
};

ReactantGroup operator+(const ReactantGroup &left, const ReactantGroup &right);

struct ReactionBuilder {
  std::vector<uint8_t> reactant_ids;
  double rate;

  ReactionBuilder(ReactantGroup reactant_group, double rate)
      : reactant_ids(reactant_group.reactant_ids), rate(rate) {};

  friend ReactionBuilder operator>>(const ReactantGroup &left,
                                    const double &rate);
  friend Reaction operator>>=(const ReactionBuilder &left,
                              const ReactantGroup &right);
};

struct Reaction final : NodeVisitor {
  const std::vector<uint8_t> inputs;
  const double rate;
  const std::vector<uint8_t> outputs;

  Reaction(const ReactionBuilder reaction_builder,
           const ReactantGroup reactant_group);

  void accept(Visitor &visitor) override { visitor.visit(*this); }
};

ReactionBuilder operator>>(const ReactantGroup &left, const double &rate);
Reaction operator>>=(const ReactionBuilder &left, const ReactantGroup &right);

} // namespace stochastic
