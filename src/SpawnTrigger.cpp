#include "include.hpp"
#include "Geode/modify/SetupSpawnPopup.hpp"

class $modify(SpawnPopup, SetupSpawnPopup) { // could be refactored but im lazy
    bool init(EffectGameObject* p0, CCArray* p1) {
        if (!SetupSpawnPopup::init(p0, p1)) return false;
        if (mod->getSettingValue<bool>("spawn-enabled") && mod->getSettingValue<bool>("spawn-auto-spawn")) {
            auto layer = this->getChildByType<CCLayer>(0);
            auto buttonMenu = layer->getChildByType<CCMenu>(0);
            buttonMenu->getChildByType<CCMenuItemToggler>(5)->setPosition(ccp(-170, 56));
            layer->getChildByType<CCLabelBMFont>(11)->setPosition(ccp(135.25, 100));

            auto autoSpawn = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(SpawnPopup::autoSpawn));
            autoSpawn->toggle(mod->getSavedValue<bool>("autospawn"));
            autoSpawn->setPosition(ccp(170, -4));
            autoSpawn->setScale(0.675);
            buttonMenu->addChild(autoSpawn);
            auto autoMulti = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(SpawnPopup::autoMulti));
            autoMulti->toggle(mod->getSavedValue<bool>("automulti"));
            autoMulti->setPosition(ccp(170, 26));
            autoMulti->setScale(0.675);
            buttonMenu->addChild(autoMulti);
            auto labelSpawn = CCLabelBMFont::create("Auto Spawn", "bigFont.fnt");
            labelSpawn->setPosition(ccp(115, -4));
            labelSpawn->setScale(0.375);
            buttonMenu->addChild(labelSpawn);
            auto labelMulti = CCLabelBMFont::create("Auto Multi", "bigFont.fnt");
            labelMulti->setPosition(ccp(117, 26));
            labelMulti->setScale(0.375);
            buttonMenu->addChild(labelMulti); 
        }
        return true;
    }

    void autoSpawn(CCObject* sender) {
        bool toggled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
        mod->setSavedValue<bool>("autospawn", toggled);
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemToggler>(4)->toggleWithCallback(toggled);
    }

    void autoMulti(CCObject* sender) {
        bool toggled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
        mod->setSavedValue<bool>("automulti", toggled);
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemToggler>(5)->toggleWithCallback(toggled);
    }
};