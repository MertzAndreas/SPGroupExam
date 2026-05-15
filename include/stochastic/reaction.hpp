#pragma once

#include "stochastic/visitor.hpp"
#include <cstddef>
#include <vector>

namespace stochastic {

class ReactionBuilder;
class Reaction;

class Reactant {
  size_t id;

public:
  explicit Reactant(size_t id) : id(id) {}
  size_t getId() const { return id; }
};

class ReactantGroup {
  std::vector<size_t> reactant_ids;

public:
  ReactantGroup(const Reactant &r) : reactant_ids{r.getId()} {};
  ReactantGroup(const ReactantGroup &g1, const ReactantGroup &g2);

  const std::vector<size_t> &getIds() const { return reactant_ids; }

  friend ReactantGroup operator+(const ReactantGroup &left,
                                 const ReactantGroup &right);
};

ReactantGroup operator+(const ReactantGroup &left, const ReactantGroup &right);

struct ReactionBuilder {
  ReactantGroup reactant_group;
  double rate;

  ReactionBuilder(ReactantGroup reactant_group, double rate)
      : reactant_group(reactant_group), rate(rate) {};

  friend ReactionBuilder operator>>(const ReactantGroup &left,
                                    const double &rate);
  friend Reaction operator>>=(const ReactionBuilder &left,
                              const ReactantGroup &right);
};

class Reaction final : NodeVisitor {
  static inline size_t id_generator = 0;

public:
  const size_t id;
  ReactionBuilder reaction_builder;
  ReactantGroup reactant_group;

  Reaction(const ReactionBuilder reaction_builder,
           const ReactantGroup reactant_group);

  void accept(Visitor &visitor) override { visitor.visit(*this); }
};

ReactionBuilder operator>>(const ReactantGroup &left, const double &rate);
Reaction operator>>=(const ReactionBuilder &left, const ReactantGroup &right);

} // namespace stochastic
