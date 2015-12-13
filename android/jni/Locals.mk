# These are definitions for LOCAL_ variables for PPSSPP.
# They are shared between ppsspp_jni (lib for Android app) and ppsspp_headless.

LOCAL_CFLAGS := -DUSE_FFMPEG -DUSING_GLES2 -DMOBILE_DEVICE -O3 -fsigned-char -Wall -Wno-multichar -Wno-psabi -Wno-unused-variable -fno-strict-aliasing -D__STDC_CONSTANT_MACROS -Wno-format
# yes, it's really CPPFLAGS for C++
# literal-suffix is generated by Android default code and causes noise.
LOCAL_CPPFLAGS := -fno-exceptions -std=gnu++11 -fno-rtti -Wno-reorder -Wno-literal-suffix -Wno-format
LOCAL_C_INCLUDES := \
  $(LOCAL_PATH)/../../Common \
  $(LOCAL_PATH)/../.. \
  $(LOCAL_PATH)/$(NATIVE)/base \
  $(LOCAL_PATH)/$(NATIVE)/ext \
  $(LOCAL_PATH)/$(NATIVE)/ext/libzip \
  $(LOCAL_PATH)/$(NATIVE) \
  $(LOCAL_PATH)

LOCAL_STATIC_LIBRARIES := native libzip
LOCAL_LDLIBS := -lz -lGLESv2 -landroid -lOpenSLES -lEGL -ldl -llog

# ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
ifeq ($(findstring armeabi-v7a,$(TARGET_ARCH_ABI)),armeabi-v7a)
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv7/lib/libavformat.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv7/lib/libavcodec.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv7/lib/libswresample.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv7/lib/libswscale.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv7/lib/libavutil.a
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ffmpeg/android/armv7/include

  LOCAL_CFLAGS := $(LOCAL_CFLAGS) -D_ARCH_32 -DARM -DARMEABI_V7A
endif
ifeq ($(TARGET_ARCH_ABI),armeabi)
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv6/lib/libavformat.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv6/lib/libavcodec.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv6/lib/libswresample.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv6/lib/libswscale.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/armv6/lib/libavutil.a
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ffmpeg/android/armv6/include

  LOCAL_CFLAGS := $(LOCAL_CFLAGS) -D_ARCH_32 -DARM -DARMEABI -march=armv6
endif
ifeq ($(TARGET_ARCH_ABI),x86)
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/x86/lib/libavformat.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/x86/lib/libavcodec.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/x86/lib/libswresample.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/x86/lib/libswscale.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/x86/lib/libavutil.a
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ffmpeg/android/x86/include

  LOCAL_CFLAGS := $(LOCAL_CFLAGS) -D_ARCH_32 -D_M_IX86 -fomit-frame-pointer -mtune=atom -mfpmath=sse -mssse3 -mstackrealign
endif
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/arm64/lib/libavformat.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/arm64/lib/libavcodec.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/arm64/lib/libswresample.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/arm64/lib/libswscale.a
  LOCAL_LDLIBS += $(LOCAL_PATH)/../../ffmpeg/android/arm64/lib/libavutil.a
  LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ffmpeg/android/arm64/include

  LOCAL_CFLAGS := $(LOCAL_CFLAGS) -D_ARCH_64 -DARM64
endif

# Compile with profiling.
ifeq ($(ANDROID_NDK_PROFILER),1)
  LOCAL_CFLAGS += -pg -DANDROID_NDK_PROFILER
  LOCAL_STATIC_LIBRARIES += android-ndk-profiler
endif
