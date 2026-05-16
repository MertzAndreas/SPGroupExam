#pragma once

#include <cstddef>
#include <vector>

namespace stochastic {

class ReactionBuilder;

class Reactant {
  size_t id;

public:
  explicit Reactant(size_t id) : id(id) {}
  size_t getId() const { return id; }
};

class ReactantGroup {
  std::vector<size_t> reactant_ids;

public:
  ReactantGroup(const Reactant &r) : reactant_ids(r.getId()) {};
  ReactantGroup(const ReactantGroup &g1, const ReactantGroup &g2);

  const std::vector<size_t> &getIds() const { return reactant_ids; }

  friend ReactantGroup operator+(const ReactantGroup &left,
                                 const ReactantGroup &right);
};

ReactantGroup operator+(const ReactantGroup &left, const ReactantGroup &right);

struct Reaction {
  std::vector<size_t> inputs;
  double rate;
  std::vector<size_t> outputs;

  Reaction(const ReactionBuilder reaction_builder,
           const ReactantGroup reactant_group);
};

struct ReactionBuilder {
  std::vector<size_t> input_ids;
  double rate;

public:
  ReactionBuilder(std::vector<size_t> inputs, double rate)
      : input_ids(inputs), rate(rate) {};

  friend ReactionBuilder operator>>(const ReactantGroup &left,
                                    const double &rate);
  friend Reaction operator>>=(const ReactionBuilder &left,
                              const ReactantGroup &right);
};

ReactionBuilder operator>>(const ReactantGroup &left, const double &rate);
Reaction operator>>=(const ReactionBuilder &left, const ReactantGroup &right);

} // namespace stochastic
