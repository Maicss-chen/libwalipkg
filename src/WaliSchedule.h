//
// Created by maicss on 2022/1/15.
//

#ifndef PACKAGEKITMM_WALISCHEDULE_H
#define PACKAGEKITMM_WALISCHEDULE_H
#ifndef I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#define I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#endif
#include <packagekit-glib2/packagekit.h>

class WaliPkgCmd;
class WaliPkg;
class WaliSchedule {
    friend WaliPkgCmd;
public:
    //获取百分比进度
    int get_percentage();
    //获取速度，单位bit/s
    int get_speed();
    //计算当前预计剩余时间,单位s
    int get_remaining_time();
    //获取当前正在操作的包
    WaliPkg get_package();
private:
    _PkProgress m_progress;
};


#endif //PACKAGEKITMM_WALISCHEDULE_H
