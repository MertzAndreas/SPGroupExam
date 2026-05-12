#pragma once
#include <concepts>
#include <map>

namespace stochastic {
template <typename K, typename V>
  requires std::equality_comparable<K>
class SymbolTable {
private:
  std::map<K, V> symbols;

public:
  void add(K key, V val) {
    if (symbols.contains(key))
      throw new std::exception();
    symbols[key] = val;
  }

  V get(K key) {
    if (!symbols.contains(key))
      throw new std::exception();
    return symbols[key];
  }
};
} // namespace stochastic
