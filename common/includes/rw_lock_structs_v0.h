//
// Created by Subhabrata Ghosh on 25/11/16.
//

#ifndef REACTFS_RW_LOCK_STRUCTS_V0_H
#define REACTFS_RW_LOCK_STRUCTS_V0_H

#include "common/includes/common.h"

#define DEFAULT_RW_LOCK_RETRY 10
#define SIZE_LOCK_NAME 32
#define MAX_READER_LOCKS 256
#define MAX_SHARED_LOCKS 4096

namespace com {
    namespace wookler {
        namespace reactfs {
            namespace common {
                typedef struct __owner_v0__ {
                    char owner[SIZE_USER_NAME];
                    char txn_id[SIZE_UUID];
                    pid_t process_id;
                    char thread_id[SIZE_THREAD_ID];
                    uint64_t lock_timestamp = 0;
                } __owner_v0;

                typedef struct __lock_readers_v0__ {
                    bool used = false;
                    pid_t process_id;
                    char thread_id[SIZE_THREAD_ID];
                    uint64_t lock_timestamp = 0;
                } __lock_readers_v0;

                typedef struct __lock_struct_v0__ {
                    bool used = false;
                    char name[SIZE_LOCK_NAME];
                    uint64_t last_used = 0;
                    bool write_locked = false;
                    uint32_t ref_count = 0;
                    __owner_v0 owner;
                    uint64_t reader_count;
                    __lock_readers_v0 readers[MAX_READER_LOCKS];
                } __lock_struct_v0;

                typedef struct __shared_lock_data_v0__ {
                    __version_header version;
                    uint32_t max_count = 0;
                    uint32_t used_count = 0;
                } __shared_lock_data_v0;
            }
        }
    }
}
#endif //REACTFS_RW_LOCK_STRUCTS_V0_H
