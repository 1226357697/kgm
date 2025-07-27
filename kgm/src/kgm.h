#pragma once

#include <string>
#include "util.h"

class Kgm
{
public:
  Kgm();
  ~Kgm();

  bool open(const std::string& filePath);

  bool dump(const std::string& outFilePath);

private:
  bool isValid();

private:
  buffer_t rawData_;
};

