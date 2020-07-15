#include "pngdecoder.h"
#include "pngutils.h"
#include <png.h>
PngDecoder::PngDecoder() {
    m_color_type = 0;
    m_png_ptr = 0;
    m_info_ptr = m_end_info = 0;
    m_f = 0;
    // m_buf_supported = true;
    m_bit_depth = 0;
}

PngDecoder::~PngDecoder() { close(); }

bool PngDecoder::inputFile(const std::string &filename) {
    m_filename = filename;
    return true;
}

void PngDecoder::inputPngBuffer(uint8_t *pngBuffer, size_t size) {
    m_pngBuffer.data = new uint8_t[size];
    memcpy(m_pngBuffer.data, pngBuffer, size);
    m_pngBuffer.size = size;
}

static void readDataFromBuf(png_structp png_ptr, png_bytep data, png_size_t length) {
    PngSource *isource = (PngSource *)png_get_io_ptr(png_ptr);
    if ((int)(isource->offset + length) <= isource->size) {
        memcpy(data, isource->data + isource->offset, length);
        isource->offset += length;
    }
    else {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}

void PngDecoder::close() {
    std::cout << "delete" << std::endl;
    if (m_f) {
        fclose(m_f);
        m_f = 0;
    }
}

bool PngDecoder::run() {
    volatile bool result = false;
    close();

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);

    if (png_ptr) {
        png_infop info_ptr = png_create_info_struct(png_ptr);
        png_infop end_info = png_create_info_struct(png_ptr);

        m_png_ptr = png_ptr;
        m_info_ptr = info_ptr;
        m_end_info = end_info;

        if (info_ptr && end_info) {
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                if (m_pngBuffer.data != nullptr)
                    png_set_read_fn(png_ptr, &m_pngBuffer, (png_rw_ptr)readDataFromBuf);
                else {
                    m_f = fopen(m_filename.c_str(), "rb");
                    if (m_f) png_init_io(png_ptr, m_f);
                }

                if (m_f or m_pngBuffer.data != nullptr) {
                    png_uint_32 wdth, hght;
                    int bit_depth, color_type, num_trans = 0;
                    png_bytep trans;
                    png_color_16p trans_values;

                    png_read_info(png_ptr, info_ptr);

                    png_get_IHDR(png_ptr, info_ptr, &wdth, &hght, &bit_depth, &color_type, 0, 0, 0);

                    m_width = (int)wdth;
                    m_height = (int)hght;
                    m_color_type = color_type;
                    m_bit_depth = bit_depth;
                    result = true;
                }
            }
        }
    }
    if (!result) {
        std::cerr << "Fail to read header of Png" << std::endl;
        close();
    }

    png_infop info_ptr = (png_infop)m_info_ptr;
    png_infop end_info = (png_infop)m_end_info;

    if (m_color_type == PNG_COLOR_TYPE_PALETTE) {
        m_color = 3;
    }
    else if (m_color_type == PNG_COLOR_TYPE_GRAY) {
        m_color = 1;
    }
    else if (m_color_type == PNG_COLOR_TYPE_RGB) {
        m_color = 3;
    }
    else if (m_color_type == PNG_COLOR_TYPE_RGBA) {
        m_color = 4;
    }
    else if (m_color_type == PNG_COLOR_TYPE_GA) {
        m_color = 2;
    }
    else {
        m_color = 3;
    }

    int step = m_height * m_bit_depth / 8 * m_color;

    uint8_t **buffer = (uint8_t **)malloc(m_height);
    for (auto i = 0; i < m_height; i++) {
        buffer[i] = (uint8_t *)malloc(step);
    }
    m_outputStream.resize(m_width * m_height * m_bit_depth / 8 * m_color);

    if (m_png_ptr && m_info_ptr && m_end_info && m_width && m_height) {
        if (setjmp(png_jmpbuf(png_ptr)) == 0) {
            int y;

            if (!PngUtils::isBigEndian()) {
                png_set_swap(png_ptr);
            }

            if (m_color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);

            if ((m_color_type & PNG_COLOR_MASK_COLOR) == 0 && m_bit_depth < 8)
#if (PNG_LIBPNG_VER_MAJOR * 10000 + PNG_LIBPNG_VER_MINOR * 100 + PNG_LIBPNG_VER_RELEASE >= 10209) ||                   \
    (PNG_LIBPNG_VER_MAJOR == 1 && PNG_LIBPNG_VER_MINOR == 0 && PNG_LIBPNG_VER_RELEASE >= 18)
                png_set_expand_gray_1_2_4_to_8(png_ptr);
#else
                png_set_gray_1_2_4_to_8(png_ptr);
#endif

            png_set_bgr(png_ptr);

            png_set_interlace_handling(png_ptr);
            png_read_update_info(png_ptr, info_ptr);

            for (y = 0; y < m_height; y++) {
                buffer[y] = m_outputStream.data() + y * step;
            }

            png_read_image(png_ptr, buffer);
            png_read_end(png_ptr, end_info);

            if (m_png_ptr) {
                png_structp png_ptr = (png_structp)m_png_ptr;
                png_infop info_ptr = (png_infop)m_info_ptr;
                png_infop end_info = (png_infop)m_end_info;
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
                m_png_ptr = nullptr;
                m_info_ptr = nullptr;
                m_end_info = nullptr;
            }

            result = true;
        }
    }

    return result;
}