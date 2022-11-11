echo "ABI=%1%"

set NDK_TOOLCHAIN=d:/Android/sdk/ndk/21.4.7075529/build/cmake/android.toolchain.cmake
set ANDROID_SDK_ROOT=D:/Android/sdk
set ANDROID_PLATFORM=22
set ANDROID_ABI=%1%
set CURRENT_DIR="%cd%"
set _OpenCV_DIR=%CURRENT_DIR%/build-ocv-android-%ANDROID_ABI%/install/sdk/native/jni
set BOOST_ROOT=D:/libs/source/boost_1_77_0
@REM set _Boost_DIR=D:/libs/install/boost/lib/cmake/Boost-1.77.0

set SOURCE_ROOT_DIR=../../../
set BUILD_DIR=build-jnilib-%ANDROID_ABI%
set INSTALL_DIR=%BUILD_DIR%/install

if exist %BUILD_DIR% rmdir %BUILD_DIR% /s /q
mkdir %BUILD_DIR%

cmake -S %SOURCE_ROOT_DIR% -B %BUILD_DIR% -G Ninja^
    -DOpenCV_DIR=%_OpenCV_DIR%^
    -DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%^
    -DCMAKE_TOOLCHAIN_FILE=%NDK_TOOLCHAIN%^
    -DANDROID_PLATFORM=%ANDROID_PLATFORM%^
    -DANDROID_ABI=%ANDROID_ABI%^
    -DBUILD_APPS=OFF ^
    -DBUILD_C_INTERFACE=ON

cmake --build %BUILD_DIR%