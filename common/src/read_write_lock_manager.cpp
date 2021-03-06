//
// Created by Subhabrata Ghosh on 25/11/16.
//

#include "common/includes/read_write_lock_manager.h"


void com::wookler::reactfs::common::read_write_lock_manager::init(mode_t mode, bool reset) {
    try {
        create(mode, true, reset);

        if (reset)
            this->reset();

        if (use_manager_thread) {
            manager_thread = new thread(read_write_lock_manager::run, this);
            CHECK_ALLOC(manager_thread, TYPE_NAME(thread));
        }
    } catch (const exception &e) {
        lock_error le = LOCK_ERROR("Error creating lock manager instance. [error=%s]", e.what());
        state.set_error(&le);
        throw le;
    } catch (...) {
        lock_error le = LOCK_ERROR("Error creating lock manager instance. [error=Unknown]");
        state.set_error(&le);
        throw le;
    }
}

void com::wookler::reactfs::common::read_write_lock_manager::run(read_write_lock_manager *manager) {
    PRECONDITION(NOT_NULL(manager));
    try {
        LOG_INFO("Starting lock manager thread...");
        NEW_ALARM(DEFAULT_LOCK_MGR_SLEEP, 0);
        while (manager->state.get_state() == __state_enum::Available) {
            PRECONDITION(NOT_NULL(manager));
            manager->check_lock_states();

            START_ALARM(0);
        }
        LOG_INFO("Lock manager thread terminated...");
    } catch (const exception &e) {
        LOG_ERROR("Lock manager thread terminated with error. [error=%s]", e.what());
        manager->state.set_error(&e);
    } catch (...) {
        lock_error le = LOCK_ERROR("Lock manager thread terminated with unhandled exception.");
        manager->state.set_error(&le);
    }
}

void com::wookler::reactfs::common::read_write_lock_manager::check_lock_states() {
    if (state.get_state() == __state_enum::Available) {
        uint32_t count = table->get_max_size();
        for (uint32_t ii = 0; ii < count; ii++) {
            __rw_lock_struct *ptr = table->get_at(ii);
            if (NOT_NULL(ptr)) {
                if (ptr->w_lock_struct.write_locked) {
                    uint64_t now = time_utils::now();
                    if ((now - ptr->w_lock_struct.owner.lock_timestamp) > DEFAULT_WRITE_LOCK_TIMEOUT) {
                        ptr->w_lock_struct.owner.lock_timestamp = 0;
                        ptr->w_lock_struct.owner.process_id = -1;
                        memset(ptr->w_lock_struct.owner.txn_id, 0, SIZE_UUID + 1);
                        memset(ptr->w_lock_struct.owner.owner, 0, SIZE_MAX_NAME + 1);
                        ptr->w_lock_struct.write_locked = false;
                    }
                }
                __lock_readers *r_ptr = ptr->readers;
                for (int ii = 0; ii < MAX_READER_LOCKS; ii++) {
                    if (r_ptr[ii].used) {
                        uint64_t now = time_utils::now();
                        if ((now - r_ptr->lock_timestamp) > DEFAULT_READ_LOCK_TIMEOUT) {
                            r_ptr[ii].used = false;
                            ptr->reader_count--;
                        }
                    }
                }
                uint64_t delta = (time_utils::now() - ptr->w_lock_struct.last_used);
                if (ptr->w_lock_struct.ref_count <= 0 && delta >= DEFAULT_RW_LOCK_EXPIRY) {
                    table->remove_lock(ptr->w_lock_struct.name);
                }
            }
        }
    }
}

void com::wookler::reactfs::common::read_write_lock_manager::reset() {
    CHECK_STATE_AVAILABLE(state);
    std::lock_guard<std::mutex> guard(thread_mutex);
    uint32_t count = table->get_max_size();
    for (uint32_t ii = 0; ii < count; ii++) {
        __rw_lock_struct *ptr = table->get_at(ii);
        if (NOT_NULL(ptr)) {
            if (ptr->w_lock_struct.write_locked) {
                pid_t pid = ptr->w_lock_struct.owner.process_id;
                if (!process_utils::check_process(pid)) {
                    ptr->w_lock_struct.owner.lock_timestamp = 0;
                    ptr->w_lock_struct.owner.process_id = -1;
                    memset(ptr->w_lock_struct.owner.txn_id, 0, SIZE_UUID + 1);
                    memset(ptr->w_lock_struct.owner.owner, 0, SIZE_MAX_NAME + 1);
                    ptr->w_lock_struct.write_locked = false;
                } else {
                    uint64_t now = time_utils::now();
                    if ((now - ptr->w_lock_struct.owner.lock_timestamp) > DEFAULT_WRITE_LOCK_TIMEOUT) {
                        ptr->w_lock_struct.owner.lock_timestamp = 0;
                        ptr->w_lock_struct.owner.process_id = -1;
                        memset(ptr->w_lock_struct.owner.txn_id, 0, SIZE_UUID + 1);
                        memset(ptr->w_lock_struct.owner.owner, 0, SIZE_MAX_NAME + 1);
                        ptr->w_lock_struct.write_locked = false;
                    }
                }
            }
            __lock_readers *r_ptr = ptr->readers;
            int reader_count = 0;
            for (int ii = 0; ii < MAX_READER_LOCKS; ii++) {
                if (r_ptr[ii].used) {
                    bool used = true;
                    pid_t pid = r_ptr[ii].process_id;
                    if (!process_utils::check_process(pid)) {
                        r_ptr[ii].used = false;
                        used = false;
                    } else {
                        uint64_t now = time_utils::now();
                        if ((now - r_ptr->lock_timestamp) > DEFAULT_READ_LOCK_TIMEOUT) {
                            r_ptr[ii].used = false;
                            used = false;
                        }
                    }
                    if (used) {
                        reader_count++;
                    }
                }
            }
            ptr->reader_count = reader_count;
        }
    }
}
