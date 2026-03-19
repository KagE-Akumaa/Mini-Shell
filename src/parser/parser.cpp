#include "parser/parser.hpp"
#include <sstream>
std::optional<Command> Parser::parse(const std::string &input) const {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;
    std::vector<Redirection> redirection;

    // HACK: We can use the arguments and then parse from there?
    while (ss >> token) {
        // If token contains operand seperate from arguments
        if (token == "<" || token == ">" || token == ">>" || token == "2>") {
            Redirection r;
            // First is the operand
            r.operand = token;
            // Next the fileName
            ss >> r.fileDesc;
            redirection.push_back(r);
        } else {
            tokens.push_back(token);
        }
    }
    if (tokens.empty()) {
        return std::nullopt;
    }

    std::string program = tokens[0];

    std::vector<std::string> arguments(tokens.begin() + 1, tokens.end());

    return Command(program, arguments, redirection);
}
