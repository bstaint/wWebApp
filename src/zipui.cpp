#include "zipui.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>

int ZipUi::readFromZip(ByteVector &bytes)
{
    unsigned char buffer[1024] = { 0 };

    int offset = 0;
    int ret = unzReadCurrentFile(zip_, buffer, 1024);
    while(ret > 0)
    {
        memcpy(&bytes[0] + offset, buffer, ret);
        offset += ret;

        ret = unzReadCurrentFile(zip_, buffer, 1024);
    }

    return ret;
}

void ZipUi::readFromFile(const std::string &path)
{
//    std::streampos size;
    std::ifstream in(path, std::ios::binary);

    in.seekg(0, std::ios::end);
    unsigned int size = in.tellg();
    in.seekg(0, std::ios::beg);

    local_.resize(size);
    in.read(local_.data(), size);
}

int ZipUi::readBytes(ByteVector &bytes, uint32_t size)
{
    int ret = MZ_OK;

    bytes.resize(size);
    if((ret = unzOpenCurrentFile(zip_)) == MZ_OK)
    {
        ret = readFromZip(bytes);
        unzCloseCurrentFile(zip_);
    }

    return ret;
}

ZipUi::ZipUi(const std::string &path) :
    zip_(nullptr),
    zero_(0),
    numbers_(0),
    path_(path)
{
    zip_ = unzOpen(path.c_str());

    unz_global_info info;
    unzGetGlobalInfo(zip_, &info);

    numbers_ = info.number_entry;
}

ZipUi::~ZipUi()
{
    unzClose(zip_);
}

ByteVector &ZipUi::getBytes(const std::string &path)
{
#if 1
    int pos = path_.rfind('/');
    if(pos != std::string::npos)
    {
        std::stringstream sstream;
        sstream << path_.substr(0, pos + 1) << path;

        readFromFile(sstream.str());
        return local_;
    }
#else
    int ret = MZ_OK;
    auto it = map_.find(path);

    if(it != map_.end())
        return it->second;

    if((ret = unzLocateFile(zip_, path.c_str(), NULL)) == MZ_OK)
    {
        unz_file_info info;

        ret = unzGetCurrentFileInfo(zip_, &info, NULL, 0, NULL,0, NULL, 0);
        if(ret == MZ_OK)
        {
            readBytes(map_[path], info.uncompressed_size);
            return map_[path];
        }
    }
#endif

    return zero_;
}
