#include "../include/image.h"

void lab_bmp::image::crop(int32_t x, int32_t y, int32_t w, int32_t h) {
    if (!((0 <= x) && (x < x + w) && (x + w <= FILEHEADER.biWidth) &&
          (0 <= y) && (y < y + h) && (y + h <= FILEHEADER.biHeight))) {
        throw lab_bmp::wrong_arguments{};
    }

    uint32_t outbytesperrow = (3 * w + 3) / 4 * 4;
    uint32_t inbytesperrow = (3 * FILEHEADER.biWidth + 3) / 4 * 4;
    auto newdata = std::make_unique<char[]>((outbytesperrow * h));
    for (uint32_t i = 0; i < outbytesperrow * h; i++) {
        newdata.get()[i] = 0;
    }

    for (int ix = 0; ix < w; ix++) {
        for (int iy = 0; iy < h; iy++) {
            char *newdatap =                                      // NOLINT
                newdata.get() + ((h - 1 - iy) * outbytesperrow);  // NOLINT
            char *datap =                                         // NOLINT
                data.get() +                                      // NOLINT
                ((this->FILEHEADER.biHeight - 1 - (iy + y)) *     // NOLINT
                 inbytesperrow);                                  // NOLINT
            newdatap[3 * ix + 0] = datap[3 * (x + ix) + 0];
            newdatap[3 * ix + 1] = datap[3 * (x + ix) + 1];
            newdatap[3 * ix + 2] = datap[3 * (x + ix) + 2];
        }
    }
    data = std::move(newdata);
    INFOHEADER.bfsize +=
        outbytesperrow * h - FILEHEADER.biHeight * inbytesperrow;
    FILEHEADER.biHeight = h;
    FILEHEADER.biWidth = w;
}

void lab_bmp::image::rotate_clockwise() {
    uint32_t outbytesperrow = (3 * this->FILEHEADER.biHeight + 3) / 4 * 4;
    uint32_t inbytesperrow = (3 * this->FILEHEADER.biWidth + 3) / 4 * 4;
    this->FILEHEADER.biSizeImage = this->FILEHEADER.biWidth * outbytesperrow;
    auto newdata =
        std::make_unique<char[]>((outbytesperrow * this->FILEHEADER.biWidth));
    for (uint32_t i = 0; i < this->FILEHEADER.biSizeImage; i++) {
        newdata.get()[i] = 0;
    }

    for (int ix = 0; ix < this->FILEHEADER.biWidth; ix++) {
        for (int iy = 0; iy < this->FILEHEADER.biHeight; iy++) {
            int ny = ix;
            int nx = -iy + this->FILEHEADER.biHeight - 1;
            char *newdatap =                                          // NOLINT
                newdata.get() +                                       // NOLINT
                ((this->FILEHEADER.biWidth - 1 - ny) *                // NOLINT
                 outbytesperrow);                                     // NOLINT
            char *datap =                                             // NOLINT
                data.get() + ((this->FILEHEADER.biHeight - 1 - iy) *  // NOLINT
                              inbytesperrow);                         // NOLINT
            newdatap[3 * nx + 0] = datap[3 * ix + 0];
            newdatap[3 * nx + 1] = datap[3 * ix + 1];
            newdatap[3 * nx + 2] = datap[3 * ix + 2];
        }
    }
    data = std::move(newdata);
    struct BITMAPFILEHEADER tmp;
    std::memcpy(reinterpret_cast<char *>(&tmp),
                reinterpret_cast<char *>(&this->FILEHEADER.biHeight),
                sizeof(BITMAPFILEHEADER::biHeight));
    std::memcpy(reinterpret_cast<char *>(&this->FILEHEADER.biHeight),
                reinterpret_cast<char *>(&this->FILEHEADER.biWidth),
                sizeof(BITMAPFILEHEADER::biHeight));
    std::memcpy(reinterpret_cast<char *>(&this->FILEHEADER.biWidth),
                reinterpret_cast<char *>(&tmp),
                sizeof(BITMAPFILEHEADER::biHeight));
    // std::swap(BITMAPFILEHEADER.biWidth, BITMAPFILEHEADER.biHeight);
}

lab_bmp::image::image(const image &another_image) {
    *this = another_image;
}

lab_bmp::image::image(image &&another_image) noexcept {
    *this = std::move(another_image);
}

lab_bmp::image &lab_bmp::image::operator=(const image &another_image) {
    std::memcpy(reinterpret_cast<char *>(&INFOHEADER),
                reinterpret_cast<const char *>(&another_image.INFOHEADER),
                sizeof(INFOHEADER));
    std::memcpy(reinterpret_cast<char *>(&FILEHEADER),
                reinterpret_cast<const char *>(&another_image.FILEHEADER),
                sizeof(FILEHEADER));

    uint32_t bytesperrow = (3 * another_image.FILEHEADER.biWidth + 3) / 4 * 4;
    data = std::make_unique<char[]>(
        (bytesperrow * another_image.FILEHEADER.biHeight));

    std::memcpy(data.get(), another_image.data.get(),
                bytesperrow * another_image.FILEHEADER.biHeight);
    return *this;
}

lab_bmp::image &lab_bmp::image::operator=(image &&another_image) noexcept {
    std::memcpy(reinterpret_cast<char *>(&INFOHEADER),
                reinterpret_cast<const char *>(&another_image.INFOHEADER),
                sizeof(INFOHEADER));
    std::memcpy(reinterpret_cast<char *>(&FILEHEADER),
                reinterpret_cast<const char *>(&another_image.FILEHEADER),
                sizeof(FILEHEADER));
    data = std::move(another_image.data);
    return *this;
}
