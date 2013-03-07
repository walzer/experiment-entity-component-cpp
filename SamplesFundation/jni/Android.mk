LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE        := SamplesFundation
FUNDATION_SRC_FILES := $(addprefix  ../fundation/, \
    Runtime.cpp \
    Surface.cpp  \
)
JNI_SRC_FILES := $(addprefix ./, \
    Debug_Android.cpp \
    Runtime_Android.cpp \
    samples_fundation_NativeWrapper_GameGLES2Render.cpp  \
    Surface_Android.cpp \
)
LOCAL_SRC_FILES     := ${FUNDATION_SRC_FILES} ${JNI_SRC_FILES}

LOCAL_C_INCLUDES    := $(addprefix ${LOCAL_PATH}/../, \
    include \
    fundation \
)

LOCAL_LDLIBS        := -lEGL -lGLESv2 -llog

include $(BUILD_STATIC_LIBRARY)
