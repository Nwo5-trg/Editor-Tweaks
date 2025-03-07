#include "include.hpp"
#include "Geode/modify/SetupTimeWarpPopup.hpp"

class $modify(TimewarpPopup, SetupTimeWarpPopup) {
    bool init(EffectGameObject* p0, CCArray* p1) {
        if (!SetupTimeWarpPopup::init(p0, p1)) return false;
            auto button = CCMenuItemSpriteExtra::create(CircleButtonSprite::create(nullptr), this, menu_selector(TimewarpPopup::createEase));
            button->setPosition(ccp(100, 100));
        
            this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->addChild(button);
        return true;
    }

    void createEase(CCObject* sender) {
        auto editUI = EditorUI::get();
        if (auto objs = editUI->getSelectedObjects(); objs->count() == 1) {    
            auto firstObj = static_cast<EffectGameObject*>(objs->objectAtIndex(0));
            int units = 90;
            int amount = 10;
            float toEase = 0.15;
            float step = std::round(units / (amount - 1));
            float timeStep = std::round(((firstObj->m_timeWarpTimeMod - toEase) / (amount - 1)) * 100) / 100;

            for (int i = 0; i < amount - 1; i++) {
                editUI->onDuplicate(nullptr);
                auto obj = static_cast<EffectGameObject*>(editUI->getSelectedObjects()->objectAtIndex(0));
                editUI->moveObject(obj, ccp(step, 0));
                obj->m_timeWarpTimeMod = obj->m_timeWarpTimeMod - timeStep;
                if (i == amount - 2) obj->m_timeWarpTimeMod = toEase;
            }
        }
    }
};