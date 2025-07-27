#include "kgm.h"
#include "decryptor.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <memory>


static unsigned char s_kgm_magic[] =
{
  0x7C, 0xD5, 0x32, 0xEB, 0x86, 0x02, 0x7F, 0x4B, 0xA8, 0xAF,
  0xA6, 0x8E, 0x0F, 0xFF, 0x99, 0x14
};

unsigned char s_kgm_enc_magic[] =
{
  0x38, 0x85, 0xED, 0x92, 0x79, 0x5F, 0xF8, 0x4C, 0xB3, 0x03,
  0x61, 0x41, 0x16, 0xA0, 0x1D, 0x47
};

static unsigned char s_temp_key[] = {
  0x6C, 0x2C, 0x2F, 0x27
};

struct KGMHeader
{
  uint8_t magic[16];
  uint32_t data_off;
  uint32_t encrypt_type;
  uint32_t block_size;
  uint8_t enc_magic[16];
  uint8_t key1[16];
};

Kgm::Kgm()
{
}

Kgm::~Kgm()
{
}

bool Kgm::open(const std::string& filePath)
{
  auto optFileData = util::read_binrary_file(filePath);
  if (!optFileData)
    return false;
  rawData_ = std::move(*optFileData);



  return isValid();
}

bool Kgm::dump(const std::string& outFilePath)
{
  auto header = util::buffer_as<KGMHeader>(rawData_);
  
  decryptor dec(buffer_t(std::begin(s_temp_key), std::end(s_temp_key)), buffer_t(std::begin(header->key1), std::end(header->key1)));

  uint8_t enc_magic[16];
  memcpy(enc_magic, header->enc_magic, sizeof(enc_magic));
  dec.decryp(enc_magic, sizeof(enc_magic), 0, 0);

  if (memcmp(enc_magic, s_kgm_enc_magic, 16) !=0)
  {
    return false;
  }

  size_t data_size  = rawData_.size() - header->data_off;
  buffer_t decryptData(data_size);
  memcpy(decryptData.data(), rawData_.data() + header->data_off, decryptData.size());
  dec.decryp(decryptData.data(), data_size, 0, 0);

  return util::write_binaray_file(decryptData, outFilePath);
}

bool Kgm::isValid()
{
  if (rawData_.size() < sizeof(KGMHeader))
  {
    return false;
  }

  auto header = util::buffer_as<KGMHeader>(rawData_);
  if (memcmp(header->magic, s_kgm_magic, sizeof(s_kgm_magic)) != 0)
  { 
    return false;
  }

  if (header->encrypt_type != 3)
  {
    return false;
  }

  return true;
}
