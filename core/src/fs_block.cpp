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

#include <core/includes/fs_block.h>
#include "core/includes/fs_block.h"
#include "common/includes/file_utils.h"
#include "common/includes/timer.h"

string com::wookler::reactfs::core::fs_block::open(uint64_t block_id, string filename) {
    CHECK_NOT_EMPTY(filename);
    try {

        void *base_ptr = _open(block_id, filename);

        char *cptr = static_cast<char *>(base_ptr);
        data_ptr = cptr + sizeof(__block_header);

        char *wptr = static_cast<char *>(data_ptr);
        if (header->write_offet > 0) {
            write_ptr = wptr + header->write_offet;
        } else {
            write_ptr = wptr;
        }
        return header->block_uid;
    } catch (const exception &e) {
        throw FS_BASE_ERROR_E(e);
    } catch (...) {
        throw FS_BASE_ERROR("Unkown error occurred while opening block.");
    }
}

void *com::wookler::reactfs::core::fs_block::_open(uint64_t block_id, string filename) {
    Path p(filename);
    if (!p.exists()) {
        throw FS_BASE_ERROR("File not found. [path=%s]", filename.c_str());
    }
    stream = new fmstream(filename.c_str());
    CHECK_NOT_NULL(stream);
    void *base_ptr = stream->data();

    header = static_cast<__block_header *>(base_ptr);
    PRECONDITION(header->block_id == block_id);

    string lock_name = get_lock_name(header->block_id);
    block_lock = new exclusive_lock(&lock_name);
    block_lock->create();

    return base_ptr;
}

string
com::wookler::reactfs::core::fs_block::create(uint64_t block_id, string filename, uint64_t block_size, bool overwrite) {
    CHECK_NOT_EMPTY(filename);
    PRECONDITION(block_size > 0);
    PRECONDITION(block_id > 0);
    try {
        string uuid = _create(block_id, filename, block_size, overwrite);

        delete (block_lock);

        close();

        header = nullptr;

        return uuid;
    } catch (const exception &e) {
        throw FS_BASE_ERROR_E(e);
    } catch (...) {
        throw FS_BASE_ERROR("Unkown error occurred while opening block.");
    }
}

string com::wookler::reactfs::core::fs_block::_create(uint64_t block_id, string filename, uint64_t block_size,
                                                      bool overwrite) {
    Path p(filename);
    if (p.exists()) {
        if (!overwrite) {
            throw FS_BASE_ERROR("File with specified path already exists. [path=%s]", filename.c_str());
        } else {
            if (!p.remove()) {
                throw FS_BASE_ERROR("Error deleting existing file. [file=%s]", filename.c_str());
            }
        }
    }
    uint64_t ts = sizeof(__block_header) + block_size;

    stream = new fmstream(filename.c_str(), ts);
    CHECK_NOT_NULL(stream);
    void *base_ptr = stream->data();

    memset(base_ptr, 0, ts);

    header = static_cast<__block_header *>(base_ptr);
    header->block_id = block_id;

    string uuid = common_utils::uuid();
    memcpy(header->block_uid, uuid.c_str(), uuid.length());

    header->block_size = block_size;
    header->create_time = time_utils::now();
    header->update_time = header->create_time;

    string lock_name = get_lock_name(header->block_id);
    block_lock = new exclusive_lock(&lock_name);
    block_lock->create();
    block_lock->reset();


    return uuid;
}

const void *com::wookler::reactfs::core::fs_block::read(uint64_t size, uint64_t offset) {
    CHECK_NOT_NULL(stream);
    CHECK_NOT_NULL(header);
    PRECONDITION(offset + size <= header->block_size);

    char *ptr = static_cast<char *>(data_ptr);
    if (offset > 0)
        ptr += offset;

    return ptr;
}

const void *com::wookler::reactfs::core::fs_block::write(const void *source, uint32_t len) {
    CHECK_NOT_NULL(stream);
    CHECK_NOT_NULL(header);
    CHECK_NOT_NULL(source);
    PRECONDITION(len > 0);
    PRECONDITION(header->used_bytes + len <= header->block_size);

    char *start_ptr = static_cast<char *>(write_ptr);
    if (block_lock->wait_lock()) {
        try {
            write_r(source, len);
        } catch (const exception &e) {
            block_lock->release_lock();
            throw FS_BASE_ERROR_E(e);
        } catch (...) {
            block_lock->release_lock();
            throw FS_BASE_ERROR("Unhandled type exception.");
        }
        block_lock->release_lock();

        return start_ptr;
    } else {
        throw FS_BASE_ERROR("Error getting write lock on file. [block id=%s]", header->block_uid);
    }
}

void *com::wookler::reactfs::core::fs_block::write_r(const void *source, uint32_t len) {
    void *ptr = write_ptr;

    memcpy(write_ptr, source, len);

    header->used_bytes += len;
    header->write_offet += len;

    char *cptr = static_cast<char *>(data_ptr);
    write_ptr = cptr + header->write_offet;

    return ptr;
}