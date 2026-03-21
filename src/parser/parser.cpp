#include "parser/parser.hpp"
#include <sstream>
std::optional<Command> Parser::parse(const std::string &input) const {
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;
    std::vector<Redirection> redirection;

    std::unique_ptr<Pipes> piping;

    while (ss >> token) {
        if (token == "|") {
            // Up until now all the arguments + redirection belongs to leftSide
            // of piping
            piping = std::make_unique<Pipes>();
            piping->leftSide = std::make_unique<Command>(
                tokens[0],
                std::vector<std::string>(tokens.begin() + 1, tokens.end()),
                redirection, nullptr);
            tokens.clear();
            redirection.clear();
        }
        // If token contains operand seperate from arguments
        else if (token == "<" || token == ">" || token == ">>" ||
                 token == "2>") {
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

    std::string program = "";

    std::vector<std::string> arguments;

    if (piping) {
        piping->rightSide = std::make_unique<Command>(
            tokens[0],
            std::vector<std::string>(tokens.begin() + 1, tokens.end()),
            redirection, nullptr);

        // For piping everything exists inside piping object rest is dummy
        return Command(program, arguments, {}, std::move(piping));
    }

    program = tokens[0];

    arguments = std::vector<std::string>(tokens.begin() + 1, tokens.end());

    return Command(program, arguments, redirection, std::move(piping));
}
