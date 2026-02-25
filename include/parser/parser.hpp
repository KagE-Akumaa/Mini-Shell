#pragma once

#include "../command/command.hpp"
#include <optional>

class Parser {
public:
  std::optional<Command> parse(const std::string &line) const;
};
