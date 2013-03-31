#include "NativeWrapper.h"

#include "Debug.h"

const char * NativeWrapper::kJavaName = "samples/fundation/NativeWrapper";
static struct
{
    jmethodID readDataFromAsset;
} s_NativeWrapperMethods;

bool NativeWrapper::init()
{
    static bool s_inited = false;
    do
    {
        if (s_inited) break;
        // Find java methods will called later.
        JNIEnv * env = JNIHelper::getJNIEnv();
        if (! env)
        {
            Debug::loge("%d %s", __LINE__, __FUNCTION__);
            break;
        }
        static JNINativeMethod javaMethods[] =
        {
            {
                "readDataFromAsset",
                "(Ljava/lang/String;)[B",
                &s_NativeWrapperMethods.readDataFromAsset
            },
        };
        static int numMethods = sizeof(javaMethods) / sizeof(JNINativeMethod);
        if (false == JNIHelper::findClassMethods(
                env,
                kJavaName,
                javaMethods,
                numMethods))
        {
            Debug::loge("%d %s", __LINE__, __FUNCTION__);
            break;
        }
        if (false == NativeWrapper::GameGLES2Render::init(env))
        {
            Debug::loge("%d %s", __LINE__, __FUNCTION__);
            break;
        }
        if (false == NativeWrapper::MotionEvent::init(env))
        {
            Debug::loge("%d %s", __LINE__, __FUNCTION__);
            break;
        }
        s_inited = true;
    } while (false);

    return s_inited;
}

jbyteArray NativeWrapper::readDataFormAsset(const char * path)
{
    jbyteArray ret = 0;
    JNIEnv * env = JNIHelper::getJNIEnv();
    do
    {
        if (!path || !env) break;
        jstring jPath = env->NewStringUTF(path);
        if (!jPath) break;
        ret = (jbyteArray)env->CallStaticObjectMethod(
                env->FindClass(kJavaName),
                s_NativeWrapperMethods.readDataFromAsset,
                jPath);
        env->DeleteLocalRef(jPath);
    } while (false);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
// Implement GameGLES2Render
#include "Context.h"
#include "Debug.h"
#include "Surface_Android.h"
#include "Runtime_Android.h"

const char * NativeWrapper::GameGLES2Render::kJavaName =
        "samples/fundation/NativeWrapper$GameGLES2Render";

bool NativeWrapper::GameGLES2Render::init(JNIEnv * env)
{
    JNINativeMethod nativeMethods[] =
    {
        {
            "onDrawFrame",
            "()V",
            (void*)&NativeWrapper::GameGLES2Render::onDrawFrame
        },
        {
            "onSurfaceChanged",
            "(II)V",
            (void*)&NativeWrapper::GameGLES2Render::onSurfaceChanged
        },
        {
            "onSurfaceCreated",
            "()V",
            (void*)&NativeWrapper::GameGLES2Render::onSurfaceCreated
        },
    };
    int numMethods = sizeof(nativeMethods) / sizeof(JNINativeMethod);
    return JNIHelper::registerNativeMethods(
            env,
            kJavaName,
            nativeMethods,
            numMethods);
}

void NativeWrapper::GameGLES2Render::onDrawFrame(JNIEnv *, jclass)
{
    Surface * surface = Runtime::getSurface(0);
    surface->onEvents();
    surface->getContext()->run();
}

void NativeWrapper::GameGLES2Render::onSurfaceChanged(JNIEnv *, jclass, jint w, jint h)
{
    dynamic_cast<Surface_Android*>(Runtime::getSurface(0))->setSize(w, h);
}

void NativeWrapper::GameGLES2Render::onSurfaceCreated(JNIEnv *, jclass)
{
    Runtime_Android::createSurface();
}

////////////////////////////////////////////////////////////////////////////////
// Implement Motion Event
#include "Runtime_Android.h"
#include "Surface_Android.h"

static ActionType s_actionTable[256] =
{
    ACTION_DOWN,    //  0:  MotionEvent.ACTION_DOWN
    ACTION_UP,      //  1:  MotionEvent.ACTION_UP
    ACTION_MOVE,    //  2:  MotionEvent.ACTION_MOVE
    ACTION_CANCEL,  //  3:  MotionEvent.ACTION_CANCEL
    ACTION_UNKNOWN, //  4:
    ACTION_DOWN,    //  5:  MotionEvent.ACTION_POINTER_DOWN
    ACTION_UP,      //  6:  MotionEvent.ACTION_POINTER_UP
    ACTION_UNKNOWN  //  ...
};

const char * NativeWrapper::MotionEvent::kJavaName =
        "samples/fundation/NativeWrapper$MotionEvent";

bool NativeWrapper::MotionEvent::init(JNIEnv * env)
{
    JNINativeMethod nativeMethods[] =
    {
        {
            "onPointerAction",
            "(III[I[I[F[F)Z",
            (void*)&NativeWrapper::MotionEvent::onPointerAction
        },
    };
    int numMethods = sizeof(nativeMethods) / sizeof(JNINativeMethod);
    return JNIHelper::registerNativeMethods(
            env,
            kJavaName,
            nativeMethods,
            numMethods);
}

jboolean NativeWrapper::MotionEvent::onPointerAction(
    JNIEnv * env, jclass,
    jint action, jint source , jint count,
    jintArray idList, jintArray toolTypeList, jfloatArray xList, jfloatArray yList)
{
    jint    ids[count];
    jint    ts[count];
    jfloat  xs[count];
    jfloat  ys[count];
    env->GetIntArrayRegion(idList, 0, count, ids);
    env->GetIntArrayRegion(toolTypeList, 0, count, ts);
    env->GetFloatArrayRegion(xList, 0, count, xs);
    env->GetFloatArrayRegion(yList, 0, count, ys);
#if 0
    char msg[1024];
    int offset = 0;
    offset += sprintf(msg + offset, __FUNCTION__);
    offset += sprintf(msg + offset, "( action: %d, source: %d { ", action, source);

    for (int i = 0; i < count; ++i)
    {
        offset += sprintf(msg + offset, "%d, %d, %f, %f, ",
                ids[i], ts[i], xs[i], ys[i]);
    }
    offset += sprintf(msg + offset, " } )");
    Debug::logd(msg);
#endif
    jboolean ret = JNI_FALSE;
    PointerData data;

    do
    {
        data.action = s_actionTable[action];
        if (ACTION_UNKNOWN == data.action)
        {
            Debug::logd("Unknown action from motion action: %d", action);
            break;
        }
        data.count = count;
        for (int i = 0; i < count; ++i)
        {
            data.args[i].id = ids[i];
            data.args[i].x  = xs[i];
            data.args[i].y  = ys[i];
        }

        bool writed = static_cast<Surface_Android*>(Runtime::getSurface(0))->
                pointerQueue.write(data);

        if (writed == false)
        {
            Debug::logd("pointerQueue.write failed!");
        }
        ret = JNI_TRUE;
    } while (false);

    return ret;
}
