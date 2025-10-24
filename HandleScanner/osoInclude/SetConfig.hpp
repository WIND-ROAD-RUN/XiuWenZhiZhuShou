#pragma once

#include"oso_core.h"
#include <string>

namespace cdm {
    class SetConfig
    {
    public:
        SetConfig() = default;
        ~SetConfig() = default;

        SetConfig(const rw::oso::ObjectStoreAssembly& assembly);
        SetConfig(const SetConfig& obj);

        SetConfig& operator=(const SetConfig& obj);
        operator rw::oso::ObjectStoreAssembly() const;
        bool operator==(const SetConfig& obj) const;
        bool operator!=(const SetConfig& obj) const;

    public:
        int zhongyangxianwei1{ 0 };
        int zhongyangxianwei2{ 0 };
        bool qiyongerxiangji{ false };
        bool qiyongyundongkongzhiqi{ false };
        bool yundongkongzhiqichonglian{ false };
        double yanshixiachawai1{ 0 };
        double boliaoshijianwai1{ 0 };
        double yanshixiachawai2{ 0 };
        double boliaoshijianwai2{ 0 };
        double yanshixiachanei1{ 0 };
        double boliaoshijiannei1{ 0 };
        double yanshixiachanei2{ 0 };
        double boliaoshijiannei2{ 0 };
        double xiangjichufachangdu{ 0 };
        double meizhuanmaichongshu{ 0 };
        double shedingzhouchang{ 0 };
        double waicetifeijuli1{ 0 };
        double neicetifeijuli1{ 0 };
        double tifeirongyufangdou1{ 0 };
        double waicetifeijuli2{ 0 };
        double neicetifeijuli2{ 0 };
        double tifeirongyufangdou2{ 0 };
        int tuxiangshunshizhenxuanzhuancishu1{ 0 };
        int tuxiangshunshizhenxuanzhuancishu2{ 0 };
        double xunzhaoquexiansuoshubashoudepianchazhi{ 0 };
        double shangXianWei1{ 0 };
        double xiaXianWei1{ 0 };
        double zuoXianWei1{ 0 };
        double youXianWei1{ 0 };
        double xiangSuDangLiang1{ 0 };
        double shangXianWei2{ 0 };
        double xiaXianWei2{ 0 };
        double zuoXianWei2{ 0 };
        double youXianWei2{ 0 };
        double xiangSuDangLiang2{ 0 };
        double qiangbaoguang{ 0 };
        double qiangzengyi{ 0 };
        double zhongbaoguang{ 0 };
        double zhongzengyi{ 0 };
        double ruobaoguang{ 0 };
        double ruozengyi{ 0 };
        bool takeWork1Pictures{ false };
        bool takeWork2Pictures{ false };
        bool saveNGImg{ false };
        bool saveMaskImg{ false };
        bool saveOKImg{ false };
        bool isSaveJPEG{ false };
        bool isSaveBMP{ false };
        bool isSavePNG{ false };
        int saveImgQuality{ 66 };
        int DOpaizhao1{ 0 };
        int DOpaizhao2{ 0 };
        int DOguangyuan{ 0 };
        int DObaojing{ 0 };
        int DOtingji{ 0 };
        int DOxiachaqigangwai1{ 0 };
        int DOxiachaqigangnei1{ 0 };
        int DOxiachaqigangwai2{ 0 };
        int DOxiachaqigangnei2{ 0 };
        int DOboliaoqigangwai1{ 0 };
        int DOboliaoqigangnei1{ 0 };
        int DOboliaoqigangwai2{ 0 };
        int DOboliaoqigangnei2{ 0 };
        bool debugMode{ false };
        bool istangshang{ false };
        bool isqueliao{ false };
        bool isyiwu{ false };
        bool iszangwu{ false };
        bool isextra2{ false };
        bool isextra3{ false };
        bool isextra4{ false };
        bool isextra5{ false };
        bool isextra6{ false };
        bool isextra7{ false };
        bool isextra8{ false };
        bool isextra9{ false };
    };

