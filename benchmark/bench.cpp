#include "stochastic/vessel.hpp"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <execution>

stochastic::Vessel circadian() {
  const auto alphaA = 50;
  const auto alpha_A = 500;
  const auto alphaR = 0.01;
  const auto alpha_R = 50;
  const auto betaA = 50;
  const auto betaR = 5;
  const auto gammaA = 1;
  const auto gammaR = 1;
  const auto gammaC = 2;
  const auto deltaA = 1;
  const auto deltaR = 0.2;
  const auto deltaMA = 10;
  const auto deltaMR = 0.5;
  const auto thetaA = 50;
  const auto thetaR = 100;

  auto v = stochastic::Vessel{"Circadian Rhythm "};

  const auto env = v.environment();
  const auto DA = v.add("DA", 1);
  const auto D_A = v.add("D_A", 0);
  const auto DR = v.add("DR", 1);
  const auto D_R = v.add("D_R", 0);
  const auto MA = v.add("MA", 0);
  const auto MR = v.add("MR", 0);
  const auto A = v.add("A", 0);
  const auto R = v.add("R", 0);
  const auto C = v.add("C", 0);
  v.add((A + DA) >> gammaA >>= D_A);
  v.add(D_A >> thetaA >>= DA + A);
  v.add((A + DR) >> gammaR >>= D_R);
  v.add(D_R >> thetaR >>= DR + A);
  v.add(D_A >> alpha_A >>= MA + D_A);
  v.add(DA >> alphaA >>= MA + DA);
  v.add(D_R >> alpha_R >>= MR + D_R);
  v.add(DR >> alphaR >>= MR + DR);
  v.add(MA >> betaA >>= MA + A);
  v.add(MR >> betaR >>= MR + R);
  v.add((A + R) >> gammaC >>= C);
  v.add(C >> deltaA >>= R);
  v.add(A >> deltaA >>= env);
  v.add(R >> deltaR >>= env);
  v.add(MA >> deltaMA >>= env);
  v.add(MR >> deltaMR >>= env);
  return v;
}

stochastic::Vessel circular_chain() {
  auto v = stochastic::Vessel{"Large Circular Chain"};

  const auto A = v.add("A", 500);
  const auto B = v.add("B", 100);
  const auto C = v.add("C", 0);
  const auto D = v.add("D", 0);
  const auto E = v.add("E", 0);
  const auto F = v.add("F", 0);
  const auto G = v.add("G", 100);
  const auto H = v.add("H", 0);
  const auto I = v.add("I", 0);
  const auto J = v.add("J", 0);
  const auto K = v.add("K", 50);
  const auto L = v.add("L", 0);
  const auto M = v.add("M", 0);
  const auto N = v.add("N", 0);
  const auto O = v.add("O", 0);
  const auto P = v.add("P", 50);
  const auto Q = v.add("Q", 0);
  const auto R = v.add("R", 0);
  const auto S = v.add("S", 0);
  const auto T = v.add("T", 0);

  v.add(A >> 0.30 >>= B);
  v.add(B >> 0.15 >>= C);
  v.add(C >> 0.40 >>= D);
  v.add(D >> 0.10 >>= E);
  v.add(E >> 0.35 >>= F);
  v.add(F >> 0.20 >>= G);
  v.add(G >> 0.50 >>= H);
  v.add(H >> 0.05 >>= I);
  v.add(I >> 0.45 >>= J);
  v.add(J >> 0.25 >>= K);
  v.add(K >> 0.30 >>= L);
  v.add(L >> 0.12 >>= M);
  v.add(M >> 0.55 >>= N);
  v.add(N >> 0.20 >>= O);
  v.add(O >> 0.38 >>= P);
  v.add(P >> 0.08 >>= Q);
  v.add(Q >> 0.60 >>= R);
  v.add(R >> 0.22 >>= S);
  v.add(S >> 0.44 >>= T);
  v.add(T >> 0.18 >>= A);

  return v;
}

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
template <typename ExecutionPolicy>
int run_covid(int runs, ExecutionPolicy &&policy) {
  const auto population = 10000;
  auto v = seihr(population);

  std::vector<int> iterators(runs);
  std::vector<int> maxes(runs);

  std::transform(policy, iterators.begin(), iterators.end(), maxes.begin(),
                 [&v](int) {
                   auto s = v.create_simulator(100);
                   auto hId = v.get_reactant_by_name("H");

                   auto max = 0;
                   for (const auto &state : s.Simulate()) {
                     if (state.quantities[hId] > max)
                       max = state.quantities[hId];
                   }
                   return max;
                 });

  return std::reduce(policy, maxes.begin(), maxes.end(), 0);
}

template <typename ExecutionPolicy>
int run_covid_fast(int runs, ExecutionPolicy &&policy) {
  const auto population = 10000;
  auto v = seihr(population);

  std::vector<int> iterators(runs);
  std::vector<int> maxes(runs);

  std::transform(policy, iterators.begin(), iterators.end(), maxes.begin(),
                 [&v](int) {
                   auto s = v.create_simulator(100);
                   auto hId = v.get_reactant_by_name("H");

                   auto max = 0;
                   for (const auto &state : s.SimulateFast()) {
                     if (state.quantities[hId] > max)
                       max = state.quantities[hId];
                   }
                   return max;
                 });

  return std::reduce(policy, maxes.begin(), maxes.end(), 0);
}

TEST_CASE("Benchmark simulation", "[!benchmark]") {
  BENCHMARK("Simulation Covid 100") {
    return run_covid(100, std::execution::seq);
  };
  BENCHMARK("Simulation Covid Fast 100") {
    return run_covid_fast(100, std::execution::seq);
  };
  BENCHMARK("Simulation Covid Parallel 100") {
    return run_covid(100, std::execution::par);
  };
  BENCHMARK("Simulation Covid Parallel Fast 100") {
    return run_covid_fast(100, std::execution::par);
  };

  BENCHMARK("Circadian naive") {
    auto c = circadian();
    auto simulator = c.create_simulator(48);
    int acc = 0;
    for (const auto &p : simulator.Simulate())
      acc += p.quantities[c.get_reactant_by_name("A")];
    return acc;
  };
  BENCHMARK("Circadian fast") {
    auto c = circadian();
    auto simulator = c.create_simulator(48);
    int acc = 0;
    for (const auto &p : simulator.SimulateFast())
      acc += p.quantities[c.get_reactant_by_name("A")];
    return acc;
  };
  BENCHMARK("Large circular chain simulation naive") {
    auto v = circular_chain();
    auto simulator = v.create_simulator(200);
    int acc = 0;
    for (const auto &state : simulator.Simulate())
      acc += state.quantities[v.get_reactant_by_name("A")];
    return acc;
  };
  BENCHMARK("Large circular chain simulation fast") {
    auto v = circular_chain();
    auto simulator = v.create_simulator(200);
    int acc = 0;
    for (const auto &state : simulator.SimulateFast())
      acc += state.quantities[v.get_reactant_by_name("A")];
    return acc;
  };
}
