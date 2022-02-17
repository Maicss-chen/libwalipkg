//
// Created by maicss on 2022/1/15.
//
#include "WaliPkg.h"
#include "WaliSchedule.h"

int WaliSchedule::get_percentage() {
    return pk_progress_get_percentage(&m_progress);
}

int WaliSchedule::get_speed() {
    return pk_progress_get_speed(&m_progress);
}

int WaliSchedule::get_remaining_time() {
    return pk_progress_get_download_size_remaining(&m_progress)/ pk_progress_get_speed(&m_progress);
}

WaliPkg WaliSchedule::get_package() {
    WaliPkg pkPackage;
    pkPackage.m_pkPackage = *pk_progress_get_package(&m_progress);
    return pkPackage;
}
