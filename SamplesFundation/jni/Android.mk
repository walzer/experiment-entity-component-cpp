LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE        := SamplesFundation
FUNDATION_SRC_FILES := $(addprefix  ../fundation/, \
    Context.cpp \
    Runtime.cpp \
    SampleTools.cpp	\
    Surface.cpp  \
)
PVRTOOLS_SRC_FILES  := $(addprefix  ../PVRTools/,\
    OGLES2/PVRTBackground.cpp  \
    OGLES2/PVRTgles2Ext.cpp  \
    OGLES2/PVRTPFXParserAPI.cpp  \
    OGLES2/PVRTPFXSemantics.cpp  \
    OGLES2/PVRTPrint3DAPI.cpp  \
    OGLES2/PVRTShader.cpp  \
    OGLES2/PVRTTextureAPI.cpp  \
    PVRTBoneBatch.cpp  \
    PVRTDecompress.cpp  \
    PVRTError.cpp  \
    PVRTFixedPoint.cpp  \
    PVRTGeometry.cpp  \
    PVRTMatrixF.cpp  \
    PVRTMatrixX.cpp  \
    PVRTMisc.cpp  \
    PVRTModelPOD.cpp  \
    PVRTPFXParser.cpp  \
    PVRTPrint3D.cpp  \
    PVRTQuaternionF.cpp  \
    PVRTQuaternionX.cpp  \
    PVRTResourceFile.cpp  \
    PVRTShadowVol.cpp  \
    PVRTString.cpp  \
    PVRTStringHash.cpp  \
    PVRTTexture.cpp  \
    PVRTTrans.cpp  \
    PVRTTriStrip.cpp  \
    PVRTUnicode.cpp  \
    PVRTVector.cpp  \
    PVRTVertex.cpp  \
)
JNI_SRC_FILES := $(addprefix ./, \
    Debug_Android.cpp  \
    JNIHelper.cpp  \
    NativeWrapper.cpp  \
    Runtime_Android.cpp  \
    samples_fundation_NativeWrapper_GameGLES2Render.cpp  \
    samples_fundation_NativeWrapper_MotionEvent.cpp  \
    SampleTools_Android.cpp  \
    Surface_Android.cpp  \
)
LOCAL_SRC_FILES     := ${FUNDATION_SRC_FILES} ${PVRTOOLS_SRC_FILES} ${JNI_SRC_FILES}

LOCAL_C_INCLUDES    := $(addprefix ${LOCAL_PATH}/../, \
    include \
    include/pal/android \
    fundation \
    PVRTools \
    PVRTools/OGLES2 \
)

LOCAL_LDLIBS        := -lEGL -lGLESv2 -llog

include $(BUILD_STATIC_LIBRARY)
