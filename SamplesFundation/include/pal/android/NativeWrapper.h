#ifndef NATIVEWRAPPER_H_
#define NATIVEWRAPPER_H_

#include "JNIHelper.h"

struct NativeWrapper
{
    static const char * kJavaName;

    static bool init();

    static jbyteArray readDataFormAsset(const char * path);

    struct GameGLES2Render
    {
        static const char * kJavaName;

        static bool init(JNIEnv* env);
        /*
         * Method:    onDrawFrame
         * Signature: ()V
         */
        static void onDrawFrame(JNIEnv *, jclass);

        /*
         * Method:    onSurfaceChanged
         * Signature: (II)V
         */
        static void onSurfaceChanged(JNIEnv *, jclass, jint, jint);

        /*
         * Method:    onSurfaceCreated
         * Signature: ()V
         */
        static void onSurfaceCreated(JNIEnv *, jclass);
    };

    struct MotionEvent
    {
        static const char * kJavaName;

        static bool init(JNIEnv* env);

        /*
         * Method:    onPointerAction
         * Signature: (III[I[I[F[F)Z
         */
        static jboolean onPointerAction(JNIEnv * env, jclass,
                jint action, jint source , jint count,
                jintArray idList, jintArray toolTypeList,
                jfloatArray xList, jfloatArray yList);
    };
};

#endif /* NATIVEWRAPPER_H_ */
