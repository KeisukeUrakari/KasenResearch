#pragma once

//https://ez-net.jp/article/0B/ydQ-Mn1E/uoSm36x6y4XR/
//https://ez-net.jp/article/B7/IhkTIBpE/CqyovJ8V6abj/

#include <iostream>

#if 1
class CExternalBuffer : public std::streambuf {
  public:
    CExternalBuffer() = delete;
    CExternalBuffer(char *buffer, std::size_t bufferLength) : start_(buffer){
        char *bufferEnd = buffer + bufferLength;
        setp(buffer, bufferEnd);
        setg(buffer, buffer, bufferEnd);
    }

    std::size_t getContentSize() const {
        const auto sz = pptr() - start_;
        return sz;
    }

    private:
       char *start_;
};
#if 0
class CExternalStream : public std::iostream {
  public:
    CExternalStream() = delete;
    CExternalStream(char *buffer, size_t bufferLength)
        : m_buffer(buffer, bufferLength), std::iostream(&m_buffer) {
    }
  private:
    CExternalBuffer m_buffer;

};
#endif

#else

class CExternalBuffer : public std::streambuf {
  public:
    CExternalBuffer() = delete;
    CExternalBuffer(char *buffer, size_t bufferLength);

    // ストリームで seekp や seekg を相対位置指定で実行した場合に呼び出されます。
    std::ios::pos_type seekoff(std::ios::off_type __off, std::ios_base::seekdir __way, std::ios_base::openmode __which = std::ios_base::in | std::ios_base::out) override;
    // ストリームで seekp や seekg を絶対位置指定で実行した場合に呼び出されます。
    std::ios::pos_type seekpos(std::ios::pos_type __sp, std::ios_base::openmode __which = std::ios_base::in | std::ios_base::out) override;
};
#endif