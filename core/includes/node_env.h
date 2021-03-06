//
// Created by Subhabrata Ghosh on 22/11/16.
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

#ifndef REACTFS_NODE_ENV_H
#define REACTFS_NODE_ENV_H

#include <unordered_map>

#include "common/includes/common.h"
#include "common/includes/common_utils.h"
#include "common/includes/config.h"
#include "common/includes/__env.h"
#include "common/includes/mapped_data.h"
#include "common/includes/init_utils.h"
#include "common/includes/exclusive_lock.h"
#include "common/includes/__threads.h"
#include "common/includes/buffer_utils.h"

#include "node_env_structs.h"
#include "common_structs.h"

#define CONTROL_CONFIG_PATH "/configuration/node/control"

#define NODE_ENV_VERSION_MAJOR ((uint16_t) 0)
#define NODE_ENV_VERSION_MINOR ((uint16_t) 1)

using namespace com::wookler::reactfs::common;
using namespace com::wookler::reactfs::core;

namespace com {
    namespace wookler {
        namespace reactfs {
            namespace core {
                class __node_env {
                protected:
                    __version_header version;

                    __state__ state;
                    bool is_server = false;

                    exclusive_lock *lock = nullptr;
                    const __env *env = nullptr;

                    file_mapped_data *data_ptr = nullptr;
                    void *base_ptr = nullptr;
                    __env_header *header = nullptr;
                    unordered_map<string, shared_mapped_ptr> data_index;


                    void *get_write_pointer() {
                        PRECONDITION(header->write_offset < header->data_size);
                        void *ptr = buffer_utils::increment_data_ptr(base_ptr, header->write_offset);
                        CHECK_NOT_NULL(ptr);

                        return ptr;
                    }

                    void *write(string *key, void *data, uint32_t size) {
                        CHECK_STATE_AVAILABLE(state);
                        CHECK_NOT_NULL(data);
                        PRECONDITION((header->write_offset + size + sizeof(uint32_t)) <= header->data_size);

                        void *ptr = get_write_pointer();

                        memset(ptr, 0, SIZE_KEY_MAX);
                        memcpy(ptr, key->c_str(), key->length());

                        ptr = buffer_utils::increment_data_ptr(ptr, SIZE_KEY_MAX);
                        memset(ptr, 0, (size + sizeof(uint32_t)));
                        memcpy(ptr, &size, sizeof(uint32_t));

                        ptr = buffer_utils::increment_data_ptr(ptr, sizeof(uint32_t));
                        CHECK_NOT_NULL(ptr);
                        memcpy(ptr, data, size);

                        header->write_offset += (size + SIZE_KEY_MAX + sizeof(uint32_t));

                        return ptr;
                    }

                    void create_new_file(Path *p, uint64_t size) {
                        data_ptr = new file_mapped_data(p->get_path(), size, false);
                        CHECK_ALLOC(data_ptr, TYPE_NAME(file_mapped_data));
                        base_ptr = data_ptr->get_base_ptr();

                        header = static_cast<__env_header *>(base_ptr);
                        header->version.major = version.major;
                        header->version.minor = version.minor;
                        header->create_date = time_utils::now();
                        header->records = 0;
                        header->data_size = size;
                        header->write_offset = sizeof(__env_header);

                        data_ptr->flush();
                    }

                    void *get_read_ptr(uint32_t offset) {
                        void *ptr = base_ptr;
                        offset += sizeof(__env_header);

                        return buffer_utils::increment_data_ptr(ptr, offset);
                    }

                    bool is_valid_offset(uint32_t offset) {
                        if ((sizeof(__env_header) + offset) < header->data_size) {
                            return true;
                        }
                        return false;
                    }

