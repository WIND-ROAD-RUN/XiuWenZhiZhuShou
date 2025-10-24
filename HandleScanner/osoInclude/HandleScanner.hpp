#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class HandleScannerConfig
    {
    public:
        HandleScannerConfig() = default;
        ~HandleScannerConfig() = default;

        HandleScannerConfig(const rw::oso::ObjectStoreAssembly& assembly);
        HandleScannerConfig(const HandleScannerConfig& obj);

        HandleScannerConfig& operator=(const HandleScannerConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const HandleScannerConfig& obj) const;
        bool operator!=(const HandleScannerConfig& obj) const;

    public:
        int totalProductionVolume{ 0 };
        int totalDefectiveVolume{ 0 };
        double productionYield{ 0.0 };
        bool isDebug{ false };
        bool isDefect{ false };
        bool isSaveImg{ false };
        bool isshibiekuang{ true };
        bool iswenzi{ true };
        int tingjigeshu{ 0 };
        bool isqiangguang{ false };
        bool iszhongguang{ false };
        bool isruoguang{ false };
    };

    inline HandleScannerConfig::HandleScannerConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$HandleScannerConfig$")
        {
            throw std::runtime_error("Assembly is not $class$HandleScannerConfig$");
        }
        auto totalProductionVolumeItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$totalProductionVolume$"));
        if (!totalProductionVolumeItem) {
            throw std::runtime_error("$variable$totalProductionVolume is not found");
        }
        totalProductionVolume = totalProductionVolumeItem->getValueAsInt();
        auto totalDefectiveVolumeItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$totalDefectiveVolume$"));
        if (!totalDefectiveVolumeItem) {
            throw std::runtime_error("$variable$totalDefectiveVolume is not found");
        }
        totalDefectiveVolume = totalDefectiveVolumeItem->getValueAsInt();
        auto productionYieldItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$productionYield$"));
        if (!productionYieldItem) {
            throw std::runtime_error("$variable$productionYield is not found");
        }
        productionYield = productionYieldItem->getValueAsDouble();
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
        auto isshibiekuangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isshibiekuang$"));
        if (!isshibiekuangItem) {
            throw std::runtime_error("$variable$isshibiekuang is not found");
        }
        isshibiekuang = isshibiekuangItem->getValueAsBool();
        auto iswenziItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$iswenzi$"));
        if (!iswenziItem) {
            throw std::runtime_error("$variable$iswenzi is not found");
        }
        iswenzi = iswenziItem->getValueAsBool();
        auto tingjigeshuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tingjigeshu$"));
        if (!tingjigeshuItem) {
            throw std::runtime_error("$variable$tingjigeshu is not found");
        }
        tingjigeshu = tingjigeshuItem->getValueAsInt();
        auto isqiangguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isqiangguang$"));
        if (!isqiangguangItem) {
            throw std::runtime_error("$variable$isqiangguang is not found");
        }
        isqiangguang = isqiangguangItem->getValueAsBool();
        auto iszhongguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$iszhongguang$"));
        if (!iszhongguangItem) {
            throw std::runtime_error("$variable$iszhongguang is not found");
        }
        iszhongguang = iszhongguangItem->getValueAsBool();
        auto isruoguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isruoguang$"));
        if (!isruoguangItem) {
            throw std::runtime_error("$variable$isruoguang is not found");
        }
        isruoguang = isruoguangItem->getValueAsBool();
    }

    inline HandleScannerConfig::HandleScannerConfig(const HandleScannerConfig& obj)
    {
        totalProductionVolume = obj.totalProductionVolume;
        totalDefectiveVolume = obj.totalDefectiveVolume;
        productionYield = obj.productionYield;
        isDebug = obj.isDebug;
        isDefect = obj.isDefect;
        isSaveImg = obj.isSaveImg;
        isshibiekuang = obj.isshibiekuang;
        iswenzi = obj.iswenzi;
        tingjigeshu = obj.tingjigeshu;
        isqiangguang = obj.isqiangguang;
        iszhongguang = obj.iszhongguang;
        isruoguang = obj.isruoguang;
    }

    inline HandleScannerConfig& HandleScannerConfig::operator=(const HandleScannerConfig& obj)
    {
        if (this != &obj) {
            totalProductionVolume = obj.totalProductionVolume;
            totalDefectiveVolume = obj.totalDefectiveVolume;
            productionYield = obj.productionYield;
            isDebug = obj.isDebug;
            isDefect = obj.isDefect;
            isSaveImg = obj.isSaveImg;
            isshibiekuang = obj.isshibiekuang;
            iswenzi = obj.iswenzi;
            tingjigeshu = obj.tingjigeshu;
            isqiangguang = obj.isqiangguang;
            iszhongguang = obj.iszhongguang;
            isruoguang = obj.isruoguang;
        }
        return *this;
    }

    inline HandleScannerConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$HandleScannerConfig$");
        auto totalProductionVolumeItem = std::make_shared<rw::oso::ObjectStoreItem>();
        totalProductionVolumeItem->setName("$variable$totalProductionVolume$");
        totalProductionVolumeItem->setValueFromInt(totalProductionVolume);
        assembly.addItem(totalProductionVolumeItem);
        auto totalDefectiveVolumeItem = std::make_shared<rw::oso::ObjectStoreItem>();
        totalDefectiveVolumeItem->setName("$variable$totalDefectiveVolume$");
        totalDefectiveVolumeItem->setValueFromInt(totalDefectiveVolume);
        assembly.addItem(totalDefectiveVolumeItem);
        auto productionYieldItem = std::make_shared<rw::oso::ObjectStoreItem>();
        productionYieldItem->setName("$variable$productionYield$");
        productionYieldItem->setValueFromDouble(productionYield);
        assembly.addItem(productionYieldItem);
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
        auto isshibiekuangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isshibiekuangItem->setName("$variable$isshibiekuang$");
        isshibiekuangItem->setValueFromBool(isshibiekuang);
        assembly.addItem(isshibiekuangItem);
        auto iswenziItem = std::make_shared<rw::oso::ObjectStoreItem>();
        iswenziItem->setName("$variable$iswenzi$");
        iswenziItem->setValueFromBool(iswenzi);
        assembly.addItem(iswenziItem);
        auto tingjigeshuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tingjigeshuItem->setName("$variable$tingjigeshu$");
        tingjigeshuItem->setValueFromInt(tingjigeshu);
        assembly.addItem(tingjigeshuItem);
        auto isqiangguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isqiangguangItem->setName("$variable$isqiangguang$");
        isqiangguangItem->setValueFromBool(isqiangguang);
        assembly.addItem(isqiangguangItem);
        auto iszhongguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        iszhongguangItem->setName("$variable$iszhongguang$");
        iszhongguangItem->setValueFromBool(iszhongguang);
        assembly.addItem(iszhongguangItem);
        auto isruoguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isruoguangItem->setName("$variable$isruoguang$");
        isruoguangItem->setValueFromBool(isruoguang);
        assembly.addItem(isruoguangItem);
        return assembly;
    }

    inline bool HandleScannerConfig::operator==(const HandleScannerConfig& obj) const
    {
        return totalProductionVolume == obj.totalProductionVolume && totalDefectiveVolume == obj.totalDefectiveVolume && productionYield == obj.productionYield && isDebug == obj.isDebug && isDefect == obj.isDefect && isSaveImg == obj.isSaveImg && isshibiekuang == obj.isshibiekuang && iswenzi == obj.iswenzi && tingjigeshu == obj.tingjigeshu && isqiangguang == obj.isqiangguang && iszhongguang == obj.iszhongguang && isruoguang == obj.isruoguang;
    }

    inline bool HandleScannerConfig::operator!=(const HandleScannerConfig& obj) const
    {
        return !(*this == obj);
    }

}

