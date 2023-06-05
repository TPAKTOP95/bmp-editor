#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include "errors.h"

namespace lab_bmp {

#pragma pack(push, 1)
struct BITMAPINFOHEADER {
    uint16_t bfType = 0;
    uint32_t bfsize = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 0;
};
struct BITMAPFILEHEADER {
    uint32_t biSize = 0;
    int32_t biWidth = 0;
    int32_t biHeight = 0;
    uint16_t biPlanes = 0;
    uint16_t biBitCount = 0;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    uint32_t biXPelsPerMeter = 0;
    uint32_t biYPelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};
#pragma pack(pop)

struct image {
    BITMAPINFOHEADER INFOHEADER;   // NOLINT
    BITMAPFILEHEADER FILEHEADER;   // NOLINT
    std::unique_ptr<char[]> data;  // NOLINT

    image() = default;
    image(const image &);
    image(image &&) noexcept;
    image &operator=(const image &);
    image &operator=(image &&) noexcept;
    ~image() = default;

    void crop(int32_t, int32_t, int32_t, int32_t);

    void rotate_clockwise();
};
}  // namespace lab_bmp

#endif  // IMAGE_H_
