#ifndef SYMBOL_INFO_H
#define SYMBOL_INFO_H

#include "token.h"
#include <string>

struct SymbolInfo {
  std::string name;
  Token token;
};

#endif
