#include "Image.hpp"
#include <OpenImageIO/imageio.h>
#include <string>

OIIO_NAMESPACE_USING

namespace euank {
  namespace cpsc404 {
    Image::Image(){}

    Image::Image(std::string filename) {
      ImageInput *in = ImageInput::open(filename);

      if(!in) {
        /* Yes everyone hates exceptions in C++, but they're not so bad now. Really */
        throw std::string(OpenImageIO::geterror());
      }

      const ImageSpec &spec = in->spec();
      width = spec.width;
      height = spec.height;
      channels = spec.nchannels;
      for(std::vector<std::string>::const_iterator it = spec.channelnames.begin(); it != spec.channelnames.end(); it++) {
        channelFormat += *it;
      }
      pixels.resize(width*height*channels);

      if(!in->read_image(TypeDesc::UINT8, &pixels[0]) || !in->close()) {
        delete in;
        throw std::string(OpenImageIO::geterror());
      }

      std::cout << "Read " << filename << ":" << std::endl;
      Image::PrintSpec(spec);

      delete in;

      /* C++ spec actually promises the vector data is contiguous, so this is A-okay */
      rawPixels = &pixels[0];
    }

    void Image::Write(std::string outfile, uint8_t *data, int xres, int yres, int chans) {
      ImageOutput *out = ImageOutput::create(outfile);
      int scanlinesize = xres * chans * sizeof(data[0]);
      int outChans = chans;


      if(!out) {
        throw std::string(OpenImageIO::geterror());
      }

      /* OpenImageIO docs say formats that don't support enough channels should silently write it with fewer channels
       * than requested, but ppm does not do that. We override it in that case. hdr noisily fails, we push it a little
       * closer to the OpenImageIO recommendations too. */
      if((strcmp(out->format_name(), "pnm") == 0  || strcmp(out->format_name(), "hdr") == 0) && outChans == 4) {
        outChans = 3;
      }
      ImageSpec spec(xres, yres, outChans, TypeDesc::UINT8);


      if(!out->open(outfile, spec)) {
        std::string err(out->geterror());
        delete out;
        throw err;
      }

      /* Write image from bottom up so it's not upside down. -scanline indicates this.
       * Also use UINT8... turns out OpenImageIO magically converts it to something else if the format 
       * thinks it should (see writing hdr; you get a float)
       */
      if(!out->write_image(TypeDesc::UINT8, (char *)data+(yres-1)*scanlinesize, chans, -scanlinesize, AutoStride) || !out->close()) {
        std::string err(out->geterror());
        delete out;
        throw err;
      }

      std::cout << "Wrote " << outfile << ":" << std::endl;

      delete out;
    }

    /** 
     * Print information about an image
     *
     * Convenience function to print the width, height, channels (and names), and depth
     * @param spec Image spec to print information about
     */
    void Image::PrintSpec(ImageSpec spec) {
      std::cout << "\tDimensions: " << spec.width << "x" << spec.height << std::endl
        << "\tChannels: " << spec.nchannels;
      if(spec.channelnames.size() > 0) {
        std::cout << " (" << spec.channelnames[0];
      }
      for(std::vector<std::string>::const_iterator it = spec.channelnames.begin(); ++it != spec.channelnames.end();) {
        std::cout << ", " << *it;
      }
      int bits = spec.get_int_attribute ("oiio:BitsPerSample", 0);
      std::cout << ")" << std::endl << "\tColor Depth: " << Image::extended_format_name(spec.format, bits) << std::endl;
    }


    /**
     * Determine color depth information about an image
     *
     * This function is taken from the source code for 'iinfo'
     * which is included with OpenImageIO.
     * @param type TypeDesc to print information about
     * @param bits bit depth information
     * @return String describing the color depth
     */
    const char * Image::extended_format_name (TypeDesc type, int bits) {
        if (bits && bits < (int)type.size()*8) {
          // The "oiio:BitsPerSample" betrays a different bit depth in the
          // file than the data type we are passing.
          if (type == TypeDesc::UINT8 || type == TypeDesc::UINT16 ||
              type == TypeDesc::UINT32 || type == TypeDesc::UINT64)
            return ustring::format("uint%d", bits).c_str();
          if (type == TypeDesc::INT8 || type == TypeDesc::INT16 ||
              type == TypeDesc::INT32 || type == TypeDesc::INT64)
            return ustring::format("int%d", bits).c_str();
        }
        return type.c_str(); // use the name implied by type
      }

  }
}
