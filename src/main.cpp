#include <iostream>
#include "lexer.h"
int main() {
    Lexer l("file.ats");
    for (std::size_t i = 0; i < 100; ++i) {
        auto x = l.get_lexeme();
        if (x.t == Undefined) {
            break;
        }
        std::cout << x.t << "," << x.raw_val << "\n";
    }
    return 0;
}
