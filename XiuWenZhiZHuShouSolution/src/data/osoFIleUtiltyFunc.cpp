#include "osoFIleUtiltyFunc.hpp"

#include "Utilty.hpp"

rw::imgPro::EliminationInfoFunc::ClassIdWithConfigMap cdm::ScoreConfigConvert::toClassIdWithEliConfigMap(
	const cdm::ScoreConfig& config, double areaFactor, double scoreFactor)
{
	rw::imgPro::EliminationInfoFunc::ClassIdWithConfigMap result{};

	rw::imgPro::EliminationInfoGetConfig configBase;
	configBase.isUsingArea = true;
	configBase.isUsingScore = true;
	configBase.areaFactor = areaFactor;
	configBase.scoreFactor = scoreFactor;
	configBase.areaIsUsingComplementarySet = false;
	configBase.scoreIsUsingComplementarySet = false;

	for (int i = ClassId::minNum; i <= ClassId::maxNum; i++)
	{
		result[i] = configBase;
	}

	// body不参与剔废
	result[ClassId::body].isUsingArea = false;
	result[ClassId::body].isUsingScore = false;

	result[ClassId::queliao].areaRange = { 0,config.queLiaoArea };
	result[ClassId::queliao].scoreRange = { 0,config.queLiaoScore };

	result[ClassId::tangshang].areaRange = { 0,config.tangShangArea };
	result[ClassId::tangshang].scoreRange = { 0,config.tangShangScore };

	result[ClassId::yiwu].areaRange = { 0,config.yiWuArea };
	result[ClassId::yiwu].scoreRange = { 0,config.yiWuScore };

	result[ClassId::zangwu].areaRange = { 0,config.zangWuArea };
	result[ClassId::zangwu].scoreRange = { 0,config.zangWuScore };

	result[ClassId::zhanlian].areaRange = { 0,config.zhanlianArea };
	result[ClassId::zhanlian].scoreRange = { 0,config.zhanlianScore };

	result[ClassId::feiliao].areaRange = { 0,config.feiliaoArea };
	result[ClassId::feiliao].scoreRange = { 0,config.feiliaoScore };

	result[ClassId::mojucuowu].areaRange = { 0,config.mojucuowuArea };
	result[ClassId::mojucuowu].scoreRange = { 0,config.mojucuowuScore };

	return result;
}

rw::imgPro::DefectResultInfoFunc::ClassIdWithConfigMap cdm::ScoreConfigConvert::toClassIdWithDefConfigMap(
	const cdm::ScoreConfig& config)
{
	rw::imgPro::DefectResultInfoFunc::ClassIdWithConfigMap result;
	rw::imgPro::DefectResultInfoFunc::Config configBase;
	configBase.isEnable = false;
	for (int i = ClassId::minNum; i <= ClassId::maxNum; i++)
	{
		result[i] = configBase;
	}

	// body不参与剔废
	result[ClassId::body].isEnable = false;

	result[ClassId::zangwu].isEnable = config.zangWu;
	result[ClassId::queliao].isEnable = config.queLiao;
	result[ClassId::tangshang].isEnable = config.tangShang;
	result[ClassId::yiwu].isEnable = config.yiWu;
	result[ClassId::zhanlian].isEnable = config.zhanlian;
	result[ClassId::feiliao].isEnable = config.feiliao;
	result[ClassId::mojucuowu].isEnable = config.mojucuowu;

	return result;
}
