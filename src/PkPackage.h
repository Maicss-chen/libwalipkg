//
// Created by maicss on 2021/12/28.
//

#ifndef PKGKIT_PKPACKAGE_H
#define PKGKIT_PKPACKAGE_H
#include <packagekit-glib2/packagekit.h>
#include <string>
#include <vector>
namespace PackageKit {
    class PackageKit;
    //该类描述一个软件包
    class PkPackage {
    public:
        friend PackageKit;
        PkPackage();
        PkPackage(_PkPackage pkPackage);
        //API
        gchar* get_package_id();
        gchar* get_package_arch();
        gchar* get_package_name();
        bool is_installed();
    private:
        _PkPackage m_pkPackage;
    };
}

#endif //PKGKIT_PKPACKAGE_H
