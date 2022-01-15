//
// Created by maicss on 2021/12/28.
//

#include "PackageKit.h"
#include <iostream>
#include <fstream>
#include <filesystem>

#include "log.h"

using namespace std;
using namespace PackageKitMM;
PackageKitMM::PackageKit::PackageKit() {
    log("Create a PackageKit object");
    init();
}
PackageKitMM::PackageKit::~PackageKit() {
    g_object_unref (m_task);
}

bool PackageKitMM::PackageKit::init() {
    m_task = pk_task_new();
    _progressCallback = [](PkProgress *progress, PkProgressType type, gpointer user_data) {
        if(user_data != nullptr)
            ((PackageKit*)user_data)->m_progressCallback(pk_progress_get_percentage(progress),((PackageKit*)user_data)->m_tasktype);
    };
    return m_task != nullptr;
}
std::vector<PackageKitMM::PkPackage>
PackageKitMM::PackageKit::find_packages_based_on_files(std::vector<std::string> files) {
    m_error = "";
#ifdef _APT
    m_tasktype = TASK_FIND_PACKAGE;
    std::vector<PackageKitMM::PkPackage> res;
    std::vector<string> contents;
    filesystem::path cacheDir("/var/lib/apt/lists/");

    if(files.empty()) {
        log(LOG_FUNCTION_NAME "Files is empty!", WARRING);
        return res;
    }

    filesystem::directory_entry entry(cacheDir);
    if(!entry.exists()){
        m_error = log("Can't find "+cacheDir.string());
        return res;
    }

    if(entry.status().type() != filesystem::file_type::directory){
        m_error = log( cacheDir.string()+" is not a directory.");
        return res;
    }

    filesystem::directory_iterator iterator(cacheDir);
    for(auto& it:iterator){
        if(it.path().filename().string().find("Contents")!=string::npos &&
            it.path().filename().string().find(".lz4")==string::npos)
            contents.emplace_back(it.path().filename().string());
    }
    if(contents.empty()){
        m_error = log(LOG_FUNCTION_NAME "Not find Contents file, Maybe you need to run \"apt update\".");
        return res;
    }
    for (const auto& content:contents) {
        auto contentsEntryList=parse_contents(cacheDir.string()+content,files);
        if(contentsEntryList.empty()) continue;
        vector<string> names;
        for(const auto& _entry:contentsEntryList){
            names.emplace_back(_entry.packageName);
        }
        auto r = find_packages_based_on_names(names);
        for(auto a : r){
            bool isHere = false;
            for (auto b:res) {
                if(b==a) isHere = true;
            }
            if (isHere == false)
                res.push_back(a);
        }
    }
    m_tasktype = TASK_NOTING_TO_DO;
    return res;
#else
    gchar **values;
    GPtrArray *array = nullptr;
    PkPackage *item;
    gchar **package_ids = nullptr;
    PkResults *results = nullptr;
    GError *error = nullptr;

    values = g_new0 (gchar*, files.size()+1);
    for (int i=0;i<files.size();i++) {
        values[i] = g_strdup(files.at(i).c_str());
    }
    values[files.size()] = nullptr;
    m_tasktype = TASK_FIND_PACKAGE;
    results = pk_task_search_files_sync(m_task, PK_FILTER_ENUM_NONE,values,nullptr,_progressCallback,this,&error);
    if (error){
        m_error = log(LOG_FUNCTION_NAME string(error->message), ERROR);
        g_error_free (error);
    }
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
#endif
}


