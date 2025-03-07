#include "include.hpp"
#include <Geode/modify/CustomizeObjectLayer.hpp>
#include <Geode/modify/SetupCameraModePopup.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

class $modify (SetupCameraMode, SetupCameraModePopup) {
    struct Fields {
        EffectGameObject* obj;
    };
    bool init(EffectGameObject* p0, CCArray* p1) {
        if(!SetupCameraModePopup::init(p0, p1)) return false;
        if (mod->getSettingValue<bool>("object-options-auto-freemode") && mod->getSettingValue<bool>("object-options-enabled")) {
            auto buttonMenu = this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0);
            auto toggler = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(SetupCameraMode::autoFreeModeToggle));
            toggler->toggle(mod->getSavedValue<bool>(std::to_string(p0->m_objectID) + "freemode"));
            toggler->setPosition(ccp(-175, 2));
            auto label = CCLabelBMFont::create("Auto Freemode", "bigFont.fnt");
            label->setPosition(ccp(-105, 2));
            label->setScale(0.375);
            buttonMenu->addChild(label);
            buttonMenu->addChild(toggler);
            m_fields->obj = p0;
        }
        return true;
    }

    void autoFreeModeToggle(CCObject* sender) {
        bool toggled = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
        mod->setSavedValue<bool>(std::to_string(m_fields->obj->m_objectID) + "freemode", toggled);
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemToggler>(0)->toggleWithCallback(toggled);
    }
};

class $modify(CustomObjectLayer, CustomizeObjectLayer) {
    bool init(GameObject* p0, CCArray* p1) {
        if (!CustomizeObjectLayer::init(p0, p1)) return false;
        if (auto input = this->m_textInput) if (mod->getSettingValue<bool>("object-options-text-input-length") && mod->getSettingValue<bool>("object-options-enabled")) input->setMaxLabelLength(999999);
        return true;
    }
};

class $modify(EditUI, EditorUI) {
	bool init(LevelEditorLayer* lel) {	
		if (!EditorUI::init(lel)) return false;
        if (mod->getSettingValue<bool>("object-options-detail-mode")) {
            auto menu = CCMenu::create();
            menu->setPosition(ccp(0, 0));
            this->m_editObjectBtn->getParent()->addChild(menu);
            auto button = CCMenuItemSpriteExtra::create(CCSprite::create(), this, nullptr);
            button->setID("open-detail-edit-object-button");
            button->setContentSize(CCSize(18, 28));
            button->setPosition(ccp(197, 102));
            menu->addChild(button);
        }
        return true;
    }

    void openDetailMenu() {
        this->m_editObjectBtn->activate();
        if (auto customLayer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<CustomizeObjectLayer>(0)) {
            if (auto button = customLayer->m_baseButton->getParent()->getChildByType<CCMenuItemSpriteExtra>(2)) {
                button->activate();
                if (auto settingsPopup = CCDirector::sharedDirector()->getRunningScene()->getChildByType<CustomizeObjectSettingsPopup>(0)) {
                    auto toggler = settingsPopup->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemToggler>(2);
                    if (!(toggler->isToggled())) toggler->activate();
                    settingsPopup->onClose(nullptr);
                }
            }
        }
    }
};

class $modify(GDButton, CCMenuItemSpriteExtra) {
    void activate() {
        if (this->getID() == "open-detail-edit-object-button") {
            static_cast<EditUI*>(EditorUI::get())->openDetailMenu();
            return;
        }
        CCMenuItemSpriteExtra::activate();
    }

    void selected() {
        if (this->getID() == "open-detail-edit-object-button") {
            EditorUI::get()->m_editObjectBtn->selected();
            return;
        }
        CCMenuItemSpriteExtra::selected();
    }

    void unselected() {
        if (this->getID() == "open-detail-edit-object-button") {
            EditorUI::get()->m_editObjectBtn->unselected();
            return;
        }
        CCMenuItemSpriteExtra::unselected();
    }
};