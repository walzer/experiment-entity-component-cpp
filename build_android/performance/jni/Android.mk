LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := performance
    
PERFORMANCE_SRC_FILES := $(addprefix ../../../performance/, \
    Performace.cpp \
    profile.cpp \
)

PROJECT_SRC_FILES := $(addprefix  ../../../performance/project_android/,\
    main.cpp \
    pch.cpp \
)

LOCAL_SRC_FILES := ${PERFORMANCE_SRC_FILES} ${PROJECT_SRC_FILES}

LOCAL_C_INCLUDES := $(addprefix ${LOCAL_PATH}/../../../performance/, \
    . \
    project_android \
)

LOCAL_LDLIBS        := -llog

include $(BUILD_SHARED_LIBRARY)
