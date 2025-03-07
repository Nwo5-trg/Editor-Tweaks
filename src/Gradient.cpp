#include "include.hpp"
#include <Geode/modify/SetupGradientPopup.hpp>
#include <Geode/binding/GradientTriggerObject.hpp>
#include <Geode/binding/LevelEditorLayer.hpp>

int getNextFreeGroupIDFromAllObjects() { // cuz getnextfreegroupid needs an array i gotta do this shit
    auto editUI = EditorUI::get();
    auto levelEditor = LevelEditorLayer::get();
    auto pauseLayer = EditorPauseLayer::create(levelEditor);
    auto originalObjs = editUI->getSelectedObjects();
    pauseLayer->getChildByType<CCMenu>(2)->getChildByType<CCMenuItemSpriteExtra>(4)->activate();
    pauseLayer->removeMeAndCleanup();
    int id = levelEditor->getNextFreeGroupID(editUI->getSelectedObjects());
    editUI->deselectAll();
    editUI->selectObjects(originalObjs, true);
    editUI->updateObjectInfoLabel();
    editUI->updateButtons();
    editUI->updateDeleteButtons();
    return id;
}

class $modify(GradientPopup, SetupGradientPopup) {
    bool init(GradientTriggerObject* p0, CCArray* p1) {
        if (!SetupGradientPopup::init(p0, p1)) return false;
        if (mod->getSettingValue<bool>("gradient-create-points") && mod->getSettingValue<bool>("gradient-enabled")) {
            auto button = CCMenuItemSpriteExtra::create(CCSprite::create("createpoints.png"_spr), this, menu_selector(GradientPopup::onCreatePoints));
            button->setPosition(ccp(158.5, 109));
            this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->addChild(button);
        }
        return true;
    }

    void onCreatePoints(CCObject* sender) {
        auto vertexToggle = this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemToggler>(0); // prolly a better way to do this ngl
        if (!vertexToggle->isToggled()) vertexToggle->toggleWithCallback(true);
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemToggler>(0)->selected();
        auto editUI = EditorUI::get();
        auto levelEditor = LevelEditorLayer::get();
        auto objs = editUI->getSelectedObjects();
        CCArray pointObjs;
        std::vector<CCPoint> points {
            ccp(-15, 30), ccp(15, 30),
            ccp(-15, 60), ccp(15, 60),
        };
        for (int i = 0; i < objs->count(); i++) {
            auto obj = static_cast<GradientTriggerObject*>(objs->objectAtIndex(i));
            auto pos = obj->getPosition();
            for (int i = 0; i < 4; i++) {
                auto point = levelEditor->createObject(mod->getSettingValue<int64_t>("gradient-point-id"), pos + points[i], false);
                auto id = getNextFreeGroupIDFromAllObjects();
                levelEditor->addObjectToGroup(point, id);
                pointObjs.addObject(point);
                this->getChildByType<CCLayer>(0)->getChildByType<CCTextInputNode>(i)->setString(std::to_string(id));
            }
        }
        editUI->deselectAll();
        if (!mod->getSettingValue<bool>("gradient-deselect")) editUI->selectObjects(&pointObjs, true);
        if (mod->getSettingValue<bool>("gradient-close")) this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemSpriteExtra>(1)->activate();
    }
};