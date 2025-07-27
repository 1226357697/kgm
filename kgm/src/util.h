#pragma once

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <filesystem>
#include <fstream>
#include <optional>

using buffer_t = std::vector<uint8_t>;
namespace fs = std::filesystem;

namespace util
{
  template<typename T>
  const T* buffer_as(const buffer_t& buffer) {
    return reinterpret_cast<T*> (buffer.data());
  }
  template<typename T>
  T* buffer_as( buffer_t& buffer) {
    return reinterpret_cast<T*> (buffer.data());
  }

  std::optional<buffer_t> read_binrary_file(const std::string& filePath);

  bool write_binaray_file(const buffer_t& data, const std::string& filePath);

  buffer_t md5_digest(const void* data, size_t size);
};

