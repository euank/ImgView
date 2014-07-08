find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenImageIO REQUIRED)

include_directories(${OPENIMAGEIO_INCLUDE_DIR})

add_executable(imgview imgview.cpp Image.cpp Image.hpp GlHelper.hpp GlHelper.cpp)

target_link_libraries(imgview ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${OPENIMAGEIO_LIBRARY})

install(TARGETS imgview
        RUNTIME DESTINATION bin)
