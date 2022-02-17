//
// Created by maicss on 2021/12/28.
//

#include "WaliPkgCmd.h"
#include <iostream>
#include <fstream>
#if __GNUC__ <= 5
    #error your compiler cannot support C++17 standard. we need at least GCC 7
#elif (__GNUC__ > 5) && (__GNUC__ < 11)
    #include <experimental/filesystem>
#else
    #include <filesystem>
#endif

#include "log.h"

using namespace std;
#if (__GNUC__ > 6) && (__GNUC__ < 11)
    namespace filesystem = experimental::filesystem;
#endif
WaliPkgCmd::WaliPkgCmd() {
    log("Create a WaliPkgCmd object");
    init();
}
WaliPkgCmd::~WaliPkgCmd() {
    g_object_unref (m_task);
}

bool WaliPkgCmd::init() {
    m_task = pk_task_new();
    _progressCallback = [](_PkProgress *progress, PkProgressType type, gpointer user_data) {
        if(user_data != nullptr) {
            WaliSchedule progress1{};
            progress1.m_progress = *progress;
            ((WaliPkgCmd *) user_data)->m_progressCallback(progress1, ((WaliPkgCmd *) user_data)->m_tasktype);
        }
    };
    return m_task != nullptr;
}
std::vector<WaliPkg>
WaliPkgCmd::find_packages_based_on_files(std::vector<std::string> files) {
    m_error = "";
#ifdef _APT
    m_tasktype = TASK_FIND_PACKAGE;
    std::vector<WaliPkg> res;
    std::vector<string> contents;
    filesystem::path cacheDir("/var/lib/apt/lists/");

    if(files.empty()) {
        log(LOG_FUNCTION_NAME "Files is empty!", WARRING);
        return res;
    }

    filesystem::directory_entry entry(cacheDir);
    if(!filesystem::exists(entry.path())){
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
    WaliPkg *item;
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
    std::vector<WaliPkg> res;
    for (int i = 0; i < array->len; i++) {
        item = (WaliPkg*)g_ptr_array_index(array, i);
        package_ids[i] = g_strdup (pk_package_get_id ((_PkPackage*)item));
        res.emplace_back(*item);
    }
    g_strfreev(values);
    g_strfreev(package_ids);
    m_tasktype = TASK_NOTING_TO_DO;
    return res;
#endif
}


std::vector<WaliPkg>
WaliPkgCmd::find_packages_based_on_names(std::vector<std::string> names) {
    m_error = "";
    m_tasktype = TASK_FIND_PACKAGE;
    gchar **values;
    GPtrArray *array = nullptr;
    WaliPkg *item;
    gchar **package_ids = nullptr;
    PkResults *results = nullptr;
    GError *error = nullptr;
    std::vector<WaliPkg> res;
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
        item = (WaliPkg*)g_ptr_array_index(array, i);
        package_ids[i] = g_strdup (pk_package_get_id ((_PkPackage*)item));
        res.emplace_back(*item);
    }
    g_strfreev(values);
    g_strfreev(package_ids);
    m_tasktype = TASK_NOTING_TO_DO;
    return res;
}

void WaliPkgCmd::install_packages(std::vector<WaliPkg> packages) {
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

void WaliPkgCmd::setProgressCallback(ScheduleCallback progressCallback) {
    m_progressCallback = progressCallback;
}

void WaliPkgCmd::remove_packages(std::vector<WaliPkg> packages, bool allow_deps, bool autoremove) {
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

gchar **WaliPkgCmd::to_package_id_list(std::vector<WaliPkg> packages) {
    gchar **package_ids = nullptr;
    package_ids = g_new0 (gchar *, packages.size()+1);

    for (int i = 0; i < packages.size(); ++i) {
        package_ids[i] = g_strdup(packages[i].get_package_id().c_str());
    }
    package_ids[packages.size()]= nullptr;
    return package_ids;
}

void WaliPkgCmd::refresh_cache(bool force) {
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

void WaliPkgCmd::install_local_packages(std::vector<std::string> files) {
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

std::string WaliPkgCmd::error() {
    return m_error;
}

std::vector<WaliRepo> WaliPkgCmd::get_repo_list() {
    vector<WaliRepo> res;
    GError *error = nullptr;
    auto _res = pk_task_get_repo_list_sync(m_task, PK_FILTER_ENUM_NONE, nullptr, nullptr, nullptr,&error);
    auto array = pk_results_get_repo_detail_array (_res);
    for (int i = 0; i < array->len; i++) {
        WaliRepo repo{};
        repo.m_repo = *(_PkRepoDetail*)g_ptr_array_index(array, i);
        res.emplace_back(repo);
    }
    return res;
}
#ifdef _APT
std::vector<WaliSrcPkg> WaliPkgCmd::get_source_package_info(std::vector<std::string> packages) {
    vector<WaliSrcPkg> res;
    std::vector<string> sources;
    filesystem::path cacheDir("/var/lib/apt/lists/");
    filesystem::directory_entry entry(cacheDir);
    if(!filesystem::exists(entry.path())){
        m_error = log("Can't find "+cacheDir.string());
        return res;
    }

    if(entry.status().type() != filesystem::file_type::directory){
        m_error = log( cacheDir.string()+" is not a directory.");
        return res;
    }

    filesystem::directory_iterator iterator(cacheDir);
    for(auto& it:iterator){
        if(it.path().filename().string().find("_Sources")!=string::npos)
            sources.emplace_back(it.path().filename().string());
    }
    if(sources.empty()){
        m_error = log(LOG_FUNCTION_NAME "Not find Sources file, Maybe you need to run \"apt update\" or open src repo.");
        return res;
    }
    for(auto source : sources){
        fstream file;
        file.open(cacheDir.string()+source, ios::in);
        if (!file)
        {
            continue;
        }
        string line;
        WaliSrcPkg sourcesEntry;
        bool thisOne = false;
        bool readPackageList = false;
        while (getline(file, line))
        {
            if(line.empty()){
                if(thisOne == true){
                    res.emplace_back(sourcesEntry);
                }
                thisOne = false;
                readPackageList = false;
                sourcesEntry.m_package_list.clear();
                continue;
            }
            if(line.substr(0,8)=="Package:"){
                sourcesEntry.m_name = line.substr(9,line.length()-9);
                for (int i = 0; i < packages.size(); ++i) {
                    if(packages.at(i) == sourcesEntry.m_name){
                        thisOne = true;
                        continue;
                    }
                }
            }
            if(thisOne && line.substr(0, 8) == "Version:"){
                sourcesEntry.m_version = line.substr(9,line.length()-9);
            }
            if(thisOne && line.substr(0, 13) == "Package-List:"){
                readPackageList = true;
                continue;
            }
            if(line.find(':') != string::npos){
                readPackageList = false;
            }
            if (thisOne && readPackageList) {
                sourcesEntry.m_package_list.emplace_back(line.substr(1,line.length()-1));
            }
        }
        file.close();
    }
    return res;
}
#endif

#ifdef _APT
std::vector<ContentsEntry>
WaliPkgCmd::parse_contents(const std::string &filename, std::vector<std::string> targets) {

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

