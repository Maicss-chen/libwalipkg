//
// Created by maicss on 2022/1/19.
//

#ifndef PACKAGEKITMM_WALISRCPKG_H
#define PACKAGEKITMM_WALISRCPKG_H

#include "string"
#include "vector"
    class WaliPkgCmd;
    class WaliSrcPkg {
        friend WaliPkgCmd;
    public:
        std::string get_name();
        std::string get_version();
        std::vector<std::string> get_package_name_list();
    private:
        std::string m_name;
        std::string m_version;
        std::vector<std::string> m_package_list;
    };

#endif //PACKAGEKITMM_WALISRCPKG_H
