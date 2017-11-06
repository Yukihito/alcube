#include "FileUtil.h"

namespace utils {
  std::string FileUtil::readFile(const char *filePath) {
    std::string str;
    std::ifstream stream(filePath, std::ios::in);
    if(stream.is_open()) {
      std::string line;
      while(getline(stream, line))
        str += "\n" + line;
      stream.close();
    } else {
      std::cout << "Failed to open " << filePath << std::endl;
    }
    return str;
  }
}
