LOCAL_PATH := $(call my-dir)

#///////////////////////////////////////////////////////////////////////////////

include $(CLEAR_VARS)

LOCAL_MODULE := gtest
LOCAL_SRC_FILES := obj/local/$(TARGET_ARCH_ABI)/libgtest.a
LOCAL_EXPORT_C_INCLUDES := $(addprefix ${LOCAL_PATH}/, \
    include \
)
#LOCAL_EXPORT_LDLIBS := -lGLESv2 -llog

include $(PREBUILT_STATIC_LIBRARY)

#///////////////////////////////////////////////////////////////////////////////

include $(CLEAR_VARS)

LOCAL_MODULE := gtest_main
LOCAL_SRC_FILES := obj/local/$(TARGET_ARCH_ABI)/libgtest_main.a
#LOCAL_EXPORT_LDLIBS := -lGLESv2 -llog

include $(PREBUILT_STATIC_LIBRARY)

#///////////////////////////////////////////////////////////////////////////////

include $(CLEAR_VARS)

LOCAL_MODULE := gtest_samples
LOCAL_SRC_FILES := obj/local/$(TARGET_ARCH_ABI)/libgtest_samples.a
#LOCAL_EXPORT_LDLIBS := -lGLESv2 -llog

include $(PREBUILT_STATIC_LIBRARY)
