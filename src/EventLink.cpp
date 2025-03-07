#include "include.hpp"
#include <Geode/modify/SetupEventLinkPopup.hpp>
#include <Geode/modify/SelectEventLayer.hpp>

bool updateSideBar = true; //dumbest solution but it works so idc

class $modify(EventPopup, SetupEventLinkPopup) {
    bool init(EventLinkTrigger* p0, CCArray* p1) {
        if (!SetupEventLinkPopup::init(p0, p1)) return false;
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector( // workaround cuz not waiting a frame makes the menu not open
        schedule_selector(EventPopup::createSidebar), this, 0, false, 0, false);
        
        auto layer = this->getChildByType<CCLayer>(0);
        auto buttonMenu = layer->getChildByType<CCMenu>(0);
        if (mod->getSettingValue<bool>("event-enabled") && mod->getSettingValue<bool>("event-edit-text")) {
            layer->getChildByType<CCLabelBMFont>(5)->setString("Material:");
            layer->getChildByType<CCLabelBMFont>(6)->setString("Player:");
        }
        if (mod->getSettingValue<bool>("event-enabled") && mod->getSettingValue<bool>("event-sidebar")) {
            layer->getChildByType<CCLabelBMFont>(0)->setPosition(ccp(210, 286));
            auto selectEventLayerButton = buttonMenu->getChildByType<CCMenuItemSpriteExtra>(8);
            selectEventLayerButton->setPosition(ccp(-172, 208));
            selectEventLayerButton->setScale(0.8);
            selectEventLayerButton->m_baseScale = 0.8;
            buttonMenu->getChildByType<CCMenuItemToggler>(2)->setPosition(ccp(-85, 26));
            layer->getChildByType<CCLabelBMFont>(3)->setPosition(ccp(205.25, 70));
            for (int i = 0; i < 3; i++) {
                layer->getChildByType<CCLabelBMFont>(i + 1)->setPosition(ccp(130.25, layer->getChildByType<CCLabelBMFont>(i + 1)->getPosition().y));
            }
            std::vector<CCPoint> points {ccp(210, 242), ccp(210, 188), ccp(210, 134)};
            for (int i = 0; i < 3; i++) {
                layer->getChildByType<CCLabelBMFont>(i + 4)->setPosition(points[i]);
                layer->getChildByType<CCScale9Sprite>(i + 1)->setPosition(ccp(points[i].x, points[i].y - 20));
                layer->getChildByType<CCTextInputNode>(i)->setPosition(ccp(points[i].x, points[i].y - 20));
                buttonMenu->getChildByType<CCMenuItemSpriteExtra>(2 + (i * 2))->setPosition(ccp(points[i].x - 333, points[i].y - 64));
                buttonMenu->getChildByType<CCMenuItemSpriteExtra>(3 + (i * 2))->setPosition(ccp(points[i].x - 237, points[i].y - 64));
            }
        }
        return true;
    }

