#ifndef ALCUBE_FILEUTIL_H
#define ALCUBE_FILEUTIL_H

#include <string>
#include <iostream>
#include <fstream>

namespace utils {
  class FileUtil {
    public:
      std::string readFile(const char* filePath);
  };
}

#endif //ALCUBE_FILEUTIL_H
