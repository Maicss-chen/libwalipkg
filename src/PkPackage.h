//
// Created by maicss on 2021/12/28.
//

#ifndef PKGKIT_PKPACKAGE_H
#define PKGKIT_PKPACKAGE_H
#include <packagekit-glib2/packagekit.h>
#include <string>
#include <vector>
namespace PackageKitMM {
    class PackageKit;
    //该类描述一个软件包
    class PkPackage {
    public:
        friend PackageKit;
        PkPackage();
        PkPackage(_PkPackage pkPackage);
        //API
        std::string get_package_id();
        std::string get_package_arch();
        std::string get_package_name();
        std::string get_package_summary();
        std::string get_package_repositoryID();

        bool is_installed();
        bool operator==(PkPackage pkPackage);
    private:
        _PkPackage m_pkPackage;
    };
}

#endif //PKGKIT_PKPACKAGE_H
