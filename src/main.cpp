#include <iostream>

#include "lexer.h"
#include "parser.h"

int main() {
    Lexer l("file.ats");
    Parser p(l);

    p.parse();
    return 0;
}
