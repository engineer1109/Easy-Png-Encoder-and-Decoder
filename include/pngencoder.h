#ifndef LIPNGCODEC_PNGENCODER_H
#define LIPNGCODEC_PNGENCODER_H
#include "pngcodec_def.h"
//#include <opencv2/opencv.hpp>

class PNGCODEC_EXPORT PngEncoder {
public:
    enum class ZLIBCompressType {
        ZLIB_NO_COMPRESSION = 0,
        ZLIB_BEST_SPEED = 1,
        ZLIB_BEST_COMPRESSION = 9,
        ZLIB_DEFAULT_COMPRESSION = -1,
    };

public:
    PngEncoder();
    ~PngEncoder() {}

    void setInputRawBuffer(std::vector<uint8_t> &rawBuffer, int width, int height, int channels, int bit_depth);

    void setOutputEncodeStream(const std::string &filename);
    void setOutputEncodeStream(std::vector<uint8_t> &buf);
    void setZLIBCompressType(ZLIBCompressType type);
    bool run();

protected:
    // String m_description;
    static void writeDataToBuf(void *png_ptr, uint8_t *src, size_t size);
    static void flushBuf(void *png_ptr);
    std::string m_filename;
    std::vector<uint8_t> *m_buf;
    int m_width;
    int m_height;
    int m_bit_depth = 8;
    int m_color_type = 0;
    int m_color = 3;
    ZLIBCompressType m_zlibCompressType = ZLIBCompressType::ZLIB_BEST_COMPRESSION;

    std::vector<uint8_t> m_inputStream;
};

#endif