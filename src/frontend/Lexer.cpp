#include <yhs/frontend/Lexer.hpp>
#include <yhs/Result.hpp>
#include <iostream>

using namespace yhs::frontend;

std::deque<Lexer::Token> Lexer::tokenize(const std::string& source) {
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
			src.pop_front();
		} else if (src[0] == "=") {
			tokens.push_back(Token(src.front(), TokenType::Equals));
			src.pop_front();
		} else if (src[0] == ";") {
			tokens.push_back(Token(src.front(), TokenType::Semicolon));
			src.pop_front();
		} else {
			if (std::isdigit(src[0][0])) {
				std::string num;
				while (src.size() > 0 && std::isdigit(src[0][0])) {
					num += src.front();
					src.pop_front();
				}

				tokens.push_back(Token(num, TokenType::Int));
			} else if (std::isalpha(src[0][0])) {
				std::string ident;
				while (src.size() > 0 && std::isalpha(src[0][0])) {
					ident += src.front();
					src.pop_front();
				}
				if (reserved.find(ident) == reserved.end()) {
					tokens.push_back(Token(ident, TokenType::Identifier));
				} else {
					tokens.push_back(Token(ident, reserved[ident]));
				}
			} else if (src[0] == "" || src[0] == " " || src[0] == "\n" || src[0] == "\r" || src[0] == "\t") {
				src.pop_front();
			} else {
				throw std::runtime_error("Unrecognized token found.");
			}
		}
	}

	tokens.push_back(Token("EOF", TokenType::EOF_));

	return tokens;
}