#pragma once
#include <concepts>
#include <cstddef>
#include <format>
#include <map>
#include <stdexcept>

namespace stochastic {
template <typename K, typename V>
  requires std::equality_comparable<K>
class SymbolTable {
private:
  std::map<K, V> symbols;

public:
  void add(K key, V val) {
    if (symbols.contains(key))
      throw std::runtime_error("Already contains key");
    symbols[key] = val;
  }

  const V &get(const K key) const {
    if (!symbols.contains(key))
      throw std::runtime_error(
          std::format("Tried to get key: {}, which isn't in the map", key));
    const auto &symbol = symbols.at(key);
    return symbol;
  }

  size_t size() const { return symbols.size(); }
  auto begin() const { return symbols.begin(); }
  auto end() const { return symbols.end(); }
};
} // namespace stochastic
