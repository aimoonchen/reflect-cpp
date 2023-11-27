#include <cassert>
#include <iostream>
#include <rfl.hpp>
#include <rfl/json.hpp>
#include <string>
#include <vector>

#include "write_and_read.hpp"

namespace test_unique_ptr2 {

struct DecisionTree {
  struct Leaf {
    using Tag = rfl::Literal<"Leaf">;
    rfl::Field<"value", double> value;
  };

  struct Node {
    using Tag = rfl::Literal<"Node">;
    rfl::Field<"criticalValue", double> critical_value;
    rfl::Field<"left", std::unique_ptr<DecisionTree>> lesser;
    rfl::Field<"right", std::unique_ptr<DecisionTree>> greater;
  };

  using LeafOrNode = rfl::TaggedUnion<"type", Leaf, Node>;

  rfl::Field<"leafOrNode", LeafOrNode> leaf_or_node;
};

void test() {
  std::cout << "test_unique_ptr2" << std::endl;

  auto leaf1 = DecisionTree::Leaf{.value = 3.0};

  auto leaf2 = DecisionTree::Leaf{.value = 5.0};

  auto node = DecisionTree::Node{
      .critical_value = 10.0,
      .lesser = std::make_unique<DecisionTree>(DecisionTree{leaf1}),
      .greater = std::make_unique<DecisionTree>(DecisionTree{leaf2})};

  const DecisionTree tree{.leaf_or_node = std::move(node)};

  write_and_read(
      tree,
      R"({"leafOrNode":{"type":"Node","criticalValue":10.0,"left":{"leafOrNode":{"type":"Leaf","value":3.0}},"right":{"leafOrNode":{"type":"Leaf","value":5.0}}}})");
}
}  // namespace test_unique_ptr2
