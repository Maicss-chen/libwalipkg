//
// Created by maicss on 2022/1/14.
//

#ifndef PACKAGEKITMM_WALIREPO_H
#define PACKAGEKITMM_WALIREPO_H
#ifndef I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#define I_KNOW_THE_PACKAGEKIT_GLIB2_API_IS_SUBJECT_TO_CHANGE
#endif
#include "packagekit-glib2/packagekit.h"
#include <string>

class WaliPkgCmd;
class WaliRepo {
public:
    friend WaliPkgCmd;
    std::string get_repo_id();
    std::string get_repo_description();
    bool get_repo_enabled();
    void set_repo_enabled(bool enable);
private:
    _PkRepoDetail m_repo;
};

#endif //PACKAGEKITMM_WALIREPO_H
