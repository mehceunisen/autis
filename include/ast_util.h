#ifndef AST_UTIL_H
#define AST_UTIL_H
#include <iostream>
#include <type_traits>

#include "ast_node.h"

template <
    typename To,
    typename From,
    std::enable_if_t<std::is_base_of_v<From, To>,
        int> = 0
>
std::unique_ptr<To> unique_ptr_cast(std::unique_ptr<From>&& from) {
    return std::unique_ptr<To>(static_cast<To*>(from.release()));
}

#endif

