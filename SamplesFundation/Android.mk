LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE            := SamplesFundation
LOCAL_SRC_FILES         := obj/local/$(TARGET_ARCH_ABI)/libSamplesFundation.a
LOCAL_EXPORT_C_INCLUDES := ${LOCAL_PATH}/include
LOCAL_EXPORT_LDLIBS     := -lEGL -lGLESv2 -llog

include $(PREBUILT_STATIC_LIBRARY)