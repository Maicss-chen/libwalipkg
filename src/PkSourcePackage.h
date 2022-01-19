//
// Created by maicss on 2022/1/19.
//

#ifndef PACKAGEKITMM_PKSOURCEPACKAGE_H
#define PACKAGEKITMM_PKSOURCEPACKAGE_H

#include "string"
#include "vector"
namespace PackageKitMM {
    class PackageKit;
    class PkSourcePackage {
        friend PackageKit;
    public:
        std::string get_name();
        std::string get_version();
        std::vector<std::string> get_package_name_list();
    private:
        std::string m_name;
        std::string m_version;
        std::vector<std::string> m_package_list;
    };
}

#endif //PACKAGEKITMM_PKSOURCEPACKAGE_H
