#include "semantic_analyzer.h"

SemanticAnalyzer::SemanticAnalyzer(std::vector<std::unique_ptr<StatementAST>>&& prog) {

}

std::vector<std::string> SemanticAnalyzer::analyze() {

}

bool SemanticAnalyzer::lookup_symbol(const std::string& symbol_identifier) {

}

bool SemanticAnalyzer::lookup_type_compatibility(
    Token current_type, const SymbolInfo& rhs) {

}
