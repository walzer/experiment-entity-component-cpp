#ifndef __JNI_HELPER_H__
#define __JNI_HELPER_H__

/*
 * Function signature is (argtypes)rettype
 *
 *  signature               type
 *  Z                       boolean
 *  B                       byte
 *  C                       char
 *  S                       short
 *  I                       int
 *  J                       long
 *  F                       float
 *  D                       double
 *  [type                   array of type
 *  Lfully-qulitied-class;  any other class
 */

#include <jni.h>

struct JNIHelper
{
    // This parameter must be set in JNI_OnLoad.
    static JavaVM * s_vm;

    static JNIEnv * getJNIEnv();

    static int      registerNativeMethods(
            JNIEnv * env,
            const char * className,
            const JNINativeMethod * methods,
            int numMethods);

    static int      findClassMethods(
            JNIEnv * env,
            const char * className,
            const JNINativeMethod * methods,
            int numMethods);
};

#endif  //  __JNI_HELPER_H__
