//
// Created by Subhabrata Ghosh on 24/11/16.
//


/*
 * Copyright [2016] [Subhabrata Ghosh]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef REACTFS_NODE_SERVER_ENV_H
#define REACTFS_NODE_SERVER_ENV_H

#include "common/includes/__threads.h"
#include "common/includes/read_write_lock_manager.h"
#include "common/includes/shared_lock_utils.h"
#include "watergate/includes/init_utils.h"
#include "core/includes/node_env.h"
#include "core/includes/block_shared_defs.h"


#define DEFAULT_THREAD_POOL_NAME "__DEFAULT_THREAD_POOL_"
#define DEFAULT_THREAD_POOL_SLEEP 5000

using namespace com::wookler::reactfs::core;

namespace com {
    namespace wookler {
        namespace reactfs {
            namespace core {
                namespace server {

                    class node_server_env : public __node_env {
                    private:
                        control_manager *priority_manager = nullptr;
                        write_lock_manager *block_lock_manager = nullptr;
                        vector<__runnable_callback *> callbacks;

                        __thread_pool *default_pool = nullptr;

                    public:
                        node_server_env() : __node_env(true) {

                        }

                        ~node_server_env() {
                            CHECK_AND_DISPOSE(state);

                            if (!IS_EMPTY(callbacks)) {
                                for (auto cb : callbacks) {
                                    if (!IS_NULL(cb)) {
                                        default_pool->remove_task(cb->get_name());
                                        CHECK_AND_FREE(cb);
                                    }
                                }
                            }
                            if (NOT_NULL(default_pool)) {
                                default_pool->stop();
                            }
                            CHECK_AND_FREE(priority_manager);
                            CHECK_AND_FREE(default_pool);
                            shared_lock_utils::dispose();
                        }

                        void init(bool reset) {
                            create(reset);
                            try {
                                priority_manager = init_utils::init_control_manager(env, CONTROL_CONFIG_PATH, reset,
                                                                                    false);
                                CHECK_NOT_NULL(priority_manager);

                                const Config *config = this->env->get_config();
                                CHECK_NOT_NULL(config);

                                const ConfigValue *node = config->find(NODE_ENV_CONFIG_THREAD_POOL);
                                CHECK_NOT_NULL(node);

                                const BasicConfigValue *s_node = Config::get_value(NODE_ENV_CONFIG_POOL_SIZE, node);
                                CHECK_NOT_NULL(s_node);
                                string s_size = s_node->get_value();
                                POSTCONDITION(!IS_EMPTY(s_size));

                                int p_size = atoi(s_size.c_str());
                                POSTCONDITION(p_size > 0);

                                default_pool = new __thread_pool(DEFAULT_THREAD_POOL_NAME, p_size);
                                CHECK_ALLOC(default_pool, TYPE_NAME(__thread_pool));
                                default_pool->create_task_registry(DEFAULT_THREAD_POOL_SLEEP);
                                default_pool->start();

                                block_lock_manager = shared_lock_utils::get()->create_w_manager(BLOCK_LOCK_GROUP,
                                                                                                DEFAULT_LOCK_MODE,
                                                                                                false, reset);
                                CHECK_NOT_NULL(block_lock_manager);

                                __runnable_callback *rwcb = new w_lock_manager_callback(
                                        block_lock_manager);
                                CHECK_ALLOC(rwcb, TYPE_NAME(__runnable_callback));
                                default_pool->add_task(rwcb);
                                callbacks.push_back(rwcb);

                                __runnable_callback *cb = new control_manager_callback(priority_manager);
                                CHECK_ALLOC(cb, TYPE_NAME(__runnable_callback));
                                default_pool->add_task(cb);
                                callbacks.push_back(cb);

                            } catch (const exception &e) {
                                base_error err = BASE_ERROR(
                                        "Error occurred while creating node envirnoment. [error=%s]",
                                        e.what());
                                state.set_error(&err);
                                throw err;
                            } catch (...) {
                                base_error err = BASE_ERROR(
                                        "Error occurred while creating node envirnoment. [error=%s]",
                                        "UNKNOWN ERROR");
                                state.set_error(&err);
                                throw err;
                            }
                        }


                        bool add_env_data(__env_loader *loader) {
                            CHECK_STATE_AVAILABLE(state);
                            CHECK_NOT_NULL(loader);
                            PRECONDITION(!IS_EMPTY_P(loader->get_key()));
                            PRECONDITION(!IS_EMPTY_P(loader->get_config_path()));

                            const Config *config = env->get_config();
                            const ConfigValue *node = config->find(*loader->get_config_path());

                            __env_record *record = loader->load(node);
                            if (NOT_NULL(record)) {
                                shared_mapped_ptr s_ptr = make_shared<__mapped_ptr>();
                                if (!record->saved) {
                                    void *ptr = write(loader->get_key(), record->data, record->size);
                                    CHECK_NOT_NULL(ptr);
                                    header->records++;
                                    (*s_ptr).set_data_ptr(ptr, record->size);

                                    data_index.insert({*(loader->get_key()), s_ptr});
                                } else {
                                    (*s_ptr).set_data_ptr(record->data, record->size);
                                }
                                loader->load_finished(s_ptr);

                                return true;
                            }
                            return false;
                        }

                        control_manager *get_priority_manager() {
                            CHECK_STATE_AVAILABLE(state);
                            return priority_manager;
                        }

                        void add_runnable(__runnable_callback *runnable) {
                            CHECK_NOT_NULL(runnable);
                            default_pool->add_task(runnable);
                            callbacks.push_back(runnable);
                        }

                        bool remove_runnable(string name) {
                            PRECONDITION(!IS_EMPTY(name));
                            bool r = default_pool->remove_task(name);
                            int index = -1;
                            for (uint32_t ii = 0; ii < callbacks.size(); ii++) {
                                __runnable_callback *rc = callbacks[ii];
                                if (NOT_NULL(rc)) {
                                    if (rc->get_name() == name) {
                                        index = ii;
                                        break;
                                    }
                                }
                            }
                            vector<__runnable_callback *>::iterator iter = callbacks.begin();
                            if (index >= 0) {
                                callbacks.erase(iter + index);
                            }
                            return r;
                        }
                    };

                }
            }
        }
    }
}
#endif //REACTFS_NODE_SERVER_ENV_H
