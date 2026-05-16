#include "stochastic/symbol_table.hpp"
#include <catch2/catch_test_macros.hpp>

void ThrowsOn2ConsecutiveInsert() {
  stochastic::SymbolTable<int, int> test_symbol_table;
  test_symbol_table.add(1, 1);
  test_symbol_table.add(1, 2);
}

void ThrowsOnGetThatIsntThere() {
  stochastic::SymbolTable<int, int> test_symbol_table;
  test_symbol_table.get(1);
}

bool InsertsWithoutThrow() {
  stochastic::SymbolTable<int, int> test_symbol_table;
  test_symbol_table.add(1, 1);
  test_symbol_table.add(2, 2);
  return true;
}

bool InsertAndGetsCorrectly() {
  stochastic::SymbolTable<int, int> test_symbol_table;
  auto insertedValue = 1;
  test_symbol_table.add(1, insertedValue);
  return insertedValue == test_symbol_table.get(1);
}

TEST_CASE("Symbol table methods") {
  REQUIRE_THROWS(ThrowsOn2ConsecutiveInsert());
  REQUIRE_THROWS(ThrowsOnGetThatIsntThere());
  REQUIRE(InsertsWithoutThrow());
  REQUIRE(InsertAndGetsCorrectly());
}
