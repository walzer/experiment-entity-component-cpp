LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := foundation

FOUNDATION_SRC_FILES := $(addprefix ../../../foundation/src/, \
    Event.cpp \
    Event2.cpp \
    profile.cpp \
)

PROJECT_SRC_FILES := $(addprefix  ./,\
    main.cpp \
)

LOCAL_SRC_FILES := ${FOUNDATION_SRC_FILES} ${PROJECT_SRC_FILES}

LOCAL_C_INCLUDES := $(addprefix ${LOCAL_PATH}/../../../foundation/, \
    include \
    src \
)

LOCAL_LDLIBS        := -llog

include $(BUILD_SHARED_LIBRARY)
