#ifndef ERRORS_H_
#define ERRORS_H_

#include <exception>
#include <stdexcept>
#include <string>

namespace lab_bmp {
struct missing_arguments : public std::runtime_error {
    explicit missing_arguments() : std::runtime_error("Missing arguments") {
    }
};

struct unable_to_open_file : public std::runtime_error {
    explicit unable_to_open_file(const std::string &s)
        : std::runtime_error("Unable to open file " + s) {
    }
};
struct bad_bmp_format : public std::runtime_error {
    explicit bad_bmp_format(const std::string &s) : std::runtime_error(s) {
    }
};

struct wrong_bmp_version : public bad_bmp_format {
    explicit wrong_bmp_version(const std::string &headersize)
        : bad_bmp_format(
              "Invalid BMP: expected version 3 and header size 40, but header "
              "size is " +
              headersize) {
    }
};

struct non_positive_height : public bad_bmp_format {
    explicit non_positive_height(const std::string &height)
        : bad_bmp_format("Invalid BMP: expected positive biHeight, got " +
                         height) {
    }
};

struct non_positive_width : public bad_bmp_format {
    explicit non_positive_width(const std::string &width)
        : bad_bmp_format("Invalid BMP: expected positive biWidth, got " +
                         width) {
    }
};

struct wrong_arguments : std::runtime_error {
    explicit wrong_arguments()
        : std::runtime_error("The requested area is not a subimage") {
    }
};

struct wrong_bit_per_pixel : public bad_bmp_format {
    explicit wrong_bit_per_pixel(const std::string &pixels)
        : bad_bmp_format("Invalid BMP: expected 24 bits per pixel, got " +
                         pixels) {
    }
};

struct color_palette_used : public bad_bmp_format {
    explicit color_palette_used()
        : bad_bmp_format("Invalid BMP: color palette is unsupported") {
    }
};

}  // namespace lab_bmp

#endif
