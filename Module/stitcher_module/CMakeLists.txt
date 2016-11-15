# Set name of the module
SET (MODULE stitcher_module)

# Set all source files module uses
SET (SRC stitcher_module.h stitcher_module.cpp stitching.hpp stitcher.cpp )


add_library(${MODULE} MODULE ${SRC})
target_link_libraries(${MODULE} ${LIBNAME} ${OpenCV_LIBRARIES} ${OpenCV_LDFLAGS})

YURI_INSTALL_MODULE(${MODULE})