    inline SetConfig::SetConfig(const rw::oso::ObjectStoreAssembly& assembly)
    {
        auto isAccountAssembly = assembly.getName();
        if (isAccountAssembly != "$class$SetConfig$")
        {
            throw std::runtime_error("Assembly is not $class$SetConfig$");
        }
        auto zhongyangxianwei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhongyangxianwei1$"));
        if (!zhongyangxianwei1Item) {
            throw std::runtime_error("$variable$zhongyangxianwei1 is not found");
        }
        zhongyangxianwei1 = zhongyangxianwei1Item->getValueAsInt();
        auto zhongyangxianwei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhongyangxianwei2$"));
        if (!zhongyangxianwei2Item) {
            throw std::runtime_error("$variable$zhongyangxianwei2 is not found");
        }
        zhongyangxianwei2 = zhongyangxianwei2Item->getValueAsInt();
        auto qiyongerxiangjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$qiyongerxiangji$"));
        if (!qiyongerxiangjiItem) {
            throw std::runtime_error("$variable$qiyongerxiangji is not found");
        }
        qiyongerxiangji = qiyongerxiangjiItem->getValueAsBool();
        auto qiyongyundongkongzhiqiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$qiyongyundongkongzhiqi$"));
        if (!qiyongyundongkongzhiqiItem) {
            throw std::runtime_error("$variable$qiyongyundongkongzhiqi is not found");
        }
        qiyongyundongkongzhiqi = qiyongyundongkongzhiqiItem->getValueAsBool();
        auto yundongkongzhiqichonglianItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yundongkongzhiqichonglian$"));
        if (!yundongkongzhiqichonglianItem) {
            throw std::runtime_error("$variable$yundongkongzhiqichonglian is not found");
        }
        yundongkongzhiqichonglian = yundongkongzhiqichonglianItem->getValueAsBool();
        auto yanshixiachawai1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yanshixiachawai1$"));
        if (!yanshixiachawai1Item) {
            throw std::runtime_error("$variable$yanshixiachawai1 is not found");
        }
        yanshixiachawai1 = yanshixiachawai1Item->getValueAsDouble();
        auto boliaoshijianwai1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$boliaoshijianwai1$"));
        if (!boliaoshijianwai1Item) {
            throw std::runtime_error("$variable$boliaoshijianwai1 is not found");
        }
        boliaoshijianwai1 = boliaoshijianwai1Item->getValueAsDouble();
        auto yanshixiachawai2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yanshixiachawai2$"));
        if (!yanshixiachawai2Item) {
            throw std::runtime_error("$variable$yanshixiachawai2 is not found");
        }
        yanshixiachawai2 = yanshixiachawai2Item->getValueAsDouble();
        auto boliaoshijianwai2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$boliaoshijianwai2$"));
        if (!boliaoshijianwai2Item) {
            throw std::runtime_error("$variable$boliaoshijianwai2 is not found");
        }
        boliaoshijianwai2 = boliaoshijianwai2Item->getValueAsDouble();
        auto yanshixiachanei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yanshixiachanei1$"));
        if (!yanshixiachanei1Item) {
            throw std::runtime_error("$variable$yanshixiachanei1 is not found");
        }
        yanshixiachanei1 = yanshixiachanei1Item->getValueAsDouble();
        auto boliaoshijiannei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$boliaoshijiannei1$"));
        if (!boliaoshijiannei1Item) {
            throw std::runtime_error("$variable$boliaoshijiannei1 is not found");
        }
        boliaoshijiannei1 = boliaoshijiannei1Item->getValueAsDouble();
        auto yanshixiachanei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$yanshixiachanei2$"));
        if (!yanshixiachanei2Item) {
            throw std::runtime_error("$variable$yanshixiachanei2 is not found");
        }
        yanshixiachanei2 = yanshixiachanei2Item->getValueAsDouble();
        auto boliaoshijiannei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$boliaoshijiannei2$"));
        if (!boliaoshijiannei2Item) {
            throw std::runtime_error("$variable$boliaoshijiannei2 is not found");
        }
        boliaoshijiannei2 = boliaoshijiannei2Item->getValueAsDouble();
        auto xiangjichufachangduItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangjichufachangdu$"));
        if (!xiangjichufachangduItem) {
            throw std::runtime_error("$variable$xiangjichufachangdu is not found");
        }
        xiangjichufachangdu = xiangjichufachangduItem->getValueAsDouble();
        auto meizhuanmaichongshuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$meizhuanmaichongshu$"));
        if (!meizhuanmaichongshuItem) {
            throw std::runtime_error("$variable$meizhuanmaichongshu is not found");
        }
        meizhuanmaichongshu = meizhuanmaichongshuItem->getValueAsDouble();
        auto shedingzhouchangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shedingzhouchang$"));
        if (!shedingzhouchangItem) {
            throw std::runtime_error("$variable$shedingzhouchang is not found");
        }
        shedingzhouchang = shedingzhouchangItem->getValueAsDouble();
        auto waicetifeijuli1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$waicetifeijuli1$"));
        if (!waicetifeijuli1Item) {
            throw std::runtime_error("$variable$waicetifeijuli1 is not found");
        }
        waicetifeijuli1 = waicetifeijuli1Item->getValueAsDouble();
        auto neicetifeijuli1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$neicetifeijuli1$"));
        if (!neicetifeijuli1Item) {
            throw std::runtime_error("$variable$neicetifeijuli1 is not found");
        }
        neicetifeijuli1 = neicetifeijuli1Item->getValueAsDouble();
        auto tifeirongyufangdou1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeirongyufangdou1$"));
        if (!tifeirongyufangdou1Item) {
            throw std::runtime_error("$variable$tifeirongyufangdou1 is not found");
        }
        tifeirongyufangdou1 = tifeirongyufangdou1Item->getValueAsDouble();
        auto waicetifeijuli2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$waicetifeijuli2$"));
        if (!waicetifeijuli2Item) {
            throw std::runtime_error("$variable$waicetifeijuli2 is not found");
        }
        waicetifeijuli2 = waicetifeijuli2Item->getValueAsDouble();
        auto neicetifeijuli2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$neicetifeijuli2$"));
        if (!neicetifeijuli2Item) {
            throw std::runtime_error("$variable$neicetifeijuli2 is not found");
        }
        neicetifeijuli2 = neicetifeijuli2Item->getValueAsDouble();
        auto tifeirongyufangdou2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tifeirongyufangdou2$"));
        if (!tifeirongyufangdou2Item) {
            throw std::runtime_error("$variable$tifeirongyufangdou2 is not found");
        }
        tifeirongyufangdou2 = tifeirongyufangdou2Item->getValueAsDouble();
        auto tuxiangshunshizhenxuanzhuancishu1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tuxiangshunshizhenxuanzhuancishu1$"));
        if (!tuxiangshunshizhenxuanzhuancishu1Item) {
            throw std::runtime_error("$variable$tuxiangshunshizhenxuanzhuancishu1 is not found");
        }
        tuxiangshunshizhenxuanzhuancishu1 = tuxiangshunshizhenxuanzhuancishu1Item->getValueAsInt();
        auto tuxiangshunshizhenxuanzhuancishu2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$tuxiangshunshizhenxuanzhuancishu2$"));
        if (!tuxiangshunshizhenxuanzhuancishu2Item) {
            throw std::runtime_error("$variable$tuxiangshunshizhenxuanzhuancishu2 is not found");
        }
        tuxiangshunshizhenxuanzhuancishu2 = tuxiangshunshizhenxuanzhuancishu2Item->getValueAsInt();
        auto xunzhaoquexiansuoshubashoudepianchazhiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xunzhaoquexiansuoshubashoudepianchazhi$"));
        if (!xunzhaoquexiansuoshubashoudepianchazhiItem) {
            throw std::runtime_error("$variable$xunzhaoquexiansuoshubashoudepianchazhi is not found");
        }
        xunzhaoquexiansuoshubashoudepianchazhi = xunzhaoquexiansuoshubashoudepianchazhiItem->getValueAsDouble();
        auto shangXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shangXianWei1$"));
        if (!shangXianWei1Item) {
            throw std::runtime_error("$variable$shangXianWei1 is not found");
        }
        shangXianWei1 = shangXianWei1Item->getValueAsDouble();
        auto xiaXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaXianWei1$"));
        if (!xiaXianWei1Item) {
            throw std::runtime_error("$variable$xiaXianWei1 is not found");
        }
        xiaXianWei1 = xiaXianWei1Item->getValueAsDouble();
        auto zuoXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zuoXianWei1$"));
        if (!zuoXianWei1Item) {
            throw std::runtime_error("$variable$zuoXianWei1 is not found");
        }
        zuoXianWei1 = zuoXianWei1Item->getValueAsDouble();
        auto youXianWei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$youXianWei1$"));
        if (!youXianWei1Item) {
            throw std::runtime_error("$variable$youXianWei1 is not found");
        }
        youXianWei1 = youXianWei1Item->getValueAsDouble();
        auto xiangSuDangLiang1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangSuDangLiang1$"));
        if (!xiangSuDangLiang1Item) {
            throw std::runtime_error("$variable$xiangSuDangLiang1 is not found");
        }
        xiangSuDangLiang1 = xiangSuDangLiang1Item->getValueAsDouble();
        auto shangXianWei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$shangXianWei2$"));
        if (!shangXianWei2Item) {
            throw std::runtime_error("$variable$shangXianWei2 is not found");
        }
        shangXianWei2 = shangXianWei2Item->getValueAsDouble();
        auto xiaXianWei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiaXianWei2$"));
        if (!xiaXianWei2Item) {
            throw std::runtime_error("$variable$xiaXianWei2 is not found");
        }
        xiaXianWei2 = xiaXianWei2Item->getValueAsDouble();
        auto zuoXianWei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zuoXianWei2$"));
        if (!zuoXianWei2Item) {
            throw std::runtime_error("$variable$zuoXianWei2 is not found");
        }
        zuoXianWei2 = zuoXianWei2Item->getValueAsDouble();
        auto youXianWei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$youXianWei2$"));
        if (!youXianWei2Item) {
            throw std::runtime_error("$variable$youXianWei2 is not found");
        }
        youXianWei2 = youXianWei2Item->getValueAsDouble();
        auto xiangSuDangLiang2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$xiangSuDangLiang2$"));
        if (!xiangSuDangLiang2Item) {
            throw std::runtime_error("$variable$xiangSuDangLiang2 is not found");
        }
        xiangSuDangLiang2 = xiangSuDangLiang2Item->getValueAsDouble();
        auto qiangbaoguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$qiangbaoguang$"));
        if (!qiangbaoguangItem) {
            throw std::runtime_error("$variable$qiangbaoguang is not found");
        }
        qiangbaoguang = qiangbaoguangItem->getValueAsDouble();
        auto qiangzengyiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$qiangzengyi$"));
        if (!qiangzengyiItem) {
            throw std::runtime_error("$variable$qiangzengyi is not found");
        }
        qiangzengyi = qiangzengyiItem->getValueAsDouble();
        auto zhongbaoguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhongbaoguang$"));
        if (!zhongbaoguangItem) {
            throw std::runtime_error("$variable$zhongbaoguang is not found");
        }
        zhongbaoguang = zhongbaoguangItem->getValueAsDouble();
        auto zhongzengyiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$zhongzengyi$"));
        if (!zhongzengyiItem) {
            throw std::runtime_error("$variable$zhongzengyi is not found");
        }
        zhongzengyi = zhongzengyiItem->getValueAsDouble();
        auto ruobaoguangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$ruobaoguang$"));
        if (!ruobaoguangItem) {
            throw std::runtime_error("$variable$ruobaoguang is not found");
        }
        ruobaoguang = ruobaoguangItem->getValueAsDouble();
        auto ruozengyiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$ruozengyi$"));
        if (!ruozengyiItem) {
            throw std::runtime_error("$variable$ruozengyi is not found");
        }
        ruozengyi = ruozengyiItem->getValueAsDouble();
        auto takeWork1PicturesItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$takeWork1Pictures$"));
        if (!takeWork1PicturesItem) {
            throw std::runtime_error("$variable$takeWork1Pictures is not found");
        }
        takeWork1Pictures = takeWork1PicturesItem->getValueAsBool();
        auto takeWork2PicturesItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$takeWork2Pictures$"));
        if (!takeWork2PicturesItem) {
            throw std::runtime_error("$variable$takeWork2Pictures is not found");
        }
        takeWork2Pictures = takeWork2PicturesItem->getValueAsBool();
        auto saveNGImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveNGImg$"));
        if (!saveNGImgItem) {
            throw std::runtime_error("$variable$saveNGImg is not found");
        }
        saveNGImg = saveNGImgItem->getValueAsBool();
        auto saveMaskImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveMaskImg$"));
        if (!saveMaskImgItem) {
            throw std::runtime_error("$variable$saveMaskImg is not found");
        }
        saveMaskImg = saveMaskImgItem->getValueAsBool();
        auto saveOKImgItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveOKImg$"));
        if (!saveOKImgItem) {
            throw std::runtime_error("$variable$saveOKImg is not found");
        }
        saveOKImg = saveOKImgItem->getValueAsBool();
        auto isSaveJPEGItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isSaveJPEG$"));
        if (!isSaveJPEGItem) {
            throw std::runtime_error("$variable$isSaveJPEG is not found");
        }
        isSaveJPEG = isSaveJPEGItem->getValueAsBool();
        auto isSaveBMPItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isSaveBMP$"));
        if (!isSaveBMPItem) {
            throw std::runtime_error("$variable$isSaveBMP is not found");
        }
        isSaveBMP = isSaveBMPItem->getValueAsBool();
        auto isSavePNGItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isSavePNG$"));
        if (!isSavePNGItem) {
            throw std::runtime_error("$variable$isSavePNG is not found");
        }
        isSavePNG = isSavePNGItem->getValueAsBool();
        auto saveImgQualityItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$saveImgQuality$"));
        if (!saveImgQualityItem) {
            throw std::runtime_error("$variable$saveImgQuality is not found");
        }
        saveImgQuality = saveImgQualityItem->getValueAsInt();
        auto DOpaizhao1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOpaizhao1$"));
        if (!DOpaizhao1Item) {
            throw std::runtime_error("$variable$DOpaizhao1 is not found");
        }
        DOpaizhao1 = DOpaizhao1Item->getValueAsInt();
        auto DOpaizhao2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOpaizhao2$"));
        if (!DOpaizhao2Item) {
            throw std::runtime_error("$variable$DOpaizhao2 is not found");
        }
        DOpaizhao2 = DOpaizhao2Item->getValueAsInt();
        auto DOguangyuanItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOguangyuan$"));
        if (!DOguangyuanItem) {
            throw std::runtime_error("$variable$DOguangyuan is not found");
        }
        DOguangyuan = DOguangyuanItem->getValueAsInt();
        auto DObaojingItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DObaojing$"));
        if (!DObaojingItem) {
            throw std::runtime_error("$variable$DObaojing is not found");
        }
        DObaojing = DObaojingItem->getValueAsInt();
        auto DOtingjiItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOtingji$"));
        if (!DOtingjiItem) {
            throw std::runtime_error("$variable$DOtingji is not found");
        }
        DOtingji = DOtingjiItem->getValueAsInt();
        auto DOxiachaqigangwai1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOxiachaqigangwai1$"));
        if (!DOxiachaqigangwai1Item) {
            throw std::runtime_error("$variable$DOxiachaqigangwai1 is not found");
        }
        DOxiachaqigangwai1 = DOxiachaqigangwai1Item->getValueAsInt();
        auto DOxiachaqigangnei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOxiachaqigangnei1$"));
        if (!DOxiachaqigangnei1Item) {
            throw std::runtime_error("$variable$DOxiachaqigangnei1 is not found");
        }
        DOxiachaqigangnei1 = DOxiachaqigangnei1Item->getValueAsInt();
        auto DOxiachaqigangwai2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOxiachaqigangwai2$"));
        if (!DOxiachaqigangwai2Item) {
            throw std::runtime_error("$variable$DOxiachaqigangwai2 is not found");
        }
        DOxiachaqigangwai2 = DOxiachaqigangwai2Item->getValueAsInt();
        auto DOxiachaqigangnei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOxiachaqigangnei2$"));
        if (!DOxiachaqigangnei2Item) {
            throw std::runtime_error("$variable$DOxiachaqigangnei2 is not found");
        }
        DOxiachaqigangnei2 = DOxiachaqigangnei2Item->getValueAsInt();
        auto DOboliaoqigangwai1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOboliaoqigangwai1$"));
        if (!DOboliaoqigangwai1Item) {
            throw std::runtime_error("$variable$DOboliaoqigangwai1 is not found");
        }
        DOboliaoqigangwai1 = DOboliaoqigangwai1Item->getValueAsInt();
        auto DOboliaoqigangnei1Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOboliaoqigangnei1$"));
        if (!DOboliaoqigangnei1Item) {
            throw std::runtime_error("$variable$DOboliaoqigangnei1 is not found");
        }
        DOboliaoqigangnei1 = DOboliaoqigangnei1Item->getValueAsInt();
        auto DOboliaoqigangwai2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOboliaoqigangwai2$"));
        if (!DOboliaoqigangwai2Item) {
            throw std::runtime_error("$variable$DOboliaoqigangwai2 is not found");
        }
        DOboliaoqigangwai2 = DOboliaoqigangwai2Item->getValueAsInt();
        auto DOboliaoqigangnei2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$DOboliaoqigangnei2$"));
        if (!DOboliaoqigangnei2Item) {
            throw std::runtime_error("$variable$DOboliaoqigangnei2 is not found");
        }
        DOboliaoqigangnei2 = DOboliaoqigangnei2Item->getValueAsInt();
        auto debugModeItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$debugMode$"));
        if (!debugModeItem) {
            throw std::runtime_error("$variable$debugMode is not found");
        }
        debugMode = debugModeItem->getValueAsBool();
        auto istangshangItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$istangshang$"));
        if (!istangshangItem) {
            throw std::runtime_error("$variable$istangshang is not found");
        }
        istangshang = istangshangItem->getValueAsBool();
        auto isqueliaoItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isqueliao$"));
        if (!isqueliaoItem) {
            throw std::runtime_error("$variable$isqueliao is not found");
        }
        isqueliao = isqueliaoItem->getValueAsBool();
        auto isyiwuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isyiwu$"));
        if (!isyiwuItem) {
            throw std::runtime_error("$variable$isyiwu is not found");
        }
        isyiwu = isyiwuItem->getValueAsBool();
        auto iszangwuItem = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$iszangwu$"));
        if (!iszangwuItem) {
            throw std::runtime_error("$variable$iszangwu is not found");
        }
        iszangwu = iszangwuItem->getValueAsBool();
        auto isextra2Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra2$"));
        if (!isextra2Item) {
            throw std::runtime_error("$variable$isextra2 is not found");
        }
        isextra2 = isextra2Item->getValueAsBool();
        auto isextra3Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra3$"));
        if (!isextra3Item) {
            throw std::runtime_error("$variable$isextra3 is not found");
        }
        isextra3 = isextra3Item->getValueAsBool();
        auto isextra4Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra4$"));
        if (!isextra4Item) {
            throw std::runtime_error("$variable$isextra4 is not found");
        }
        isextra4 = isextra4Item->getValueAsBool();
        auto isextra5Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra5$"));
        if (!isextra5Item) {
            throw std::runtime_error("$variable$isextra5 is not found");
        }
        isextra5 = isextra5Item->getValueAsBool();
        auto isextra6Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra6$"));
        if (!isextra6Item) {
            throw std::runtime_error("$variable$isextra6 is not found");
        }
        isextra6 = isextra6Item->getValueAsBool();
        auto isextra7Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra7$"));
        if (!isextra7Item) {
            throw std::runtime_error("$variable$isextra7 is not found");
        }
        isextra7 = isextra7Item->getValueAsBool();
        auto isextra8Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra8$"));
        if (!isextra8Item) {
            throw std::runtime_error("$variable$isextra8 is not found");
        }
        isextra8 = isextra8Item->getValueAsBool();
        auto isextra9Item = rw::oso::ObjectStoreCoreToItem(assembly.getItem("$variable$isextra9$"));
        if (!isextra9Item) {
            throw std::runtime_error("$variable$isextra9 is not found");
        }
        isextra9 = isextra9Item->getValueAsBool();
    }

