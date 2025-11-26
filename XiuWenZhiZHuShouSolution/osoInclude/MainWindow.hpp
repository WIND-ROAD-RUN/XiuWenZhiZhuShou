#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class MainWindowConfig
    {
    public:
        MainWindowConfig() = default;
        ~MainWindowConfig() = default;

        MainWindowConfig(const rw::oso::ObjectStoreAssembly& assembly);
        MainWindowConfig(const MainWindowConfig& obj);

        MainWindowConfig& operator=(const MainWindowConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const MainWindowConfig& obj) const;
        bool operator!=(const MainWindowConfig& obj) const;

    public:
        bool isDebug{ false };
        bool isDefect{ false };
        bool isSaveImg{ false };
        double xiangsudangliang{ 0.0 };
        double xiandingtiji{ 0.0 };
        double zuoxianwei{ 0.0 };
        double youxianwei{ 0.0 };
    };

    inline MainWindowConfig::MainWindowConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$MainWindowConfig$")
        {
            throw std::runtime_error("Assembly is not $class$MainWindowConfig$");
        }
        auto isDebugItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isDebug$"));
        if (!isDebugItem) {
            throw std::runtime_error("$variable$isDebug is not found");
        }
        isDebug = isDebugItem->getValueAsBool();
        auto isDefectItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isDefect$"));
        if (!isDefectItem) {
            throw std::runtime_error("$variable$isDefect is not found");
        }
        isDefect = isDefectItem->getValueAsBool();
        auto isSaveImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isSaveImg$"));
        if (!isSaveImgItem) {
            throw std::runtime_error("$variable$isSaveImg is not found");
        }
        isSaveImg = isSaveImgItem->getValueAsBool();
        auto xiangsudangliangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangsudangliang$"));
        if (!xiangsudangliangItem) {
            throw std::runtime_error("$variable$xiangsudangliang is not found");
        }
        xiangsudangliang = xiangsudangliangItem->getValueAsDouble();
        auto xiandingtijiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiandingtiji$"));
        if (!xiandingtijiItem) {
            throw std::runtime_error("$variable$xiandingtiji is not found");
        }
        xiandingtiji = xiandingtijiItem->getValueAsDouble();
        auto zuoxianweiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zuoxianwei$"));
        if (!zuoxianweiItem) {
            throw std::runtime_error("$variable$zuoxianwei is not found");
        }
        zuoxianwei = zuoxianweiItem->getValueAsDouble();
        auto youxianweiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$youxianwei$"));
        if (!youxianweiItem) {
            throw std::runtime_error("$variable$youxianwei is not found");
        }
        youxianwei = youxianweiItem->getValueAsDouble();
    }

    inline MainWindowConfig::MainWindowConfig(const MainWindowConfig& obj)
    {
        isDebug = obj.isDebug;
        isDefect = obj.isDefect;
        isSaveImg = obj.isSaveImg;
        xiangsudangliang = obj.xiangsudangliang;
        xiandingtiji = obj.xiandingtiji;
        zuoxianwei = obj.zuoxianwei;
        youxianwei = obj.youxianwei;
    }

    inline MainWindowConfig& MainWindowConfig::operator=(const MainWindowConfig& obj)
    {
        if (this != &obj) {
            isDebug = obj.isDebug;
            isDefect = obj.isDefect;
            isSaveImg = obj.isSaveImg;
            xiangsudangliang = obj.xiangsudangliang;
            xiandingtiji = obj.xiandingtiji;
            zuoxianwei = obj.zuoxianwei;
            youxianwei = obj.youxianwei;
        }
        return *this;
    }

    inline MainWindowConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$MainWindowConfig$");
        auto isDebugItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isDebugItem->setName("$variable$isDebug$");
        isDebugItem->setValueFromBool(isDebug);
        assembly.addItem(isDebugItem);
        auto isDefectItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isDefectItem->setName("$variable$isDefect$");
        isDefectItem->setValueFromBool(isDefect);
        assembly.addItem(isDefectItem);
        auto isSaveImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isSaveImgItem->setName("$variable$isSaveImg$");
        isSaveImgItem->setValueFromBool(isSaveImg);
        assembly.addItem(isSaveImgItem);
        auto xiangsudangliangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangsudangliangItem->setName("$variable$xiangsudangliang$");
        xiangsudangliangItem->setValueFromDouble(xiangsudangliang);
        assembly.addItem(xiangsudangliangItem);
        auto xiandingtijiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiandingtijiItem->setName("$variable$xiandingtiji$");
        xiandingtijiItem->setValueFromDouble(xiandingtiji);
        assembly.addItem(xiandingtijiItem);
        auto zuoxianweiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zuoxianweiItem->setName("$variable$zuoxianwei$");
        zuoxianweiItem->setValueFromDouble(zuoxianwei);
        assembly.addItem(zuoxianweiItem);
        auto youxianweiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        youxianweiItem->setName("$variable$youxianwei$");
        youxianweiItem->setValueFromDouble(youxianwei);
        assembly.addItem(youxianweiItem);
        return assembly;
    }

    inline bool MainWindowConfig::operator==(const MainWindowConfig& obj) const
    {
        return isDebug == obj.isDebug && isDefect == obj.isDefect && isSaveImg == obj.isSaveImg && xiangsudangliang == obj.xiangsudangliang && xiandingtiji == obj.xiandingtiji && zuoxianwei == obj.zuoxianwei && youxianwei == obj.youxianwei;
    }

    inline bool MainWindowConfig::operator!=(const MainWindowConfig& obj) const
    {
        return !(*this == obj);
    }

}

