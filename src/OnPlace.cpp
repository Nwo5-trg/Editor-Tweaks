#include "include.hpp"
#include <Geode/modify/LevelEditorLayer.hpp>

std::unordered_set<int> triggerIDs { // ethically sourced from object summary
    22, 23, 24, 25, 26, 27, 28, 32, 33, 55, 56, 57, 58, 59, 899, 901, 1006, 1007, 1049, 1268, 1346, 1347, 1520, 1585, 1595, 
    1611, 1612, 1613, 1616, 1811, 1812, 1814, 1815, 1817, 1818, 1819, 1912, 1913, 1914, 1915, 1916, 1917, 1932, 1934, 1935, 
    2015, 2016, 2062, 2066, 2067, 2068, 2899, 2900, 2901, 2903, 2904, 2905, 2907, 2909, 2910, 2911, 2912, 2913, 2914, 2915, 
    2916, 2917, 2919, 2920, 2921, 2922, 2923, 2924, 2925, 2999, 3006, 3007, 3008, 3009, 3010, 3011, 3012, 3013, 3014, 3015, 
    3016, 3017, 3018, 3019, 3020, 3021, 3022, 3023, 3024, 3029, 3030, 3031, 3032, 3033, 3600, 3602, 3603, 3604, 3605, 3606, 
    3607, 3608, 3609, 3612, 3613, 3614, 3615, 3617, 3618, 3619, 3620, 3641, 3642, 3660, 3661, 3662
};

std::unordered_set<int> portalIDs {
    12, 13, 47, 111, 286, 287, 660, 745, 1331, 1933
};


class $modify(LevelEditor, LevelEditorLayer) {
    GameObject* createObject(int p0, CCPoint p1, bool p2) {
        auto obj = LevelEditorLayer::createObject(p0, p1 ,p2);
        if (mod->getSettingValue<bool>("spawn-auto-spawn") && mod->getSettingValue<bool>("spawn-enabled") && triggerIDs.contains(obj->m_objectID)) {
            // log::error("ima kms fr fr {} {}", mod->getSavedValue<bool>("autospawn"), mod->getSavedValue<bool>("automulti"));
            static_cast<EffectGameObject*>(obj)->m_isSpawnTriggered = mod->getSavedValue<bool>("autospawn");
            static_cast<EffectGameObject*>(obj)->m_isMultiTriggered = mod->getSavedValue<bool>("automulti");
        }
        if (mod->getSettingValue<bool>("object-options-auto-freemode") && mod->getSettingValue<bool>("object-options-enabled") && portalIDs.contains(obj->m_objectID)) {
            static_cast<EffectGameObject*>(obj)->m_cameraIsFreeMode = mod->getSavedValue<bool>(std::to_string(obj->m_objectID) + "freemode");
        }
        return obj;
    }
};