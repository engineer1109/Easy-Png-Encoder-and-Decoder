#ifndef LIPNGCODEC_PNGDECODER_H
#define LIPNGCODEC_PNGDECODER_H
#include "pngcodec_def.h"
#include <opencv2/opencv.hpp>
class PNGCODEC_EXPORT PngDecoder {
public:
    PngDecoder();
    ~PngDecoder();

    bool inputFile(const std::string &filename);
    void inputPngBuffer(uint8_t *pngBuffer, size_t size) {
        m_pngBuffer.data = new uint8_t[size];
        memcpy(m_pngBuffer.data, pngBuffer, size);
        m_pngBuffer.size = size;
    }
    bool setSource(const cv::Mat &buf) {
        m_filename = std::string();
        m_buf = buf;
        return true;
    }
    bool run();
    std::vector<uint8_t> &getDecodeStream() { return m_outputStream; }
    cv::Mat &getBuf() { return m_buf; }

    int &getWidth() { return m_width; }
    int &getHeight() { return m_height; }
    int &getBitDepth() { return m_bit_depth; }
    int &getColor() { return m_color; }

    int width() const { return m_width; }
    int height() const { return m_height; }
    int bitDepth() const { return m_bit_depth; }
    int color() const { return m_color; }
    int size() const { return m_width * m_height * m_bit_depth * m_color; }

    // static void readDataFromBuf(png_structp png_ptr, png_bytep data, png_size_t length);
    void close();

protected:
    int m_width = 0;  // width  of the image ( filled by readHeader )
    int m_height = 0; // height of the image ( filled by readHeader )
    int m_type = 0;
    int m_scale_denom;
    std::string m_filename;
    // String m_signature;
    cv::Mat m_buf;
    PngSource m_pngBuffer;
    int m_bit_depth = 8;
    void *m_png_ptr;  // pointer to decompression structure
    void *m_info_ptr; // pointer to image information structure
    void *m_end_info; // pointer to one more image information structure
    FILE *m_f;
    int m_color_type = 0;
    int m_color = 3;
    size_t m_buf_pos = 0;
    std::vector<uint8_t> m_outputStream;
};
#endif