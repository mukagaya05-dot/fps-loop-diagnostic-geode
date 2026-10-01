#include <Geode/Geode.hpp>
#include <Geode/modify/CCDisplayLinkDirector.hpp>
#include <chrono>

using namespace geode::prelude;

class $modify(FPSLoopDiagnostic, CCDisplayLinkDirector) {
    void mainLoop() {
        static uint64_t frames = 0;
        static auto start = std::chrono::steady_clock::now();

        frames++;

        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - start
        ).count();

        if (elapsed >= 1000) {
            log::info("MAINLOOP CALLS: {}", frames);
            frames = 0;
            start = now;
        }

        CCDisplayLinkDirector::mainLoop();
    }
};
