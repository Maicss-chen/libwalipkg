//
// Created by maicss on 2022/1/15.
//

#include "PkProgress.h"
#include "PkPackage.h"
int PackageKitMM::PkProgress::get_percentage() {
    return pk_progress_get_percentage(&m_progress);
}

int PackageKitMM::PkProgress::get_speed() {
    return pk_progress_get_speed(&m_progress);
}

int PackageKitMM::PkProgress::get_remaining_time() {
    return pk_progress_get_download_size_remaining(&m_progress)/ pk_progress_get_speed(&m_progress);
}

PackageKitMM::PkPackage PackageKitMM::PkProgress::get_package() {
    PkPackage pkPackage;
    pkPackage.m_pkPackage = *pk_progress_get_package(&m_progress);
    return pkPackage;
}
