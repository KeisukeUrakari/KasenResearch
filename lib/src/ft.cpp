
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

#if 0
    error = FT_Set_Pixel_Sizes(face_, 0, 16); //ピクセル単位でサイズ指定
    // error = FT_Set_Pixel_Sizes( face_, 0, 120 ); //ピクセル単位でサイズ指定
    // error = FT_Set_Char_Size( face, 0, 16 * 64, 300, 300); //ポイント単位でサイズ指定
    if(error) {
        assert(false);
    }
#endif
}

FreeType::~FreeType() {
    assert(FT_Done_Face(face_) == 0);
    assert(FT_Done_FreeType(library_) == 0);
}

    /*
I solved this issue with a simple fix, after I noticed that the actual height of the image was not calculated correctly.
This is because the height of the "tallest" glyph does not represent the actual height of the image. For example,
a character might be smaller than the tallest glyph, but placed higher above the baseline, and thus being out of bounds.

I introduced another variable called "maxDescent", which stores the maximum amount of pixels below the baseline.
It is calculated as following in the first loop:

Together with "maxAscent", which stores the maximum amount of pixels above the baseline,
I can calculate the actual height needed for the image by simply adding them together.

この問題は、画像の実際の高さが正しく計算されていないことに気づき、簡単な修正で解決しました。
これは、「最も高い」グリフの高さが、画像の実際の高さを表していないためです。
例えば、あるキャラクターが最も高いグリフよりも小さくても、
ベースラインよりも高い位置に配置されているため、範囲外になってしまうのです。

そこで「maxDescent」という別の変数を導入し、ベースラインより下にあるピクセルの最大量を格納しました。
この変数は、最初のループで以下のように計算されます。

ベースラインからの最大ピクセル数を格納する "maxAscent "と合わせて、
画像に必要な実際の高さを計算することができます。


if ((glyph->metrics.height >> 6) - glyph->bitmap_top > (int)maxDescent) {
    maxDescent = (glyph->metrics.height >> 6) - glyph->bitmap_top;
}

 if (glyph->bitmap_top > (int)maxAscent) {
      maxAscent = glyph->bitmap_top;
  }

imageHeight = maxAscent + maxDescent
*/


std::vector<FTBitmap> FreeType::draw(const std::string &text, int size) {
    assert(FT_Set_Pixel_Sizes(face_, 0, size) == 0); //ピクセル単位でサイズ指定

    std::u32string u32str = char2u32str(text.c_str());

    auto &slot = face_->glyph;
    auto &bitmap = slot->bitmap;
    std::vector<FTBitmap> ret;

    for(const auto &u32char : u32str) {
        const auto error = FT_Load_Char(face_, u32char, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
        assert(!error);

        FTBitmap fb;

        for(auto y = decltype(bitmap.rows)(0); y < bitmap.rows; ++y) {
            auto row = std::vector<bool>();
            for(auto x = decltype(bitmap.width)(0); x < bitmap.width; ++x) {
                const auto byte_pos = x / 8;
                const auto bit_pos = (x % 8);
                const auto &tmp = bitmap.buffer[y * bitmap.pitch + byte_pos];
                row.push_back((tmp & (0x80 >> bit_pos)) != 0);
            }
            fb.rows.push_back(row);
        }
        fb.width = fb.rows[0].size();
        fb.height = fb.rows.size();
        fb.advanceX = slot->advance.x >> 6;
        fb.advanceY = slot->advance.y >> 6;
        fb.top = slot->bitmap_top;
        fb.left = slot->bitmap_left;
        fb.decender = (face_->size->metrics.descender >> 6);
        //        pen_x += slot->advance.x >> 6;
        //        pen_y += slot->advance.y >> 6;

        fb.desc = (slot->metrics.height >> 6) - slot->bitmap_top ;
        fb.asc = slot->bitmap_top;

        ret.push_back(fb);
    }

    return ret;
}

std::u32string FreeType::char2u32str(const char text[]) {
    std::u32string u32str = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(text);
    return u32str;
}