    inline SetConfig::SetConfig(const SetConfig& obj)
    {
        zhongyangxianwei1 = obj.zhongyangxianwei1;
        zhongyangxianwei2 = obj.zhongyangxianwei2;
        qiyongerxiangji = obj.qiyongerxiangji;
        qiyongyundongkongzhiqi = obj.qiyongyundongkongzhiqi;
        yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
        yanshixiachawai1 = obj.yanshixiachawai1;
        boliaoshijianwai1 = obj.boliaoshijianwai1;
        yanshixiachawai2 = obj.yanshixiachawai2;
        boliaoshijianwai2 = obj.boliaoshijianwai2;
        yanshixiachanei1 = obj.yanshixiachanei1;
        boliaoshijiannei1 = obj.boliaoshijiannei1;
        yanshixiachanei2 = obj.yanshixiachanei2;
        boliaoshijiannei2 = obj.boliaoshijiannei2;
        xiangjichufachangdu = obj.xiangjichufachangdu;
        meizhuanmaichongshu = obj.meizhuanmaichongshu;
        shedingzhouchang = obj.shedingzhouchang;
        waicetifeijuli1 = obj.waicetifeijuli1;
        neicetifeijuli1 = obj.neicetifeijuli1;
        tifeirongyufangdou1 = obj.tifeirongyufangdou1;
        waicetifeijuli2 = obj.waicetifeijuli2;
        neicetifeijuli2 = obj.neicetifeijuli2;
        tifeirongyufangdou2 = obj.tifeirongyufangdou2;
        tuxiangshunshizhenxuanzhuancishu1 = obj.tuxiangshunshizhenxuanzhuancishu1;
        tuxiangshunshizhenxuanzhuancishu2 = obj.tuxiangshunshizhenxuanzhuancishu2;
        xunzhaoquexiansuoshubashoudepianchazhi = obj.xunzhaoquexiansuoshubashoudepianchazhi;
        shangXianWei1 = obj.shangXianWei1;
        xiaXianWei1 = obj.xiaXianWei1;
        zuoXianWei1 = obj.zuoXianWei1;
        youXianWei1 = obj.youXianWei1;
        xiangSuDangLiang1 = obj.xiangSuDangLiang1;
        shangXianWei2 = obj.shangXianWei2;
        xiaXianWei2 = obj.xiaXianWei2;
        zuoXianWei2 = obj.zuoXianWei2;
        youXianWei2 = obj.youXianWei2;
        xiangSuDangLiang2 = obj.xiangSuDangLiang2;
        qiangbaoguang = obj.qiangbaoguang;
        qiangzengyi = obj.qiangzengyi;
        zhongbaoguang = obj.zhongbaoguang;
        zhongzengyi = obj.zhongzengyi;
        ruobaoguang = obj.ruobaoguang;
        ruozengyi = obj.ruozengyi;
        takeWork1Pictures = obj.takeWork1Pictures;
        takeWork2Pictures = obj.takeWork2Pictures;
        saveNGImg = obj.saveNGImg;
        saveMaskImg = obj.saveMaskImg;
        saveOKImg = obj.saveOKImg;
        isSaveJPEG = obj.isSaveJPEG;
        isSaveBMP = obj.isSaveBMP;
        isSavePNG = obj.isSavePNG;
        saveImgQuality = obj.saveImgQuality;
        DOpaizhao1 = obj.DOpaizhao1;
        DOpaizhao2 = obj.DOpaizhao2;
        DOguangyuan = obj.DOguangyuan;
        DObaojing = obj.DObaojing;
        DOtingji = obj.DOtingji;
        DOxiachaqigangwai1 = obj.DOxiachaqigangwai1;
        DOxiachaqigangnei1 = obj.DOxiachaqigangnei1;
        DOxiachaqigangwai2 = obj.DOxiachaqigangwai2;
        DOxiachaqigangnei2 = obj.DOxiachaqigangnei2;
        DOboliaoqigangwai1 = obj.DOboliaoqigangwai1;
        DOboliaoqigangnei1 = obj.DOboliaoqigangnei1;
        DOboliaoqigangwai2 = obj.DOboliaoqigangwai2;
        DOboliaoqigangnei2 = obj.DOboliaoqigangnei2;
        debugMode = obj.debugMode;
        istangshang = obj.istangshang;
        isqueliao = obj.isqueliao;
        isyiwu = obj.isyiwu;
        iszangwu = obj.iszangwu;
        isextra2 = obj.isextra2;
        isextra3 = obj.isextra3;
        isextra4 = obj.isextra4;
        isextra5 = obj.isextra5;
        isextra6 = obj.isextra6;
        isextra7 = obj.isextra7;
        isextra8 = obj.isextra8;
        isextra9 = obj.isextra9;
    }

