//
// Created by Subhabrata Ghosh on 28/10/16.
//

#ifndef REACTFS_BLOCK_UTILS_H
#define REACTFS_BLOCK_UTILS_H


#include "common/includes/common_utils.h"
#include "common/includes/time_utils.h"
#include "common_structs.h"
#include "base_block.h"
#include "base_indexed_block.h"
#include "typed_block.h"
#include "clients/mount_client.h"
#include "types/includes/schema_def.h"

using namespace com::wookler::reactfs::core::client;
using namespace com::wookler::reactfs::core::types;

namespace com {
    namespace wookler {
        namespace reactfs {
            namespace core {
                class block_utils {
                public:
                    static string
                    create_typed_block(__complex_type *datatype, uint64_t block_id, uint64_t parent_id,
                                       const string filename, __block_usage usage, uint32_t block_size,
                                       uint32_t est_record_size, uint64_t start_index, vector<record_index *> *indexes,
                                       bool overwrite = false) {
                        typed_block *block = new typed_block();
                        CHECK_ALLOC(block, TYPE_NAME(typed_block));
                        block->set_datatype(datatype);
                        string uuid = block->create(block_id, parent_id, filename, usage, block_size, start_index,
                                                    est_record_size,
                                                    overwrite);
                        POSTCONDITION(!IS_EMPTY(uuid));
                        if (!NOT_EMPTY_P(indexes)) {
                            for (record_index *ri : *indexes) {
                                block->add_index(ri);
                            }
                        }
                        CHECK_AND_FREE(block);

                        return uuid;
                    }

                    /*!
                     * Create a new basic block based on the specified parameters.
                     *
                     * @param block_id - Block id of the new block.
                     * @param parent_id - Unique ID of the parent structure.
                     * @param filename - Filename to be used for this block.
                     * @param block_type - Type of the data block to create.
                     * @param usage - Type of the block.
                     * @param block_size - Data size for this block.
                     * @param est_record_size - Estimated size for the records in this block. (to be used to estimate index size)
                     * @param start_index - Start index for the records in this block.
                     * @param overwrite - Overwrite block data file, if already exists.
                     * @return - UUID of the new block created.
                     */
                    static string
                    create_new_block(uint64_t block_id, uint64_t parent_id, const string filename,
                                     __block_def block_type, __block_usage usage, uint32_t block_size,
                                     uint32_t est_record_size, uint64_t start_index, bool overwrite = false) {
                        if (block_type == __block_def::BASIC) {
                            base_block *block = new base_block();
                            CHECK_ALLOC(block, TYPE_NAME(base_block));
                            string uuid = block->create(block_id, parent_id, filename, usage, block_size, start_index,
                                                        est_record_size,
                                                        overwrite);
                            POSTCONDITION(!IS_EMPTY(uuid));
                            CHECK_AND_FREE(block);

                            return uuid;
                        } else if (block_type == __block_def::INDEXED) {
                            base_block *block = new base_indexed_block();
                            CHECK_ALLOC(block, TYPE_NAME(base_indexed_block));
                            string uuid = block->create(block_id, parent_id, filename, usage, block_size, start_index,
                                                        est_record_size,
                                                        overwrite);
                            POSTCONDITION(!IS_EMPTY(uuid));
                            CHECK_AND_FREE(block);

                            return uuid;
                        }

                        throw FS_BASE_ERROR("Un-supported block type. [type=%d]", block_type);
                    }

                    /*!
                     * Delete the specified block from the file system.
                     *
                     * @param m_client - Mount client handle.
                     * @param block_id - Block ID of the block to be deleted.
                     * @param filename - File name of the block file.
                     * @param block_type - Type of block that is being deleted.
                     */
                    static void
                    delete_block(mount_client *m_client, uint64_t block_id, string filename, __block_def block_type) {
                        if (block_type == __block_def::BASIC) {
                            base_block *block = new base_block();
                            CHECK_ALLOC(block, TYPE_NAME(base_block));
                            block->open(block_id, filename, false);
                            CHECK_AND_FREE(block);
                        } else if (block_type == __block_def::INDEXED) {
                            base_block *block = new base_indexed_block();
                            CHECK_ALLOC(block, TYPE_NAME(base_indexed_block));
                            block->open(block_id, filename, false);
                            CHECK_AND_FREE(block);
                        } else {
                            throw FS_BASE_ERROR("Un-supported block type. [type=%d]", block_type);
                        }
                        Path p(filename);
                        if (p.exists()) {
                            uint64_t f_size = file_utils::get_file_size(p.get_path());
                            LOG_INFO("Deleting block file. [filename=%s][size=%lu]", p.get_path().c_str(), f_size);
                            m_client->release_block(p.get_path_p(), f_size);
                            p.remove();
                        }
                    }

                    /*!
                     * Get the directory for a new block being created.
                     *
                     * @param m_client - Mount client handle.
                     * @param size - Total file size estimated for this block.
                     * @return - Directory path where the block should be created.
                     */
                    static string get_block_dir(mount_client *m_client, uint64_t size) {
                        CHECK_NOT_NULL(m_client);


                        string mount = m_client->get_next_mount(size);
                        if (IS_EMPTY(mount)) {
                            throw FS_BLOCK_ERROR(fs_block_error::ERRCODE_BLOCK_OUT_OF_SPACE, "Requested size = %lu",
                                                 size);
                        }

                        if (!m_client->reserve_block(&mount, size)) {
                            throw FS_BLOCK_ERROR(fs_block_error::ERRCODE_ALLOCATING_BLOCK_SPACE, "[mount=%s][size=%lu]",
                                                 mount.c_str(), size);
                        }

                        if (!string_utils::ends_with(&mount, "/")) {
                            mount.append("/");
                        }
                        Path path(mount);
                        POSTCONDITION(path.exists());

                        string dir = time_utils::get_hour_dir();
                        POSTCONDITION(!IS_EMPTY(dir));
                        path.append(dir);
                        if (!path.exists()) {
                            path.create(DEFAULT_RESOURCE_MODE);
                        }

                        string p(file_utils::cannonical_path(path.get_path()));
                        TRACE("Returned block location : [%s]", p.c_str());

                        return p;
                    }
                };
            }
        }
    }
}
#endif //REACTFS_BLOCK_UTILS_H