std::vector<PackageKitMM::PkPackage>
PackageKitMM::PackageKit::find_packages_based_on_names(std::vector<std::string> names) {
    m_error = "";
    m_tasktype = TASK_FIND_PACKAGE;
    gchar **values;
    GPtrArray *array = nullptr;
    PkPackage *item;
    gchar **package_ids = nullptr;
    PkResults *results = nullptr;
    GError *error = nullptr;
    std::vector<PkPackage> res;
    if (names.empty()){
        m_error = log(LOG_FUNCTION_NAME "Names is empty!", WARRING);
        return res;
    }

    values = g_new0 (gchar*, names.size()+1);
    for (int i=0;i<names.size();i++) {
        values[i] = g_strdup(names.at(i).c_str());
    }
    values[names.size()] = nullptr;
    results = pk_task_resolve_sync(m_task,PK_FILTER_ENUM_NONE,values,nullptr,_progressCallback,this,&error);
    if (error){
        m_error = log(LOG_FUNCTION_NAME string(error->message), ERROR);
        g_error_free (error);
    }
    array = pk_results_get_package_array (results);
    package_ids = g_new0 (gchar *, array->len+1);
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

void PackageKitMM::PackageKit::install_packages(std::vector<PkPackage> packages) {
    m_error = "";
    gchar **package_ids = to_package_id_list(std::move(packages));
    GError *error = nullptr;
    m_tasktype = TASK_INSTALL_PACKAGE;
    //安装所有软件包
    pk_task_install_packages_sync(m_task, package_ids, nullptr, _progressCallback, this, &error);
    if (error){
        m_error = log(LOG_FUNCTION_NAME string(error->message), ERROR);
        g_error_free (error);
    }
    m_tasktype = TASK_NOTING_TO_DO;
}

void PackageKitMM::PackageKit::setProgressCallback(ProgressCallback progressCallback) {
    m_progressCallback = progressCallback;
}

void PackageKitMM::PackageKit::remove_packages(std::vector<PkPackage> packages, bool allow_deps, bool autoremove) {
    m_error = "";
    gchar **package_ids = to_package_id_list(std::move(packages));
    GError *error = nullptr;
    m_tasktype = TASK_REMOVE_PACKAGE;
    //卸载所有软件包
    pk_task_remove_packages_sync(m_task, package_ids, allow_deps, autoremove, nullptr, _progressCallback, this, &error);
    if (error){
        m_error = log(LOG_FUNCTION_NAME string(error->message), ERROR);
        g_error_free (error);
    }
    m_tasktype = TASK_NOTING_TO_DO;
}

gchar **PackageKitMM::PackageKit::to_package_id_list(std::vector<PkPackage> packages) {
    gchar **package_ids = nullptr;
    package_ids = g_new0 (gchar *, packages.size()+1);

    for (int i = 0; i < packages.size(); ++i) {
        package_ids[i] = g_strdup(packages[i].get_package_id().c_str());
    }
    package_ids[packages.size()]= nullptr;
    return package_ids;
}

void PackageKitMM::PackageKit::refresh_cache(bool force) {
    m_error = "";
    m_tasktype = TASK_REFRESH_CACHE;
    GError *error = nullptr;
    pk_task_refresh_cache_sync(m_task,force, nullptr,_progressCallback, this, &error);
    if (error){
        m_error = log(LOG_FUNCTION_NAME string(error->message), ERROR);
        g_error_free (error);
    }
    m_tasktype = TASK_NOTING_TO_DO;
}

void PackageKit::install_local_packages(std::vector<std::string> files) {
    m_error = "";
    m_tasktype = TASK_INSTALL_PACKAGE;
    GError *error = nullptr;
    gchar **files_array = g_new0(gchar*,files.size()+1);
    for (int i = 0; i < files.size(); ++i) {
        filesystem::path file(files.at(i));
        if(!filesystem::exists(file)){
            m_error = log(LOG_FUNCTION_NAME "Not find "+files.at(i),ERROR);
        }
        files_array[i] = g_strdup(files.at(i).c_str());
    }
    files_array[files.size()]= nullptr;
    pk_task_install_files_sync(m_task,files_array, nullptr,_progressCallback,this,&error);
    if (error){
        m_error = log(LOG_FUNCTION_NAME string(error->message), ERROR);
        g_error_free (error);
    }
    m_tasktype = TASK_NOTING_TO_DO;
}

std::string PackageKit::error() {
    return m_error;
}

std::vector<PkRepo> PackageKit::get_repo_list() {
    vector<PkRepo> res;
    GError *error = nullptr;
    auto _res = pk_task_get_repo_list_sync(m_task, PK_FILTER_ENUM_NONE, nullptr, nullptr, nullptr,&error);
    auto array = pk_results_get_repo_detail_array (_res);
    for (int i = 0; i < array->len; i++) {
        PkRepo repo{};
        repo.m_repo = *(_PkRepoDetail*)g_ptr_array_index(array, i);
        res.emplace_back(repo);
    }
    return res;
}

#ifdef _APT
std::vector<PackageKitMM::ContentsEntry>
PackageKitMM::PackageKit::parse_contents(const std::string &filename, std::vector<std::string> targets) {

    fstream file;
    vector<ContentsEntry> res;
    file.open(filename, ios::in);
    if (!file)
    {
        log(LOG_FUNCTION_NAME "Can't open file:" + filename, ERROR);
        return res;
    }
    string line;
    while (getline(file, line))
    {
        uint index;
        index = line.find('\t');
        if (int(index) == string::npos)
        {
            index = line.find(' ');
        }
        string opt_left = line.substr(0, index);

        vector<string>::iterator it;
        for (it = targets.begin();it!=targets.end(); it++)
        {
            if(opt_left.length()<(*it).length()) continue;
            if (opt_left.substr(opt_left.length()-(*it).length(),(*it).length())==*it)
            {
                index = line.find_last_of('\t');
                uint index2 = line.find_last_of(' ');
                if (int(index) == string::npos)
                {
                    index = line.length();
                }
                if(int(index2) == string::npos)
                {
                    index2 = line.length();
                }
                index = min(index,index2)+1;
                string opt_right = line.substr(index, line.length() - index);
                int lastindex = opt_right.find_last_of('/');
                if (int(lastindex) == string::npos)
                    lastindex = -1;
                string pkgname = opt_right.substr(lastindex + 1, opt_right.length() - lastindex);
                ContentsEntry a;
                a.path = opt_left;
                a.packageName = pkgname;
                res.push_back(a);
            }
        }
    }
    file.close();
    return res;
}
#endif