    inline SetConfig& SetConfig::operator=(const SetConfig& obj)
    {
        if (this != &obj) {
            zhongyangxianwei1 = obj.zhongyangxianwei1;
            zhongyangxianwei2 = obj.zhongyangxianwei2;
            qiyongerxiangji = obj.qiyongerxiangji;
            qiyongyundongkongzhiqi = obj.qiyongyundongkongzhiqi;
            yundongkongzhiqichonglian = obj.yundongkongzhiqichonglian;
            yanshixiachawai1 = obj.yanshixiachawai1;
            boliaoshijianwai1 = obj.boliaoshijianwai1;
            yanshixiachawai2 = obj.yanshixiachawai2;
            boliaoshijianwai2 = obj.boliaoshijianwai2;
            yanshixiachanei1 = obj.yanshixiachanei1;
            boliaoshijiannei1 = obj.boliaoshijiannei1;
            yanshixiachanei2 = obj.yanshixiachanei2;
            boliaoshijiannei2 = obj.boliaoshijiannei2;
            xiangjichufachangdu = obj.xiangjichufachangdu;
            meizhuanmaichongshu = obj.meizhuanmaichongshu;
            shedingzhouchang = obj.shedingzhouchang;
            waicetifeijuli1 = obj.waicetifeijuli1;
            neicetifeijuli1 = obj.neicetifeijuli1;
            tifeirongyufangdou1 = obj.tifeirongyufangdou1;
            waicetifeijuli2 = obj.waicetifeijuli2;
            neicetifeijuli2 = obj.neicetifeijuli2;
            tifeirongyufangdou2 = obj.tifeirongyufangdou2;
            tuxiangshunshizhenxuanzhuancishu1 = obj.tuxiangshunshizhenxuanzhuancishu1;
            tuxiangshunshizhenxuanzhuancishu2 = obj.tuxiangshunshizhenxuanzhuancishu2;
            xunzhaoquexiansuoshubashoudepianchazhi = obj.xunzhaoquexiansuoshubashoudepianchazhi;
            shangXianWei1 = obj.shangXianWei1;
            xiaXianWei1 = obj.xiaXianWei1;
            zuoXianWei1 = obj.zuoXianWei1;
            youXianWei1 = obj.youXianWei1;
            xiangSuDangLiang1 = obj.xiangSuDangLiang1;
            shangXianWei2 = obj.shangXianWei2;
            xiaXianWei2 = obj.xiaXianWei2;
            zuoXianWei2 = obj.zuoXianWei2;
            youXianWei2 = obj.youXianWei2;
            xiangSuDangLiang2 = obj.xiangSuDangLiang2;
            qiangbaoguang = obj.qiangbaoguang;
            qiangzengyi = obj.qiangzengyi;
            zhongbaoguang = obj.zhongbaoguang;
            zhongzengyi = obj.zhongzengyi;
            ruobaoguang = obj.ruobaoguang;
            ruozengyi = obj.ruozengyi;
            takeWork1Pictures = obj.takeWork1Pictures;
            takeWork2Pictures = obj.takeWork2Pictures;
            saveNGImg = obj.saveNGImg;
            saveMaskImg = obj.saveMaskImg;
            saveOKImg = obj.saveOKImg;
            isSaveJPEG = obj.isSaveJPEG;
            isSaveBMP = obj.isSaveBMP;
            isSavePNG = obj.isSavePNG;
            saveImgQuality = obj.saveImgQuality;
            DOpaizhao1 = obj.DOpaizhao1;
            DOpaizhao2 = obj.DOpaizhao2;
            DOguangyuan = obj.DOguangyuan;
            DObaojing = obj.DObaojing;
            DOtingji = obj.DOtingji;
            DOxiachaqigangwai1 = obj.DOxiachaqigangwai1;
            DOxiachaqigangnei1 = obj.DOxiachaqigangnei1;
            DOxiachaqigangwai2 = obj.DOxiachaqigangwai2;
            DOxiachaqigangnei2 = obj.DOxiachaqigangnei2;
            DOboliaoqigangwai1 = obj.DOboliaoqigangwai1;
            DOboliaoqigangnei1 = obj.DOboliaoqigangnei1;
            DOboliaoqigangwai2 = obj.DOboliaoqigangwai2;
            DOboliaoqigangnei2 = obj.DOboliaoqigangnei2;
            debugMode = obj.debugMode;
            istangshang = obj.istangshang;
            isqueliao = obj.isqueliao;
            isyiwu = obj.isyiwu;
            iszangwu = obj.iszangwu;
            isextra2 = obj.isextra2;
            isextra3 = obj.isextra3;
            isextra4 = obj.isextra4;
            isextra5 = obj.isextra5;
            isextra6 = obj.isextra6;
            isextra7 = obj.isextra7;
            isextra8 = obj.isextra8;
            isextra9 = obj.isextra9;
        }
        return *this;
    }

