//
// Created by wjl on 2020/7/12.
//

#ifndef PNGCODEC_PNGUTILS_H
#define PNGCODEC_PNGUTILS_H
#include <array>
#include <memory>
#include <vector>

class PngUtils {
public:
    static bool isBigEndian();
};

enum ImwritePNGFlags_zc {
    IMWRITE_PNG_STRATEGY_DEFAULT = 0,
    IMWRITE_PNG_STRATEGY_FILTERED = 1,
    IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY = 2,
    IMWRITE_PNG_STRATEGY_RLE = 3,
    IMWRITE_PNG_STRATEGY_FIXED = 4
};

enum ImwriteFlags_zc { IMWRITE_PNG_COMPRESSION = 16, IMWRITE_PNG_STRATEGY = 17, IMWRITE_PNG_BILEVEL = 18 };

#endif // PNGCODEC_PNGUTILS_H
