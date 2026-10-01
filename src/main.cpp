#include <Geode/Geode.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

class $modify(AnimationIntervalTest, CCDirector) {
    bool init() {
        if (!CCDirector::init())
            return false;

        // Request a 240 FPS animation interval.
        this->setAnimationInterval(1.0 / 240.0);

        log::info("Animation interval set to 1/240");

        return true;
    }
};
