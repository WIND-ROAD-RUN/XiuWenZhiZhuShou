#include "Utilty.hpp"

QString Utility::cameraIp1 = "1";
QString Utility::cameraIp2 = "2";
QString Utility::zmotionIp = "192.168.0.11";

size_t Modbus::LocationGet = 0;

size_t ControlLines::DOpaizhao1 = 0;
size_t ControlLines::DOpaizhao2 = 0;
size_t ControlLines::DOguangyuan = 0;
size_t ControlLines::DObaojing = 0;
size_t ControlLines::DOtingji = 0;
size_t ControlLines::DOxiachaqigangwai1 = 0;
size_t ControlLines::DOxiachaqigangwai2 = 0;
size_t ControlLines::DOxiachaqigangnei1 = 0;
size_t ControlLines::DOxiachaqigangnei2 = 0;
size_t ControlLines::DOboliaoqigangwai1 = 0;
size_t ControlLines::DOboliaoqigangwai2 = 0;
size_t ControlLines::DOboliaoqigangnei1 = 0;
size_t ControlLines::DOboliaoqigangnei2 = 0;

std::unordered_map<rw::imgPro::ClassId, rw::imgPro::ClassIdName> ClassId::classIdNameMap = {
	{ClassId::body,"把手"},
	{ClassId::queliao,"缺料"},
	{ClassId::tangshang,"烫伤"},
	{ClassId::yiwu,"异物"},
	{ClassId::zangwu,"脏污"},
	{ClassId::zhanlian,"粘连"},
	{ClassId::feiliao,"废料"},
	{ClassId::mojucuowu,"模具错误"}
};

std::vector<rw::imgPro::ClassId> ClassId::classids = {
    ClassId::body,
    ClassId::queliao,
    ClassId::tangshang,
    ClassId::yiwu,
    ClassId::zangwu,
	ClassId::zhanlian,
	ClassId::feiliao,
	ClassId::mojucuowu
};