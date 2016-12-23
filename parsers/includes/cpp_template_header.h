#ifndef REACTFS_CPP_TEMPLATE_HEADER_H
#define REACTFS_CPP_TEMPLATE_HEADER_H
#include <unordered_map>
#include "common/includes/common.h"
#include "common/includes/common_utils.h"
#include "common/includes/base_error.h"
#include "common/includes/log_utils.h"
#include "core/includes/core.h"

using namespace REACTFS_NS_COMMON_PREFIX;
REACTFS_NS_CORE
namespace parsers {		
					class cpp_template_header {
					private:
						unordered_map<string, vector<string> *> __cpp_template;

						void __init() {
							std::vector<string> *values = nullptr;

							// KEY [FILE_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FILE_DEF", values});
							values->push_back("${header}");
							values->push_back("");
							values->push_back("#ifndef ${guard_name}");
							values->push_back("#define ${guard_name}");
							values->push_back("${includes}");
							values->push_back("");
							values->push_back("${body}");
							values->push_back("#endif // ${guard_name}");
							// END KEY [FILE_DEF]

							// KEY [NAMESPACE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"NAMESPACE", values});
							values->push_back("namespace ${name} {");
							values->push_back("    ${nested}");
							values->push_back("}");
							// END KEY [NAMESPACE]

							// KEY [FUNC_TYPE_MAP_SERIALIZER]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_TYPE_MAP_SERIALIZER", values});
							values->push_back("std::unordered_map<${key_type}, __struct_datatype__ *> *serialize_${name}(std::unordered_map<${key_type}, ${value_type *}> *__i_map) {");
							values->push_back("    CHECK_NOT_EMPTY_P(map);");
							values->push_back("    std::unordered_map<${key_type}, __struct_datatype__ *> *__map = new std::unordered_map<${key_type}, __struct_datatype__ *>();");
							values->push_back("    CHECK_ALLOC(__map, TYPE_NAME(unordered_map));");
							values->push_back("");
							values->push_back("    std::unordered_map<${key_type}, ${value_type *}>::iterator iter;");
							values->push_back("    for(iter = __i_map->begin(); iter != __i_map.end(); iter++) {");
							values->push_back("        const ${key_type} key = iter->first;");
							values->push_back("        ${value_type} *value = iter->second;");
							values->push_back("        CHECK_NOT_NULL(value);");
							values->push_back("        __struct_datatype__ *rv = this->serialize_${value_type}(value);");
							values->push_back("        CHECK_NOT_NULL(rv);");
							values->push_back("        __i_map->insert({key, rv});");
							values->push_back("    }");
							values->push_back("    return __map;");
							values->push_back("}");
							// END KEY [FUNC_TYPE_MAP_SERIALIZER]

							// KEY [FUNC_TYPE_LIST_SERIALIZER]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_TYPE_LIST_SERIALIZER", values});
							values->push_back("std::vector<__struct_datatype__ *> *serialize_${name}(std::vector<${type} *> *__value) {");
							values->push_back("    CHECK_NOT_EMPTY_P(__value);");
							values->push_back("    std::vector<__struct_datatype__ *> *__list = new std::vector<__struct_datatype__ *>();");
							values->push_back("    CHECK_ALLOC(__list, TYPE_NAME(vector));");
							values->push_back("    for (${type} *rec : *(__value)) {");
							values->push_back("        CHECK_NOT_NULL(rec);");
							values->push_back("        __struct_datatype__ *rv = this->serialize_${type}(rec);");
							values->push_back("        CHECK_NOT_NULL(rv);");
							values->push_back("        __list->push_back(rv);");
							values->push_back("    }");
							values->push_back("    return __list;");
							values->push_back("}");
							// END KEY [FUNC_TYPE_LIST_SERIALIZER]

							// KEY [FUNC_TYPE_SERIALIZER]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_TYPE_SERIALIZER", values});
							values->push_back("void *serialize_${type}(${type} *__value) {");
							values->push_back("    CHECK_NOT_NULL(__value);");
							values->push_back("    __struct_datatype__ *__data = new __struct_datatype__();");
							values->push_back("    CHECK_ALLOC(__data, TYPE_NAME(__struct_datatype__));");
							values->push_back("    ${set_map_calls}");
							values->push_back("    return __data;");
							values->push_back("}");
							// END KEY [FUNC_TYPE_SERIALIZER]

							// KEY [FUNC_DESERIALIZE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_DESERIALIZE", values});
							values->push_back("void deserialize(__struct_datatype__ *__data) {");
							values->push_back("    CHECK_NOT_NULL(__data);");
							values->push_back("    ${read_map_calls};");
							values->push_back("}");
							// END KEY [FUNC_DESERIALIZE]

							// KEY [CALL_SETTER_FROM_MAP]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"CALL_SETTER_FROM_MAP", values});
							values->push_back("this->set_value_${name}(${data_var});");
							// END KEY [CALL_SETTER_FROM_MAP]

							// KEY [FUNC_SETTER_TO_MAP]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_SETTER_TO_MAP", values});
							values->push_back("void set_map_${name}(__struct_datatype__ *__data) {");
							values->push_back("    CHECK_NOT_NULL(__data);");
							values->push_back("    if (NOT_NULL(this->${name})) {");
							values->push_back("        if (this->is_native(\"${type}\")) {");
							values->push_back("            __data->insert({\"${name}\", this->${name}});");
							values->push_back("        } else if (this->is_list(\"${type}\") || this->is_map(\"${type}\")) {");
							values->push_back("            void *__ptr = serialize_${type}(this->${name});");
							values->push_back("            CHECK_NOT_NULL(__ptr);");
							values->push_back("            __data->insert({\"${name}\", __ptr});");
							values->push_back("        } else {");
							values->push_back("            void *__ptr = serialize_${type}(this->${name});");
							values->push_back("            CHECK_NOT_NULL(__ptr);");
							values->push_back("            __data->insert({\"${name}\", __ptr});");
							values->push_back("        }");
							values->push_back("    }");
							values->push_back("}");
							// END KEY [FUNC_SETTER_TO_MAP]

							// KEY [FUNC_SETTER_FROM_MAP]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_SETTER_FROM_MAP", values});
							values->push_back("void set_value_${name}(__struct_datatype__ *__data) {");
							values->push_back("    if (NOT_NULL(__data)) {");
							values->push_back("        __struct_datatype__::const_iterator iter = __data->find(\"${name}\");");
							values->push_back("        if (iter != __data->end()) {");
							values->push_back("            void *__ptr = iter->second;");
							values->push_back("            if (NOT_NULL(__ptr)) {");
							values->push_back("                if (this->is_native(\"${type}\")) {");
							values->push_back("                    ${type} *__var = static_cast<${type} *>(__ptr);");
							values->push_back("                    CHECK_CAST(__var, TYPE_NAME(void *), TYPE_NAME(${type}));");
							values->push_back("                    this->${name} = __var;");
							values->push_back("                } else {");
							values->push_back("                    ${type} *__var = deserialze_${name}(__ptr);");
							values->push_back("                    CHECK_NOT_NULL(__var);");
							values->push_back("                    this->${name} = __var;");
							values->push_back("                }");
							values->push_back("            } else {");
							values->push_back("                this->${name} = nullptr;");
							values->push_back("            }");
							values->push_back("        } else {");
							values->push_back("            this->${name} = nullptr;");
							values->push_back("        }");
							values->push_back("    } else {");
							values->push_back("        this->${name} = nullptr;");
							values->push_back("    }");
							values->push_back("}");
							// END KEY [FUNC_SETTER_FROM_MAP]

							// KEY [CLASS_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"CLASS_DEF", values});
							values->push_back("class ${name} ${parent} {");
							values->push_back("private:");
							values->push_back("    ${declarations}");
							values->push_back("");
							values->push_back("    ${private_functions}");
							values->push_back("public:");
							values->push_back("    ${constructor}");
							values->push_back("");
							values->push_back("    ${destructor}");
							values->push_back("");
							values->push_back("    ${public_functions}");
							values->push_back("};");
							// END KEY [CLASS_DEF]

							// KEY [FUNC_MAP_NATIVE_ADD_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_MAP_NATIVE_ADD_DEF", values});
							values->push_back("void add_to_${name}(${key_type} ${v_key}, ${value_type} &${v_value}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    if (IS_NULL(this->${name})) {");
							values->push_back("        this->${name} = new std::unordered_map<${key_type}, ${value_type} *>();");
							values->push_back("        CHECK_ALLOC(this->${name}, TYPE_NAME(unordered_map));");
							values->push_back("    }");
							values->push_back("    ${value_type} *__var = (${value_type} *)malloc(sizeof(${value_type}));");
							values->push_back("    CHECK_ALLOC(__var, TYPE_NAME(${value_type}));");
							values->push_back("    *__var = ${value};");
							values->push_back("    this->${name}->insert(${key}, __var});");
							values->push_back("}");
							// END KEY [FUNC_MAP_NATIVE_ADD_DEF]

							// KEY [VARIABLE_TYPE_FREE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_TYPE_FREE", values});
							values->push_back("CHECK_AND_FREE(this->${name});");
							// END KEY [VARIABLE_TYPE_FREE]

							// KEY [VARIABLE_MAP_NATIVE_FREE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_MAP_NATIVE_FREE", values});
							values->push_back("for(auto kv = this->${name}->being(); kv != this->${name}->end(); kv++) {");
							values->push_back("    FREE_PTR(kv->second);");
							values->push_back("}");
							values->push_back("CHECK_AND_FREE(this->${name});");
							// END KEY [VARIABLE_MAP_NATIVE_FREE]

							// KEY [FUNC_LIST_TYPE_ADD_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_LIST_TYPE_ADD_DEF", values});
							values->push_back("void add_to_${name}(${type} *${name}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    CHECK_NOT_NULL(${name});");
							values->push_back("    if (IS_NULL(this->${name})) {");
							values->push_back("        this->${name} = new std::vector<${type} *>();");
							values->push_back("        CHECK_ALLOC(this->${name}, TYPE_NAME(vector));");
							values->push_back("    }");
							values->push_back("    his->${name}->push_back(${name});");
							values->push_back("}");
							// END KEY [FUNC_LIST_TYPE_ADD_DEF]

							// KEY [FILE_COMMENT]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FILE_COMMENT", values});
							values->push_back("/*!");
							values->push_back(" *  Auto-generated types for schema definition [source schema: ${schema_name}]");
							values->push_back(" *");
							values->push_back(" *  Reference Schema Version : ${version}");
							values->push_back(" *  Generated by : ${username}");
							values->push_back(" *  Generated on : ${date}");
							values->push_back(" */");
							// END KEY [FILE_COMMENT]

							// KEY [FUNC_STRING_SETTER_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_STRING_SETTER_DEF", values});
							values->push_back("void set_${name}(string &${name}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    FREE_PTR(this->${name});");
							values->push_back("    if (!IS_EMPTY(${name}) {");
							values->push_back("        uint32_t __size = name.length() + 1;");
							values->push_back("        this->${name} = (char) malloc(sizeof(char) * __size);");
							values->push_back("        CHECK_ALLOC(this->${name}, TYPE_NAME(char *));");
							values->push_back("        memset(this->${name}, 0, __size);");
							values->push_back("        memcpy(this->${name}, ${name}.c_str(), (__size - 1));");
							values->push_back("    }");
							values->push_back("}");
							// END KEY [FUNC_STRING_SETTER_DEF]

							// KEY [VARIABLE_LIST_TYPE_FREE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_LIST_TYPE_FREE", values});
							values->push_back("for(auto elem : this->${name}) {");
							values->push_back("    CHECK_AND_FREE(elem);");
							values->push_back("}");
							values->push_back("CHECK_AND_FREE(this->${name});");
							// END KEY [VARIABLE_LIST_TYPE_FREE]

							// KEY [FUNC_MAP_NATIVE_TYPE_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_MAP_NATIVE_TYPE_DEF", values});
							values->push_back("void add_to_${name}(${key_type} ${v_key}, ${value_type} *${v_value}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    if (IS_NULL(this->${name})) {");
							values->push_back("        this->${name} = new std::unordered_map<${key_type}, ${value_type} *>();");
							values->push_back("        CHECK_ALLOC(this->${name}, TYPE_NAME(unordered_map));");
							values->push_back("    }");
							values->push_back("    his->${name}->insert(${key}, ${value}});");
							values->push_back("}");
							// END KEY [FUNC_MAP_NATIVE_TYPE_DEF]

							// KEY [INCLUDE_BASE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"INCLUDE_BASE", values});
							values->push_back("#include \"types/includes/type_defs.h\"");
							values->push_back("#include \"types/includes/__base_type.h\"");
							values->push_back("");
							values->push_back("${includes}");
							values->push_back("");
							values->push_back("using namespace REACTFS_NS_COMMON_PREFIX;");
							values->push_back("using namespace REACTFS_NS_CORE_PREFIX::types;");
							values->push_back("");
							// END KEY [INCLUDE_BASE]

							// KEY [FUNC_GETTER_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_GETTER_DEF", values});
							values->push_back("${return} get_${name}(void) {");
							values->push_back("    return this->${name};");
							values->push_back("}");
							// END KEY [FUNC_GETTER_DEF]

							// KEY [VARIABLE_NATIVE_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_NATIVE_DEF", values});
							values->push_back("${type} *${name} = nullptr;");
							// END KEY [VARIABLE_NATIVE_DEF]

							// KEY [FUNC_NATIVE_SETTER_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_NATIVE_SETTER_DEF", values});
							values->push_back("void set_${name}(${type} ${name}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    FREE_PTR(this->${name});");
							values->push_back("    this->${name} = (${type} *)malloc(sizeof(${type});");
							values->push_back("    CHECK_ALLOC(this->${name}, TYPE_NAME(${type}));");
							values->push_back("    *(this->${name}) = ${name};");
							values->push_back("}");
							// END KEY [FUNC_NATIVE_SETTER_DEF]

							// KEY [VARIABLE_LIST_NATIVE_FREE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_LIST_NATIVE_FREE", values});
							values->push_back("for(auto elem : this->${name}) {");
							values->push_back("    FREE_PTR(elem);");
							values->push_back("}");
							values->push_back("CHECK_AND_FREE(this->${name});");
							// END KEY [VARIABLE_LIST_NATIVE_FREE]

							// KEY [VARIABLE_MAP_TYPE_FREE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_MAP_TYPE_FREE", values});
							values->push_back("for(auto kv = this->${name}->being(); kv != this->${name}->end(); kv++) {");
							values->push_back("    CHECK_AND_FREE(kv->second);");
							values->push_back("}");
							values->push_back("CHECK_AND_FREE(this->${name});");
							// END KEY [VARIABLE_MAP_TYPE_FREE]

							// KEY [VARIABLE_NATIVE_FREE]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_NATIVE_FREE", values});
							values->push_back("FREE_PTR(this->${name});");
							// END KEY [VARIABLE_NATIVE_FREE]

							// KEY [CALL_SETTER_TO_MAP]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"CALL_SETTER_TO_MAP", values});
							values->push_back("this->set_map_${name}(${data_var});");
							values->push_back("");
							values->push_back("FUNC_SERIALIZE:");
							values->push_back("__struct_datatype__ *serialize() override {");
							values->push_back("    __struct_datatype__ *__map = new __struct_datatype__();");
							values->push_back("    CHECK_ALLOC(__map, TYPE_NAME(__struct_datatype__));");
							values->push_back("    ${set_map_calls}");
							values->push_back("    return __map;");
							values->push_back("}");
							// END KEY [CALL_SETTER_TO_MAP]

							// KEY [VARIABLE_INIT_PTR]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_INIT_PTR", values});
							values->push_back("this->${name} = nullptr;");
							// END KEY [VARIABLE_INIT_PTR]

							// KEY [FUNC_SETTER_PTR_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_SETTER_PTR_DEF", values});
							values->push_back("void set_${name}(${type} ${name}) {");
							values->push_back("    this->${name} = ${name};");
							values->push_back("}");
							// END KEY [FUNC_SETTER_PTR_DEF]

							// KEY [VARIABLE_MAP_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_MAP_DEF", values});
							values->push_back("std::unordered_map<${key_type}, ${value_type} *> *${name} = nullptr;");
							// END KEY [VARIABLE_MAP_DEF]

							// KEY [FUNC_LIST_STRING_ADD_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_LIST_STRING_ADD_DEF", values});
							values->push_back("void add_to_${name}(string &${name}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    if (IS_NULL(this->${name})) {");
							values->push_back("        this->${name} = new std::vector<${type} *>();");
							values->push_back("        CHECK_ALLOC(this->${name}, TYPE_NAME(vector));");
							values->push_back("    }");
							values->push_back("    if (!IS_EMPTY(${name}) {");
							values->push_back("        uint32_t __size = name.length() + 1;");
							values->push_back("        char *__var = (char *)malloc(sizeof(char) * __size);");
							values->push_back("        CHECK_ALLOC(__var, TYPE_NAME(char *));");
							values->push_back("        memset(__var, 0, __size);");
							values->push_back("        memcpy(__var, ${name}.c_str(), (__size - 1));");
							values->push_back("        this->${name}->push_back(var);");
							values->push_back("    }");
							values->push_back("}");
							// END KEY [FUNC_LIST_STRING_ADD_DEF]

							// KEY [FUNC_LIST_NATIVE_ADD_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"FUNC_LIST_NATIVE_ADD_DEF", values});
							values->push_back("void add_to_${name}(${type} &${name}) {");
							values->push_back("    PRECONDITION(${modevar} == true);");
							values->push_back("    if (IS_NULL(this->${name})) {");
							values->push_back("        this->${name} = new std::vector<${type} *>();");
							values->push_back("        CHECK_ALLOC(this->${name}, TYPE_NAME(vector));");
							values->push_back("    }");
							values->push_back("    ${type} *__var = (${type} *)malloc(sizeof(${type}));");
							values->push_back("    CHECK_ALLOC(__var, TYPE_NAME(${type}));");
							values->push_back("    *__var = ${name};");
							values->push_back("    this->${name}->push_back(__var);");
							values->push_back("}");
							// END KEY [FUNC_LIST_NATIVE_ADD_DEF]

							// KEY [VARIABLE_LIST_DEF]
							values = new std::vector<string>();
							CHECK_ALLOC(values, TYPE_NAME(vector));
							__cpp_template.insert({"VARIABLE_LIST_DEF", values});
							values->push_back("std::vector<${type} *> *${name} = nullptr;");
							// END KEY [VARIABLE_LIST_DEF]

						}

