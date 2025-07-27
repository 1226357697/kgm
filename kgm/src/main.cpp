
#include "util.h"
#include "md5.h"
#include "kgm.h"
#include <format>

using namespace std;


static bool decrypt_one_file(const string& kgmPath)
{
  string outMp3Path = fs::path(kgmPath).replace_extension("mp3").string();
  if (fs::exists(outMp3Path))
  {
    cerr << format("found mp3 file: {0}", outMp3Path) << endl;
    return true;
  }

  Kgm kgm;
  if (!kgm.open(kgmPath))
  {
    cerr << format("invalid kgm file: {0}", kgmPath) << endl;
    return false;
  }

  if (!kgm.dump(outMp3Path))
  {
    cerr << format("fialed to dump file: {0}", outMp3Path) << endl;
    return false;
  }
  cout << "Decrypted Files: "<< outMp3Path<<endl;
  return true;
}


int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cerr << "invalid argments"<< endl;
    cerr << "usage: KG.exe <kgg dir|path>"<<endl;
    return 1;
  }
  fs::path kgmPath =  argv[1];
  if (!fs::exists(kgmPath))
  {
    cerr <<"not found path: " << kgmPath<< endl;
    return 1;
  }

  bool is_dir = fs::is_directory(kgmPath);

  if (is_dir)
  {

    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ kgmPath })
    {
      if (dir_entry.is_regular_file())
      {
        auto file = dir_entry.path();
        decrypt_one_file(file.string());

      }
    }
  }
  else
  {
    decrypt_one_file(kgmPath.string());
  }
  
  return 0;
}