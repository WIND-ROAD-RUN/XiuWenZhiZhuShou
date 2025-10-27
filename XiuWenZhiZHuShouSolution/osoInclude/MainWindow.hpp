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
        double xiangsudangliang{ 0.0 };
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
        auto xiangsudangliangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangsudangliang$"));
        if (!xiangsudangliangItem) {
            throw std::runtime_error("$variable$xiangsudangliang is not found");
        }
        xiangsudangliang = xiangsudangliangItem->getValueAsDouble();
    }

    inline MainWindowConfig::MainWindowConfig(const MainWindowConfig& obj)
    {
        isDebug = obj.isDebug;
        isDefect = obj.isDefect;
        xiangsudangliang = obj.xiangsudangliang;
    }

    inline MainWindowConfig& MainWindowConfig::operator=(const MainWindowConfig& obj)
    {
        if (this != &obj) {
            isDebug = obj.isDebug;
            isDefect = obj.isDefect;
            xiangsudangliang = obj.xiangsudangliang;
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
        auto xiangsudangliangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangsudangliangItem->setName("$variable$xiangsudangliang$");
        xiangsudangliangItem->setValueFromDouble(xiangsudangliang);
        assembly.addItem(xiangsudangliangItem);
        return assembly;
    }

    inline bool MainWindowConfig::operator==(const MainWindowConfig& obj) const
    {
        return isDebug == obj.isDebug && isDefect == obj.isDefect && xiangsudangliang == obj.xiangsudangliang;
    }

    inline bool MainWindowConfig::operator!=(const MainWindowConfig& obj) const
    {
        return !(*this == obj);
    }

}

