#include <yhs/Lexer.hpp>

using namespace yhs::frontend;

void Lexer::tokenize(const std::string& source) {
	std::deque<std::string> src;
	for (char ch : source) {
		src.push_back(std::string(1, ch));
	}
	std::deque<Token> tokens;

	std::function<void()> skipComments = [&src]() {
		while (!src.empty()) {
			if (src.size() >= 2 && src[0] == "/" && src[1] == "/") {
				while (!src.empty() && src[0] != "\n") {
					src.pop_front();
				}
				if (!src.empty()) {
					src.pop_front();
				}
			} else if (src.size() >= 2 && src[0] == "/" && src[1] == "*") {
				src.pop_front();
				src.pop_front();
				while (src.size() >= 2 && !(src[0] == "*" && src[1] == "/")) {
					src.pop_front();
				}
				if (src.size() >= 2) {
					src.pop_front();
					src.pop_front();
				}
			} else {
				break;
			}
		}
	};

	while (src.size() > 0) {
		skipComments();

		if (src.empty()) break;
		
		if (src[0] == "(") {
			tokens.push_back(Token(src.front(), TokenType::OpenParen));
			src.pop_front();
		} else if (src[0] == ")") {
			tokens.push_back(Token(src.front(), TokenType::CloseParen));
			src.pop_front();
		}

		else if (src[0] == "+" || src[0] == "-" || src[0] == "*" || src[0] == "/" || src[0] == "%") {
			tokens.push_back(Token(src.front(), TokenType::BinaryOperator));
			tokens.pop_front();
		} else if (src[0] == "=") {
			tokens.push_back(Token(src.front(), TokenType::BinaryOperator));
			tokens.pop_front();
		}
	}
}