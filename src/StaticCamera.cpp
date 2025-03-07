#include "include.hpp"
#include <Geode/modify/SetupStaticCameraPopup.hpp>
#include <Geode/binding/CameraTriggerGameObject.hpp>

class $modify(StaticPopup, SetupStaticCameraPopup) {
    bool init(CameraTriggerGameObject* p0, CCArray* p1) {
        if (!SetupStaticCameraPopup::init(p0, p1)) return false;
            if (mod->getSettingValue<bool>("static-camera-follow-fix") && mod->getSettingValue<bool>("static-camera-enabled")) {
                auto button = CCMenuItemSpriteExtra::create(CCSprite::create("followfix.png"_spr), this, menu_selector(StaticPopup::onFollowFix));
                button->setPosition(ccp(76.5, -4));
                this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->addChild(button);
            }
        return true;
    }
    void onFollowFix(CCObject* sender) {
        auto editUI = EditorUI::get();
        auto objs = editUI->getSelectedObjects();
        if (objs->count() == 2) {
            auto obj1 = static_cast<CameraTriggerGameObject*>(objs->objectAtIndex(0));
            auto obj2 = static_cast<CameraTriggerGameObject*>(objs->objectAtIndex(1));
            if ((obj1->m_exitStatic && obj2->m_followObject) || (obj2->m_exitStatic && obj1->m_followObject)) {
                auto exitTrigger = obj1->m_exitStatic ? obj1 : obj2;
                auto followTrigger = obj1->m_exitStatic ? obj2 : obj1;

                editUI->selectObject(followTrigger, true);
                editUI->onDuplicate(nullptr);
                auto followFixTrigger = static_cast<CameraTriggerGameObject*>(editUI->getSelectedObjects()->objectAtIndex(0));
                followFixTrigger->setPosition(ccp(exitTrigger->getPosition().x, exitTrigger->getPosition().y + 30));
                followFixTrigger->m_ordValue = 1;
                followFixTrigger->m_followObject = false;
                exitTrigger->m_ordValue = 2;
                editUI->deselectAll();
                if (!mod->getSettingValue<bool>("static-camera-deselect")) editUI->selectObjects(objs, true);
                if (mod->getSettingValue<bool>("static-camera-close")) this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemSpriteExtra>(1)->activate();
            } else FLAlertLayer::create("Trigger Utils", "One trigger has to have exit static on and the other follow object on.", "OK")->show();
        } else FLAlertLayer::create("Trigger Utils", "You need to have 2 objects selected.", "OK")->show();
    }
};