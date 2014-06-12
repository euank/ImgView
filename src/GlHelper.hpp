#ifndef EUANK_CPSC404_GLHELPER
#define EUANK_CPSC404_GLHELPER
#include <iostream>
#include "Image.hpp"
#include <GL/gl.h>
namespace euank {
  namespace cpsc404 {
    /**
     * @brief Utility OpenGL wrapper to display an image
     *
     * Provides methods to load and display an image, as well
     * as handlers.
     */
    class GlHelper {
      public:
        /**
         * Create a GlHelper with the given image.
         * 
         * The given image will be loaded into a texture and
         * opengl will be initialized in preparation of running.
         * 
         * @param i The image to display
         */
        GlHelper(Image i);

        /**
         * Set the filename to save to with GlHelper::Writeimage
         * 
         * @param filename The file to save to
         */
        void SetOutputFilename(std::string filename);
        /**
         * Starts the opngl loop, thus displaying the image and reacting to events
         */
        void Run();
        /**
         * Write out what OpenGL is currently displaying to the filename specified in GlHelper::SetOutputFilename
         */
        void WriteImage();

      private:
        /** Image passed in */
        Image img;
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

        /** Name of file to write to */
        std::string outfile;

        /** Texture id for the image 2d texture */
        GLuint texture;
        /** Internal and image format as the OpenGL GL_* type */
        GLuint glFormat;

        void Init();
        void LoadTexture();
        static void StaticKeypressHandler(unsigned char key, int x, int y);
        void KeypressHandler(unsigned char key, int x, int y);



        static GlHelper *staticHelper;
        static void StaticDrawLoop();
        void DrawLoop();

        static void StaticReshapeHandler(int width, int height);
    };
  }
}
#endif
