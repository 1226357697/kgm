#include "util.h"
#include "md5.h"

using namespace std;

optional<buffer_t> util::read_binrary_file(const std::string& filePath) {
  buffer_t fileData;
  ifstream ifs(filePath, ios::binary | ios::in);
  if (!ifs.is_open())
    return nullopt;

  ifs.seekg(0, ios::end);
  size_t fileSize = ifs.tellg();
  if (fileSize == 0)
    return nullopt;

  fileData.resize(fileSize);

  ifs.seekg(0, ios::beg);
  ifs.read(reinterpret_cast<char*>(fileData.data()), fileData.size());

  return fileData;
}

bool util::write_binaray_file(const buffer_t& data, const std::string& filePath)
{
  fs::path absPath = fs::absolute(filePath);

  fs::path parentDir = absPath.parent_path();

  if (!fs::exists(parentDir)) {
    try {
      fs::create_directories(parentDir);
    }
    catch (const fs::filesystem_error& e) {
      std::cerr << "Error creating directories: " << e.what() << std::endl;
      return false;
    }
  }

  std::ofstream outFile(filePath, std::ios::binary);
  if (!outFile) {
    return false;
  }

  outFile.write(reinterpret_cast<const char*>(data.data()), data.size());

  return outFile.good();
}

buffer_t util::md5_digest(const void* data, size_t size)
{
  buffer_t digest(16);
  md5(reinterpret_cast<const uint8_t*>(data), size, digest.data());
  return digest;
}
