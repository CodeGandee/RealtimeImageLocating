add_executable(testapp testapp.cpp)
target_include_directories(testapp PUBLIC ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(testapp PUBLIC ${OpenCV_LIBS} ${_IMGREGIONLOC_LIB_NAME})
set_property(TARGET testapp PROPERTY
    VS_DEBUGGER_ENVIRONMENT "PATH=$(PATH);${_OpenCV_LIB_PATH}"
)

add_executable(c_apptest c_testapp.cpp)
target_include_directories(c_apptest PUBLIC ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/bindings/c/include)
target_link_libraries(c_apptest PUBLIC ${OpenCV_LIBS} imgregionloc_c)
set_property(TARGET c_apptest PROPERTY
    VS_DEBUGGER_ENVIRONMENT "PATH=$(PATH);${_OpenCV_LIB_PATH}"
)

add_executable(c_api_test c_api_test.cpp)
target_include_directories(c_api_test PUBLIC ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/bindings/c/include)
target_link_libraries(c_api_test PUBLIC ${OpenCV_LIBS} imgregionloc_c)
set_property(TARGET c_api_test PROPERTY
    VS_DEBUGGER_ENVIRONMENT "PATH=$(PATH);${_OpenCV_LIB_PATH}"
)
