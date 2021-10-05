#include <cassert>
#include <sstream>

#include "../iconvwrapper.h"

const std::size_t IConvWrppaer::srcBufSize = 4096;

const std::size_t UTFtoiso2022Converter::srcBufSize = 4096;
const std::size_t UTFtoiso2022Converter::dstBufSize = 4096;

const std::vector<uint8_t> UTFtoiso2022Converter::convert(const std::string &utfstr, bool rmEsscSeq) const {
    strncpy(srcBuff_, utfstr.c_str(), srcBufSize - 1);
    char *src = srcBuff_;
    std::size_t srcSize = strnlen(src, srcBufSize);

    char *dst = dstBuff_;
    std::size_t dstSize = dstBufSize;

    std::size_t err = iconv(cd_, &src, &srcSize, &dst, &dstSize);
    assert(std::size_t(-1) != err);

    auto iso2022 = std::vector<uint8_t>(dstBufSize - dstSize);
    for(std::uint8_t i = 0; i < iso2022.size(); ++i) {
        iso2022[i] = uint8_t(dstBuff_[i]);
    }

    for(bool loop = true; loop && rmEsscSeq;) {
        loop = false;
        for(std::uint8_t i = 0; i < iso2022.size() - 2; i++) {
            if((iso2022[i] == 0x1B) && (iso2022[i + 1] == 0x24) && (iso2022[i + 2] == 0x42)) {
                iso2022.erase(iso2022.begin() + i, iso2022.begin() + i + 3);
                loop = true;
                break;
            }
        }
    }

    return iso2022;
}

const std::size_t iso2022toUTFConverter::srcBufSize = 4096;
const std::size_t iso2022toUTFConverter::dstBufSize = 4096;

const std::string iso2022toUTFConverter::convert(const std::vector<uint8_t> &iso2022str, bool addEscSeq) const {
    if(addEscSeq) {
        srcBuff_[0] = 0x1B;
        srcBuff_[1] = 0x24;
        srcBuff_[2] = 0x42;
    }

    for(std::size_t i = 0; i < iso2022str.size() + (addEscSeq ? 3 : 0); ++i) {
        srcBuff_[i + (addEscSeq ? 3 : 0)] = char(iso2022str[i]);
    }
    char *src = srcBuff_;
    std::size_t srcSize = iso2022str.size() + (addEscSeq ? 3 : 0);

    char *dst = dstBuff_;
    std::size_t dstSize = dstBufSize;

    std::size_t err = iconv(cd_, &src, &srcSize, &dst, &dstSize);
    assert(std::size_t(-1) != err);

    auto ss = std::stringstream();
    for(std::size_t i = 0; i < dstBufSize - dstSize; ++i) {
        ss << dstBuff_[i];
    }
    return ss.str();
}