    inline SetConfig::operator rw::oso::ObjectStoreAssembly() const
    {
        rw::oso::ObjectStoreAssembly assembly;
        assembly.setName("$class$SetConfig$");
        auto zhongyangxianwei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        zhongyangxianwei1Item->setName("$variable$zhongyangxianwei1$");
        zhongyangxianwei1Item->setValueFromInt(zhongyangxianwei1);
        assembly.addItem(zhongyangxianwei1Item);
        auto zhongyangxianwei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        zhongyangxianwei2Item->setName("$variable$zhongyangxianwei2$");
        zhongyangxianwei2Item->setValueFromInt(zhongyangxianwei2);
        assembly.addItem(zhongyangxianwei2Item);
        auto qiyongerxiangjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        qiyongerxiangjiItem->setName("$variable$qiyongerxiangji$");
        qiyongerxiangjiItem->setValueFromBool(qiyongerxiangji);
        assembly.addItem(qiyongerxiangjiItem);
        auto qiyongyundongkongzhiqiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        qiyongyundongkongzhiqiItem->setName("$variable$qiyongyundongkongzhiqi$");
        qiyongyundongkongzhiqiItem->setValueFromBool(qiyongyundongkongzhiqi);
        assembly.addItem(qiyongyundongkongzhiqiItem);
        auto yundongkongzhiqichonglianItem = std::make_shared<rw::oso::ObjectStoreItem>();
        yundongkongzhiqichonglianItem->setName("$variable$yundongkongzhiqichonglian$");
        yundongkongzhiqichonglianItem->setValueFromBool(yundongkongzhiqichonglian);
        assembly.addItem(yundongkongzhiqichonglianItem);
        auto yanshixiachawai1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        yanshixiachawai1Item->setName("$variable$yanshixiachawai1$");
        yanshixiachawai1Item->setValueFromDouble(yanshixiachawai1);
        assembly.addItem(yanshixiachawai1Item);
        auto boliaoshijianwai1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        boliaoshijianwai1Item->setName("$variable$boliaoshijianwai1$");
        boliaoshijianwai1Item->setValueFromDouble(boliaoshijianwai1);
        assembly.addItem(boliaoshijianwai1Item);
        auto yanshixiachawai2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        yanshixiachawai2Item->setName("$variable$yanshixiachawai2$");
        yanshixiachawai2Item->setValueFromDouble(yanshixiachawai2);
        assembly.addItem(yanshixiachawai2Item);
        auto boliaoshijianwai2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        boliaoshijianwai2Item->setName("$variable$boliaoshijianwai2$");
        boliaoshijianwai2Item->setValueFromDouble(boliaoshijianwai2);
        assembly.addItem(boliaoshijianwai2Item);
        auto yanshixiachanei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        yanshixiachanei1Item->setName("$variable$yanshixiachanei1$");
        yanshixiachanei1Item->setValueFromDouble(yanshixiachanei1);
        assembly.addItem(yanshixiachanei1Item);
        auto boliaoshijiannei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        boliaoshijiannei1Item->setName("$variable$boliaoshijiannei1$");
        boliaoshijiannei1Item->setValueFromDouble(boliaoshijiannei1);
        assembly.addItem(boliaoshijiannei1Item);
        auto yanshixiachanei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        yanshixiachanei2Item->setName("$variable$yanshixiachanei2$");
        yanshixiachanei2Item->setValueFromDouble(yanshixiachanei2);
        assembly.addItem(yanshixiachanei2Item);
        auto boliaoshijiannei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        boliaoshijiannei2Item->setName("$variable$boliaoshijiannei2$");
        boliaoshijiannei2Item->setValueFromDouble(boliaoshijiannei2);
        assembly.addItem(boliaoshijiannei2Item);
        auto xiangjichufachangduItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangjichufachangduItem->setName("$variable$xiangjichufachangdu$");
        xiangjichufachangduItem->setValueFromDouble(xiangjichufachangdu);
        assembly.addItem(xiangjichufachangduItem);
        auto meizhuanmaichongshuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        meizhuanmaichongshuItem->setName("$variable$meizhuanmaichongshu$");
        meizhuanmaichongshuItem->setValueFromDouble(meizhuanmaichongshu);
        assembly.addItem(meizhuanmaichongshuItem);
        auto shedingzhouchangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        shedingzhouchangItem->setName("$variable$shedingzhouchang$");
        shedingzhouchangItem->setValueFromDouble(shedingzhouchang);
        assembly.addItem(shedingzhouchangItem);
        auto waicetifeijuli1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        waicetifeijuli1Item->setName("$variable$waicetifeijuli1$");
        waicetifeijuli1Item->setValueFromDouble(waicetifeijuli1);
        assembly.addItem(waicetifeijuli1Item);
        auto neicetifeijuli1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        neicetifeijuli1Item->setName("$variable$neicetifeijuli1$");
        neicetifeijuli1Item->setValueFromDouble(neicetifeijuli1);
        assembly.addItem(neicetifeijuli1Item);
        auto tifeirongyufangdou1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeirongyufangdou1Item->setName("$variable$tifeirongyufangdou1$");
        tifeirongyufangdou1Item->setValueFromDouble(tifeirongyufangdou1);
        assembly.addItem(tifeirongyufangdou1Item);
        auto waicetifeijuli2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        waicetifeijuli2Item->setName("$variable$waicetifeijuli2$");
        waicetifeijuli2Item->setValueFromDouble(waicetifeijuli2);
        assembly.addItem(waicetifeijuli2Item);
        auto neicetifeijuli2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        neicetifeijuli2Item->setName("$variable$neicetifeijuli2$");
        neicetifeijuli2Item->setValueFromDouble(neicetifeijuli2);
        assembly.addItem(neicetifeijuli2Item);
        auto tifeirongyufangdou2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        tifeirongyufangdou2Item->setName("$variable$tifeirongyufangdou2$");
        tifeirongyufangdou2Item->setValueFromDouble(tifeirongyufangdou2);
        assembly.addItem(tifeirongyufangdou2Item);
        auto tuxiangshunshizhenxuanzhuancishu1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        tuxiangshunshizhenxuanzhuancishu1Item->setName("$variable$tuxiangshunshizhenxuanzhuancishu1$");
        tuxiangshunshizhenxuanzhuancishu1Item->setValueFromInt(tuxiangshunshizhenxuanzhuancishu1);
        assembly.addItem(tuxiangshunshizhenxuanzhuancishu1Item);
        auto tuxiangshunshizhenxuanzhuancishu2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        tuxiangshunshizhenxuanzhuancishu2Item->setName("$variable$tuxiangshunshizhenxuanzhuancishu2$");
        tuxiangshunshizhenxuanzhuancishu2Item->setValueFromInt(tuxiangshunshizhenxuanzhuancishu2);
        assembly.addItem(tuxiangshunshizhenxuanzhuancishu2Item);
        auto xunzhaoquexiansuoshubashoudepianchazhiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        xunzhaoquexiansuoshubashoudepianchazhiItem->setName("$variable$xunzhaoquexiansuoshubashoudepianchazhi$");
        xunzhaoquexiansuoshubashoudepianchazhiItem->setValueFromDouble(xunzhaoquexiansuoshubashoudepianchazhi);
        assembly.addItem(xunzhaoquexiansuoshubashoudepianchazhiItem);
        auto shangXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        shangXianWei1Item->setName("$variable$shangXianWei1$");
        shangXianWei1Item->setValueFromDouble(shangXianWei1);
        assembly.addItem(shangXianWei1Item);
        auto xiaXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaXianWei1Item->setName("$variable$xiaXianWei1$");
        xiaXianWei1Item->setValueFromDouble(xiaXianWei1);
        assembly.addItem(xiaXianWei1Item);
        auto zuoXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        zuoXianWei1Item->setName("$variable$zuoXianWei1$");
        zuoXianWei1Item->setValueFromDouble(zuoXianWei1);
        assembly.addItem(zuoXianWei1Item);
        auto youXianWei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        youXianWei1Item->setName("$variable$youXianWei1$");
        youXianWei1Item->setValueFromDouble(youXianWei1);
        assembly.addItem(youXianWei1Item);
        auto xiangSuDangLiang1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangSuDangLiang1Item->setName("$variable$xiangSuDangLiang1$");
        xiangSuDangLiang1Item->setValueFromDouble(xiangSuDangLiang1);
        assembly.addItem(xiangSuDangLiang1Item);
        auto shangXianWei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        shangXianWei2Item->setName("$variable$shangXianWei2$");
        shangXianWei2Item->setValueFromDouble(shangXianWei2);
        assembly.addItem(shangXianWei2Item);
        auto xiaXianWei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        xiaXianWei2Item->setName("$variable$xiaXianWei2$");
        xiaXianWei2Item->setValueFromDouble(xiaXianWei2);
        assembly.addItem(xiaXianWei2Item);
        auto zuoXianWei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        zuoXianWei2Item->setName("$variable$zuoXianWei2$");
        zuoXianWei2Item->setValueFromDouble(zuoXianWei2);
        assembly.addItem(zuoXianWei2Item);
        auto youXianWei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        youXianWei2Item->setName("$variable$youXianWei2$");
        youXianWei2Item->setValueFromDouble(youXianWei2);
        assembly.addItem(youXianWei2Item);
        auto xiangSuDangLiang2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        xiangSuDangLiang2Item->setName("$variable$xiangSuDangLiang2$");
        xiangSuDangLiang2Item->setValueFromDouble(xiangSuDangLiang2);
        assembly.addItem(xiangSuDangLiang2Item);
        auto qiangbaoguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        qiangbaoguangItem->setName("$variable$qiangbaoguang$");
        qiangbaoguangItem->setValueFromDouble(qiangbaoguang);
        assembly.addItem(qiangbaoguangItem);
        auto qiangzengyiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        qiangzengyiItem->setName("$variable$qiangzengyi$");
        qiangzengyiItem->setValueFromDouble(qiangzengyi);
        assembly.addItem(qiangzengyiItem);
        auto zhongbaoguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zhongbaoguangItem->setName("$variable$zhongbaoguang$");
        zhongbaoguangItem->setValueFromDouble(zhongbaoguang);
        assembly.addItem(zhongbaoguangItem);
        auto zhongzengyiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        zhongzengyiItem->setName("$variable$zhongzengyi$");
        zhongzengyiItem->setValueFromDouble(zhongzengyi);
        assembly.addItem(zhongzengyiItem);
        auto ruobaoguangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        ruobaoguangItem->setName("$variable$ruobaoguang$");
        ruobaoguangItem->setValueFromDouble(ruobaoguang);
        assembly.addItem(ruobaoguangItem);
        auto ruozengyiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        ruozengyiItem->setName("$variable$ruozengyi$");
        ruozengyiItem->setValueFromDouble(ruozengyi);
        assembly.addItem(ruozengyiItem);
        auto takeWork1PicturesItem = std::make_shared<rw::oso::ObjectStoreItem>();
        takeWork1PicturesItem->setName("$variable$takeWork1Pictures$");
        takeWork1PicturesItem->setValueFromBool(takeWork1Pictures);
        assembly.addItem(takeWork1PicturesItem);
        auto takeWork2PicturesItem = std::make_shared<rw::oso::ObjectStoreItem>();
        takeWork2PicturesItem->setName("$variable$takeWork2Pictures$");
        takeWork2PicturesItem->setValueFromBool(takeWork2Pictures);
        assembly.addItem(takeWork2PicturesItem);
        auto saveNGImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveNGImgItem->setName("$variable$saveNGImg$");
        saveNGImgItem->setValueFromBool(saveNGImg);
        assembly.addItem(saveNGImgItem);
        auto saveMaskImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveMaskImgItem->setName("$variable$saveMaskImg$");
        saveMaskImgItem->setValueFromBool(saveMaskImg);
        assembly.addItem(saveMaskImgItem);
        auto saveOKImgItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveOKImgItem->setName("$variable$saveOKImg$");
        saveOKImgItem->setValueFromBool(saveOKImg);
        assembly.addItem(saveOKImgItem);
        auto isSaveJPEGItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isSaveJPEGItem->setName("$variable$isSaveJPEG$");
        isSaveJPEGItem->setValueFromBool(isSaveJPEG);
        assembly.addItem(isSaveJPEGItem);
        auto isSaveBMPItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isSaveBMPItem->setName("$variable$isSaveBMP$");
        isSaveBMPItem->setValueFromBool(isSaveBMP);
        assembly.addItem(isSaveBMPItem);
        auto isSavePNGItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isSavePNGItem->setName("$variable$isSavePNG$");
        isSavePNGItem->setValueFromBool(isSavePNG);
        assembly.addItem(isSavePNGItem);
        auto saveImgQualityItem = std::make_shared<rw::oso::ObjectStoreItem>();
        saveImgQualityItem->setName("$variable$saveImgQuality$");
        saveImgQualityItem->setValueFromInt(saveImgQuality);
        assembly.addItem(saveImgQualityItem);
        auto DOpaizhao1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOpaizhao1Item->setName("$variable$DOpaizhao1$");
        DOpaizhao1Item->setValueFromInt(DOpaizhao1);
        assembly.addItem(DOpaizhao1Item);
        auto DOpaizhao2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOpaizhao2Item->setName("$variable$DOpaizhao2$");
        DOpaizhao2Item->setValueFromInt(DOpaizhao2);
        assembly.addItem(DOpaizhao2Item);
        auto DOguangyuanItem = std::make_shared<rw::oso::ObjectStoreItem>();
        DOguangyuanItem->setName("$variable$DOguangyuan$");
        DOguangyuanItem->setValueFromInt(DOguangyuan);
        assembly.addItem(DOguangyuanItem);
        auto DObaojingItem = std::make_shared<rw::oso::ObjectStoreItem>();
        DObaojingItem->setName("$variable$DObaojing$");
        DObaojingItem->setValueFromInt(DObaojing);
        assembly.addItem(DObaojingItem);
        auto DOtingjiItem = std::make_shared<rw::oso::ObjectStoreItem>();
        DOtingjiItem->setName("$variable$DOtingji$");
        DOtingjiItem->setValueFromInt(DOtingji);
        assembly.addItem(DOtingjiItem);
        auto DOxiachaqigangwai1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOxiachaqigangwai1Item->setName("$variable$DOxiachaqigangwai1$");
        DOxiachaqigangwai1Item->setValueFromInt(DOxiachaqigangwai1);
        assembly.addItem(DOxiachaqigangwai1Item);
        auto DOxiachaqigangnei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOxiachaqigangnei1Item->setName("$variable$DOxiachaqigangnei1$");
        DOxiachaqigangnei1Item->setValueFromInt(DOxiachaqigangnei1);
        assembly.addItem(DOxiachaqigangnei1Item);
        auto DOxiachaqigangwai2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOxiachaqigangwai2Item->setName("$variable$DOxiachaqigangwai2$");
        DOxiachaqigangwai2Item->setValueFromInt(DOxiachaqigangwai2);
        assembly.addItem(DOxiachaqigangwai2Item);
        auto DOxiachaqigangnei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOxiachaqigangnei2Item->setName("$variable$DOxiachaqigangnei2$");
        DOxiachaqigangnei2Item->setValueFromInt(DOxiachaqigangnei2);
        assembly.addItem(DOxiachaqigangnei2Item);
        auto DOboliaoqigangwai1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOboliaoqigangwai1Item->setName("$variable$DOboliaoqigangwai1$");
        DOboliaoqigangwai1Item->setValueFromInt(DOboliaoqigangwai1);
        assembly.addItem(DOboliaoqigangwai1Item);
        auto DOboliaoqigangnei1Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOboliaoqigangnei1Item->setName("$variable$DOboliaoqigangnei1$");
        DOboliaoqigangnei1Item->setValueFromInt(DOboliaoqigangnei1);
        assembly.addItem(DOboliaoqigangnei1Item);
        auto DOboliaoqigangwai2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOboliaoqigangwai2Item->setName("$variable$DOboliaoqigangwai2$");
        DOboliaoqigangwai2Item->setValueFromInt(DOboliaoqigangwai2);
        assembly.addItem(DOboliaoqigangwai2Item);
        auto DOboliaoqigangnei2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        DOboliaoqigangnei2Item->setName("$variable$DOboliaoqigangnei2$");
        DOboliaoqigangnei2Item->setValueFromInt(DOboliaoqigangnei2);
        assembly.addItem(DOboliaoqigangnei2Item);
        auto debugModeItem = std::make_shared<rw::oso::ObjectStoreItem>();
        debugModeItem->setName("$variable$debugMode$");
        debugModeItem->setValueFromBool(debugMode);
        assembly.addItem(debugModeItem);
        auto istangshangItem = std::make_shared<rw::oso::ObjectStoreItem>();
        istangshangItem->setName("$variable$istangshang$");
        istangshangItem->setValueFromBool(istangshang);
        assembly.addItem(istangshangItem);
        auto isqueliaoItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isqueliaoItem->setName("$variable$isqueliao$");
        isqueliaoItem->setValueFromBool(isqueliao);
        assembly.addItem(isqueliaoItem);
        auto isyiwuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        isyiwuItem->setName("$variable$isyiwu$");
        isyiwuItem->setValueFromBool(isyiwu);
        assembly.addItem(isyiwuItem);
        auto iszangwuItem = std::make_shared<rw::oso::ObjectStoreItem>();
        iszangwuItem->setName("$variable$iszangwu$");
        iszangwuItem->setValueFromBool(iszangwu);
        assembly.addItem(iszangwuItem);
        auto isextra2Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra2Item->setName("$variable$isextra2$");
        isextra2Item->setValueFromBool(isextra2);
        assembly.addItem(isextra2Item);
        auto isextra3Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra3Item->setName("$variable$isextra3$");
        isextra3Item->setValueFromBool(isextra3);
        assembly.addItem(isextra3Item);
        auto isextra4Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra4Item->setName("$variable$isextra4$");
        isextra4Item->setValueFromBool(isextra4);
        assembly.addItem(isextra4Item);
        auto isextra5Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra5Item->setName("$variable$isextra5$");
        isextra5Item->setValueFromBool(isextra5);
        assembly.addItem(isextra5Item);
        auto isextra6Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra6Item->setName("$variable$isextra6$");
        isextra6Item->setValueFromBool(isextra6);
        assembly.addItem(isextra6Item);
        auto isextra7Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra7Item->setName("$variable$isextra7$");
        isextra7Item->setValueFromBool(isextra7);
        assembly.addItem(isextra7Item);
        auto isextra8Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra8Item->setName("$variable$isextra8$");
        isextra8Item->setValueFromBool(isextra8);
        assembly.addItem(isextra8Item);
        auto isextra9Item = std::make_shared<rw::oso::ObjectStoreItem>();
        isextra9Item->setName("$variable$isextra9$");
        isextra9Item->setValueFromBool(isextra9);
        assembly.addItem(isextra9Item);
        return assembly;
    }

