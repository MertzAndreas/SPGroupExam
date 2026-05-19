
#include "stochastic/vessel.hpp"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>

stochastic::Vessel seihr(uint32_t N) {
  auto v = stochastic::Vessel{"COVID19 SEIHR: " + std::to_string(N)};
  const auto eps = 0.0009;                     // initial fraction of infectious
  const auto I0 = size_t(std::round(eps * N)); // initial infectious
  const auto E0 = size_t(std::round(eps * N * 15)); // initial exposed
  const auto S0 = N - I0 - E0;                      // initial susceptible
  const auto R0 = 2.4;          // initial basic reproductive number
  const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
  const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
  const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
  const auto P_H = 0.9e-3;      // probability of hospitalization
  const auto kappa = gamma * P_H * (1.0 - P_H); // hospitalization rate (I -> H)
  const auto tau =
      1.0 / 10.12; // removal rate in hospital (H -> R) 10.12 days 16

  const auto S = v.add("S", S0); // susceptible
  const auto E = v.add("E", E0); // exposed
  const auto I = v.add("I", I0); // infectious
  const auto H = v.add("H", 0);  // hospitalized
  const auto R = v.add("R", 0);  // removed/immune (recovered + dead)
  v.add((S + I) >> beta / N >>=
        E + I);            // susceptible becomes exposed by infectious
  v.add(E >> alpha >>= I); // exposed becomes infectious
  v.add(I >> gamma >>= R); // infectious becomes removed
  v.add(I >> kappa >>= H); // infectious becomes hospitalized
  v.add(H >> tau >>= R);   // hospitalized becomes removed
  return v;
}

int estimate_max_h(int N) {
  auto v = seihr(N);
  auto s = v.create_simulator(100);
  auto hId = v.get_reactant_by_name("H");

  auto max = 0;
  for (const auto &state : s.Simulate()) {
    if (state.quantities[hId] > max)
      max = state.quantities[hId];
  }

  auto st = v.create_simulator(100);
  v.draw_simulation_chart(st, 50);

  return max;
}

int main() {
  const auto NDK = 5822763;
  const auto NNJ = 589755;
  auto maxdk = estimate_max_h(NDK);
  auto maxnj = estimate_max_h(NNJ);

  std::cout << "MAXDK: " << maxdk << std::endl;
  std::cout << "MAXNJ: " << maxnj << std::endl;
}
