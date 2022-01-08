#include "../memstream.h"

#if 0
CExternalBuffer::CExternalBuffer(char *buffer, std::size_t bufferLength) {
    char *bufferEnd = buffer + bufferLength;
    setp(buffer, bufferEnd);
    setg(buffer, buffer, bufferEnd);
}

std::ios::pos_type CExternalBuffer::seekoff(std::ios::off_type __off, std::ios_base::seekdir __way, std::ios_base::openmode __which) {
    std::ios::pos_type result;

    // 入出力の処理対象を取得します。
    bool isIn = (__which & std::ios_base::in) == std::ios_base::in;
    bool isOut = (__which & std::ios_base::out) == std::ios_base::out;

    if(!isIn && !isOut) {
        // 入力にも出力にも該当しない場合はエラーとします。
        result = std::ios::pos_type(std::ios::off_type(-1));
    } else if((isIn && isOut) && (__way == std::ios_base::cur)) {
        // 入出力両方が対象で、現在位置からの移動の場合はエラーとします。
        result = std::ios::pos_type(std::ios::off_type(-1));
    } else {
        // 基本的な入出力指定に誤りがなかった場合の処理です。
        // 入出力それぞれの、バッファの移動先の位置を示すポインタをここに揃えます。
        char *targetIn;
        char *targetOut;

        // 入出力それぞれの、先頭からの移動先の位置を示すインデックスをここに揃えます。
        std::ios::pos_type posIn;
        std::ios::pos_type posOut;

        // 入力であれば、入力位置を計算します。
        if(isIn) {
            // どこからの相対位置かによって、移動先の位置を特定します。
            switch(__way) {
            case std::ios_base::beg:
                targetIn = this->eback() + __off;
                break;
            case std::ios_base::end:
                targetIn = this->egptr() + __off - 1;
                break;
            case std::ios_base::cur:
                targetIn = this->gptr() + __off;
                break;
            }

            // 移動先がバッファの範囲の収まるかどうかを調べます。
            if(this->eback() <= targetIn && targetIn < this->egptr()) {
                // 収まるときは、先頭からのインデックスを計算します。
                posIn = targetIn - this->eback();
            } else {
                // 収まらない場合は、エラーの状態にします。
                targetIn = nullptr;
                posIn = std::ios::pos_type(std::ios::off_type(-1));
            }
        } else {
            // 入力が対象でないときは、とりあえず入力についてはエラーの状態にします。
            targetIn = nullptr;
            posIn = std::ios::pos_type(std::ios::off_type(-1));
        }

        // 出力であれば、出力位置を計算します。

        if(isOut) {
            // どこからの相対位置かによって、移動先の位置を特定します。
            switch(__way) {
            case std::ios_base::beg:
                targetOut = this->pbase() + __off;
                break;
            case std::ios_base::end:
                targetOut = this->epptr() + __off - 1;
                break;
            case std::ios_base::cur:
                targetOut = this->pptr() + __off;
                break;
            }

            // 移動先がバッファの範囲の収まるかどうかを調べます。
            if(this->pbase() <= targetOut && targetOut < this->epptr()) {
                // 収まるときは、先頭からのインデックスを計算します。
                posOut = targetOut - this->pbase();
            } else {
                // 収まらない場合は、エラーの状態にします。
                targetOut = nullptr;
                posOut = std::ios::pos_type(std::ios::off_type(-1));
            }
        } else {
            // 出力が対象でないときは、とりあえず出力についてはエラーの状態にします。
            targetOut = nullptr;
            posOut = std::ios::pos_type(std::ios::off_type(-1));
        }

        // 実際の移動の前に、エラーがないか確認します。
        if((isIn && !targetIn) || (isOut && !targetOut)) {
            // 入力位置が特定できなかったか、出力位置が特定できなかった場合はエラーとします。
            result = std::ios::pos_type(std::ios::off_type(-1));
        } else if((isIn && isOut) && (posIn != posOut)) {
            // 入出力双方で位置が異なった場合はエラーとします。
            result = std::ios::pos_type(std::ios::off_type(-1));
        } else {
            // 戻り値を確定します。In も Out も同じ値なので、有効な方を取得します。
            result = (isIn ? posIn : posOut);

            // 入力指定で、現在位置が異なる場合に再設定します。
            if(isIn && (gptr() != targetIn)) {
                this->setg(this->eback(), targetIn, this->egptr());
            }

            // 出力指定で、現在位置が異なる場合に再設定します。
            if(isOut && (pptr() != targetOut)) {
                this->pbump(targetOut - this->pptr());
            }
        }
    }

    return result;
}

std::ios::pos_type CExternalBuffer::seekpos(std::ios::pos_type __sp, std::ios_base::openmode __which) {
    return seekoff(off_type(__sp), std::ios_base::beg, __which);
}
#endif