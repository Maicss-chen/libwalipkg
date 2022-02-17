//
// Created by maicss on 2022/1/14.
//

#include "WaliRepo.h"
#include "log.h"

#include <string>

std::string WaliRepo::get_repo_id() {
    return pk_repo_detail_get_id(&m_repo);
}

std::string WaliRepo::get_repo_description() {
    return pk_repo_detail_get_description(&m_repo);
}

bool WaliRepo::get_repo_enabled() {
    return pk_repo_detail_get_enabled(&m_repo);
}

void WaliRepo::set_repo_enabled(bool enable) {
    PkTask *pkTask = pk_task_new();
    GError *error;
    gchar *id = g_strdup(pk_repo_detail_get_id(&m_repo));
    pk_task_repo_enable_sync(pkTask,id,enable, nullptr, nullptr, nullptr, &error);
    if (error){
        log(LOG_FUNCTION_NAME std::string(error->message), ERROR);
        g_error_free (error);
    }
}
