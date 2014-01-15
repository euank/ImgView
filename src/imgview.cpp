#include <iostream>
#include <string> /* strerror */
#include "Image.hpp"
#include "GlHelper.hpp"

/**
 * The main function
 * Checks args, does appropriate file-related actions, and passes it on to opengl
 * @param argc number of args
 * @param argv Args
 * @return N/A, opengl's inifinite loop means this never returns.
 */
int main(int argc, char **argv) {
  /* Only support 1 or 2 arguments */
  if(argc < 2 || argc > 3) {
    std::cerr << "Usage:\n\t" << argv[0] << " <in> [<out>]\n" <<std::endl;
    return 1;
  }

  euank::cpsc404::Image i;

  try {
    i = euank::cpsc404::Image(argv[1]);
  } catch(std::string message) {
    std::cerr << "Error loading file \"" << argv[1] << "\". " << message << std::endl;
    return errno; /* Fatal */
  }

  euank::cpsc404::GlHelper gh = euank::cpsc404::GlHelper(i);
  std::string infile = std::string(argv[1]);
  if(argc == 3) {
    gh.SetOutputFilename(std::string(argv[2]));
  } else {
    std::string::size_type dotndx = infile.rfind('.');
    if(dotndx == std::string::npos) {
      gh.SetOutputFilename(infile + "_copy");
    } else {
      gh.SetOutputFilename(infile.substr(0, dotndx) + "_copy" + infile.substr(dotndx));
    }
  }
  gh.Run();

  return 0; /* Unreachable code ;_; */
}
