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
// Created by Subhabrata Ghosh on 31/08/16.
//

#ifndef WATERGATE_CONTROL_OWNER_H
#define WATERGATE_CONTROL_OWNER_H

#include <stdio.h>
#include <mutex>

#include "common/includes/common_utils.h"
#include "common/includes/file_utils.h"
#include "common/includes/config.h"
#include "common/includes/__state__.h"
#include "common/includes/__alarm.h"
#include "common/includes/metrics.h"
#include "resource_def.h"
#include "control.h"
#include "control_errors.h"


#define CONFIG_DEF_NODE_PATH "./def"
#define DEFAULT_MAX_TIMEOUT 30 * 1000
#define DEFAULT_LOCK_LOOP_SLEEP_TIME 10
#define METRIC_LOCK_PREFIX "metrics.lock.time"
#define METRIC_LOCK_TIMEOUT_PREFIX "metrics.lock.timeouts"
#define METRIC_QUOTA_PREFIX "metrics.quota.total"
#define METRIC_QUOTA_REACHED_PREFIX "metrics.quota.exhausted"

using namespace com::wookler::reactfs::common;

namespace com {
    namespace wookler {
        namespace watergate {
            namespace core {
                class control_def {
                protected:
                    __state__ state;
                    unordered_map<string, __semaphore *> semaphores;

                    void add_resource_lock(const __app *app, const ConfigValue *config, bool server, bool overwrite);

                    __semaphore *get_lock(string name) const {
                        CHECK_STATE_AVAILABLE(state);
                        if (!IS_EMPTY(name)) {
                            unordered_map<string, __semaphore *>::const_iterator iter = semaphores.find(name);
                            if (iter != semaphores.end()) {
                                return iter->second;
                            }
                        }
                        return nullptr;
                    }

                    void create(const __app *app, const ConfigValue *config, bool server, bool overwrite);

                public:

                    virtual ~control_def();

                    virtual void init(const __app *app, const ConfigValue *config) = 0;

                    const __state__ get_state() const {
                        return state;
                    }

                    string get_metrics_name(string prefix, string name, int priority) const {
                        if (priority >= 0)
                            return common_utils::format("%s::%s::priority_%d", prefix.c_str(), name.c_str(), priority);
                        else
                            return common_utils::format("%s::%s", prefix.c_str(), name.c_str());
                    }
                };

                class control_client : public control_def {
                private:

                    __lock_state try_lock(string name, int priority, int base_priority, double quota) const;

                    __lock_state wait_lock(string name, int priority, int base_priority, double quota) const;

                    bool release_lock(string name, int priority, int base_priority) const;

                    __lock_state lock_get(string name, int priority, double quota, long timeout, int *err) const;

                    bool has_valid_lock(string name, int priority) const;


                public:
                    ~control_client() override {

                    }

                    void init(const __app *app, const ConfigValue *config) override {
                        create(app, config, false, false);
                    }

                    bool has_loaded_lock(string name) const;

                    const string find_lock(const string name, resource_type_enum type) const {
                        CHECK_STATE_AVAILABLE(state);
                        if (!IS_EMPTY(name)) {
                            unordered_map<string, __semaphore *>::const_iterator iter;
                            for (iter = semaphores.begin(); iter != semaphores.end(); iter++) {
                                __semaphore *sem = iter->second;
                                if (NOT_NULL(sem)) {
                                    __semaphore_client *c = static_cast<__semaphore_client *>(sem);
                                    CHECK_CAST(c, TYPE_NAME(__semaphore), TYPE_NAME(__semaphore_client));
                                    if (c->is_resource_type(type)) {
                                        if (c->accept(name)) {
                                            return *c->get_name();
                                        }
                                    }
                                }
                            }
                        }

                        return common_consts::EMPTY_STRING;
                    }

