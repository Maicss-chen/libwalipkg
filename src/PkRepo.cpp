//
// Created by maicss on 2022/1/14.
//

#include "PkRepo.h"

std::string PackageKitMM::PkRepo::get_repo_id() {
    return pk_repo_detail_get_id(&m_repo);
}

std::string PackageKitMM::PkRepo::get_repo_description() {
    return pk_repo_detail_get_description(&m_repo);
}

bool PackageKitMM::PkRepo::get_repo_enabled() {
    return pk_repo_detail_get_enabled(&m_repo);
}
