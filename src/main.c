#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <eggx.h> /* EGGX を使う時に必要 */

int strwlen(char *buf)
{
    // LC_CTYPE をネイティブロケールに変更
    if( setlocale( LC_CTYPE, "" ) == NULL ){
        fprintf(stderr, "do not set locale.\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int count = 0;
    while( buf[i] != '\0' ){
        int res = mblen( &buf[i], MB_CUR_MAX );
        if( res < 0 ){
            fprintf(stderr, "Contains illegal characters.\n");
            exit(EXIT_FAILURE);
        }

        i += res;
        count++;
    }

    return count;
}

#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))

int main(int argc,char *argv[]){
    FT_Library library;
    FT_Face    face;
    FT_GlyphSlot slot;
    int error,win;
    char text[] = "こんにちは、世界！";
    wchar_t ws[256]; /* ワイド文字保存用 */
    int pen_x, pen_y;
    int num_chars = strwlen(text);
    
    int len = mbstowcs( ws, text, SIZE_OF_ARRAY(ws) );
    if( len == -1 ){
        fprintf(stderr, "mbstowcs error\n");
        exit( EXIT_FAILURE );
    }

    gsetinitialattributes(DISABLE, BOTTOM_LEFT_ORIGIN); /*eggxの座標設定を通常のグラフィックスと同じに*/
    win = gopen(1000,200); /* 1000x200 ピクセルのグラフィックス用ウィンドゥを開く */
    //layer(win, 0, 1); /* 描画レイヤを設定 */
    // FreeTypeの初期化とTrueTypeフォントの読み込み
    error = FT_Init_FreeType( &library );
    if(error) {
        fprintf(stderr,"error FT_Init_FreeType\n");
        exit(EXIT_FAILURE);
    }
    error = FT_New_Face( library, "/usr/share/fonts/google-noto-cjk/NotoSansCJK-Bold.ttc", 0, &face );
//    error = FT_New_Face( library, "/usr/share/fonts/opentype/noto/NotoSansCJK-Bold.ttc", 0, &face );
//    error = FT_New_Face( library, "/usr/share/fonts/noto-cjk/NotoSansCJK-Bold.ttc", 0, &face );

    if(error == FT_Err_Unknown_File_Format) {
        fprintf(stderr,"unsupport fonts format\n");
        exit(EXIT_FAILURE);
    } else if(error) {
        fprintf(stderr,"fonts file not found\n");
        exit(EXIT_FAILURE);
    }
    slot = face->glyph;

    error = FT_Set_Pixel_Sizes( face, 0, 120 ); //ピクセル単位でサイズ指定
    //error = FT_Set_Char_Size( face, 0, 16 * 64, 300, 300); //ポイント単位でサイズ指定
    if(error) {
        fprintf(stderr,"FT_Set_Pixel_Sizes\n");
        exit(EXIT_FAILURE);
    }
    pen_x = 10; // 文字の表示座標 左下のベースライン
    pen_y = 150;
    newrgbcolor(win,0,0,255);
    drawline(win,pen_x,pen_y,1000,pen_y);
    for (int n = 0;n < num_chars; n++ ){
        FT_Bitmap bitmap;

        // n文字目の文字をビットマップ化
        //error = FT_Load_Char( face, text[n], FT_LOAD_VERTICAL_LAYOUT|FT_LOAD_RENDER );//縦書き
        error = FT_Load_Char( face, ws[n], FT_LOAD_RENDER |FT_LOAD_MONOCHROME );// 横書き アンチエリアシングなし
        if(error) {
            fprintf(stderr,"FT_Load_Char error\n");
            exit(EXIT_FAILURE);
        }
        bitmap = slot->bitmap;

        newrgbcolor(win,255,255,255);/*白色*/
        for( int loopy = 0; loopy < bitmap.rows ; loopy++){
            for(int byte_index=0;byte_index<bitmap.pitch;byte_index++) {
                int byte_value,rowstart,end_loop,num_bits_done;
                byte_value = bitmap.buffer[loopy*bitmap.pitch+byte_index];
                num_bits_done = byte_index*8;
                rowstart = loopy*bitmap.width + byte_index*8;
                if(8 > (bitmap.width - num_bits_done)) {
                    end_loop = bitmap.width - num_bits_done;
                } else {
                    end_loop = 8;
                }
                for(int bit_index=0;bit_index<end_loop;bit_index++) {
                    int bit = byte_value & (1<<(7-bit_index));
                    if(bit!=0) {
                        int x = (bit_index+rowstart)%bitmap.width + pen_x + slot->bitmap_left;
                        int y = (bit_index+rowstart)/bitmap.width + pen_y - slot->bitmap_top + (face->size->metrics.descender >>6);
                        pset(win,x,y); /* 点を描く */
                    }
                }
            }
        }

        pen_x += slot->advance.x >> 6;
        pen_y += slot->advance.y >> 6;

    }
    //copylayer(win, 1, 0);  /* レイヤ1をレイヤ0に瞬時にコピー */
    ggetch(); /* キー入力があるまで待つ */
    gclose(win); /* グラフィックス用ウィンドゥを閉じる */
    error = FT_Done_Face( face );
    if(error) {
        fprintf(stderr,"error FT_Done_Face\n");
        exit(EXIT_FAILURE);
    }
    error = FT_Done_FreeType( library );
    if(error) {
        fprintf(stderr,"error FT_Done_FreeType\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}