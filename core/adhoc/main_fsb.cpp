//
// Created by Subhabrata Ghosh on 27/10/16.
//

#include "common/includes/log_utils.h"
#include "common/includes/__env.h"
#include "common/includes/init_utils.h"
#include "core/includes/base_block.h"
#include "core/includes/block_utils.h"

#define REUSE_BLOCK_FILE "/tmp/block_reused.raw"
#define REUSE_BLOCK_ID 1024

#define REUSE_BLOCK_FILE_COMP "/tmp/block_reused.comp"
#define REUSE_BLOCK_ID_COMP 1025

#define DEFAULT_BLOCK_SIZE 1024 * 1024 * 32
#define RECORD_START_INDEX 100000
#define COUNT_RECORDS 500

using namespace com::wookler::reactfs::common;
using namespace com::wookler::reactfs::core;

typedef struct {
    uint64_t timestamp;
    char value[128];
    uint32_t index;
} _test_typed;

void test_raw() {
    Path p(REUSE_BLOCK_FILE);
    if (p.exists()) {
        p.remove();
    }
    string ifile = common_utils::format("%s.index", REUSE_BLOCK_FILE);
    Path ip(ifile);
    if (ip.exists()) {
        ip.remove();
    }

    string uuid = block_utils::create_new_block(REUSE_BLOCK_ID, REUSE_BLOCK_FILE, __block_type::PRIMARY,
                                                DEFAULT_BLOCK_SIZE,
                                                sizeof(_test_typed), RECORD_START_INDEX);
    POSTCONDITION(!IS_EMPTY(uuid));

    base_block *block = new base_block();
    block->open(REUSE_BLOCK_ID, REUSE_BLOCK_FILE);
    POSTCONDITION(block->get_block_state() == __state_enum::Available);

    string txid = block->start_transaction();
    POSTCONDITION(!IS_EMPTY(txid))
    int count = 0;
    for (int ii = 0; ii < COUNT_RECORDS; ii++) {
        _test_typed t;
        t.index = ii;
        t.timestamp = time_utils::now() + (ii * 10000);
        string uuid = common_utils::uuid();
        strncpy(t.value, uuid.c_str(), uuid.length());

        if (block->get_free_space() < sizeof(_test_typed))
            break;
        uint64_t index = block->write(&t, sizeof(_test_typed), txid);
        POSTCONDITION(index >= 0);
        count++;
    }
    block->commit(txid);
    LOG_INFO("Written [%d] records to block. [used bytes=%lu]", count, block->get_used_space());

    CHECK_AND_FREE(block);
    block = new base_block();
    block->open(REUSE_BLOCK_ID, REUSE_BLOCK_FILE);
    POSTCONDITION(block->get_block_state() == __state_enum::Available);

    int fetched = 0;
    vector<shared_read_ptr> r;
    while (fetched <= count) {
        uint32_t r_count = block->read((RECORD_START_INDEX + fetched), 5, &r);
        if (r_count > 0) {
            for (int ii = 0; ii < r.size(); ii++) {
                const void *ptr = r[ii].get()->get_data_ptr();
                CHECK_NOT_NULL(ptr);
                POSTCONDITION(r[ii].get()->get_size() == sizeof(_test_typed));
                const _test_typed *t = static_cast<const _test_typed *>(ptr);
                LOG_DEBUG("[index=%d][uuid=%s]", t->index, t->value);
            }
            fetched += r.size();
            r.clear();
        } else
            break;
    }

    CHECK_AND_FREE(block);
}

void test_compressed() {

}

int main(int argc, char **argv) {
    try {

        PRECONDITION(argc > 0);
        char *cf = argv[1];
        CHECK_NOT_NULL(cf);

        string configf(cf);
        env_utils::create_env(configf);

        lock_env_utils::create_manager(0755);
        lock_env *manager = lock_env_utils::get_manager();
        CHECK_NOT_NULL(manager);

        test_raw();

        test_compressed();

        lock_env_utils::dispose();

        exit(0);
    } catch (const exception &e) {
        LOG_ERROR(e.what());
        exit(-1);
    } catch (...) {
        LOG_ERROR("Unhandled exception occurred.");
        exit(-2);
    }
}