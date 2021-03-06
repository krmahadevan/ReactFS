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
// Created by Subhabrata Ghosh on 10/10/16.
//

#ifndef WATERGATE_FILESYSTEM_DRIVER_H
#define WATERGATE_FILESYSTEM_DRIVER_H

#include <stdlib.h>
#include <unordered_map>

#include "resource_creator.h"

#ifdef __APPLE__

#include <sys/syslimits.h>

#elif __linux__
#include <linux/limits.h>
#endif

#include "resource_def.h"
#include "common/includes/file_utils.h"
#include "common/includes/log_utils.h"

using namespace com::wookler::watergate::core;

namespace com {
    namespace wookler {
        namespace watergate {
            namespace core {
                namespace io {
                    class fs_driver_constants {
                    public:
                        static const string CONFIG_PARAM_ROOT_PATH;
                        static const string CONFIG_PARAM_QUOTA_BYTES;
                        static const string CONFIG_PARAM_QUOTA_LEASE_TIME;
                        static const string CONFIG_PARAM_MAX_CONCURRENCY;
                        static const string FS_RESOURCE_CLASS;
                    };

                    class filesystem_driver : public resource_def {
                    private:
                        Path *root_path;
                        int concurrency;

                    protected:
                        void setup() override;

                    public:
                        filesystem_driver() : resource_def(FS) {}

                        ~filesystem_driver() {
                            CHECK_AND_FREE(root_path);
                        }

                        int get_control_size() override {
                            return concurrency;
                        }

                        const string *get_resource_name() override;

                        bool accept(const string name) override {
                            CHECK_NOT_EMPTY(name);
                            char path[PATH_MAX];
                            memset(path, 0, PATH_MAX);

                            if (NOT_NULL(realpath(name.c_str(), path))) {
                                LOG_DEBUG("Requested file path [%s]", path);

                                string str(path);
                                if (!IS_EMPTY(str)) {
                                    size_t pos = str.find(root_path->get_path());
                                    if (pos == 0) {
                                        return true;
                                    }
                                }
                            }
                            return false;
                        }
                    };

                    class filesystem_driver_creator : public resource_creator {
                    public:
                        resource_def *create(string name, const ConfigValue *node) override {
                            CHECK_NOT_NULL(node);
                            const BasicConfigValue *cn = Config::get_value(CONFIG_NODE_RESOURCE_CLASS, node);
                            CHECK_NOT_NULL(cn);
                            POSTCONDITION(cn->get_value() == fs_driver_constants::FS_RESOURCE_CLASS);

                            filesystem_driver *resource = new filesystem_driver();
                            CHECK_ALLOC(resource, TYPE_NAME(filesystem_driver));
                            resource->configure(node);

                            return resource;
                        }
                    };
                }
            }
        }
    }
}

#endif //WATERGATE_FILESYSTEM_DRIVER_H
