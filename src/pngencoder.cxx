#include "pngencoder.h"
#include "pngutils.h"
#include <png.h>

#define Z_NO_COMPRESSION 0
#define Z_BEST_SPEED 1
#define Z_BEST_COMPRESSION 9
#define Z_DEFAULT_COMPRESSION (-1)

#define Z_FILTERED 1
#define Z_HUFFMAN_ONLY 2
#define Z_RLE 3
#define Z_FIXED 4
#define Z_DEFAULT_STRATEGY 0

PngEncoder::PngEncoder() { m_buf = 0; }

bool PngEncoder::setDestination(const std::string &filename) {
    m_filename = filename;
    m_buf = 0;
    return true;
}

bool PngEncoder::setDestination(std::vector<uchar> &buf) {
    m_buf = &buf;
    m_buf->clear();
    m_filename = std::string();
    return true;
}

void PngEncoder::writeDataToBuf(void *_png_ptr, uchar *src, size_t size) {
    if (size == 0) return;
    png_structp png_ptr = reinterpret_cast<png_structp>(_png_ptr);
    PngEncoder *encoder = (PngEncoder *)(png_get_io_ptr(png_ptr));
    CV_Assert(encoder && encoder->m_buf);
    size_t cursz = encoder->m_buf->size();
    encoder->m_buf->resize(cursz + size);
    memcpy(&(*encoder->m_buf)[cursz], src, size);
}

void PngEncoder::flushBuf(void *) {}

bool PngEncoder::write(const cv::Mat &img, const std::vector<int> &params) {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop info_ptr = 0;
    FILE *volatile f = 0;
    int y, width = img.cols, height = img.rows;
    int depth = img.depth(), channels = img.channels();
    volatile bool result = false;
    cv::AutoBuffer<uchar *> buffer;

    if (depth != CV_8U && depth != CV_16U) return false;

    if (png_ptr) {
        info_ptr = png_create_info_struct(png_ptr);

        if (info_ptr) {
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                if (m_buf) {
                    png_set_write_fn(png_ptr, this, (png_rw_ptr)writeDataToBuf, (png_flush_ptr)flushBuf);
                }
                else {
                    std::cout << "write file" << std::endl;
                    f = fopen(m_filename.c_str(), "wb");
                    if (f) png_init_io(png_ptr, (png_FILE_p)f);
                }

                int compression_level = -1;                          // Invalid value to allow setting 0-9 as valid
                int compression_strategy = IMWRITE_PNG_STRATEGY_RLE; // Default strategy
                bool isBilevel = false;

                for (size_t i = 0; i < params.size(); i += 2) {
                    if (params[i] == IMWRITE_PNG_COMPRESSION) {
                        compression_strategy = IMWRITE_PNG_STRATEGY_DEFAULT; // Default strategy
                        compression_level = params[i + 1];
                        compression_level = MIN(MAX(compression_level, 0), Z_BEST_COMPRESSION);
                    }
                    if (params[i] == IMWRITE_PNG_STRATEGY) {
                        compression_strategy = params[i + 1];
                        compression_strategy = MIN(MAX(compression_strategy, 0), Z_FIXED);
                    }
                    if (params[i] == IMWRITE_PNG_BILEVEL) {
                        isBilevel = params[i + 1] != 0;
                    }
                }

                if (m_buf || f) {
                    if (compression_level >= 0) {
                        png_set_compression_level(png_ptr, compression_level);
                    }
                    else {
                        png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_SUB);
                        png_set_compression_level(png_ptr, Z_BEST_SPEED);
                    }
                    png_set_compression_strategy(png_ptr, compression_strategy);

                    png_set_IHDR(png_ptr, info_ptr, width, height, depth == CV_8U ? isBilevel ? 1 : 8 : 16,
                                 channels == 1 ? PNG_COLOR_TYPE_GRAY
                                               : channels == 3 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
                                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

                    png_write_info(png_ptr, info_ptr);

                    if (isBilevel) png_set_packing(png_ptr);

                    png_set_bgr(png_ptr);
                    if (!PngUtils::isBigEndian()) {
                        png_set_swap(png_ptr);
                    }

                    buffer.allocate(height);
                    for (y = 0; y < height; y++)
                        buffer[y] = img.data + y * img.step;

                    unsigned char **ptr = buffer.operator uchar **();
                    // png_write_image(png_ptr, buffer.data());
                    png_write_image(png_ptr, ptr);
                    png_write_end(png_ptr, info_ptr);

                    result = true;
                }
            }
        }
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);
    if (f) fclose((FILE *)f);

    return result;
}