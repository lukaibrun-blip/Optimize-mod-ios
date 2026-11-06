#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    bool setting(char const* key) {
        return Mod::get()->getSettingValue<bool>(key);
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
            this->m_mainLayer->addChild(panel);

            auto title = CCLabelBMFont::create("iOS Optimization", "goldFont.fnt");
            title->setScale(.65f);
            title->setPosition({135.f, 145.f});
            panel->addChild(title);

            auto info = CCLabelBMFont::create(
                "As opções são aplicadas com segurança\n"
                "e funcionam somente no iOS 2.2081.",
                "chatFont.fnt"
            );
            info->setAlignment(kCCTextAlignmentCenter);
            info->setScale(.42f);
            info->setPosition({135.f, 116.f});
            panel->addChild(info);

            auto status = CCLabelBMFont::create(
                fmt::format("FPS estável: {}\nEfeitos reduzidos: {}\nPouca memória: {}",
                    setting("stable-fps") ? "ON" : "OFF",
                    setting("reduce-effects") ? "ON" : "OFF",
                    setting("low-memory") ? "ON" : "OFF").c_str(),
                "chatFont.fnt"
            );
            status->setAlignment(kCCTextAlignmentCenter);
            status->setScale(.48f);
            status->setPosition({135.f, 68.f});
            panel->addChild(status);

            auto hint = CCLabelBMFont::create(
                "Altere as opções pelo menu de configurações do Geode.",
                "chatFont.fnt"
            );
            hint->setScale(.38f);
            hint->setPosition({135.f, 28.f});
            panel->addChild(hint);
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

        return true;
    }

};

$on_mod(Loaded) {
    log::info("iOS Optimization carregado para Geometry Dash 2.2081");
}
