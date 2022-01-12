//
// Created by maicss on 2021/12/28.
//

#include "PackageKit.h"
#include <iostream>
#include <utility>
PackageKit::PackageKit::PackageKit() {
    init();
}
PackageKit::PackageKit::~PackageKit() {
    g_object_unref (m_task);
}

bool PackageKit::PackageKit::init() {
    m_task = pk_task_new();
    m_progressCallback = nullptr;
    _progressCallback = [](PkProgress *progress, PkProgressType type, gpointer user_data) {
        if(user_data != nullptr)
            ((PackageKit*)user_data)->m_progressCallback(pk_progress_get_percentage(progress),((PackageKit*)user_data)->m_tasktype);
    };
    return m_task != nullptr;
}

std::vector<PackageKit::PkPackage>
PackageKit::PackageKit::find_packages_based_on_files_sync(std::vector<std::string> files, PkBitfield filters) {
    gchar **values;
    GPtrArray *array = nullptr;
    PkPackage *item;
    gchar **package_ids = nullptr;
    PkResults *results = nullptr;

    values = g_new0 (gchar*, files.size()+1);
    for (int i=0;i<files.size();i++) {
        values[i] = g_strdup(files.at(i).c_str());
    }
    values[files.size()] = nullptr;
    m_tasktype = TASK_FIND_PACKAGE;
    results = pk_task_search_files_sync(m_task,filters,values,nullptr,_progressCallback,this,nullptr);
    array = pk_results_get_package_array (results);
    package_ids = g_new0 (gchar *, array->len+1);
    std::vector<PkPackage> res;
    for (int i = 0; i < array->len; i++) {
        item = (PkPackage*)g_ptr_array_index(array, i);
        package_ids[i] = g_strdup (pk_package_get_id ((_PkPackage*)item));
        res.emplace_back(*item);
    }
    g_strfreev(values);
    g_strfreev(package_ids);
    m_tasktype = TASK_NOTING_TO_DO;
    return res;
}
std::vector<PackageKit::PkPackage>
PackageKit::PackageKit::find_packages_based_on_names_sync(std::vector<std::string> names, PkBitfield filters) {
    m_tasktype = TASK_FIND_PACKAGE;
    gchar **values;
    GPtrArray *array = nullptr;
    PkPackage *item;
    gchar **package_ids = nullptr;
    PkResults *results = nullptr;

    values = g_new0 (gchar*, names.size()+1);
    for (int i=0;i<names.size();i++) {
        values[i] = g_strdup(names.at(i).c_str());
    }
    values[names.size()] = nullptr;
    results = pk_task_resolve_sync(m_task,filters,values,nullptr,_progressCallback,this,nullptr);
    array = pk_results_get_package_array (results);
    package_ids = g_new0 (gchar *, array->len+1);
    std::vector<PkPackage> res;
    for (int i = 0; i < array->len; i++) {
        item = (PkPackage*)g_ptr_array_index(array, i);
        package_ids[i] = g_strdup (pk_package_get_id ((_PkPackage*)item));
        res.emplace_back(*item);
    }
    g_strfreev(values);
    g_strfreev(package_ids);
    m_tasktype = TASK_NOTING_TO_DO;
    return res;
}

void PackageKit::PackageKit::install_packages(std::vector<PkPackage> packages) {
    gchar **package_ids = to_package_id_list(std::move(packages));
    m_tasktype = TASK_INSTALL_PACKAGE;
    //安装所有软件包
    pk_task_install_packages_sync(m_task, package_ids, nullptr, _progressCallback, this, nullptr);
    m_tasktype = TASK_NOTING_TO_DO;
}

void PackageKit::PackageKit::setProgressCallback(ProgressCallback progressCallback) {
    m_progressCallback = progressCallback;
}

void PackageKit::PackageKit::remove_packages(std::vector<PkPackage> packages,bool allow_deps,bool autoremove) {
    gchar **package_ids = to_package_id_list(std::move(packages));
    m_tasktype = TASK_REMOVE_PACKAGE;
    //卸载所有软件包
    pk_task_remove_packages_sync(m_task, package_ids, allow_deps, autoremove, nullptr, _progressCallback, this, nullptr);
    m_tasktype = TASK_NOTING_TO_DO;
}

gchar **PackageKit::PackageKit::to_package_id_list(std::vector<PkPackage> packages) {
    gchar **package_ids = nullptr;
    package_ids = g_new0 (gchar *, packages.size()+1);

    for (int i = 0; i < packages.size(); ++i) {
        package_ids[i] = packages[i].get_package_id();
    }
    package_ids[packages.size()]= nullptr;
    return package_ids;
}

void PackageKit::PackageKit::refresh_cache(bool force) {
    m_tasktype = TASK_REFRESH_CACHE;
    pk_task_refresh_cache_sync(m_task,force, nullptr,_progressCallback, this, nullptr);
    m_tasktype = TASK_NOTING_TO_DO;
}



