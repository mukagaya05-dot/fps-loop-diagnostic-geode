#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <jni.h>
#include <chrono>

using namespace geode::prelude;

class $modify(GLSurfaceViewDiagnostic, CCDirector) {
    bool init() {
        if (!CCDirector::init())
            return false;

        JavaVM* vm = nullptr;

        // Get the JVM through the Android runtime.
        if (JNI_GetCreatedJavaVMs(&vm, 1, nullptr) != JNI_OK || !vm) {
            log::info("JNI: JavaVM not found");
            return true;
        }

        JNIEnv* env = nullptr;
        bool attached = false;

        jint result = vm->GetEnv(
            reinterpret_cast<void**>(&env),
            JNI_VERSION_1_6
        );

        if (result == JNI_EDETACHED) {
            if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
                log::info("JNI: failed to attach thread");
                return true;
            }

            attached = true;
        }

        if (!env) {
            log::info("JNI: JNIEnv not found");
            return true;
        }

        // Find the current Activity.
        jclass activityThread = env->FindClass(
            "android/app/ActivityThread"
        );

        if (!activityThread) {
            log::info("JNI: ActivityThread not found");
            if (attached)
                vm->DetachCurrentThread();
            return true;
        }

        jmethodID currentActivityThread = env->GetStaticMethodID(
            activityThread,
            "currentActivityThread",
            "()Landroid/app/ActivityThread;"
        );

        jobject thread = env->CallStaticObjectMethod(
            activityThread,
            currentActivityThread
        );

        jmethodID getActivities = env->GetMethodID(
            activityThread,
            "getActivities",
            "()Ljava/util/Map;"
        );

        if (!thread || !getActivities) {
            log::info("JNI: couldn't access ActivityThread");
            if (attached)
                vm->DetachCurrentThread();
            return true;
        }

        log::info("JNI: Java environment available");

        if (attached)
            vm->DetachCurrentThread();

        return true;
    }

    void drawScene() {
        static uint64_t frames = 0;
        static auto start = std::chrono::steady_clock::now();

        frames++;

        auto now = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - start
            ).count();

        if (elapsed >= 1000) {
            log::info("RENDER CALLS: {}", frames);

            frames = 0;
            start = now;
        }

        CCDirector::drawScene();
    }
};
