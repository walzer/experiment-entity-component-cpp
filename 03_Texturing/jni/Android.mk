LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Texturing
JNI_SRC_FILES       := $(addprefix  ./,\
    MipmapContext.cpp \
    MultiTextureContext.cpp  \
    Texturing.cpp  \
)
LOCAL_SRC_FILES     := ${JNI_SRC_FILES}

LOCAL_WHOLE_STATIC_LIBRARIES := SamplesFundation

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, ${LOCAL_PATH}/../..)
$(call import-module, SamplesFundation)
