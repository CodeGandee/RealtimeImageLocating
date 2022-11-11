echo "building android static lib ABI=%1%"

set OPENCV_ROOT=D:/libs/source/opencv-4.6.0
set NDK_TOOLCHAIN=d:/Android/sdk/ndk/21.4.7075529/build/cmake/android.toolchain.cmake
set ANDROID_SDK_ROOT=D:/Android/sdk

set ANDROID_PLATFORM=22
set ANDROID_ABI=%1%
set BUILD_DIR=build-ocv-android-%ANDROID_ABI%-static
set INSTALL_DIR=%BUILD_DIR%/install

if exist %BUILD_DIR% rmdir %BUILD_DIR% /q /s
mkdir %BUILD_DIR%

cmake -S %OPENCV_ROOT% -B %BUILD_DIR% -G Ninja^
    -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%^
    -DCMAKE_TOOLCHAIN_FILE=%NDK_TOOLCHAIN%^
    -DANDROID_PLATFORM=%ANDROID_PLATFORM%^
    -DANDROID_ABI=%ANDROID_ABI%^
    -DBUILD_FAT_JAVA_LIB=OFF^
    -DBUILD_JAVA=OFF^
    -DBUILD_opencv_java=OFF^
    -DBUILD_opencv_highgui=OFF^
    -DBUILD_TESTS=OFF^
    -DBUILD_PERF_TESTS=OFF^
    -DBUILD_ANDROID_EXAMPLES=OFF^
    -DBUILD_ANDROID_PROJECTS=OFF^
    -DBUILD_opencv_world=ON

cmake --build %BUILD_DIR%
cmake --install %BUILD_DIR%