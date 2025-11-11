#ifndef AST_UTIL_H
#define AST_UTIL_H
#include <iostream>

#include "ast_node.h"

template <typename To, typename From>
std::unique_ptr<To> unique_ptr_cast(std::unique_ptr<From>&& from) {
    return std::unique_ptr<To>(static_cast<To*>(from.release()));
}

//static void print_node(const ASTNode* node, int depth = 0) {
//    if (!node) return;
//
//    // Indentation based on depth
//    for (int i = 0; i < depth; ++i) std::cout << "  ";
//
//    //std::cout << node->name << "\n";
//    if (node->
//    // Recursively print all children
//    for (const auto* child : node->children) {
//        print_node(child, depth + 1);
//    }
//}
//
//// Entry point for printing all AST roots
//static void print_tree(const std::vector<ASTNode*>& nodes) {
//    for (const auto* node : nodes) {
//        print_node(node, 0);
//    }
//}

#endif

