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
// Created by Subhabrata Ghosh on 20/09/16.
//

#include "test_file_utils.h"

TEST_CASE("Basic path tests", "[com::watergate::common::file_utils]") {
    string cfgFile = file_utils::cannonical_path("test/data/test-conf.json");
    env_utils::create_env(cfgFile);
    const __env *env = env_utils::get_env();
    REQUIRE(NOT_NULL(env));

    const Config *config = env_utils::get_config();
    REQUIRE(NOT_NULL(config));

    Path p("/tmp/adhoc/dir/file.txt");
    REQUIRE((p.get_parent_dir().compare("/tmp/adhoc/dir")) ==0);
    LOG_DEBUG("Parent dir [%s]", p.get_parent_dir().c_str());
    p = Path("who/cares/file.txt");
    REQUIRE((p.get_parent_dir().compare("who/cares")) ==0);
    LOG_DEBUG("Parent dir [%s]", p.get_parent_dir().c_str());

    env_utils::dispose();
}

TEST_CASE("Test file copy/delete.", "[com::watergate::common::file_utils]") {
    string cfgFile = file_utils::cannonical_path("test/data/test-conf.json");
    env_utils::create_env(cfgFile);
    const __env *env = env_utils::get_env();
    REQUIRE(NOT_NULL(env));

    const Config *config = env_utils::get_config();
    REQUIRE(NOT_NULL(config));

    Path source(file_utils::cannonical_path(cfgFile));
    Path *dest = env->get_temp_dir(file_utils::cannonical_path(COPY_DEST_PATH), 0755);
    dest->append(source.get_filename());

    uint64_t t = file_copy::copy(&source, dest);
    REQUIRE( t > 0);

    LOG_DEBUG("Copied file [source=%s] to [dest=%s]", source.get_path().c_str(), dest->get_path().c_str());

    vector<string> lines;
    file_helper::copy_lines(*dest, &lines);
    CHECK_NOT_EMPTY(lines);
    for (auto line : lines) {
        LOG_DEBUG("LINE = [%s]", line.c_str());
    }

    dest->remove();
    REQUIRE(!dest->exists());
    LOG_DEBUG("Deleted file %s", dest->get_path().c_str());
    env_utils::dispose();
}