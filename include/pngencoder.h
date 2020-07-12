#ifndef LIPNGCODEC_PNGENCODER_H
#define LIPNGCODEC_PNGENCODER_H
#include "pngcodec_def.h"
#include <opencv2/opencv.hpp>

class PNGCODEC_EXPORT PngEncoder
{
public:
    PngEncoder();
    ~PngEncoder() {}
    //bool isFormatSupported(int depth) const;

    bool setDestination(const std::string& filename);
    bool setDestination(std::vector<uchar>& buf);
    bool write(const cv::Mat& img, const std::vector<int>& params);
    //virtual bool writemulti(const std::vector<Mat>& img_vec, const std::vector<int>& params);

    //virtual String getDescription() const;
    //PngImageEncoder newEncoder() const;

    //virtual void throwOnEror() const;

protected:
    //String m_description;
    static void writeDataToBuf(void* png_ptr, uchar* src, size_t size);
    static void flushBuf(void* png_ptr);
    std::string m_filename;
    std::vector<uchar>* m_buf;
    //bool m_buf_supported;

    //String m_last_error;
};

#endif