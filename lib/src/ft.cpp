
#include "../ft.h"

#include <cassert>
#include <codecvt>
#include <locale>

FreeType::FreeType(const std::string &fontfile) {
    auto error = FT_Init_FreeType(&library_);
    assert(error == 0);

    error = FT_New_Face(library_, fontfile.c_str(), 0, &face_);
    //    error = FT_New_Face( library, "/usr/share/fonts/noto-cjk/NotoSansCJK-Bold.ttc", 0, &face );

    if(error == FT_Err_Unknown_File_Format) {
        // [ERROR] Font file format is not supported!!
        assert(false);
    } else if(error) {
        // [ERROR] Font file not found or it is broken!
        assert(false);
    }

    error = FT_Set_Pixel_Sizes(face_, 0, 16); //ピクセル単位でサイズ指定
    // error = FT_Set_Pixel_Sizes( face_, 0, 120 ); //ピクセル単位でサイズ指定
    // error = FT_Set_Char_Size( face, 0, 16 * 64, 300, 300); //ポイント単位でサイズ指定
    if(error) {
        assert(false);
    }
}

FreeType::~FreeType() {
    assert(FT_Done_Face(face_) == 0);
    assert(FT_Done_FreeType(library_) == 0);
}

std::vector<std::vector<bool>> FreeType::draw(const std::string &text) {
    std::u32string u32str = char2u32str(text.c_str());

    auto &slot = face_->glyph;
    auto &bitmap = slot->bitmap;
    auto rows = std::vector<std::vector<bool>>();

    for(const auto &u32char : u32str) {
        const auto error = FT_Load_Char(face_, u32char, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
        assert(!error);

        for(auto y = decltype(bitmap.rows)(0); y < bitmap.rows; ++y) {
            auto row = std::vector<bool>();
#if 0
            for(auto x = decltype(bitmap.pitch)(0); x < bitmap.pitch; ++x) {

#if 0
                row.push_back(bitmap.buffer[y * bitmap.pitch + x]);
#else
                const auto &tmp = bitmap.buffer[y * bitmap.pitch + x];
                for(auto bit = decltype(tmp)(0); bit < 8; ++bit) {
                    row.push_back((tmp & (0x80 >> bit)) != 0);
                }
        }
#endif
#else
            for(auto x = decltype(bitmap.width)(0); x < bitmap.width; ++x) {
                const auto byte_pos = x / 8;
                const auto bit_pos = (x % 8);
                const auto &tmp = bitmap.buffer[y * bitmap.pitch + byte_pos];
                row.push_back((tmp & (0x80 >> bit_pos)) != 0);
            }
#endif
            rows.push_back(row);
        }
    }

    return rows;
#if 0
      if (FT_Load_Char( face, u32str[n], FT_LOAD_RENDER )) continue; //一文字レンダリング
      // int yMax = face->bbox.yMax;
      // int yMin = face->bbox.yMin;
      // int baseline = bitmap->rows * yMax / (yMax - yMin);
      draw_bitmap(curPosX, curPosY - slot->bitmap_top); //imageにslot->bitmapの中身をコピーする
    }

  int  i, j, p, q;
  const int  x_max = x + (slot->bitmap).width;
  const int  y_max = y + (slot->bitmap).rows;

  for ( i = x, p = 0; i < x_max; i++, p++ ){
    for ( j = y, q = 0; j < y_max; j++, q++ ){
      if ( i < 0      || j < 0       || i >= WIDTH || j >= HEIGHT) continue;
      image[j*WIDTH + i] |= (slot->bitmap).buffer[q *(slot->bitmap).width + p];
    }
  }
#endif
#if 0
            const auto &error = FT_Load_Char(face_, u32char, FT_LOAD_RENDER | FT_LOAD_MONOCHROME); // 横書き アンチエリアシングなし
            if(error) {
                assert(false);
            }
            auto bitmap = slot->bitmap;
            for(int loopy = 0; loopy < bitmap.rows; loopy++) {
                for(int byte_index = 0; byte_index < bitmap.pitch; byte_index++) {
                    byte_value = bitmap.buffer[loopy * bitmap.pitch + byte_index];


                }
            }
#endif
    //        pen_x += slot->advance.x >> 6;
    //        pen_y += slot->advance.y >> 6;
}

std::u32string FreeType::char2u32str(const char text[]) {
    std::u32string u32str = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(text);
    return u32str;
}
