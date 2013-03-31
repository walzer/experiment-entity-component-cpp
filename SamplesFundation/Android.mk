LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE            := SamplesFundation
LOCAL_SRC_FILES         := obj/local/$(TARGET_ARCH_ABI)/libSamplesFundation.a
LOCAL_EXPORT_C_INCLUDES := $(addprefix ${LOCAL_PATH}/, \
    include \
    PVRTools/OGLES2 \
)
LOCAL_EXPORT_LDLIBS     := -lGLESv2 -llog

include $(PREBUILT_STATIC_LIBRARY)