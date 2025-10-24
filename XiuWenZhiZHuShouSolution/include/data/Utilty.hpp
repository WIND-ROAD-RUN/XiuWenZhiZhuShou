#pragma once
#include <QString>

#include "imgPro_ImageProcessUtilty.hpp"

struct DefectInfoForPriorityQueue
{
	float centerYLoc = -1;
	bool isInner = false;
	int index = 0;

	// 比较仅根据 centerY
	constexpr bool operator==(const DefectInfoForPriorityQueue& other) const noexcept {
		return centerYLoc == other.centerYLoc;
	}
	constexpr bool operator!=(const DefectInfoForPriorityQueue& other) const noexcept {
		return !(*this == other);
	}
	constexpr bool operator<(const DefectInfoForPriorityQueue& other) const noexcept {
		return centerYLoc < other.centerYLoc;
	}
	constexpr bool operator>(const DefectInfoForPriorityQueue& other) const noexcept {
		return other < *this;
	}
	constexpr bool operator<=(const DefectInfoForPriorityQueue& other) const noexcept {
		return !(other < *this);
	}
	constexpr bool operator>=(const DefectInfoForPriorityQueue& other) const noexcept {
		return !(*this < other);
	}
};

using DefectIndex = rw::imgPro::ProcessResultIndex;
using BodyIndex = rw::imgPro::ProcessResultIndex;
using BodyIndexWithDefectsIndexMap = std::unordered_map<BodyIndex,
	std::unordered_map<rw::imgPro::ClassId, std::vector<DefectIndex>>>;
using DefectsIndexWithBodyIndex = std::unordered_map<DefectIndex, BodyIndex>;
using DefectBoxs = std::vector<DefectInfoForPriorityQueue>;

struct Utility
{
	static QString cameraIp1;
	static QString cameraIp2;
	static QString zmotionIp;
};

struct ClassId
{
	static constexpr int minNum = 0;
	static constexpr int body = 0;
	static constexpr int queliao = 1;
	static constexpr int yiwu = 2;
	static constexpr int tangshang = 3;
	static constexpr int zangwu = 4;
	static constexpr int zhanlian = 5;
	static constexpr int feiliao = 6;
	static constexpr int mojucuowu = 7;
	static constexpr int maxNum = 7;
	static constexpr int indexCount = 8;
	static std::unordered_map<rw::imgPro::ClassId, rw::imgPro::ClassIdName> classIdNameMap;
	static std::vector<rw::imgPro::ClassId> classids;
};

struct Modbus
{
public:
	static size_t LocationGet;
};

struct ControlLines
{
public:	// 输入
	
public: // 输出
	static size_t DOpaizhao1;
	static size_t DOpaizhao2;
	static size_t DOguangyuan;
	static size_t DObaojing;
	static size_t DOtingji;
	static size_t DOxiachaqigangwai1;
	static size_t DOxiachaqigangwai2;
	static size_t DOxiachaqigangnei1;
	static size_t DOxiachaqigangnei2;
	static size_t DOboliaoqigangwai1;
	static size_t DOboliaoqigangwai2;
	static size_t DOboliaoqigangnei1;
	static size_t DOboliaoqigangnei2;
};

inline struct GlobalPath
{
public:
	QString projectHome = R"(D:\zfkjData\HandleScanner\)";
public:
	QString configRootPath = projectHome + R"(config\)";
	QString modelRootPath = projectHome + R"(model\)";
	QString txtPath = projectHome + R"(txt\)";

	QString HandleScannerConfigPath = configRootPath + R"(HandleScannerConfig.xml)";
	QString DlgProductScoreConfigPath = configRootPath + R"(DlgProductScoreConfig.xml)";
	QString DlgProductSetConfigPath = configRootPath + R"(DlgProductSetConfig.xml)";
	QString modelPath = modelRootPath + R"(HandleScanner.engine)";
	QString companyTxtPath = txtPath + R"(Company.txt)";
	QString saveAddressTxtPath = txtPath + R"(SaveAddress.txt)";
public:
	QString imageSaveRootPath = R"(D:\Images\)";
public:
	QString testImgDirPath = R"(D:\zfkjDevelopment\ImgStorage\handle)";
}globalPath;
