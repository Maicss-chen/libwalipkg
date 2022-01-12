//
// Created by maicss on 2021/12/28.
//

#include "PkPackage.h"
#include <iostream>
PackageKitMM::PkPackage::PkPackage() {
}

PackageKitMM::PkPackage::PkPackage(_PkPackage pkPackage) {
    m_pkPackage = pkPackage;
}

std::string PackageKitMM::PkPackage::get_package_id() {
    return g_strdup (pk_package_get_id (&m_pkPackage));

}

std::string PackageKitMM::PkPackage::get_package_arch() {
    return g_strdup(pk_package_get_arch(&m_pkPackage));
}

bool PackageKitMM::PkPackage::is_installed() {
    return pk_package_get_info(&m_pkPackage) == PK_INFO_ENUM_INSTALLED;
}

std::string PackageKitMM::PkPackage::get_package_name() {
    return g_strdup(pk_package_get_name(&m_pkPackage));
}

bool PackageKitMM::PkPackage::operator==(PkPackage pkPackage) {
    return pk_package_equal(&m_pkPackage,&pkPackage.m_pkPackage);
}

std::string PackageKitMM::PkPackage::get_package_summary() {
    return g_strdup(pk_package_get_summary(&m_pkPackage));
}

std::string PackageKitMM::PkPackage::get_package_repositoryID() {
    std::string data = pk_package_get_data(&m_pkPackage);
    if(data=="local") return "";
    int l = data.find_last_of(":");
    if(l==std::string::npos){
        return data;
    } else{
        return data.substr(l+1,data.length()-l);
    }
}

