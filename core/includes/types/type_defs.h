//
// Created by Subhabrata Ghosh on 03/12/16.
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

#ifndef REACTFS_TYPE_DEFS_H
#define REACTFS_TYPE_DEFS_H

#include <type_traits>
#include <unordered_map>

#include "common/includes/common.h"
#include "common/includes/common_utils.h"
#include "common/includes/time_utils.h"
#include "common/includes/base_error.h"
#include "core/includes/core.h"

using namespace REACTFS_NS_COMMON_PREFIX;

REACTFS_NS_CORE
                namespace types {
                    /*!
                     * Enum defines the supported data types.
                     */
                    typedef enum __type_def_enum__ {
                        /// Unknown data type.
                                TYPE_UNKNOWN,
                        /// Boolean data type.
                                TYPE_BOOL,
                        /// Basic byte data type (8-bits)
                                TYPE_BYTE,
                        /// Basic char data type (8-bits)
                                TYPE_CHAR,
                        /// Short data type (16-bits)
                                TYPE_SHORT,
                        /// Integer data type (32-bits)
                                TYPE_INTEGER,
                        /// Long data type (64-bits)
                                TYPE_LONG,
                        /// Float data type
                                TYPE_FLOAT,
                        /// Double data type
                                TYPE_DOUBLE,
                        /// Timestamp data type (LONG)
                                TYPE_TIMESTAMP,
                        /// Date/time data type.
                                TYPE_DATETIME,
                        /// String data type (max size limited)
                                TYPE_STRING,
                        /// Text string datatype (no size limit)
                                TYPE_TEXT,
                        /// Array of Basic datatypes. (max size limited)
                                TYPE_ARRAY,
                        /// List of Basic datatypes. (no size limit)
                                TYPE_LIST,
                        /// Key/Value Map of basic types.
                                TYPE_MAP,
                        /// A complex structure.
                                TYPE_STRUCT
                    } __type_def_enum;

                    class __type_enum_helper {
                    public:
                        static bool is_native(__type_def_enum type) {
                            switch (type) {
                                case __type_def_enum::TYPE_TIMESTAMP:
                                case __type_def_enum::TYPE_BOOL:
                                case __type_def_enum::TYPE_BYTE:
                                case __type_def_enum::TYPE_DOUBLE:
                                case __type_def_enum::TYPE_SHORT:
                                case __type_def_enum::TYPE_CHAR:
                                case __type_def_enum::TYPE_FLOAT:
                                case __type_def_enum::TYPE_INTEGER:
                                case __type_def_enum::TYPE_LONG:
                                case __type_def_enum::TYPE_STRING:
                                case __type_def_enum::TYPE_DATETIME:
                                    return true;
                                default:
                                    return false;
                            }
                        }

                        static bool is_inner_type_valid(__type_def_enum type) {
                            if (is_native(type)) {
                                return true;
                            } else if (type == __type_def_enum::TYPE_STRUCT) {
                                return true;
                            }
                            return false;
                        }
                    };

                    class __base_datatype {
                    protected:
                        __type_def_enum type = __type_def_enum::TYPE_UNKNOWN;

                        __base_datatype(__type_def_enum type) {
                            PRECONDITION(type != __type_def_enum::TYPE_UNKNOWN);
                            this->type = type;
                        }

                    public:
                        __type_def_enum get_type() {
                            return this->type;
                        }

                        virtual uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) =0;

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...)  = 0;

                    };

                    template<typename __T>
                    class __datatype : public __base_datatype {
                    protected:

                        __datatype(__type_def_enum type) : __base_datatype(type) {
                        }

                        void *get_data_ptr(void *source, uint64_t size, uint64_t offset, uint64_t max_length) {
                            CHECK_NOT_NULL(source);
                            PRECONDITION((offset + size) < max_length);
                            return common_utils::increment_data_ptr(source, offset);
                        }

                    public:

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override =0;

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override = 0;

                    };

                    class __dt_byte : public __datatype<uint8_t> {

                    public:
                        __dt_byte() : __datatype(__type_def_enum::TYPE_BYTE) {
                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            uint8_t **T = (uint8_t **) t;

                            void *ptr = get_data_ptr(buffer, sizeof(uint8_t), offset, max_length);
                            *T = (uint8_t *) ptr;
                            return sizeof(uint8_t);
                        }

                        uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(uint8_t), offset, max_length);
                            memcpy(ptr, value, sizeof(uint8_t));
                            return sizeof(uint8_t);
                        }

                    };

                    class __dt_char : public __datatype<char> {

                    public:
                        __dt_char() : __datatype(__type_def_enum::TYPE_CHAR) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            char **T = (char **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(char), offset, max_length);
                            *T = (char *) ptr;
                            return sizeof(char);
                        }

                        uint64_t write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(char), offset, max_length);
                            memcpy(ptr, 0, sizeof(uint8_t));
                            return sizeof(char);
                        }

                    };

                    class __dt_bool : public __datatype<bool> {

                    public:
                        __dt_bool() : __datatype(__type_def_enum::TYPE_BOOL) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            bool **T = (bool **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(bool), offset, max_length);
                            *T = (bool *) ptr;
                            return sizeof(bool);
                        }

                        uint64_t write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(bool), offset, max_length);
                            memcpy(ptr, value, sizeof(bool));
                            return sizeof(bool);
                        }

                    };

                    class __dt_short : public __datatype<short> {

                    public:
                        __dt_short() : __datatype(__type_def_enum::TYPE_SHORT) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            short **T = (short **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(short), offset, max_length);
                            *T = (short *) ptr;
                            return sizeof(short);
                        }

                        uint64_t write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(short), offset, max_length);
                            memcpy(ptr, value, sizeof(short));
                            return sizeof(short);
                        }

                    };

                    class __dt_int : public __datatype<int> {

                    public:
                        __dt_int() : __datatype(__type_def_enum::TYPE_INTEGER) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            int **T = (int **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(int), offset, max_length);
                            *T = (int *) ptr;
                            return sizeof(int);
                        }

                        uint64_t write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(int), offset, max_length);
                            memcpy(ptr, value, sizeof(int));
                            return sizeof(int);
                        }

                    };

                    class __dt_long : public __datatype<long> {

                    public:
                        __dt_long() : __datatype(__type_def_enum::TYPE_LONG) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            long **T = (long **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(long), offset, max_length);
                            *T = (long *) ptr;
                            return sizeof(long);
                        }

                        uint64_t write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(long), offset, max_length);
                            memcpy(ptr, value, sizeof(long));
                            return sizeof(long);
                        }

                    };

                    class __dt_timestamp : public __datatype<uint64_t> {

                    public:
                        __dt_timestamp() : __datatype(__type_def_enum::TYPE_TIMESTAMP) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            uint64_t **T = (uint64_t **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(uint64_t), offset, max_length);
                            *T = (uint64_t *) ptr;
                            return sizeof(uint64_t);
                        }

                        uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(uint64_t), offset, max_length);
                            memcpy(ptr, value, sizeof(uint64_t));
                            return sizeof(uint64_t);
                        }

                    };

                    class __dt_datetime : public __dt_timestamp {

                    public:
                        __dt_datetime() {

                        }

                        uint64_t readstr(void *buffer, string *str, uint64_t offset, uint64_t max_length,
                                         const string &format = common_consts::EMPTY_STRING) {
                            uint64_t *ts = nullptr;
                            uint64_t r = read(buffer, &ts, offset, max_length);
                            CHECK_NOT_NULL(ts);
                            POSTCONDITION(r == sizeof(uint64_t));
                            if (!IS_EMPTY(format))
                                str->assign(time_utils::get_time_string(*ts, format));
                            else
                                str->assign(time_utils::get_time_string(*ts));
                            return r;
                        }

                        uint64_t writestr(void *buffer, string *value, uint64_t offset, uint64_t max_length,
                                          const string &format = common_consts::EMPTY_STRING) {
                            CHECK_NOT_EMPTY_P(value);
                            uint64_t ts = 0;
                            if (IS_EMPTY(format))
                                ts = time_utils::parse_time(*value);
                            else
                                ts = time_utils::parse_time(*value, format);
                            return write(buffer, &ts, offset, max_length);
                        }
                    };

                    class __dt_float : public __datatype<float> {

                    public:
                        __dt_float() : __datatype(__type_def_enum::TYPE_FLOAT) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            float **T = (float **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(float), offset, max_length);
                            *T = (float *) ptr;
                            return sizeof(float);
                        }

                        uint64_t write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(float), offset, max_length);
                            memcpy(ptr, value, sizeof(float));
                            return sizeof(float);
                        }

                    };

                    class __dt_double : public __datatype<double> {

                    public:
                        __dt_double() : __datatype(__type_def_enum::TYPE_DOUBLE) {

                        }

                        uint64_t read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            double **T = (double **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(double), offset, max_length);
                            *T = (double *) ptr;
                            return sizeof(double);
                        }

                        uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(double), offset, max_length);
                            memcpy(ptr, value, sizeof(double));
                            return sizeof(double);
                        }

                    };

                    class __dt_text : public __datatype<string> {

                    public:
                        __dt_text() : __datatype(__type_def_enum::TYPE_STRING) {

                        }

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            string **T = (string **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(uint64_t), offset, max_length);
                            uint64_t size = 0;
                            memcpy(&size, ptr, sizeof(uint64_t));
                            if (size > 0) {
                                uint64_t d_size = (size * sizeof(char));
                                ptr = common_utils::increment_data_ptr(ptr, sizeof(uint64_t));
                                PRECONDITION((offset + d_size + sizeof(uint64_t)) < max_length);
                                char *cptr = (char *) ptr;
                                *T = new string(cptr, d_size);
                                CHECK_ALLOC(*T, TYPE_NAME(string));
                                return (d_size + sizeof(uint64_t));
                            }
                            return sizeof(uint64_t);
                        }

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(uint64_t), offset, max_length);
                            string *s = (string *) value;
                            uint64_t size = (s->length() * sizeof(char));
                            memcpy(ptr, &size, sizeof(uint64_t));
                            if (size > 0) {
                                PRECONDITION((offset + sizeof(uint64_t) + size) < max_length);
                                ptr = common_utils::increment_data_ptr(ptr, sizeof(uint64_t));
                                memcpy(ptr, s->c_str(), s->length());
                                return (size + sizeof(uint64_t));
                            }
                            return sizeof(uint64_t);
                        }

                    };

                    class __dt_string : public __dt_text {

                    public:

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            string **T = (string **) t;
                            void *ptr = get_data_ptr(buffer, sizeof(uint64_t), offset, max_length);
                            uint16_t size = 0;
                            memcpy(&size, ptr, sizeof(uint16_t));
                            if (size > 0) {
                                uint64_t d_size = (size * sizeof(char));
                                ptr = common_utils::increment_data_ptr(ptr, sizeof(uint64_t));
                                PRECONDITION((offset + d_size + sizeof(uint64_t)) < max_length);
                                char *cptr = (char *) ptr;
                                *T = new string(cptr, d_size);
                                CHECK_ALLOC(*T, TYPE_NAME(string));
                                return (d_size + sizeof(uint16_t));
                            }
                            return sizeof(uint16_t);
                        }

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            void *ptr = get_data_ptr(buffer, sizeof(uint64_t), offset, max_length);
                            string *s = (string *) value;
                            PRECONDITION(s->length() <= USHRT_MAX);
                            uint16_t size = (s->length() * sizeof(char));
                            memcpy(ptr, &size, sizeof(uint16_t));
                            if (size > 0) {
                                PRECONDITION((offset + sizeof(uint16_t) + size) < max_length);
                                ptr = common_utils::increment_data_ptr(ptr, sizeof(uint16_t));
                                memcpy(ptr, s->c_str(), s->length());
                                return (size + sizeof(uint16_t));
                            }
                            return sizeof(uint16_t);
                        }
                    };

                    class __type_defs_utils {
                    private:
                        static unordered_map<string, __base_datatype *> type_handlers;

                    public:

                        static __base_datatype *get_type_handler(__type_def_enum type) {
                            PRECONDITION(__type_enum_helper::is_inner_type_valid(type));
                            string type_n = get_type_string(type);
                            unordered_map<string, __base_datatype *>::iterator iter = type_handlers.find(type_n);
                            if (iter != type_handlers.end()) {
                                return iter->second;
                            } else {
                                __base_datatype *ptr = create_type_handler(type);
                                CHECK_NOT_NULL(ptr);
                                type_handlers.insert({type_n, ptr});
                                return ptr;
                            }
                        }

                        static __base_datatype *create_type_handler(__type_def_enum type) {
                            __base_datatype *t = nullptr;
                            switch (type) {
                                case __type_def_enum::TYPE_BYTE:
                                    t = new __dt_byte();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_byte));
                                    break;
                                case __type_def_enum::TYPE_CHAR:
                                    t = new __dt_char();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_char));
                                    break;
                                case __type_def_enum::TYPE_DOUBLE:
                                    t = new __dt_double();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_double));
                                    break;
                                case __type_def_enum::TYPE_FLOAT:
                                    t = new __dt_float();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_float));
                                    break;
                                case __type_def_enum::TYPE_INTEGER:
                                    t = new __dt_int();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_int));
                                    break;
                                case __type_def_enum::TYPE_LONG:
                                    t = new __dt_long();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_long));
                                    break;
                                case __type_def_enum::TYPE_SHORT:
                                    t = new __dt_short();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_short));
                                    break;
                                case __type_def_enum::TYPE_STRING:
                                    t = new __dt_string();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_string));
                                    break;
                                case __type_def_enum::TYPE_STRUCT:
                                    t = new __dt_byte();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_byte));
                                    break;
                                case __type_def_enum::TYPE_TEXT:
                                    t = new __dt_text();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_text));
                                    break;
                                case __type_def_enum::TYPE_TIMESTAMP:
                                    t = new __dt_timestamp();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_timestamp));
                                    break;
                                case __type_def_enum::TYPE_BOOL:
                                    t = new __dt_bool();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_bool));
                                    break;
                                case __type_def_enum::TYPE_DATETIME:
                                    t = new __dt_datetime();
                                    CHECK_ALLOC(t, TYPE_NAME(__dt_datetime));
                                    break;
                                default:
                                    throw BASE_ERROR("Type not supported as basic type handler. [type=%s]",
                                                     get_type_string(type).c_str());
                            }
                            return t;
                        }

                        static string get_type_string(__type_def_enum type) {
                            switch (type) {
                                case __type_def_enum::TYPE_ARRAY:
                                    return "ARRAY";
                                case __type_def_enum::TYPE_BYTE:
                                    return "BYTE";
                                case __type_def_enum::TYPE_CHAR:
                                    return "CHAR";
                                case __type_def_enum::TYPE_DOUBLE:
                                    return "DOUBLE";
                                case __type_def_enum::TYPE_FLOAT:
                                    return "FLOAT";
                                case __type_def_enum::TYPE_INTEGER:
                                    return "INTEGER";
                                case __type_def_enum::TYPE_LIST:
                                    return "LIST";
                                case __type_def_enum::TYPE_LONG:
                                    return "LONG";
                                case __type_def_enum::TYPE_MAP:
                                    return "MAP";
                                case __type_def_enum::TYPE_SHORT:
                                    return "SHORT";
                                case __type_def_enum::TYPE_STRING:
                                    return "STRING";
                                case __type_def_enum::TYPE_STRUCT:
                                    return "STRUCT";
                                case __type_def_enum::TYPE_TEXT:
                                    return "TEXT";
                                case __type_def_enum::TYPE_TIMESTAMP:
                                    return "TIMESTAMP";
                                case __type_def_enum::TYPE_BOOL:
                                    return "BOOLEAN";
                                case __type_def_enum::TYPE_DATETIME:
                                    return "DATETIME";
                                default:
                                    return "EMPTY_STRING";
                            }
                        }

                        static __type_def_enum parse_type(const string &type) {
                            CHECK_NOT_EMPTY(type);
                            string t = string_utils::toupper(type);
                            if (t == get_type_string(__type_def_enum::TYPE_STRING)) {
                                return __type_def_enum::TYPE_STRING;
                            } else if (t == get_type_string(__type_def_enum::TYPE_INTEGER)) {
                                return __type_def_enum::TYPE_INTEGER;
                            } else if (t == get_type_string(__type_def_enum::TYPE_LONG)) {
                                return __type_def_enum::TYPE_LONG;
                            } else if (t == get_type_string(__type_def_enum::TYPE_DOUBLE)) {
                                return __type_def_enum::TYPE_DOUBLE;
                            } else if (t == get_type_string(__type_def_enum::TYPE_TIMESTAMP)) {
                                return __type_def_enum::TYPE_TIMESTAMP;
                            } else if (t == get_type_string(__type_def_enum::TYPE_TEXT)) {
                                return __type_def_enum::TYPE_TEXT;
                            } else if (t == get_type_string(__type_def_enum::TYPE_ARRAY)) {
                                return __type_def_enum::TYPE_ARRAY;
                            } else if (t == get_type_string(__type_def_enum::TYPE_BYTE)) {
                                return __type_def_enum::TYPE_BYTE;
                            } else if (t == get_type_string(__type_def_enum::TYPE_CHAR)) {
                                return __type_def_enum::TYPE_CHAR;
                            } else if (t == get_type_string(__type_def_enum::TYPE_FLOAT)) {
                                return __type_def_enum::TYPE_FLOAT;
                            } else if (t == get_type_string(__type_def_enum::TYPE_LIST)) {
                                return __type_def_enum::TYPE_LIST;
                            } else if (t == get_type_string(__type_def_enum::TYPE_MAP)) {
                                return __type_def_enum::TYPE_MAP;
                            } else if (t == get_type_string(__type_def_enum::TYPE_SHORT)) {
                                return __type_def_enum::TYPE_SHORT;
                            } else if (t == get_type_string(__type_def_enum::TYPE_BOOL)) {
                                return __type_def_enum::TYPE_BOOL;
                            } else if (t == get_type_string(__type_def_enum::TYPE_STRUCT)) {
                                return __type_def_enum::TYPE_STRUCT;
                            } else if (t == get_type_string(__type_def_enum::TYPE_DATETIME)) {
                                return __type_def_enum::TYPE_DATETIME;
                            }
                            return __type_def_enum::TYPE_UNKNOWN;
                        }
                    };

                    template<typename __T>
                    class __dt_array : public __datatype<__T> {
                    protected:
                        __type_def_enum inner_type;

                    public:
                        __dt_array(__type_def_enum inner_type) : __datatype<__T>(
                                __type_def_enum::TYPE_ARRAY) {
                            this->inner_type = inner_type;
                        }

                        __type_def_enum get_inner_type() {
                            return this->inner_type;
                        }

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            __base_datatype *type_handler = __type_defs_utils::get_type_handler(this->inner_type);
                            CHECK_NOT_NULL(type_handler);

                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            uint64_t r_count = *((uint64_t *) ptr);
                            __T **d_ptr = (__T **) t;
                            va_list vl;
                            va_start(vl, max_length);
                            uint64_t a_size = va_arg(vl, uint64_t);
                            va_end(vl);

                            PRECONDITION(a_size >= r_count);
                            if (a_size > 0) {
                                uint64_t r_offset = offset + sizeof(uint64_t);
                                uint64_t t_size = sizeof(uint64_t);
                                for (uint64_t ii = 0; ii < r_count; ii++) {
                                    uint64_t r = type_handler->read(buffer, d_ptr[ii], r_offset, max_length);
                                    r_offset += r;
                                    t_size += r;
                                }
                                return t_size;
                            }
                            return sizeof(uint64_t);
                        }

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            __base_datatype *type_handler = __type_defs_utils::get_type_handler(this->inner_type);
                            CHECK_NOT_NULL(type_handler);
                            va_list vl;
                            va_start(vl, max_length);
                            uint64_t a_size = va_arg(vl, uint64_t);
                            va_end(vl);

                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            memcpy(ptr, &a_size, sizeof(uint64_t));

                            if (a_size > 0) {
                                __T **d_ptr = (__T **) value;
                                uint64_t r_offset = offset + sizeof(uint64_t);
                                uint64_t t_size = sizeof(uint64_t);
                                for (uint64_t ii = 0; ii < a_size; ii++) {
                                    uint64_t r = type_handler->write(buffer, d_ptr[ii], r_offset, max_length);
                                    r_offset += r;
                                    t_size += r;
                                }
                                return t_size;
                            }
                            return sizeof(uint64_t);
                        }

                    };

                    template<typename __T>
                    class __dt_list : public __datatype<__T> {
                    protected:
                        __type_def_enum inner_type;

                    public:
                        __dt_list(__type_def_enum inner_type) : __datatype<__T>(
                                __type_def_enum::TYPE_LIST) {
                            this->inner_type = inner_type;
                        }

                        __type_def_enum get_inner_type() {
                            return this->inner_type;
                        }

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            __base_datatype *type_handler = __type_defs_utils::get_type_handler(this->inner_type);
                            CHECK_NOT_NULL(type_handler);

                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            uint64_t r_count = *((uint64_t *) ptr);
                            vector<__T *> **list = (vector<__T *> **) t;

                            if (r_count > 0) {
                                *list = new vector<__T *>(r_count);
                                CHECK_ALLOC(*list, TYPE_NAME(vector));

                                uint64_t r_offset = offset + sizeof(uint64_t);
                                uint64_t t_size = sizeof(uint64_t);
                                for (uint64_t ii = 0; ii < r_count; ii++) {
                                    __T *t = nullptr;
                                    uint64_t r = type_handler->read(buffer, &t, r_offset, max_length);
                                    CHECK_NOT_NULL(t);
                                    list[ii] = t;
                                    r_offset += r;
                                    t_size += r;
                                }
                                return t_size;
                            }
                            return sizeof(uint64_t);
                        }

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            __base_datatype *type_handler = __type_defs_utils::get_type_handler(this->inner_type);
                            CHECK_NOT_NULL(type_handler);

                            vector<__T *> *list = (vector<__T *>) value;

                            uint64_t a_size = list->size();
                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            memcpy(ptr, &a_size, sizeof(uint64_t));

                            if (a_size > 0) {
                                __T **d_ptr = (__T **) value;
                                uint64_t r_offset = offset + sizeof(uint64_t);
                                uint64_t t_size = sizeof(uint64_t);
                                for (uint64_t ii = 0; ii < a_size; ii++) {
                                    uint64_t r = type_handler->write(buffer, list[ii], r_offset, max_length);
                                    r_offset += r;
                                    t_size += r;
                                }
                                return t_size;
                            }
                            return sizeof(uint64_t);
                        }

                    };

                    template<typename __K, typename __V>
                    class __dt_map : public __datatype<__V> {
                    private:
                        __type_def_enum key_type;
                        __type_def_enum value_type;
                    public:
                        __dt_map(__type_def_enum key_type, __type_def_enum value_type) : __datatype<__V>(
                                __type_def_enum::TYPE_MAP) {
                            PRECONDITION(__type_enum_helper::is_inner_type_valid(value_type));
                            PRECONDITION(__type_enum_helper::is_native(key_type));
                            this->key_type = key_type;
                            this->value_type = value_type;
                        }

                        __type_def_enum get_key_type() {
                            return this->key_type;
                        }

                        __type_def_enum get_value_type() {
                            return this->value_type;
                        }

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            uint64_t r_count = *((uint64_t *) ptr);
                            if (r_count > 0) {
                                unordered_map<__K, __V *> **T = (unordered_map<__K, __V *> **) t;
                                *T = new unordered_map<__K, __V *>();

                                __base_datatype *kt_handler = __type_defs_utils::get_type_handler(this->key_type);
                                CHECK_NOT_NULL(kt_handler);
                                __base_datatype *vt_handler = __type_defs_utils::get_type_handler(this->value_type);
                                CHECK_NOT_NULL(vt_handler);

                                uint64_t r_offset = offset + sizeof(uint64_t);
                                uint64_t t_size = sizeof(uint64_t);

                                for (uint64_t ii = 0; ii < r_count; ii++) {
                                    __K *key = nullptr;
                                    __V *value = nullptr;

                                    uint64_t r = kt_handler->read(buffer, key, r_offset, max_length);
                                    CHECK_NOT_NULL(key);
                                    r_offset += r;
                                    t_size += r;
                                    r = vt_handler->read(buffer, value, r_offset, max_length);
                                    CHECK_NOT_NULL(value);
                                    r_offset += r;
                                    t_size += r;

                                    (*T)->insert({*key, value});
                                }

                                return t_size;
                            }
                            return sizeof(uint64_t);
                        }

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            unordered_map<__K, __V *> *map = (unordered_map<__K, __V *> *) value;
                            CHECK_NOT_NULL(map);

                            uint64_t m_size = map->size();
                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            memcpy(ptr, &m_size, sizeof(uint64_t));

                            if (!map->empty()) {
                                __base_datatype *kt_handler = __type_defs_utils::get_type_handler(this->key_type);
                                CHECK_NOT_NULL(kt_handler);
                                __base_datatype *vt_handler = __type_defs_utils::get_type_handler(this->value_type);
                                CHECK_NOT_NULL(vt_handler);

                                uint64_t r_offset = offset + sizeof(uint64_t);
                                uint64_t t_size = sizeof(uint64_t);

                                for (auto iter = map->begin(); iter != map->end(); iter++) {
                                    uint64_t r = kt_handler->write(buffer, iter->first, r_offset, max_length);
                                    r_offset += r;
                                    t_size += r;
                                    r = vt_handler->write(buffer, iter->second, r_offset, max_length);
                                    r_offset += r;
                                    t_size += r;
                                }
                                return t_size;
                            }
                            return sizeof(uint64_t);
                        }
                    };

                    class __type {
                    protected:
                        __type *parent = nullptr;
                        string name;
                        __type_def_enum type;
                        __base_datatype *handler = nullptr;
                        uint32_t max_size = 0;
                        bool nullable = true;

                    public:
                        __type(const string &name, __type_def_enum type, __type *parent = nullptr) {
                            PRECONDITION(__type_enum_helper::is_native(type));
                            this->parent = parent;
                            this->name = string(name);
                            this->handler = __type_defs_utils::get_type_handler(type);
                            this->type = type;
                        }

                        __type(const string &name, __type_def_enum type, __base_datatype *handler,
                               __type *parent = nullptr) {
                            CHECK_NOT_NULL(handler);
                            this->parent = parent;
                            this->name = string(name);
                            this->handler = handler;
                            this->type = type;
                        }

                        __type(const __type *type) {
                            this->parent = type->parent;
                            this->handler = type->handler;
                            this->name = string(type->name);
                            this->type = type->type;
                            this->max_size = type->max_size;
                            this->nullable = type->nullable;
                        }

                        __type(const __type &type) {
                            this->parent = type.parent;
                            this->handler = type.handler;
                            this->name = string(type.name);
                            this->type = type.type;
                            this->max_size = type.max_size;
                            this->nullable = type.nullable;
                        }

                        void set_max_size(uint32_t max_size) {
                            this->max_size = max_size;
                        }

                        uint32_t get_max_size() {
                            return this->max_size;
                        }

                        string get_name() {
                            return this->name;
                        }

                        __type_def_enum get_type() {
                            return this->type;
                        }

                        __base_datatype *get_handler() {
                            return this->handler;
                        }

                        __type *get_parent() {
                            return this->parent;
                        }

                        void set_nullable(bool nullable) {
                            this->nullable = nullable;
                        }

                        bool is_nullable() {
                            return this->nullable;
                        }

                        string get_canonical_name() {
                            if (IS_NULL(parent)) {
                                return name;
                            } else {
                                string p_name = parent->get_canonical_name();
                                CHECK_NOT_EMPTY(p_name);
                                return common_utils::format("%s.%s", p_name.c_str(), name.c_str());
                            }
                        }

                    };

                    class __dt_struct : public __base_datatype {
                    private:
                        __version_header *version;
                        vector<__type *> fields;

                        void *get_field_value(unordered_map<string, void *> *map, __type *type) {
                            CHECK_NOT_NULL(type);
                            unordered_map<string, void *>::iterator iter = map->find(type->get_canonical_name());
                            if (iter == map->end())
                                return nullptr;
                            return iter->second;
                        }

                    public:
                        __dt_struct(__version_header version) : __base_datatype(__type_def_enum::TYPE_STRUCT) {
                            this->version = (__version_header *) malloc(sizeof(__version_header));
                            CHECK_ALLOC(this->version, TYPE_NAME(__version_header));
                            this->version->major = version.major;
                            this->version->minor = version.minor;
                        }

                        ~__dt_struct() {
                            if (!IS_EMPTY(fields)) {
                                vector<__type *>::iterator iter;
                                for (iter = fields.begin(); iter != fields.end(); iter++) {
                                    CHECK_AND_FREE(*iter);
                                }
                                fields.clear();
                            }
                            FREE_PTR(this->version);
                        }

                        void add_field(uint16_t index, __type *type) {
                            CHECK_NOT_NULL(type);
                            if (index == fields.size()) {
                                fields.push_back(type);
                            } else if (index > fields.size()) {
                                for (uint16_t ii = fields.size(); ii <= index; ii++) {
                                    fields.push_back(nullptr);
                                }
                                fields[index] = type;
                            } else {
                                PRECONDITION(IS_NULL(fields[index]));
                                fields[index] = type;
                            }
                        }

                        bool remove_field(string name) {
                            if (!IS_EMPTY(fields)) {
                                vector<__type *>::iterator iter;
                                for (iter = fields.begin(); iter != fields.end(); iter++) {
                                    __type *t = *iter;
                                    if (t->get_canonical_name() == name) {
                                        break;
                                    }
                                }
                                if (iter != fields.end()) {
                                    fields.erase(iter);
                                    return true;
                                }
                            }
                            return false;
                        }

                        virtual uint64_t
                        read(void *buffer, void *t, uint64_t offset, uint64_t max_length, ...) override {
                            CHECK_NOT_NULL(t);
                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            __version_header *v = static_cast<__version_header *>(ptr);
                            CHECK_NOT_NULL(v);

                            POSTCONDITION(version_utils::compatible(*(this->version), *v));
                            unordered_map<string, void *> **T = (unordered_map<string, void *> **) t;
                            *T = new unordered_map<string, void *>();

                            vector<__type *>::iterator iter;
                            uint64_t r_offset = offset + sizeof(__version_header);
                            uint64_t t_size = sizeof(__version_header);
                            while (r_offset < max_length) {
                                ptr = common_utils::increment_data_ptr(buffer, r_offset);
                                uint8_t *ci = (uint8_t *) ptr;
                                POSTCONDITION(*ci < fields.size());
                                __type *type = fields[*ci];
                                CHECK_NOT_NULL(type);
                                __base_datatype *handler = type->get_handler();
                                CHECK_NOT_NULL(handler);
                                r_offset += sizeof(uint8_t);
                                t_size += sizeof(uint8_t);
                                uint64_t r = 0;
                                void *value = nullptr;
                                if (type->get_type() == __type_def_enum::TYPE_ARRAY) {
                                    uint32_t a_size = type->get_max_size();
                                    r = handler->read(buffer, &value, r_offset, max_length, a_size);
                                } else {
                                    r = handler->read(buffer, &value, r_offset, max_length);
                                }
                                (*T)->insert({type->get_canonical_name(), value});
                                t_size += r;
                                r_offset += r;
                            }
                            return t_size;
                        }

                        virtual uint64_t
                        write(void *buffer, void *value, uint64_t offset, uint64_t max_length, ...) override {
                            unordered_map<string, void *> *map = (unordered_map<string, void *> *) value;
                            CHECK_NOT_EMPTY_P(map);
                            void *ptr = common_utils::increment_data_ptr(buffer, offset);
                            memcpy(ptr, this->version, sizeof(__version_header));
                            uint64_t r_offset = offset + sizeof(__version_header);
                            uint64_t t_size = sizeof(__version_header);

                            vector<__type *>::iterator iter;
                            for (iter = fields.begin(); iter != fields.end(); iter++) {
                                __base_datatype *handler = (*iter)->get_handler();
                                CHECK_NOT_NULL(handler);
                                void *d = get_field_value(map, *iter);
                                if (IS_NULL(d) && !(*iter)->is_nullable()) {
                                    throw NOT_FOUND_ERROR("Null/Missing required field value. [field=%s]",
                                                          (*iter)->get_canonical_name().c_str());
                                } else if (IS_NULL(d)) {
                                    continue;
                                }
                                uint64_t r = handler->write(buffer, d, r_offset, max_length);
                                r_offset += r;
                                t_size += r;
                            }
                            return t_size;
                        }
                    };
                }
REACTFS_NS_CORE_END

#endif //REACTFS_TYPE_DEFS_H
