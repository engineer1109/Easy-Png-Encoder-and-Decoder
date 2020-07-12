#include "pngdecoder.h"
#include "pngencoder.h"
#include <opencv2/opencv.hpp>

void Test_PngDecoder() {
    PngDecoder decoder;
    std::fstream fileStream(std::string(PNG_DEMO_DATA_DIR) + "/test1.png", std::ios::in);
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

    std::vector<uint8_t> out = decoder.getDecodeStream();
    std::cout << out.size();

    cv::Mat img(decoder.width(), decoder.height(), CV_8UC3, out.data());

    // decoder.readData(img);

    cv::imshow("img", img);
    cv::waitKey(0);
}

void Test_PngEncoder() {
    PngEncoder encoder;
    encoder.setDestination(std::string(PNG_DEMO_DATA_DIR) + "/test1_copy.png");
    cv::Mat img = cv::imread(std::string(PNG_DEMO_DATA_DIR) + "/test1.png");
    encoder.write(img, {9});
}

int main(int argc, char **argv) {
    Test_PngDecoder();
    Test_PngEncoder();
}