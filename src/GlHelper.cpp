/** Have opengl define prototypes */
#define GL_GLEXT_PROTOTYPES 1
#include <GL/glut.h>
#include <GL/gl.h>
#include <cstring>
#include <string>
#include <iostream>

#include "GlHelper.hpp"
#include "Image.hpp"

/** Keycode for the escape key */
#define ESCAPE_KEYCODE 27

namespace euank {
  namespace cpsc404 {
    GlHelper *GlHelper::staticHelper = NULL;

    GlHelper::GlHelper(Image i) {
      inverted = false;
      redChannel = true;
      greenChannel = true;
      blueChannel = true;
      img = i;

      Init();
    }

    void GlHelper::SetOutputFilename(std::string filename) {
      outfile = filename;
    }

    void GlHelper::WriteImage() {
      int width, height, channels;
      channels = img.channels;
      width = glutGet(GLUT_WINDOW_WIDTH);
      height = glutGet(GLUT_WINDOW_HEIGHT);

      GLubyte *currentDisplay = (uint8_t *)malloc(width * height * channels);

      glReadPixels(0, 0, width, height, glFormat, GL_UNSIGNED_BYTE, currentDisplay);
      try {
        Image::Write(outfile, currentDisplay, width, height, channels);
      } catch(std::string message) {
        std::cerr << "Unable to write file " << outfile << ". " << message << std::endl;
      }
    }

    void GlHelper::Init() {
      int fakeArgc = 1;
      char *fakeArgv[1];
      fakeArgv[0] = strdup("imgview");
      glutInit(&fakeArgc, fakeArgv);
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);

      GlHelper::staticHelper = this;

      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
    }

    void GlHelper::LoadTexture() {
      glEnable(GL_TEXTURE_2D);

      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      /* Because I have tighly packed rows, the following is required for images that
       * have non-multiple-of-4 widths to display correctly. Thanks opengl
       */
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_PACK_ALIGNMENT, 1);

      /* Maybe I should check for BGR, but it turns out OpenImageIO actually converts that to RGB for me... so nope */
      glFormat = img.channels == 2 ? GL_RG : img.channels == 3 ? GL_RGB : img.channels == 4 ? GL_RGBA : GL_RED;
      glTexImage2D(GL_TEXTURE_2D, 0,
          glFormat, img.width, img.height, 0, glFormat,
          GL_UNSIGNED_BYTE, img.rawPixels);

      glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
      glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    void GlHelper::Run() {
      glutInitWindowSize(img.width, img.height);

      std::string winTitle;
      winTitle += img.filename + " - imgview";
      glutCreateWindow(winTitle.c_str());

      LoadTexture();

      glutDisplayFunc(StaticDrawLoop);
      glutKeyboardFunc(StaticKeypressHandler);
      glutReshapeFunc(StaticReshapeHandler);

      glutMainLoop();
    }

    void GlHelper::StaticReshapeHandler(int width, int height) {
      /* This is exactly what the default reshape handler calls, but since
       * the assignment requirements said we needed one of these functions....
       * Yeah, this function is a waste of 4 lines. Omit it and the gltuReshapeFunc call and
       * everything behaves *exactly* the same
       */
      glViewport(0,0,width,height);
    }

    void GlHelper::StaticDrawLoop() {
      /* Need this because OpenGL doesn't play nice with C++ */
      staticHelper->DrawLoop();
    }

    void GlHelper::DrawLoop() {
      int textureTop, textureBottom;
      unsigned int viewWidth, viewHeight;


      /* Do necessary stuff to center it and, if the window is too small, scale it, before drawing */
      int leftOffset, topOffset, scaledWidth, scaledHeight;
      viewWidth = glutGet(GLUT_WINDOW_WIDTH);
      viewHeight = glutGet(GLUT_WINDOW_HEIGHT);
      if(img.width <= viewWidth && img.height <= viewHeight) {
        scaledWidth = img.width;
        scaledHeight = img.height;
      } else {
        float aspectRatio = (float)img.width / (float)img.height;
        if(viewWidth * 1.0 / (float)viewHeight > aspectRatio) {
          scaledHeight =  viewHeight;
          scaledWidth = scaledHeight / (float)img.height  * img.width;
        } else {
          leftOffset = 0;
          scaledWidth = viewWidth;
          scaledHeight = scaledWidth / (float)img.width * img.height;
        }

      }

      leftOffset = (viewWidth - scaledWidth)/2;
      topOffset = (viewHeight - scaledHeight)/2;

      /* Flip the texture-top and texture-bottom to invert it, no need
       * to fiddle with pixels */
      if(inverted) {
        textureTop = 1, textureBottom = 0;
      } else {
        textureTop = 0, textureBottom = 1;
      }

      glLoadIdentity();
      gluOrtho2D(0.0, viewWidth, viewHeight, 0.0);

      /* Set all colors on so the background is properly colored. If you don't do this,
       * artifacts remain when resizing the window, among other things. In fact, with a
       * transparent window you'll have artifacts from the background follow you as you 
       * move it around. Looked kind of trippy, but obviously wrong */
      glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
      glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_2D, texture);

      /* Enable color masks if desired */
      glColorMask(redChannel ? GL_TRUE : GL_FALSE,
          greenChannel ? GL_TRUE : GL_FALSE,
          blueChannel ? GL_TRUE : GL_FALSE,
          GL_TRUE);

      glBegin(GL_QUADS);

      glTexCoord2i(0, textureTop);
      glVertex2i(leftOffset, topOffset);

      glTexCoord2i(0, textureBottom);
      glVertex2i(leftOffset, topOffset + scaledHeight);

      glTexCoord2i(1, textureBottom);
      glVertex2i(leftOffset + scaledWidth, topOffset + scaledHeight);

      glTexCoord2i(1, textureTop);
      glVertex2i(leftOffset + scaledWidth, topOffset);

      glEnd();
      glFlush();
      glutSwapBuffers();

    }

    void GlHelper::StaticKeypressHandler(unsigned char key, int x, int y) {
      GlHelper::staticHelper->KeypressHandler(key, x, y);
    }

    /**
     * The glut keyboard function
     * This function allows q to quit, i to invert, rgb to each apply masks, and o to 
     * remove all masks. It redisplays after any keyboard input which definitely changed the image.
     * To do this it stores a copy of the state before and checks if it differs after.
     * @param key Key that was pressed 
     * @param x Mouse x coord in the window
     * @param y Mouse y coord in the window
     */
    void GlHelper::KeypressHandler(unsigned char key, int x, int y) {
      switch(key) {
        case 'q':
        case 'Q':
        case ESCAPE_KEYCODE:
          exit(0);
        case 'i':
        case 'I':
          inverted = !inverted;
          break;
        case 'r':
        case 'R':
          redChannel = true;
          greenChannel = false;
          blueChannel= false;
          break;
        case 'g':
        case 'G':
          redChannel = false;
          greenChannel = true;
          blueChannel= false;
          break;
        case 'b':
        case 'B':
          redChannel = false;
          greenChannel = false;
          blueChannel= true;
          break;
        case 'o':
        case 'O':
          redChannel = true;
          greenChannel = true;
          blueChannel= true;
          break;
        case 'w':
        case 'W':
          WriteImage();
          break;
      }
      glutPostRedisplay();
    }
  }
}

