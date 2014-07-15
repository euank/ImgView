#ifndef EUANK_CPSC404_CONFIG
#define EUANK_CPSC404_CONFIG

#include <string>
#include <vector>

namespace euank {
  namespace cpsc404 {
    /**
     * Stores a viewing configuration per instancee of a photoviewer
     * including the photos to view, filters, and scaling things.
     */
    class ImageConfig {
      public:

        ImageConfig();
        ImageConfig(int argc, char **argv);

        /** Invert the image? */
        bool inverted;
        /** Display the red channel? */
        bool redChannel;
        /** Display the blue channel? */
        bool blueChannel;
        /** Display the green channel? */
        bool greenChannel;
        /** Preserve aspect ratio? */
        bool preserveAR;

        std::vector<std::string> images;
        uint64_t current_image;
      private:


    };
  }
}

#endif
