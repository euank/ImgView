#ifndef EUANK_CPSC404_GLHELPER
#define EUANK_CPSC404_GLHELPER
#include <iostream>
#include "Image.hpp"
#include "ImageConfig.hpp"
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
        GlHelper(ImageConfig conf);

        /**
         * Starts the opngl loop, thus displaying the image and reacting to events
         */
        void Run();

      private:
        /** Image passed in */
        Image img;

        ImageConfig config;

        /** Texture id for the image 2d texture */
        GLuint texture;
        /** Internal and image format as the OpenGL GL_* type */
        GLuint glFormat;

        void Init();
        void LoadTexture();
        void LoadImage();
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
