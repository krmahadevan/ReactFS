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
// Created by Subhabrata Ghosh on 15/09/16.
//

#ifndef WATERGATE_RESOURCE_DEF_H
#define WATERGATE_RESOURCE_DEF_H

#include <string>
#include <algorithm>

#include "common/includes/config.h"

#define CONFIG_NODE_RESOURCE_NAME "name"
#define CONFIG_NODE_RESOURCE_CLASS "class"

#define DEFAULT_LEASE_TIME 1000
#define DEFAULT_QUOTA -1.00

using namespace com::wookler::reactfs::common;
using namespace std;

namespace com {
    namespace wookler {
        namespace watergate {
            namespace core {
                enum resource_type_enum {
                    UNKNOWN = 0, IO, NET, FS
                };

                class resource_def : public __configurable {
                private:
                    resource_type_enum type = resource_type_enum::UNKNOWN;

                protected:
                    long lease_time = DEFAULT_LEASE_TIME;
                    double resource_quota = DEFAULT_QUOTA;

                    virtual void setup() override = 0;

                public:
                    resource_def(resource_type_enum type);

                    virtual ~resource_def() {}

                    resource_type_enum get_type();

                    virtual int get_control_size() = 0;

                    virtual const string *get_resource_name() = 0;

                    virtual bool accept(const string name) = 0;

                    long get_lease_time() {
                        return lease_time;
                    }

                    double get_resource_quota() {
                        return resource_quota;
                    }

                    static resource_type_enum parse_type(string value) {
                        transform(value.begin(), value.end(), value.begin(), static_cast<int (*)(int)>(toupper));

                        if (value == "IO") {
                            return resource_type_enum::IO;
                        } else if (value == "NET") {
                            return resource_type_enum::NET;
                        } else if (value == "FS") {
                            return resource_type_enum::FS;
                        }
                        return resource_type_enum::UNKNOWN;
                    }

                    static resource_type_enum parse_type(int value) {
                        switch (value) {
                            case 0:
                                return resource_type_enum::UNKNOWN;
                            case 1:
                                return resource_type_enum::IO;
                            case 2:
                                return resource_type_enum::NET;
                            case 3:
                                return resource_type_enum::FS;
                        }
                        return resource_type_enum::UNKNOWN;
                    }
                };

            }
        }
    }
}

#endif //WATERGATE_RESOURCE_DEF_H
