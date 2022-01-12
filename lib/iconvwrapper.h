#pragma once
#include <cassert>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
#include <iconv.h>
}

class UTF16toiso2022Converter {
  public:
    UTF16toiso2022Converter() : cd_(iconv_open("ISO2022JP", "UTF-16BE")),
                                srcBuff_(new char[srcBufSize]),
                                dstBuff_(new char[dstBufSize]) {
        assert(((iconv_t)-1) != cd_);
        assert(dstBuff_ != nullptr);
    }
    ~UTF16toiso2022Converter() {
        delete[] dstBuff_;
        delete[] srcBuff_;
        iconv_close(cd_);
    }

    const std::vector<uint8_t> convert(const std::vector<uint16_t> &utf16str, bool rmEsscSeq = true) const;

  private:
    iconv_t cd_;
    char *srcBuff_;
    char *dstBuff_;

    static const std::size_t srcBufSize;
    static const std::size_t dstBufSize;
};

class iso2022toUTF16Converter {
  public:
    iso2022toUTF16Converter() : cd_(iconv_open("UTF-16BE", "ISO2022JP")),
                                srcBuff_(new char[srcBufSize]),
                                dstBuff_(new char[dstBufSize]) {
        assert(((iconv_t)-1) != cd_);
        assert(dstBuff_ != nullptr);
    }
    ~iso2022toUTF16Converter() {
        delete[] dstBuff_;
        delete[] srcBuff_;
        iconv_close(cd_);
    }

    const std::vector<uint16_t> convert(const std::vector<uint8_t> &iso2022str, bool addEscSeq = true) const;

  private:
    iconv_t cd_;
    char *srcBuff_;
    char *dstBuff_;

    static const std::size_t srcBufSize;
    static const std::size_t dstBufSize;
};

class UTFtoiso2022Converter {
  public:
    UTFtoiso2022Converter() : cd_(iconv_open("ISO2022JP", "UTF-8")),
                              srcBuff_(new char[srcBufSize]),
                              dstBuff_(new char[dstBufSize]) {
        assert(((iconv_t)-1) != cd_);
        assert(dstBuff_ != nullptr);
    }
    ~UTFtoiso2022Converter() {
        delete[] dstBuff_;
        delete[] srcBuff_;
        iconv_close(cd_);
    }

    const std::vector<uint8_t> convert(const std::string &utfstr, bool rmEsscSeq = true) const;

  private:
    iconv_t cd_;
    char *srcBuff_;
    char *dstBuff_;

    static const std::size_t srcBufSize;
    static const std::size_t dstBufSize;
};

class iso2022toUTFConverter {
  public:
    iso2022toUTFConverter() : cd_(iconv_open("UTF-8", "ISO2022JP")),
                              srcBuff_(new char[srcBufSize]),
                              dstBuff_(new char[dstBufSize]) {
        assert(((iconv_t)-1) != cd_);
        assert(dstBuff_ != nullptr);
    }
    ~iso2022toUTFConverter() {
        delete[] dstBuff_;
        delete[] srcBuff_;
        iconv_close(cd_);
    }

    const std::string convert(const std::vector<uint8_t> &iso2022str, bool addEscSeq = true) const;

  private:
    iconv_t cd_;
    char *srcBuff_;
    char *dstBuff_;

    static const std::size_t srcBufSize;
    static const std::size_t dstBufSize;
};

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
        if(err != std::size_t(-1)) {
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