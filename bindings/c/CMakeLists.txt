# # header guard
# if(TARGET imgregionloc_c)
#         return()
# endif()

set(srcfiles
        src/IMGRGLOC_Image2Map.cpp
        src/IMGRGLOC_Image2MapIndirect.cpp
        src/IMGRGLOC_Test.cpp
)

set(hfiles
        include/IMGRGLOC_master.h
        include/IMGRGLOC_Image2Map.h
        include/IMGRGLOC_Image2MapIndirect.h
        include/IMGRGLOC_Test.h
)

# opencvc
set(OPENCVC_DIR ${CMAKE_SOURCE_DIR}/extern/opencvc)
set(opencvc_src
        ${OPENCVC_DIR}/CVCTypes.cpp
        ${OPENCVC_DIR}/CVCTypes.h
        ${OPENCVC_DIR}/CVCBase.h
        ${OPENCVC_DIR}/CVCCore.cpp
        ${OPENCVC_DIR}/CVCCore.h
        ${OPENCVC_DIR}/CVCImgCodecs.cpp
        ${OPENCVC_DIR}/CVCImgCodecs.h
        ${OPENCVC_DIR}/CVCImgProc.cpp
        ${OPENCVC_DIR}/CVCImgProc.h
        ${OPENCVC_DIR}/CVCObjectDetect.cpp
        ${OPENCVC_DIR}/CVCObjectDetect.h
        ${OPENCVC_DIR}/CVCPhoto.cpp
        ${OPENCVC_DIR}/CVCPhoto.h
        ${OPENCVC_DIR}/CVCPrivate.h
        ${OPENCVC_DIR}/CVCStitching.cpp
        ${OPENCVC_DIR}/CVCStitching.h
        # ${OPENCVC_DIR}/OpenCVC.cpp
        # ${OPENCVC_DIR}/OpenCVC.h
)

add_library(imgregionloc_c ${srcfiles} ${hfiles} ${opencvc_src})
target_include_directories(imgregionloc_c PUBLIC include)
target_link_libraries(imgregionloc_c PUBLIC ${OpenCV_LIBS} ${_IMGREGIONLOC_LIB_NAME})
target_include_directories(imgregionloc_c PUBLIC ${OpenCV_INCLUDE_DIRS})

source_group(c_src FILES ${srcfiles})
source_group(c_include FILES ${hfiles})
source_group(opencvc FILES ${opencvc_src})