    void createSidebar() {
        auto layer = this->getChildByType<CCLayer>(0);
        auto buttonMenu = layer->getChildByType<CCMenu>(0);
        if (mod->getSettingValue<bool>("event-show-sidebar")) {
            auto sideBarSprite = IconButtonSprite::create("GJ_button_05.png", nullptr, nullptr, nullptr); // probably another object that works better for this but idk what it is
            sideBarSprite->setPosition(ccp(400, 160));
            sideBarSprite->getChildByType<CCScale9Sprite>(0)->setContentSize(CCSize(150, 280));
            layer->addChild(sideBarSprite);
        }
        
        std::vector<std::pair<CCPoint, std::string>> togglerParameters {
            {ccp(72, 224.5), "event-sidebar-one"}, {ccp(72, 193.64), "event-sidebar-two"},
            {ccp(72, 162.78), "event-sidebar-three"}, {ccp(72, 131.92), "event-sidebar-four"},
            {ccp(72, 101.06), "event-sidebar-five"}, {ccp(72, 70.2), "event-sidebar-six"},
            {ccp(72, 39.34), "event-sidebar-seven"}, {ccp(72, 8.5), "event-sidebar-eight"}
        };

        for (auto& [pos, setting] : togglerParameters) {
            auto toggler = CCMenuItemToggler::create(CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png"), this, menu_selector(EventPopup::onCustomEvent));
            toggler->setPosition(pos);
            toggler->setTag(std::floor(1000 + pos.y));
            toggler->setScale(0.7);
            auto valueNode = CCNode::create();
            valueNode->setID(std::to_string(mod->getSettingValue<int64_t>(setting)));
            toggler->addChild(valueNode);
            auto label = CCLabelBMFont::create(getEventLabel(mod->getSettingValue<int64_t>(setting))->getString(), "bigFont.fnt");
            label->setScale(80 / label->getContentSize().width);
            label->setPosition(ccp(pos.x + ((label->getContentSize().width / 2) * (1 * label->getScale()) + 20), pos.y));
            buttonMenu->addChild(label);
            buttonMenu->addChild(toggler);
            if (getEvent(mod->getSettingValue<int64_t>(setting), false)->isToggled()) toggler->toggle(true);
        }
    }

    void onCustomEvent(CCObject* sender) {
        getEvent(stoi(static_cast<CCMenuItemToggler*>(sender)->getChildByType<CCNode>(2)->getID()), true);
    }

    void onOpenEventSelectLayer(CCObject* sender) {
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemSpriteExtra>(8)->activate();
    }

    CCMenuItemToggler* getEvent(int id, bool toggle) {
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemSpriteExtra>(8)->activate();
        auto menu = CCDirector::sharedDirector()->getRunningScene()->getChildByType<SelectEventLayer>(0)->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0);
        auto toggler = static_cast<CCMenuItemToggler*>(menu->getChildByTag(id));
        updateSideBar = false;
        if (toggle) toggler->activate();
        updateSideBar = true;
        menu->getChildByType<CCMenuItemSpriteExtra>(0)->activate();
        return toggler;
    }

    CCLabelBMFont* getEventLabel(int id) {
        this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemSpriteExtra>(8)->activate();
        auto layer = CCDirector::sharedDirector()->getRunningScene()->getChildByType<SelectEventLayer>(0)->getChildByType<CCLayer>(0);
        auto label = static_cast<CCLabelBMFont*>(layer->getChildByType<CCNode>(1 + (id * 2)));
        layer->getChildByType<CCMenu>(0)->getChildByType<CCMenuItemSpriteExtra>(0)->activate();
        return label;
    }
};

class $modify(SelectEvent, SelectEventLayer) {
    void onCustomToggleTriggerValue(CCObject* sender) {
        SelectEventLayer::onCustomToggleTriggerValue(sender);
        if (mod->getSettingValue<bool>("event-enabled") && mod->getSettingValue<bool>("event-sidebar")) {
            if (auto toggler = dynamic_cast<CCMenuItemToggler*>(sender)) {
                if (!updateSideBar) return;
                std::vector<std::pair<int, int>> values {
                    {mod->getSettingValue<int64_t>("event-sidebar-one"), 1224}, {mod->getSettingValue<int64_t>("event-sidebar-two"), 1193}, 
                    {mod->getSettingValue<int64_t>("event-sidebar-three"), 1162}, {mod->getSettingValue<int64_t>("event-sidebar-four"), 1131}, 
                    {mod->getSettingValue<int64_t>("event-sidebar-five"), 1101}, {mod->getSettingValue<int64_t>("event-sidebar-six"), 1070}, 
                    {mod->getSettingValue<int64_t>("event-sidebar-seven"), 1039}, {mod->getSettingValue<int64_t>("event-sidebar-eight"), 1008}
                };
                for (auto& [setting, index] : values) {
                    if (toggler->getTag() == setting) {
                        if (auto toggle = dynamic_cast<CCMenuItemToggler*>(CCDirector::sharedDirector()->getRunningScene()->getChildByType<SetupEventLinkPopup>(0)->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0)->getChildByTag(index))) {
                            toggle->toggle(toggler->isToggled());
                            return;
                        }
                    }
                }
            }
        }
    }
};