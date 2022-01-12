//
// Created by maicss on 2021/12/28.
//

#ifndef TEST_PACKAGEKIT_H
#define TEST_PACKAGEKIT_H

#include "PkPackage.h"

#include <packagekit-glib2/packagekit.h>
#include <vector>

namespace PackageKitMM{
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
    typedef void (*ProgressCallback)(int progress,TaskType type);
    class PackageKit {
    public:
        PackageKit();
        ~PackageKit();

        //API
        bool init();
        //更新apt缓存文件
        void refresh_cache(bool force=false);

        //根据文件查找包
        std::vector<PkPackage> find_packages_based_on_files_sync(std::vector<std::string> files,
                                                                 PkBitfield filters=PK_FILTER_ENUM_NONE);
        //根据包名查找包
        std::vector<PkPackage> find_packages_based_on_names_sync(std::vector<std::string> names,
                                                                 PkBitfield filters=PK_FILTER_ENUM_NONE);
        //安装软件包
        void install_packages(std::vector<PkPackage> packages);

        //卸载软件包
        void remove_packages(std::vector<PkPackage> packages,bool allow_deps = true,bool autoremove = false);

        //设置进度更新时的回调函数
        void setProgressCallback(ProgressCallback progressCallback);

    private:
        PkTask *m_task{};
        ProgressCallback m_progressCallback{};
        TaskType m_tasktype;  //用于记录目前正在执行的任务类型

        //内部调用
        void (*_progressCallback)(PkProgress *progress,PkProgressType type,gpointer user_data){};
        gchar** to_package_id_list(std::vector<PkPackage> packages);
        std::vector<ContentsEntry> parse_contents(const std::string &filename, std::vector<std::string> targets);
    };

}


#endif //TEST_PACKAGEKIT_H