					public:
						cpp_template_header() {
							this->__init();
						}

						~cpp_template_header() {
							unordered_map<string, vector<string> *>::iterator iter;
							for (iter = __cpp_template.being(); iter != __cpp_template.end(); iter++) {
								CHECK_AND_FREE(iter->second);							}
						}

						std::vector<string> * find_token(string token) {
							unordered_map<string, vector<string> *>::iterator iter = __cpp_template.find(token);
							if (iter != __cpp_template.end())
								return iter->second;
							return nullptr;
						}

					};
		}
REACTFS_NS_CORE_END
#define CPPT_TOKEN_FILE_DEF "FILE_DEF"
#define CPPT_TOKEN_NAMESPACE "NAMESPACE"
#define CPPT_TOKEN_FUNC_TYPE_MAP_SERIALIZER "FUNC_TYPE_MAP_SERIALIZER"
#define CPPT_TOKEN_FUNC_TYPE_LIST_SERIALIZER "FUNC_TYPE_LIST_SERIALIZER"
#define CPPT_TOKEN_FUNC_TYPE_SERIALIZER "FUNC_TYPE_SERIALIZER"
#define CPPT_TOKEN_FUNC_DESERIALIZE "FUNC_DESERIALIZE"
#define CPPT_TOKEN_CALL_SETTER_FROM_MAP "CALL_SETTER_FROM_MAP"
#define CPPT_TOKEN_FUNC_SETTER_TO_MAP "FUNC_SETTER_TO_MAP"
#define CPPT_TOKEN_FUNC_SETTER_FROM_MAP "FUNC_SETTER_FROM_MAP"
#define CPPT_TOKEN_CLASS_DEF "CLASS_DEF"
#define CPPT_TOKEN_FUNC_MAP_NATIVE_ADD_DEF "FUNC_MAP_NATIVE_ADD_DEF"
#define CPPT_TOKEN_VARIABLE_TYPE_FREE "VARIABLE_TYPE_FREE"
#define CPPT_TOKEN_VARIABLE_MAP_NATIVE_FREE "VARIABLE_MAP_NATIVE_FREE"
#define CPPT_TOKEN_FUNC_LIST_TYPE_ADD_DEF "FUNC_LIST_TYPE_ADD_DEF"
#define CPPT_TOKEN_FILE_COMMENT "FILE_COMMENT"
#define CPPT_TOKEN_FUNC_STRING_SETTER_DEF "FUNC_STRING_SETTER_DEF"
#define CPPT_TOKEN_VARIABLE_LIST_TYPE_FREE "VARIABLE_LIST_TYPE_FREE"
#define CPPT_TOKEN_FUNC_MAP_NATIVE_TYPE_DEF "FUNC_MAP_NATIVE_TYPE_DEF"
#define CPPT_TOKEN_INCLUDE_BASE "INCLUDE_BASE"
#define CPPT_TOKEN_FUNC_GETTER_DEF "FUNC_GETTER_DEF"
#define CPPT_TOKEN_VARIABLE_NATIVE_DEF "VARIABLE_NATIVE_DEF"
#define CPPT_TOKEN_FUNC_NATIVE_SETTER_DEF "FUNC_NATIVE_SETTER_DEF"
#define CPPT_TOKEN_VARIABLE_LIST_NATIVE_FREE "VARIABLE_LIST_NATIVE_FREE"
#define CPPT_TOKEN_VARIABLE_MAP_TYPE_FREE "VARIABLE_MAP_TYPE_FREE"
#define CPPT_TOKEN_VARIABLE_NATIVE_FREE "VARIABLE_NATIVE_FREE"
#define CPPT_TOKEN_CALL_SETTER_TO_MAP "CALL_SETTER_TO_MAP"
#define CPPT_TOKEN_VARIABLE_INIT_PTR "VARIABLE_INIT_PTR"
#define CPPT_TOKEN_FUNC_SETTER_PTR_DEF "FUNC_SETTER_PTR_DEF"
#define CPPT_TOKEN_VARIABLE_MAP_DEF "VARIABLE_MAP_DEF"
#define CPPT_TOKEN_FUNC_LIST_STRING_ADD_DEF "FUNC_LIST_STRING_ADD_DEF"
#define CPPT_TOKEN_FUNC_LIST_NATIVE_ADD_DEF "FUNC_LIST_NATIVE_ADD_DEF"
#define CPPT_TOKEN_VARIABLE_LIST_DEF "VARIABLE_LIST_DEF"
#endif // REACTFS_CPP_TEMPLATE_HEADER_H
