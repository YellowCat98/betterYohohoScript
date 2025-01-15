#include <yhs/frontend/Parser.hpp>

using namespace yhs::frontend;
using namespace yhs;

Result<AST::Program> Parser::produceAST(const std::string& code) {
    tokens = Lexer().tokenize(code);
}