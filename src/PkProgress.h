//
// Created by maicss on 2022/1/15.
//

#ifndef PACKAGEKITMM_PKPROGRESS_H
#define PACKAGEKITMM_PKPROGRESS_H
#include <packagekit-glib2/packagekit.h>
namespace PackageKitMM {
    class PackageKit;
    class PkPackage;
    class PkProgress {
        friend PackageKit;
    public:
        //获取百分比进度
        int get_percentage();
        //获取速度，单位bit/s
        int get_speed();
        //计算当前预计剩余时间,单位s
        int get_remaining_time();
        //获取当前正在操作的包
        PkPackage get_package();
    private:
        _PkProgress m_progress;
    };
}

#endif //PACKAGEKITMM_PKPROGRESS_H
