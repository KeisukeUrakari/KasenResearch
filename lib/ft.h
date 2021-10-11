#pragma once

#include <string>
#include <vector>

extern "C" {
#include <ft2build.h>
}
#include FT_FREETYPE_H

class FreeType {

  public:
    FreeType(const std::string &fontfile);
    ~FreeType();

    std::vector<std::vector<bool>> draw(const std::string &text);

    static std::u32string char2u32str(const char text[]);

  private:
    FT_Library library_;
    FT_Face face_;
};

//    int len = mbstowcs( ws, text, SIZE_OF_ARRAY(ws) );
//const char text[]
//std::u32string  u32str  = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(text);
