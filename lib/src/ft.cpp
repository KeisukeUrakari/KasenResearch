
#include "../ft.h"

#include <cassert>
#include <codecvt>
#include <locale>

FreeType::FreeType(const std::string &fontfile, int dotsize)
    : dotsize_(dotsize), extsize_(0) {
    auto error = FT_Init_FreeType(&library_);
    assert(error == 0);

    error = FT_New_Face(library_, fontfile.c_str(), 0, &face_);

    if(error == FT_Err_Unknown_File_Format) {
        // [ERROR] Font file format is not supported!!
        assert(false);
    } else if(error) {
        // [ERROR] Font file not found or it is broken!
        assert(false);
    }

    calcextsize();
}

FreeType::~FreeType() {
    assert(FT_Done_Face(face_) == 0);
    assert(FT_Done_FreeType(library_) == 0);
}

FTBitmap FreeType::getinfo(FT_ULong u32char, int size) const {
    auto error = FT_Set_Pixel_Sizes(face_, 0, size); //ピクセル単位でサイズ指定
    assert(!error);

    error = FT_Load_Char(face_, u32char, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
    assert(!error);

    auto &slot = face_->glyph;
    auto &bitmap = slot->bitmap;

    FTBitmap fb;
    fb.width = bitmap.width;
    fb.desc = (slot->metrics.height >> 6) - slot->bitmap_top;
    fb.asc = slot->bitmap_top;

    return fb;
}

void FreeType::calcextsize() {
    auto sz = dotsize_;
    auto a = FTBitmap();
    auto w = FTBitmap();
    for(; ((a.asc + a.desc) < dotsize_) && (a.width < dotsize_) && ((w.asc + w.desc) < dotsize_) && (w.width < dotsize_) ; ) {
        sz++;
        a = getinfo(char2u32str("あ")[0], sz);
        w = getinfo(char2u32str("ｗ")[0], sz);
    }

    sz -= 1;
    a = getinfo(char2u32str("あ")[0], sz);
    const auto top = (dotsize_ - (a.asc + a.desc)) / 2;
    extsize_ = sz;
    baseline_ = top + a.asc;
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

std::vector<FTBitmap> FreeType::draw(const std::string &text) const {
    assert(FT_Set_Pixel_Sizes(face_, 0, extsize_) == 0); //ピクセル単位でサイズ指定

    std::u32string u32str = char2u32str(text.c_str());

    auto &slot = face_->glyph;
    auto &bitmap = slot->bitmap;
    std::vector<FTBitmap> ret;

    for(const auto &u32char : u32str) {
        const auto error = FT_Load_Char(face_, u32char, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
        assert(!error);

        FTBitmap fb;
        fb.width = bitmap.width;
        fb.asc = slot->bitmap_top;
        fb.desc = (slot->metrics.height >> 6) - slot->bitmap_top;

        auto liftup = int(0);
        if((baseline_ + fb.desc) > dotsize_) {
            liftup = (baseline_ + fb.desc) - dotsize_;
        }
        fb.asc += liftup;
        fb.desc -= liftup;

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
        ret.push_back(fb);
    }

    return ret;
}

std::u32string FreeType::char2u32str(const char text[]) {
    std::u32string u32str = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(text);
    return u32str;
}
