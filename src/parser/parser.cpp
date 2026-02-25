#include "parser/parser.hpp"
#include <sstream>
std::optional<Command> Parser::parse(const std::string &input) const {
  std::stringstream ss(input);
  std::string token;
  std::vector<std::string> tokens;

  while (ss >> token) {
    tokens.push_back(token);
  }
  if (tokens.empty()) {
    return std::nullopt;
  }

  std::string program = tokens[0];

  std::vector<std::string> arguments(tokens.begin() + 1, tokens.end());

  return Command(program, arguments);
}
