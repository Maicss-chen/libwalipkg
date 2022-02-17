//
// Created by maicss on 2021/12/28.
//

#ifndef TEST_PACKAGEKIT_H
#define TEST_PACKAGEKIT_H

#include "WaliPkg.h"
#include "WaliRepo.h"
#include "WaliSchedule.h"
#include "WaliSrcPkg.h"

#ifndef I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#define I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#endif
#include <packagekit-glib2/packagekit.h>
#include <vector>

    struct ContentsEntry{
        std::string path;
        std::string packageName;
    };
    enum TaskType{
        TASK_NOTING_TO_DO,
        TASK_FIND_PACKAGE,
        TASK_INSTALL_PACKAGE,
        TASK_REMOVE_PACKAGE,
        TASK_REFRESH_CACHE
    };
    typedef void (*ScheduleCallback)(WaliSchedule progress, TaskType type);
    class WaliPkgCmd {
    public:
        WaliPkgCmd();
        ~WaliPkgCmd();

        //API
        bool init();
        //更新apt缓存文件
        void refresh_cache(bool force=false);

        // 根据文件查找包。在Debian下，该函数能支持的参数有相对路径、绝对路径、文件名。但其他发行版下，主要取决于PackageKit对于此功能的实现。
        std::vector<WaliPkg> find_packages_based_on_files(std::vector<std::string> files);
        //根据包名查找包
        std::vector<WaliPkg> find_packages_based_on_names(std::vector<std::string> names);
        //安装软件包
        void install_packages(std::vector<WaliPkg> packages);
        //安装本地软件包
        void install_local_packages(std::vector<std::string> files);
        //卸载软件包
        void remove_packages(std::vector<WaliPkg> packages, bool allow_deps = true, bool autoremove = false);
        //获取仓库列表
        std::vector<WaliRepo> get_repo_list();

//#ifdef _APT
        //获取源码包信息
        std::vector<WaliSrcPkg> get_source_package_info(std::vector<std::string> packages);
//#endif
        //设置进度更新时的回调函数
        void setProgressCallback(ScheduleCallback progressCallback);

        //返回上一操作中的错误结果
        std::string error();

    private:
        PkTask *m_task{};
        ScheduleCallback m_progressCallback{};
        TaskType m_tasktype;  //用于记录目前正在执行的任务类型
        std::string m_error;

        //内部调用
        void (*_progressCallback)(_PkProgress *progress,PkProgressType type,gpointer user_data){};
        static gchar** to_package_id_list(std::vector<WaliPkg> packages);

#ifdef _APT
        static std::vector<ContentsEntry> parse_contents(const std::string &filename, std::vector<std::string> targets);
#endif
    };

#endif //TEST_PACKAGEKIT_H
