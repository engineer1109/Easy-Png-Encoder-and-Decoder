#include "pngdecoder.h"
#include "pngencoder.h"
#include <opencv2/opencv.hpp>

void Test_PngDecoder_PngBufferToRawBuffer() {
    PngDecoder decoder;
    std::fstream fileStream(std::string(PNG_DEMO_DATA_DIR) + "/test1.png", std::ios::in | std::ios::binary);
    if (!fileStream) {
        std::cerr << "Test Png File not found" << std::endl;
    }
    fileStream.seekg(0, fileStream.end);
    size_t streamSize = fileStream.tellg();
    fileStream.seekg(0, fileStream.beg);
    std::vector<uint8_t> pngData(streamSize);
    fileStream.read((char *)pngData.data(), streamSize);
    decoder.inputPngBuffer(pngData.data(), streamSize);
    decoder.run();
    std::cout << "width" << decoder.width() << std::endl;
    std::cout << "height" << decoder.height() << std::endl;
    std::cout << "bitDepth" << decoder.bitDepth() << std::endl;
    std::cout << "color" << decoder.color() << std::endl;

    std::vector<uint8_t> out = decoder.getOutputDecodeStream();
    std::cout << out.size();

    cv::Mat img(decoder.width(), decoder.height(), CV_8UC3, out.data());

    // decoder.readData(img);

    cv::imshow("img", img);
    cv::waitKey(0);
}

void Test_PngEncoder_RawBufferToPngBuffer() {
    PngEncoder encoder;
    std::vector<uint8_t> out;
    // encoder.setEncodeStream(std::string(PNG_DEMO_DATA_DIR) + "/test1_copy.png");
    encoder.setOutputEncodeStream(out);
    cv::Mat img = cv::imread(std::string(PNG_DEMO_DATA_DIR) + "/test1.png");
    std::vector<uint8_t> rawBuffer(img.data, img.data + img.cols * img.rows * img.channels());
    encoder.setInputRawBuffer(rawBuffer, img.cols, img.rows, img.channels(), 8);
    encoder.setZLIBCompressType(PngEncoder::ZLIBCompressType::ZLIB_BEST_COMPRESSION);
    encoder.run();
    std::cout << out.size() << std::endl;
    std::fstream outPng(std::string(PNG_DEMO_DATA_DIR) + "/test1_copy.png", std::ios::out | std::ios::binary);
    outPng.write((char *)out.data(), out.size());
    outPng.close();
}

int main(int argc, char **argv) {
    Test_PngDecoder_PngBufferToRawBuffer();
    Test_PngEncoder_RawBufferToPngBuffer();
}