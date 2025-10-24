#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class ScoreConfig
    {
    public:
        ScoreConfig() = default;
        ~ScoreConfig() = default;

        ScoreConfig(const rw::oso::ObjectStoreAssembly& assembly);
        ScoreConfig(const ScoreConfig& obj);

        ScoreConfig& operator=(const ScoreConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const ScoreConfig& obj) const;
        bool operator!=(const ScoreConfig& obj) const;

    public:
        bool tangShang{ false };
        double tangShangScore{ 0 };
        double tangShangArea{ 0 };
        bool queLiao{ false };
        double queLiaoScore{ 0 };
        double queLiaoArea{ 0 };
        bool yiWu{ false };
        double yiWuScore{ 0 };
        double yiWuArea{ 0 };
        bool zangWu{ false };
        double zangWuScore{ 0 };
        double zangWuArea{ 0 };
        bool zhanlian{ false };
        double zhanlianScore{ 0 };
        double zhanlianArea{ 0 };
        bool feiliao{ false };
        double feiliaoScore{ 0 };
        double feiliaoArea{ 0 };
        bool mojucuowu{ false };
        double mojucuowuScore{ 0 };
        double mojucuowuArea{ 0 };
    };

    inline ScoreConfig::ScoreConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$ScoreConfig$")
        {
            throw std::runtime_error("Assembly is not $class$ScoreConfig$");
        }
        auto tangShangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tangShang$"));
        if (!tangShangItem) {
            throw std::runtime_error("$variable$tangShang is not found");
        }
        tangShang = tangShangItem->getValueAsBool();
        auto tangShangScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tangShangScore$"));
        if (!tangShangScoreItem) {
            throw std::runtime_error("$variable$tangShangScore is not found");
        }
        tangShangScore = tangShangScoreItem->getValueAsDouble();
        auto tangShangAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tangShangArea$"));
        if (!tangShangAreaItem) {
            throw std::runtime_error("$variable$tangShangArea is not found");
        }
        tangShangArea = tangShangAreaItem->getValueAsDouble();
        auto queLiaoItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$queLiao$"));
        if (!queLiaoItem) {
            throw std::runtime_error("$variable$queLiao is not found");
        }
        queLiao = queLiaoItem->getValueAsBool();
        auto queLiaoScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$queLiaoScore$"));
        if (!queLiaoScoreItem) {
            throw std::runtime_error("$variable$queLiaoScore is not found");
        }
        queLiaoScore = queLiaoScoreItem->getValueAsDouble();
        auto queLiaoAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$queLiaoArea$"));
        if (!queLiaoAreaItem) {
            throw std::runtime_error("$variable$queLiaoArea is not found");
        }
        queLiaoArea = queLiaoAreaItem->getValueAsDouble();
        auto yiWuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yiWu$"));
        if (!yiWuItem) {
            throw std::runtime_error("$variable$yiWu is not found");
        }
        yiWu = yiWuItem->getValueAsBool();
        auto yiWuScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yiWuScore$"));
        if (!yiWuScoreItem) {
            throw std::runtime_error("$variable$yiWuScore is not found");
        }
        yiWuScore = yiWuScoreItem->getValueAsDouble();
        auto yiWuAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yiWuArea$"));
        if (!yiWuAreaItem) {
            throw std::runtime_error("$variable$yiWuArea is not found");
        }
        yiWuArea = yiWuAreaItem->getValueAsDouble();
        auto zangWuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zangWu$"));
        if (!zangWuItem) {
            throw std::runtime_error("$variable$zangWu is not found");
        }
        zangWu = zangWuItem->getValueAsBool();
        auto zangWuScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zangWuScore$"));
        if (!zangWuScoreItem) {
            throw std::runtime_error("$variable$zangWuScore is not found");
        }
        zangWuScore = zangWuScoreItem->getValueAsDouble();
        auto zangWuAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zangWuArea$"));
        if (!zangWuAreaItem) {
            throw std::runtime_error("$variable$zangWuArea is not found");
        }
        zangWuArea = zangWuAreaItem->getValueAsDouble();
        auto zhanlianItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhanlian$"));
        if (!zhanlianItem) {
            throw std::runtime_error("$variable$zhanlian is not found");
        }
        zhanlian = zhanlianItem->getValueAsBool();
        auto zhanlianScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhanlianScore$"));
        if (!zhanlianScoreItem) {
            throw std::runtime_error("$variable$zhanlianScore is not found");
        }
        zhanlianScore = zhanlianScoreItem->getValueAsDouble();
        auto zhanlianAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhanlianArea$"));
        if (!zhanlianAreaItem) {
            throw std::runtime_error("$variable$zhanlianArea is not found");
        }
        zhanlianArea = zhanlianAreaItem->getValueAsDouble();
        auto feiliaoItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$feiliao$"));
        if (!feiliaoItem) {
            throw std::runtime_error("$variable$feiliao is not found");
        }
        feiliao = feiliaoItem->getValueAsBool();
        auto feiliaoScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$feiliaoScore$"));
        if (!feiliaoScoreItem) {
            throw std::runtime_error("$variable$feiliaoScore is not found");
        }
        feiliaoScore = feiliaoScoreItem->getValueAsDouble();
        auto feiliaoAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$feiliaoArea$"));
        if (!feiliaoAreaItem) {
            throw std::runtime_error("$variable$feiliaoArea is not found");
        }
        feiliaoArea = feiliaoAreaItem->getValueAsDouble();
        auto mojucuowuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$mojucuowu$"));
        if (!mojucuowuItem) {
            throw std::runtime_error("$variable$mojucuowu is not found");
        }
        mojucuowu = mojucuowuItem->getValueAsBool();
        auto mojucuowuScoreItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$mojucuowuScore$"));
        if (!mojucuowuScoreItem) {
            throw std::runtime_error("$variable$mojucuowuScore is not found");
        }
        mojucuowuScore = mojucuowuScoreItem->getValueAsDouble();
        auto mojucuowuAreaItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$mojucuowuArea$"));
        if (!mojucuowuAreaItem) {
            throw std::runtime_error("$variable$mojucuowuArea is not found");
        }
        mojucuowuArea = mojucuowuAreaItem->getValueAsDouble();
    }

    inline ScoreConfig::ScoreConfig(const ScoreConfig& obj)
    {
        tangShang = obj.tangShang;
        tangShangScore = obj.tangShangScore;
        tangShangArea = obj.tangShangArea;
        queLiao = obj.queLiao;
        queLiaoScore = obj.queLiaoScore;
        queLiaoArea = obj.queLiaoArea;
        yiWu = obj.yiWu;
        yiWuScore = obj.yiWuScore;
        yiWuArea = obj.yiWuArea;
        zangWu = obj.zangWu;
        zangWuScore = obj.zangWuScore;
        zangWuArea = obj.zangWuArea;
        zhanlian = obj.zhanlian;
        zhanlianScore = obj.zhanlianScore;
        zhanlianArea = obj.zhanlianArea;
        feiliao = obj.feiliao;
        feiliaoScore = obj.feiliaoScore;
        feiliaoArea = obj.feiliaoArea;
        mojucuowu = obj.mojucuowu;
        mojucuowuScore = obj.mojucuowuScore;
        mojucuowuArea = obj.mojucuowuArea;
    }

    inline ScoreConfig& ScoreConfig::operator=(const ScoreConfig& obj)
    {
        if (this != &obj) {
            tangShang = obj.tangShang;
            tangShangScore = obj.tangShangScore;
            tangShangArea = obj.tangShangArea;
            queLiao = obj.queLiao;
            queLiaoScore = obj.queLiaoScore;
            queLiaoArea = obj.queLiaoArea;
            yiWu = obj.yiWu;
            yiWuScore = obj.yiWuScore;
            yiWuArea = obj.yiWuArea;
            zangWu = obj.zangWu;
            zangWuScore = obj.zangWuScore;
            zangWuArea = obj.zangWuArea;
            zhanlian = obj.zhanlian;
            zhanlianScore = obj.zhanlianScore;
            zhanlianArea = obj.zhanlianArea;
            feiliao = obj.feiliao;
            feiliaoScore = obj.feiliaoScore;
            feiliaoArea = obj.feiliaoArea;
            mojucuowu = obj.mojucuowu;
            mojucuowuScore = obj.mojucuowuScore;
            mojucuowuArea = obj.mojucuowuArea;
        }
        return *this;
    }

    inline ScoreConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$ScoreConfig$");
        auto tangShangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tangShangItem->setName("$variable$tangShang$");
        tangShangItem->setValueFromBool(tangShang);
        assembly.addItem(tangShangItem);
        auto tangShangScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tangShangScoreItem->setName("$variable$tangShangScore$");
        tangShangScoreItem->setValueFromDouble(tangShangScore);
        assembly.addItem(tangShangScoreItem);
        auto tangShangAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        tangShangAreaItem->setName("$variable$tangShangArea$");
        tangShangAreaItem->setValueFromDouble(tangShangArea);
        assembly.addItem(tangShangAreaItem);
        auto queLiaoItem = std::make_shared<rw::oso::ObjectStoreItem>();
        queLiaoItem->setName("$variable$queLiao$");
        queLiaoItem->setValueFromBool(queLiao);
        assembly.addItem(queLiaoItem);
        auto queLiaoScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        queLiaoScoreItem->setName("$variable$queLiaoScore$");
        queLiaoScoreItem->setValueFromDouble(queLiaoScore);
        assembly.addItem(queLiaoScoreItem);
        auto queLiaoAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        queLiaoAreaItem->setName("$variable$queLiaoArea$");
        queLiaoAreaItem->setValueFromDouble(queLiaoArea);
        assembly.addItem(queLiaoAreaItem);
        auto yiWuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        yiWuItem->setName("$variable$yiWu$");
        yiWuItem->setValueFromBool(yiWu);
        assembly.addItem(yiWuItem);
        auto yiWuScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        yiWuScoreItem->setName("$variable$yiWuScore$");
        yiWuScoreItem->setValueFromDouble(yiWuScore);
        assembly.addItem(yiWuScoreItem);
        auto yiWuAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        yiWuAreaItem->setName("$variable$yiWuArea$");
        yiWuAreaItem->setValueFromDouble(yiWuArea);
        assembly.addItem(yiWuAreaItem);
        auto zangWuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zangWuItem->setName("$variable$zangWu$");
        zangWuItem->setValueFromBool(zangWu);
        assembly.addItem(zangWuItem);
        auto zangWuScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zangWuScoreItem->setName("$variable$zangWuScore$");
        zangWuScoreItem->setValueFromDouble(zangWuScore);
        assembly.addItem(zangWuScoreItem);
        auto zangWuAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zangWuAreaItem->setName("$variable$zangWuArea$");
        zangWuAreaItem->setValueFromDouble(zangWuArea);
        assembly.addItem(zangWuAreaItem);
        auto zhanlianItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zhanlianItem->setName("$variable$zhanlian$");
        zhanlianItem->setValueFromBool(zhanlian);
        assembly.addItem(zhanlianItem);
        auto zhanlianScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zhanlianScoreItem->setName("$variable$zhanlianScore$");
        zhanlianScoreItem->setValueFromDouble(zhanlianScore);
        assembly.addItem(zhanlianScoreItem);
        auto zhanlianAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zhanlianAreaItem->setName("$variable$zhanlianArea$");
        zhanlianAreaItem->setValueFromDouble(zhanlianArea);
        assembly.addItem(zhanlianAreaItem);
        auto feiliaoItem = std::make_shared<rw::oso::ObjectStoreItem>();
        feiliaoItem->setName("$variable$feiliao$");
        feiliaoItem->setValueFromBool(feiliao);
        assembly.addItem(feiliaoItem);
        auto feiliaoScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        feiliaoScoreItem->setName("$variable$feiliaoScore$");
        feiliaoScoreItem->setValueFromDouble(feiliaoScore);
        assembly.addItem(feiliaoScoreItem);
        auto feiliaoAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        feiliaoAreaItem->setName("$variable$feiliaoArea$");
        feiliaoAreaItem->setValueFromDouble(feiliaoArea);
        assembly.addItem(feiliaoAreaItem);
        auto mojucuowuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        mojucuowuItem->setName("$variable$mojucuowu$");
        mojucuowuItem->setValueFromBool(mojucuowu);
        assembly.addItem(mojucuowuItem);
        auto mojucuowuScoreItem = std::make_shared<rw::oso::ObjectStoreItem>();
        mojucuowuScoreItem->setName("$variable$mojucuowuScore$");
        mojucuowuScoreItem->setValueFromDouble(mojucuowuScore);
        assembly.addItem(mojucuowuScoreItem);
        auto mojucuowuAreaItem = std::make_shared<rw::oso::ObjectStoreItem>();
        mojucuowuAreaItem->setName("$variable$mojucuowuArea$");
        mojucuowuAreaItem->setValueFromDouble(mojucuowuArea);
        assembly.addItem(mojucuowuAreaItem);
        return assembly;
    }

    inline bool ScoreConfig::operator==(const ScoreConfig& obj) const
    {
        return tangShang == obj.tangShang && tangShangScore == obj.tangShangScore && tangShangArea == obj.tangShangArea && queLiao == obj.queLiao && queLiaoScore == obj.queLiaoScore && queLiaoArea == obj.queLiaoArea && yiWu == obj.yiWu && yiWuScore == obj.yiWuScore && yiWuArea == obj.yiWuArea && zangWu == obj.zangWu && zangWuScore == obj.zangWuScore && zangWuArea == obj.zangWuArea && zhanlian == obj.zhanlian && zhanlianScore == obj.zhanlianScore && zhanlianArea == obj.zhanlianArea && feiliao == obj.feiliao && feiliaoScore == obj.feiliaoScore && feiliaoArea == obj.feiliaoArea && mojucuowu == obj.mojucuowu && mojucuowuScore == obj.mojucuowuScore && mojucuowuArea == obj.mojucuowuArea;
    }

    inline bool ScoreConfig::operator!=(const ScoreConfig& obj) const
    {
        return !(*this == obj);
    }

}

