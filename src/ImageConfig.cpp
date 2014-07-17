#include "ImageConfig.hpp"
#include <vector>
#include <string>
namespace euank {
  namespace cpsc404 {
    ImageConfig::ImageConfig() {
      // Sane defaults
      inverted = false;
      redChannel = true;
      greenChannel = true;
      blueChannel = true;
      preserveAR = true;

      current_image = 0;
    }

    ImageConfig::ImageConfig(int argc, char **argv) {
      // Sane defaults
      inverted = false;
      redChannel = true;
      greenChannel = true;
      blueChannel = true;
      preserveAR = true;

      images = std::vector<std::string>();

      // Store each image listed on the command line
      for(int i=0;i<argc;i++) {
        images.push_back(std::string(argv[i]));
      }
      current_image = 0;
    }
  }
}