    inline bool SetConfig::operator==(const SetConfig& obj) const
    {
        return zhongyangxianwei1 == obj.zhongyangxianwei1 && zhongyangxianwei2 == obj.zhongyangxianwei2 && qiyongerxiangji == obj.qiyongerxiangji && qiyongyundongkongzhiqi == obj.qiyongyundongkongzhiqi && yundongkongzhiqichonglian == obj.yundongkongzhiqichonglian && yanshixiachawai1 == obj.yanshixiachawai1 && boliaoshijianwai1 == obj.boliaoshijianwai1 && yanshixiachawai2 == obj.yanshixiachawai2 && boliaoshijianwai2 == obj.boliaoshijianwai2 && yanshixiachanei1 == obj.yanshixiachanei1 && boliaoshijiannei1 == obj.boliaoshijiannei1 && yanshixiachanei2 == obj.yanshixiachanei2 && boliaoshijiannei2 == obj.boliaoshijiannei2 && xiangjichufachangdu == obj.xiangjichufachangdu && meizhuanmaichongshu == obj.meizhuanmaichongshu && shedingzhouchang == obj.shedingzhouchang && waicetifeijuli1 == obj.waicetifeijuli1 && neicetifeijuli1 == obj.neicetifeijuli1 && tifeirongyufangdou1 == obj.tifeirongyufangdou1 && waicetifeijuli2 == obj.waicetifeijuli2 && neicetifeijuli2 == obj.neicetifeijuli2 && tifeirongyufangdou2 == obj.tifeirongyufangdou2 && tuxiangshunshizhenxuanzhuancishu1 == obj.tuxiangshunshizhenxuanzhuancishu1 && tuxiangshunshizhenxuanzhuancishu2 == obj.tuxiangshunshizhenxuanzhuancishu2 && xunzhaoquexiansuoshubashoudepianchazhi == obj.xunzhaoquexiansuoshubashoudepianchazhi && shangXianWei1 == obj.shangXianWei1 && xiaXianWei1 == obj.xiaXianWei1 && zuoXianWei1 == obj.zuoXianWei1 && youXianWei1 == obj.youXianWei1 && xiangSuDangLiang1 == obj.xiangSuDangLiang1 && shangXianWei2 == obj.shangXianWei2 && xiaXianWei2 == obj.xiaXianWei2 && zuoXianWei2 == obj.zuoXianWei2 && youXianWei2 == obj.youXianWei2 && xiangSuDangLiang2 == obj.xiangSuDangLiang2 && qiangbaoguang == obj.qiangbaoguang && qiangzengyi == obj.qiangzengyi && zhongbaoguang == obj.zhongbaoguang && zhongzengyi == obj.zhongzengyi && ruobaoguang == obj.ruobaoguang && ruozengyi == obj.ruozengyi && takeWork1Pictures == obj.takeWork1Pictures && takeWork2Pictures == obj.takeWork2Pictures && saveNGImg == obj.saveNGImg && saveMaskImg == obj.saveMaskImg && saveOKImg == obj.saveOKImg && isSaveJPEG == obj.isSaveJPEG && isSaveBMP == obj.isSaveBMP && isSavePNG == obj.isSavePNG && saveImgQuality == obj.saveImgQuality && DOpaizhao1 == obj.DOpaizhao1 && DOpaizhao2 == obj.DOpaizhao2 && DOguangyuan == obj.DOguangyuan && DObaojing == obj.DObaojing && DOtingji == obj.DOtingji && DOxiachaqigangwai1 == obj.DOxiachaqigangwai1 && DOxiachaqigangnei1 == obj.DOxiachaqigangnei1 && DOxiachaqigangwai2 == obj.DOxiachaqigangwai2 && DOxiachaqigangnei2 == obj.DOxiachaqigangnei2 && DOboliaoqigangwai1 == obj.DOboliaoqigangwai1 && DOboliaoqigangnei1 == obj.DOboliaoqigangnei1 && DOboliaoqigangwai2 == obj.DOboliaoqigangwai2 && DOboliaoqigangnei2 == obj.DOboliaoqigangnei2 && debugMode == obj.debugMode && istangshang == obj.istangshang && isqueliao == obj.isqueliao && isyiwu == obj.isyiwu && iszangwu == obj.iszangwu && isextra2 == obj.isextra2 && isextra3 == obj.isextra3 && isextra4 == obj.isextra4 && isextra5 == obj.isextra5 && isextra6 == obj.isextra6 && isextra7 == obj.isextra7 && isextra8 == obj.isextra8 && isextra9 == obj.isextra9;
    }

    inline bool SetConfig::operator!=(const SetConfig& obj) const
    {
        return !(*this == obj);
    }

}

