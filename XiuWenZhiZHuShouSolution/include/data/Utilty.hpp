#pragma once
#include <QString>

#include "imgPro_ImageProcessUtilty.hpp"

struct Utility
{
	static QString cameraIp1;
};

struct ClassId
{
	static constexpr int minNum = 0;
	static constexpr int body = 0;
	static constexpr int maxNum = 0;
	static constexpr int indexCount = 1;
	static std::unordered_map<rw::imgPro::ClassId, rw::imgPro::ClassIdName> classIdNameMap;
	static std::vector<rw::imgPro::ClassId> classids;
};

inline struct GlobalPath
{
public:
	QString projectHome = R"(D:\zfkjData\XiuWenZhiZHuShouSolution\)";
public:
	QString configRootPath = projectHome + R"(config\)";
	QString modelRootPath = projectHome + R"(model\)";
	QString txtPath = projectHome + R"(txt\)";

	QString MainWindowConfigPath = configRootPath + R"(MainWindowConfig.xml)";
	QString modelPath = modelRootPath + R"(XiuWenZhiZHuShouSolution.engine)";
	QString companyTxtPath = txtPath + R"(Company.txt)";
public:
	QString imageSaveRootPath = projectHome + R"(SavedImages\)";
	QString testImgDirPath = R"(D:\zfkjDevelopment\ImgStorage\zhizhushou)";
}globalPath;
