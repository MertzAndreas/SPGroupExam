#include "stochastic/vessel.hpp"
#include <string>

stochastic::Vessel create_decay_system(int initial_A, int initial_B,
                                       int initial_C) {
  auto v = stochastic::Vessel{"Decay: A=" + std::to_string(initial_A) +
                              ", B=" + std::to_string(initial_B) +
                              ", C=" + std::to_string(initial_C)};

  const double lambda = 0.001;
  const auto A = v.add("A", initial_A);
  const auto B = v.add("B", initial_B);
  const auto C = v.add("C", initial_C);

  v.add((A + C) >> lambda >>= B + C);

  return v;
}

int main() {
  // Example 1: A(0)=100, B(0)=0, C=1 [cite: 23]
  auto v1 = create_decay_system(100, 0, 1);
  auto s1 = v1.create_simulator(2000);
  v1.draw_simulation_chart(s1);

  // Example 2: A(0)=100, B(0)=0, C=2 [cite: 38, 39]
  auto v2 = create_decay_system(100, 0, 2);
  auto s2 = v2.create_simulator(1500);
  v2.draw_simulation_chart(s2);

  // Example 3: A(0)=50, B(0)=50, C=1 [cite: 47, 48]
  auto v3 = create_decay_system(50, 50, 1);
  auto s3 = v3.create_simulator(1500);
  v3.draw_simulation_chart(s3);

  return 0;
}
