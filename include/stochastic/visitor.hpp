
#pragma once

namespace stochastic {
class Reaction;
class ReactionBuilder;

class Visitor {
public:
  virtual void visit(Reaction &reaction) = 0;
  virtual ~Visitor() = default;
};

class NodeVisitor {
public:
  virtual void accept(Visitor &visitor) = 0;
  virtual ~NodeVisitor() = default;
};
} // namespace stochastic
