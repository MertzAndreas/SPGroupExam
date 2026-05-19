#include "stochastic/vessel.hpp"
#include <catch2/catch_test_macros.hpp>
#include <format>

bool ToDotCorrectly() {
  auto v = stochastic::Vessel{"Test Vessel"};
  const double lambda = 0.001;
  const auto A = v.add("A", 1);
  const auto B = v.add("B", 1);
  const auto C = v.add("C", 1);

  v.add((A + C) >> lambda >>= B + C);

  std::ostringstream stream;
  v.to_dot(stream);

  auto string = stream.str();

  // Check each reactant name is in the string
  auto containsReactantIds = string.contains("label=\"A\"") &&
                             string.contains("label=\"B\"") &&
                             string.contains("label=\"C\"");

  // Check the rate is in the string on the format r<id>
  auto containsRateValue = string.contains("label=\"0.001\"");

  // Check we have input -> rate transition
  // Check we have rate -> output transition
  auto containsTransitionIds = string.contains(std::format("s{} -> r", A.id)) &&
                               string.contains(std::format("s{} -> r", C.id)) &&
                               string.contains(std::format("-> s{}", B.id)) &&
                               string.contains(std::format("-> s{}", C.id));

  return containsReactantIds && containsRateValue && containsTransitionIds;
}

bool ToDotEmpty() {
  auto v = stochastic::Vessel{"Test Vessel"};
  std::ostringstream stream;
  v.to_dot(stream);

  auto string = stream.str();
  return string.contains("digraph {\n}\n");
}

TEST_CASE("Factorials are computed", "[factorial]") {
  REQUIRE(ToDotCorrectly());
  REQUIRE(ToDotEmpty());
}
