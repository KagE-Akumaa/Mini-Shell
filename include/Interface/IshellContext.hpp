#pragma once

#include <string>
class IshellContext {
public:
  virtual ~IshellContext() = default;

  virtual const std::string &getCurrentDir() const = 0;
  virtual const std::string &getPreviousDir() const = 0;
  virtual void setCurrentDir(const std::string &dir) = 0;
  virtual void setPreviousDir(const std::string &dir) = 0;
};
