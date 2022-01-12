//
// Created by maicss on 2021/12/28.
//

#include "PkPackage.h"
#include <iostream>
PackageKit::PkPackage::PkPackage() {
}

PackageKit::PkPackage::PkPackage(_PkPackage pkPackage) {
    m_pkPackage = pkPackage;
}

gchar* PackageKit::PkPackage::get_package_id() {
    return g_strdup (pk_package_get_id (&m_pkPackage));

}

gchar *PackageKit::PkPackage::get_package_arch() {
    return g_strdup(pk_package_get_arch(&m_pkPackage));
}

bool PackageKit::PkPackage::is_installed() {
    return pk_package_get_info(&m_pkPackage) == PK_INFO_ENUM_INSTALLED;
}

gchar *PackageKit::PkPackage::get_package_name() {
    return g_strdup(pk_package_get_name(&m_pkPackage));
}