                    void load_env_data(Path *p) {
                        data_ptr = new file_mapped_data(p->get_path());
                        CHECK_ALLOC(data_ptr, TYPE_NAME(file_mapped_data));
                        base_ptr = data_ptr->get_base_ptr();

                        header = static_cast<__env_header *>(base_ptr);
                        PRECONDITION(version_utils::compatible(header->version, version));

                        uint32_t count = 0;
                        uint32_t offset = 0;
                        __env_record *record = (__env_record *) malloc(sizeof(__env_record));
                        CHECK_ALLOC(record, TYPE_NAME(__env_record));
                        while (count < header->records && is_valid_offset(offset)) {
                            void *ptr = get_read_ptr(offset);
                            CHECK_NOT_NULL(ptr);
                            memset(record, 0, sizeof(__env_record));

                            memcpy(record->key, ptr, SIZE_KEY_MAX);
                            ptr = buffer_utils::increment_data_ptr(ptr, SIZE_KEY_MAX);

                            memcpy(&record->size, ptr, sizeof(uint32_t));
                            ptr = buffer_utils::increment_data_ptr(ptr, sizeof(uint32_t));

                            record->data = ptr;

                            shared_mapped_ptr s_ptr = make_shared<__mapped_ptr>();
                            (*s_ptr).set_data_ptr(ptr, record->size);

                            data_index.insert({string(record->key), s_ptr});

                            offset += (record->size + sizeof(uint32_t) + SIZE_KEY_MAX);

                            count++;
                        }
                        POSTCONDITION(count == header->records);
                    }


                    void create(bool reset) {
                        const Config *config = this->env->get_config();
                        CHECK_NOT_NULL(config);

                        const ConfigValue *node = config->find(NODE_ENV_CONFIG_NODE);
                        CHECK_NOT_NULL(node);

                        const BasicConfigValue *s_node = Config::get_value(NODE_ENV_CONFIG_SHM_SIZE, node);
                        CHECK_NOT_NULL(s_node);
                        string s_size = s_node->get_value();
                        POSTCONDITION(!IS_EMPTY(s_size));

                        uint64_t size = (uint64_t) common_utils::parse_size(s_size);
                        POSTCONDITION(size > 0);

                        string name_l = string(NODE_ENV_FILE_NAME);
                        CREATE_LOCK_P(lock, &name_l, DEFAULT_RESOURCE_MODE);
                        if (reset) {
                            lock->reset();
                        }

                        try {
                            WAIT_LOCK_GUARD(lock, 0);
                            const Path *w_dir = env->get_work_dir();
                            CHECK_NOT_NULL(w_dir);
                            Path p(w_dir->get_path());
                            if (is_server) {
                                p.append(NODE_ENV_BASE_DIR);
                                if (!p.exists()) {
                                    p.create(DEFAULT_RESOURCE_MODE);
                                }
                                p.append(NODE_ENV_FILE_NAME);
                                if (!p.exists()) {
                                    create_new_file(&p, size);
                                } else {
                                    load_env_data(&p);
                                }
                            } else {
                                p.append(NODE_ENV_BASE_DIR);
                                p.append(NODE_ENV_FILE_NAME);
                                PRECONDITION(p.exists());

                                load_env_data(&p);
                            }
                            state.set_state(__state_enum::Available);
                        } catch (const exception &e) {
                            base_error err = BASE_ERROR("Error occurred while creating node envirnoment. [error=%s]",
                                                        e.what());
                            state.set_error(&err);
                            throw err;
                        } catch (...) {
                            base_error err = BASE_ERROR("Error occurred while creating node envirnoment. [error=%s]",
                                                        "UNKNOWN ERROR");
                            state.set_error(&err);
                            throw err;
                        }
                    }


                public:
                    __node_env(bool is_server = false) {
                        this->env = env_utils::get_env();
                        CHECK_NOT_NULL(this->env);
                        CHECK_STATE_AVAILABLE(this->env->get_state());
                        this->is_server = is_server;

                        version.major = NODE_ENV_VERSION_MAJOR;
                        version.minor = NODE_ENV_VERSION_MINOR;
                    }

                    virtual ~__node_env() {
                        this->env = nullptr;
                        data_index.clear();
                        CHECK_AND_FREE(data_ptr);
                    }

                    __state__ get_state() {
                        return this->state;
                    }

                    shared_mapped_ptr* get_env_data(string key) {
                        unordered_map<string, shared_mapped_ptr>::iterator iter = data_index.find(key);
                        if (iter != data_index.end()) {
                            shared_mapped_ptr ptr = iter->second;
                            TRACE("Specified key data found. [key=%s][size=%lu]", key.c_str(), (*ptr).get_size());
                            return &iter->second;
                        }
                        TRACE("Specified key data not found. [key=%s]", key.c_str());
                        return nullptr;
                    }
                };
            }
        }
    }
}
#endif //REACTFS_NODE_ENV_H
