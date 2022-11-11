set(srcfiles
        SIFTFeatureExtractor.cpp
        ORBFeatureExtractor.cpp
        ImageWithFeature.cpp
        FeaturePointList.cpp
        SIFTRegionMatcher.cpp
        ORBRegionMatcher.cpp
        RegionTransformer.cpp
        pipeline/Image2MapBase.cpp
        pipeline/Image2MapDirect.cpp
        pipeline/Image2MapIndirect.cpp
        testlib.cpp
)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(hfiles
        ${INCLUDE_DIR}/SIFTFeatureExtractor.h
        ${INCLUDE_DIR}/ORBFeatureExtractor.h
        ${INCLUDE_DIR}/ImageWithFeature.h
        ${INCLUDE_DIR}/FeaturePointList.h
        ${INCLUDE_DIR}/SIFTRegionMatcher.h
        ${INCLUDE_DIR}/ORBRegionMatcher.h
        ${INCLUDE_DIR}/RegionTransformer.h
        ${INCLUDE_DIR}/testlib.h
        ${INCLUDE_DIR}/imgregionloc_master.h
        ${INCLUDE_DIR}/pipeline/Image2MapBase.h
        ${INCLUDE_DIR}/pipeline/Image2MapDirect.h
        ${INCLUDE_DIR}/pipeline/Image2MapIndirect.h
)


add_library(imgregionloc ${srcfiles} ${hfiles})
target_link_libraries(imgregionloc PUBLIC ${OpenCV_LIBS})
target_include_directories(imgregionloc PUBLIC ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

set(_IMGREGIONLOC_LIB_NAME imgregionloc PARENT_SCOPE)

source_group("src" FILES ${srcfiles})
source_group("include" FILES ${hfiles})