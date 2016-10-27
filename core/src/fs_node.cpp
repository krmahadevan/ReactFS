/* 
 * Copyright [2016] [Subhabrata Ghosh] 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at 
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0 
 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 *       limitations under the License. 
 * 
 */
//
// Created by Subhabrata Ghosh on 25/10/16.
//

#include "core/includes/fs_node.h"
#include "common/includes/log_utils.h"

com::wookler::reactfs::core::fs_node *com::wookler::reactfs::core::fs_node::start(string config_file) {
    try {
        env_utils::create_env(config_file);

        const __env *env = env_utils::get_env();
        CHECK_ENV_STATE(env);

        const Config *config = env->get_config();
        const ConfigValue *node = config->find(CONFIG_NODE_FS_PATH);
        CHECK_NOT_NULL(node);


        return this;
    } catch (const exception &e) {
        fs_error_base be = FS_BASE_ERROR_E(e);
        state.set_error(&be);
        LOG_ERROR(be.what());
        throw be;
    } catch (...) {
        fs_error_base be = FS_BASE_ERROR_E("Unknown error occurred.");
        state.set_error(&be);
        LOG_ERROR(be.what());
        throw be;
    }
}