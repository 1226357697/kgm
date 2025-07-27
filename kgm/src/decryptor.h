#pragma once

#include <vector>
#include "util.h"

class decryptor
{
public:
  decryptor(const buffer_t& iv, const buffer_t& key);

  void decryp(void* data, size_t size, uint64_t seed, int mode);
private:
  buffer_t initTable1(const buffer_t& data);
  buffer_t initTable2(const buffer_t& data);
private:
  
  buffer_t table1_;
  buffer_t table2_;
};

