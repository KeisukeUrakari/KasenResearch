#pragma once

#include <cassert>
#include <cstring>

extern "C" {
#include <iconv.h>
}

class IConvWrppaer {
  public:
    IConvWrppaer(const char *to, const char *from) : cd_(iconv_open(to, from)),
                                                     srcBuff_(new char[srcBufSize]) {
        assert(((iconv_t)-1) != cd_);
        assert(srcBuff_ != nullptr);
    }

    ~IConvWrppaer() {
        delete[] srcBuff_;
        iconv_close(cd_);
    }

    std::size_t conv(const char *src, char *dst, std::size_t dstSize, std::size_t *convSize) {
        char *p = srcBuff_;
        std::strncpy(p, src, srcBufSize - 1);
        std::size_t srcSize = strlen(p);
        *convSize = dstSize;
        std::size_t err = iconv(cd_, &p, &srcSize, &dst, convSize);
        if(err != std::size_t(-1)){
            *convSize = dstSize - *convSize;
            dst[*convSize] = '\0';
        }
        return err;
    }

  private:
    iconv_t cd_;
    char *srcBuff_;

    static const std::size_t srcBufSize;
};