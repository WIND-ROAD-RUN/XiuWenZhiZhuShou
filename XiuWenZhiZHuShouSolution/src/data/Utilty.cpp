#include "Utilty.hpp"

QString Utility::cameraIp1 = "0";

std::unordered_map<rw::imgPro::ClassId, rw::imgPro::ClassIdName> ClassId::classIdNameMap = {
	{ClassId::body,"本体"}
};

std::vector<rw::imgPro::ClassId> ClassId::classids = {
    ClassId::body
};