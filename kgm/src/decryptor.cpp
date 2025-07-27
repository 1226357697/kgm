#include "decryptor.h"
#include "util.h"
#include <assert.h>



#if 0

// 还原后的加密/解密函数
char* custom_cipher(
  char* data,                // 待处理数据缓冲区
  size_t data_len,           // 数据长度
  uint64_t seed,             // 加密种子
  const uint32_t table_range[2][2], // 两个表的范围 [表ID][0=起始,1=结束]
  bool mode                  // 操作模式 (true=模式A, false=模式B)
) {
  if (!data || data_len <= 0) return data;

  // 获取表1范围
  const char* table1_start = reinterpret_cast<const char*>(table_range[0][0]);
  const char* table1_end = reinterpret_cast<const char*>(table_range[0][1]);
  const size_t table1_len = table1_end - table1_start;

  // 获取表2范围
  const char* table2_start = reinterpret_cast<const char*>(table_range[1][0]);
  const char* table2_end = reinterpret_cast<const char*>(table_range[1][1]);
  const size_t table2_len = table2_end - table2_start;

  // 检查表有效性
  if (table1_len == 0 || table2_len == 0) return data;

  for (size_t i = 0; i < data_len; ++i) {
    const uint64_t dynamic_key = seed + i;
    const uint32_t key_low = static_cast<uint32_t>(dynamic_key);
    const uint32_t key_high = static_cast<uint32_t>(dynamic_key >> 32);

    if (mode) {
      // 模式 A
      uint8_t byte = data[i];

      // 第一轮异或: 动态密钥的字节
      byte ^= key_low;
      byte ^= (key_low >> 8);
      byte ^= (key_low >> 16);
      byte ^= (key_low >> 24);

      // 第二轮: 表1操作 + 位置换
      const char* table1_ptr = table1_start + (dynamic_key % table1_len);
      const uint8_t t1_val = *table1_ptr;
      byte ^= t1_val;
      byte ^= (byte << 4);  // 等价于 16*byte 的异或

      // 第三轮: 表2操作
      const char* table2_ptr = table2_start + (dynamic_key % table2_len);
      byte ^= *table2_ptr;

      data[i] = byte;
    }
    else {
      // 模式 B
      uint8_t byte = data[i];

      // 第一轮: 表2操作 + 位置换
      const char* table2_ptr = table2_start + (dynamic_key % table2_len);
      const uint8_t t2_val = *table2_ptr;
      byte ^= t2_val;
      byte ^= (byte << 4);  // 等价于 16*byte 的异或

      // 第二轮: 表1操作 + 密钥高位字节
      const char* table1_ptr = table1_start + (dynamic_key % table1_len);
      byte ^= *table1_ptr;
      byte ^= (key_low >> 16);
      byte ^= (key_low >> 24);

      // 第三轮: 动态密钥低位字节
      byte ^= key_low;
      byte ^= (key_low >> 8);

      data[i] = byte;
    }
  }
  return data;
}
#endif // 0


decryptor::decryptor(const buffer_t& iv, const buffer_t& key)
{
  table1_ = initTable1(iv);
  table2_ = initTable2(key);
}

void decryptor::decryp(void* data, size_t size, uint64_t seed, int mode)
{
  if (data == NULL || size == 0 || table1_.empty()|| table2_.empty())
  {
    assert(false);
    return;
  }

  uint8_t* _data = reinterpret_cast<uint8_t*>(data);
  if (mode)
  {
    assert(false);
  }
  else
  {
    for (size_t i = 0; i < size; ++i)
    {
      const uint64_t dynamic_key = seed + i;
      const uint32_t key_low = static_cast<uint32_t>(dynamic_key);
      const uint32_t key_high = static_cast<uint32_t>(dynamic_key >> 32);

      uint8_t byte = _data[i];

      // 第一轮: 表2操作 + 位置换
      const uint8_t t2_val = table2_.at(dynamic_key % table2_.size());
      byte ^= t2_val;
      byte ^= (byte << 4);  // 等价于 16*byte 的异或

      // 第二轮: 表1操作 + 密钥高位字节
      byte ^= table1_.at(dynamic_key % table1_.size());
      byte ^= (key_low >> 16);
      byte ^= (key_low >> 24);

      // 第三轮: 动态密钥低位字节
      byte ^= key_low;
      byte ^= (key_low >> 8);

      _data[i] = byte;


    }
  }


}

buffer_t decryptor::initTable1(const buffer_t& data)
{

  auto dataMd5 = util::md5_digest(data.data(), data.size());
  uint16_t* ptr = util::buffer_as<uint16_t>(dataMd5);

  int item_len = dataMd5.size() / 2;
  for (int i = 0; i < item_len / 2; ++i)
  {
    uint16_t temp = ptr[i];
    ptr[i] = ptr[item_len - i - 1];
    ptr[item_len - i - 1] = temp;

  }

  return dataMd5;
}

buffer_t decryptor::initTable2(const buffer_t& data)
{
  buffer_t key = initTable1(data);
  key.push_back(0x6B);
  return key;
}
