#include <fstream>
#include "../include/errors.h"
#include "../include/image.h"

int main(int argc, char *argv[]) {
    try {
        if (argc != 8) {
            throw lab_bmp::missing_arguments{};
        }
        std::ifstream in_file(argv[2], std::ios::binary);
        if (!in_file) {
            throw lab_bmp::unable_to_open_file(argv[2]);
        }
        lab_bmp::image in_image;
        in_file.read(reinterpret_cast<char *>(&in_image),
                     sizeof(lab_bmp::BITMAPFILEHEADER) +
                         sizeof(lab_bmp::BITMAPINFOHEADER));

        if (in_image.FILEHEADER.biSize != sizeof(lab_bmp::BITMAPFILEHEADER)) {
            throw lab_bmp::wrong_bmp_version(
                std::to_string(in_image.FILEHEADER.biSize));
        }

        if (in_image.FILEHEADER.biHeight <= 0) {
            throw lab_bmp::non_positive_height(
                std::to_string(in_image.FILEHEADER.biHeight));
        }

        if (in_image.FILEHEADER.biWidth <= 0) {
            throw lab_bmp::non_positive_width(
                std::to_string(in_image.FILEHEADER.biWidth));
        }

        if (in_image.FILEHEADER.biBitCount != 24) {
            throw lab_bmp::wrong_bit_per_pixel(
                std::to_string(in_image.FILEHEADER.biBitCount));
        }

        if (in_image.FILEHEADER.biClrUsed != 0) {
            throw lab_bmp::color_palette_used{};
        }

        uint32_t inbytesperrow = (3 * in_image.FILEHEADER.biWidth + 3) / 4 * 4;
        in_image.data = std::make_unique<char[]>(inbytesperrow *
                                                 in_image.FILEHEADER.biHeight);

        in_file.read(reinterpret_cast<char *>(in_image.data.get()),
                     inbytesperrow * in_image.FILEHEADER.biHeight);
        in_file.close();

        int32_t x = std::stoi(argv[4]);
        int32_t y = std::stoi(argv[5]);
        int32_t w = std::stoi(argv[6]);
        int32_t h = std::stoi(argv[7]);

        in_image.crop(x, y, w, h);
        in_image.rotate_clockwise();

        std::ofstream out_file(argv[3], std::ios::binary);
        if (!out_file.is_open()) {
            throw lab_bmp::unable_to_open_file(std::string(argv[3]));
        }
        out_file.write(reinterpret_cast<char *>(&in_image),
                       sizeof(lab_bmp::BITMAPFILEHEADER) +
                           sizeof(lab_bmp::BITMAPINFOHEADER));
        out_file.write(reinterpret_cast<char *>(&in_image.data[0]),
                       in_image.FILEHEADER.biSizeImage);
        out_file.close();
    } catch (std::bad_alloc &) {
        std::cerr << "Insufficient memory" << std::endl;
        return 1;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
