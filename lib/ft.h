#pragma once

#include <string>
#include <vector>

extern "C" {
#include <ft2build.h>
}
#include FT_FREETYPE_H

class FTBitmap {
  public:
    std::vector<std::vector<bool>> rows;
    int width;
    int asc;
    int desc;
};

class FreeType {
  public:
    FreeType(const std::string &fontfile, int dotsize);
    ~FreeType();

    std::vector<FTBitmap> draw(const std::string &text) const;

    static std::u32string char2u32str(const char text[]);

    inline int baseline() const { return baseline_; }
    inline int dotsize() const { return dotsize_; }

  private:
    void calcextsize();
    FTBitmap getinfo(FT_ULong u32char, int size) const;

    FT_Library library_;
    FT_Face face_;
    int dotsize_;
    int extsize_;
    int baseline_;
};