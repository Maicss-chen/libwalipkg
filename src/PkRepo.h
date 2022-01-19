//
// Created by maicss on 2022/1/14.
//

#ifndef PACKAGEKITMM_PKREPO_H
#define PACKAGEKITMM_PKREPO_H
#include "packagekit-glib2/packagekit.h"
#include <string>
namespace PackageKitMM{
    class PackageKit;
    class PkRepo {
    public:
        friend PackageKit;
        std::string get_repo_id();
        std::string get_repo_description();
        bool get_repo_enabled();
        void set_repo_enabled(bool enable);
    private:
        _PkRepoDetail m_repo;
    };
}


#endif //PACKAGEKITMM_PKREPO_H
