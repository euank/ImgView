#include <iostream>
#include <string> /* strerror */
#include "Image.hpp"
#include "GlHelper.hpp"
#include "ImageConfig.hpp"

/**
 * The main function
 * Checks args, does appropriate file-related actions, and passes it on to opengl
 * @param argc number of args
 * @param argv Args
 * @return N/A, opengl's inifinite loop means this never returns.
 */
int main(int argc, char **argv) {
  if(argc < 2) {
    std::cerr << "Usage:\n\t" << argv[0] << " <img1> [<img2> ...] " << std::endl;
    return 1;
  }

  euank::cpsc404::ImageConfig conf = euank::cpsc404::ImageConfig(argc, argv);

  euank::cpsc404::GlHelper gh = euank::cpsc404::GlHelper(conf);
  std::string infile = std::string(argv[1]);

  gh.Run();

  return 0; /* Unreachable code ;_; */
}
