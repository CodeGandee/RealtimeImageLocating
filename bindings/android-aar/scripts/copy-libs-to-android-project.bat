echo "ABI=%1%"

set ANDROID_JNILIBS_DIR=D:\code\ImageRegionSearch\bindings\android-aar\proj\armap\overmap\src\main\jniLibs
set ANDROID_ABI=%1%
set OPENCV_LIB=build-ocv-android-%ANDROID_ABI%\lib\%ANDROID_ABI%\*.so
set IMGREGLOC_LIB=build-jnilib-%ANDROID_ABI%\bin\*.so

xcopy /q /i /y %OPENCV_LIB% %ANDROID_JNILIBS_DIR%\%ANDROID_ABI%\
xcopy /q /i /y %IMGREGLOC_LIB% %ANDROID_JNILIBS_DIR%\%ANDROID_ABI%\