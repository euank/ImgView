#ifndef EUANK_CPSC404_IMAGE
#define EUANK_CPSC404_IMAGE
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <OpenImageIO/imageio.h>

OIIO_NAMESPACE_USING

namespace euank {
  namespace cpsc404 {
    /**
     * Stores a 2D image's data and provides methods to read and write
     */
    class Image {
      public:
        /** Pointer to tightly packed UINT8 format pixels */
        uint8_t *rawPixels;
        /** Vector of UINT8 format pixels */
        std::vector<unsigned char> pixels;
        /** Image width */
        uint64_t width;
        /** Image height */
        uint64_t height;
        /** Number of channels */
        int channels;
        /** String representing the format, e.g. RGB, RGBA, BGR, etc */
        std::string channelFormat;
        /** The file this image was read from */
        std::string filename;

        /**
         * Create a useless image object.
         *
         * This should only be called if it will shortly be overwritten
         */
        Image();
        /**
         * Create an image from the given filename
         *
         * Opens the file with OpenImageIO as UINT8 type.
         * @param filename Name of the file to read
         * @throws std::string message if it cannot open the message
         */
        Image(std::string filename);
        /**
         * Write image data to a file with OpenImageIO
         * 
         * Writes with OpenImageIO. It attempts to write it as UINT8 format, but oiio overrides that sometimes. We override a few oiio bugs too.
         * @param filename Name of the file to write to
         * @param data Buffer of size xres * yres * channels containing utf8 image data
         * @param xres Width of the image
         * @param yres Height of the image
         * @param channels Number of channels of the image
         * @throws std::string human readable message if something goes wrong
         */
        static void Write(std::string filename, uint8_t *data, int xres, int yres, int channels);

      private:
        static void PrintSpec(ImageSpec spec);
        static const char *extended_format_name(TypeDesc type, int bits);
    };
  }
}
#endif
