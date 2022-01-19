//
// Created by maicss on 2022/1/19.
//

#include "PkSourcePackage.h"
#include "log.h"

std::string PackageKitMM::PkSourcePackage::get_name() {
    return m_name;
}

std::string PackageKitMM::PkSourcePackage::get_version() {
    return m_version;
}

std::vector<std::string> PackageKitMM::PkSourcePackage::get_package_name_list() {
    std::vector<std::string> res;
    for(const std::string& pkg : m_package_list){
        res.emplace_back(pkg.substr(0,pkg.find(' ')));
    }
    return res;
}
