#include "samples_fundation_NativeWrapper_MotionEvent.h"

#include "Debug.h"
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

JNIEXPORT jboolean JNICALL Java_samples_fundation_NativeWrapper_00024MotionEvent_onPointerAction(
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
    offset += sprintf(msg + offset, "onPointerAction");
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
