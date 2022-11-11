call build-jnilib x86_64
call copy-libs-to-android-project x86_64

call build-jnilib arm64-v8a
call copy-libs-to-android-project arm64-v8a