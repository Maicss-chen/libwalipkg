//
// Created by maicss on 2021/12/28.
//

#ifndef PKGKIT_PKPACKAGE_H
#define PKGKIT_PKPACKAGE_H
#ifndef I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#define I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#endif

#include <packagekit-glib2/packagekit.h>
#include <string>
#include <vector>
    class WaliPkgCmd;
    class WaliSchedule;
    //该类描述一个软件包
    class WaliPkg {
        friend WaliPkgCmd;
        friend WaliSchedule;
    public:
        WaliPkg();
        WaliPkg(_PkPackage pkPackage);

        //API
        std::string get_package_id();
        std::string get_package_arch();
        std::string get_package_name();
        std::string get_package_summary();
        std::string get_package_repositoryID();

        bool is_installed();
        bool operator==(WaliPkg pkPackage);
    private:
        _PkPackage m_pkPackage;
    };

#endif //PKGKIT_PKPACKAGE_H
