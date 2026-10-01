#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <platform/android/jni/JniHelper.h>
#include <chrono>

using namespace geode::prelude;

class $modify(GLSurfaceViewDiagnostic, CCDirector) {
    bool init() {
        if (!CCDirector::init())
            return false;

        auto env = cocos2d::JniHelper::getEnv();
        auto activity = cocos2d::JniHelper::getActivity();

        if (!env || !activity) {
            log::info("JNI: activity/env not found");
            return true;
        }

        auto activityClass = env->GetObjectClass(activity);

        auto getGLSurfaceView = env->GetMethodID(
            activityClass,
            "getGLSurfaceView",
            "()Landroid/view/View;"
        );

        if (!getGLSurfaceView) {
            log::info("GLSurfaceView getter: NOT FOUND");
            return true;
        }

        auto view = env->CallObjectMethod(activity, getGLSurfaceView);

        if (!view) {
            log::info("GLSurfaceView: NOT FOUND");
            return true;
        }

        auto viewClass = env->GetObjectClass(view);

        auto setRenderMode = env->GetMethodID(
            viewClass,
            "setRenderMode",
            "(I)V"
        );

        auto getRenderMode = env->GetMethodID(
            viewClass,
            "getRenderMode",
            "()I"
        );

        if (!setRenderMode || !getRenderMode) {
            log::info("GLSurfaceView render-mode methods: NOT FOUND");
            return true;
        }

        // GLSurfaceView.RENDERMODE_CONTINUOUSLY = 1
        env->CallVoidMethod(view, setRenderMode, 1);

        jint mode = env->CallIntMethod(view, getRenderMode);

        log::info("GLSurfaceView found");
        log::info("GLSurfaceView render mode: {}", mode);

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