                    uint64_t get_quota(const string name) const {
                        __semaphore *sem = get_lock(name);
                        if (NOT_NULL(sem)) {
                            __semaphore_client *sem_c = static_cast<__semaphore_client *>(sem);
                            CHECK_CAST(sem_c, TYPE_NAME(__semaphore), TYPE_NAME(__semaphore_client));
                            return sem_c->get_quota();
                        }
                        return 0;
                    }

                    __lock_state lock(string name, int priority, double quota, int *err) const {
                        return lock(name, priority, quota, DEFAULT_MAX_TIMEOUT, err);
                    }

                    __lock_state lock(string name, int priority, double quota, uint64_t timeout, int *err) const {
                        CHECK_STATE_AVAILABLE(state);
                        CHECK_NOT_NULL(err);

                        string m_name = get_metrics_name(METRIC_LOCK_PREFIX, name, priority);
                        START_TIMER(m_name, 0);

                        timer t;
                        t.start();

                        __lock_state ret;
                        NEW_ALARM(DEFAULT_LOCK_LOOP_SLEEP_TIME * (priority + 1), 0);
                        while (true) {
                            *err = 0;
                            ret = lock_get(name, priority, quota, timeout, err);
                            if (ret != QuotaReached && ret != Retry) {
                                break;
                            }
                            START_ALARM(0);
                            if (t.get_current_elapsed() > timeout && (priority != 0)) {
                                TRACE("Lock call timeout. [name=%s][priority=%d][timeout=%lu]", name.c_str(), priority,
                                      timeout);
                                *err = ERR_CORE_CONTROL_TIMEOUT;
                                ret = Timeout;
                                break;
                            }
                        }
                        END_TIMER(m_name, 0);
                        return ret;
                    }

                    thread_lock_ptr *register_thread(string lock_name) const {
                        CHECK_STATE_AVAILABLE(state);

                        __semaphore *sem = get_lock(lock_name);
                        if (IS_NULL(sem)) {
                            throw CONTROL_ERROR("No registered lock with specified name. [name=%s]", lock_name.c_str());
                        }

                        __semaphore_client *sem_c = static_cast<__semaphore_client *>(sem);
                        CHECK_CAST(sem_c, TYPE_NAME(__semaphore), TYPE_NAME(__semaphore_client));
                        LOG_DEBUG("[pid=%d] Registering current thread.", getpid());

                        thread_lock_record *rec = sem_c->register_thread();
                        if (NOT_NULL(rec)) {
                            return rec->get_thread_ptr();
                        }
                        return nullptr;
                    }

                    bool release(string name, int priority) const;

                    void dump() const {
                        LOG_DEBUG("**************[REGISTERED CONTROLS:%d]**************", getpid());
                        if (!IS_EMPTY(semaphores)) {
                            unordered_map<string, __semaphore *>::const_iterator iter;
                            for (iter = semaphores.begin(); iter != semaphores.end(); iter++) {
                                __semaphore *sem = iter->second;
                                if (NOT_NULL(sem)) {
                                    __semaphore_client *c = static_cast<__semaphore_client *>(sem);
                                    CHECK_CAST(c, TYPE_NAME(__semaphore), TYPE_NAME(__semaphore_client));
                                    c->dump();
                                }
                            }
                        }
                        LOG_DEBUG("**************[REGISTERED CONTROLS:%d]**************", getpid());
                    }

                    void test_assert() const {
                        if (!IS_EMPTY(semaphores)) {
                            unordered_map<string, __semaphore *>::const_iterator iter;
                            for (iter = semaphores.begin(); iter != semaphores.end(); iter++) {
                                __semaphore *sem = iter->second;
                                if (NOT_NULL(sem)) {
                                    __semaphore_client *c = static_cast<__semaphore_client *>(sem);
                                    CHECK_CAST(c, TYPE_NAME(__semaphore), TYPE_NAME(__semaphore_client));
                                    c->test_assert();
                                }
                            }
                        }
                    }
                };
            }
        }
    }
}

#endif //WATERGATE_CONTROL_OWNER_H
