#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    bool setting(char const* key) {
        return Mod::get()->getSettingValue<bool>(key);
    }

    void releaseUnusedResources() {
        if (!setting("low-memory")) {
            return;
        }

        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
    }

    void applyFramePacing() {
        if (!setting("stable-fps")) {
            return;
        }

        auto director = CCDirector::sharedDirector();
        director->setAnimationInterval(1.0 / 120.0);
    }

    class OptimizationPopup final : public FLAlertLayer {
    protected:
        bool init() {
            if (!FLAlertLayer::init(200, 160, "Optimization", "Fechar", nullptr)) {
                return false;
            }

            auto winSize = CCDirector::sharedDirector()->getWinSize();
            auto panel = CCScale9Sprite::create("GJ_square05.png");
            panel->setContentSize({270.f, 170.f});
            panel->setPosition(winSize / 2);
            m_mainLayer->addChild(panel);

            auto title = CCLabelBMFont::create("iOS Optimization", "goldFont.fnt");
            title->setScale(.65f);
            title->setPosition({135.f, 145.f});
            panel->addChild(title);

            auto info = CCLabelBMFont::create(
                "Otimizações ativas nesta sessão:\n"
                "frame pacing e limpeza de memória.",
                "chatFont.fnt"
            );
            info->setAlignment(kCCTextAlignmentCenter);
            info->setScale(.42f);
            info->setPosition({135.f, 108.f});
            panel->addChild(info);

            auto status = CCLabelBMFont::create(
                fmt::format("FPS estável: {}\nEfeitos: {}\nPouca memória: {}",
                    setting("stable-fps") ? "ON" : "OFF",
                    setting("reduce-effects") ? "REDUZIDOS" : "PADRÃO",
                    setting("low-memory") ? "ON" : "OFF").c_str(),
                "chatFont.fnt"
            );
            status->setAlignment(kCCTextAlignmentCenter);
            status->setScale(.48f);
            status->setPosition({135.f, 58.f});
            panel->addChild(status);
            return true;
        }

    public:
        static OptimizationPopup* create() {
            auto popup = new OptimizationPopup();
            if (popup->init()) {
                popup->autorelease();
                return popup;
            }
            delete popup;
            return nullptr;
        }
    };
}

class $modify(OptimizationPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto menu = this->getChildByID("left-button-menu");
        if (!menu) {
            menu = this->getChildByID("right-button-menu");
        }
        if (!menu) {
            return;
        }

        auto label = CCLabelBMFont::create("OPT", "goldFont.fnt");
        label->setScale(.42f);
        auto button = CCMenuItemSpriteExtra::create(
            label, this, menu_selector(OptimizationPauseLayer::onOptimization)
        );
        button->setID("optimization-button");
        menu->addChild(button);
        menu->updateLayout();
    }

    void onOptimization(CCObject*) {
        OptimizationPopup::create()->show();
    }
};

class $modify(OptimizationPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        auto result = PlayLayer::init(level, useReplay, dontCreateObjects);
        if (!result) {
            return false;
        }

        applyFramePacing();
        return true;
    }

    void onExit() {
        PlayLayer::onExit();
        releaseUnusedResources();
    }
};

$on_mod(Loaded) {
    log::info("iOS Optimization carregado para Geometry Dash 2.2081");
}
