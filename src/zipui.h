#ifndef ZIPUI_H
#define ZIPUI_H

#include <map>
#include <string>
#include <vector>
#include "zip.h"

typedef std::vector<char> ByteVector;

class ZipUi
{
    unzFile zip_;
    std::map<std::string, ByteVector> map_;

    ByteVector zero_;

    ByteVector local_;
    std::string path_;

    int numbers_;

private:
    // 调试使用本地文件
    void readFromFile(const std::string &path);
    // 发行使用zip
    int readFromZip(ByteVector& bytes);
    int readBytes(ByteVector& bytes, uint32_t size);

public:
    ZipUi(const std::string& path);
    ~ZipUi();

    ByteVector& getBytes(const std::string& path);

};

#endif // ZIPUI_H
