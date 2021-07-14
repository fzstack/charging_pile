#pragma once
#include <rtthread.h>
#include <flashdb.h>
#include <Mutex.h>
#include <components/fal.hxx>

template<class T>
class Kvdb {
public:
    using Base = Kvdb<T>;
    Kvdb(const char* partName, fdb_default_kv* defKv) {
        fdb_kvdb_control(&db, FDB_KVDB_CTRL_SET_LOCK, (void*)lock);
        fdb_kvdb_control(&db, FDB_KVDB_CTRL_SET_UNLOCK, (void*)unlock);
        auto r = fdb_kvdb_init(&db, partName, partName, defKv, NULL);
        rt_kprintf("init result: %d\n", r);
    }

    void reset() {
        auto r = fdb_kv_set_default(&db);
    }

protected:
    template<class U>
    inline U readBasic(const char* name) {
        fdb_blob blob = {};
        U result = {};
        rt_kprintf("u size: %d\n", sizeof(U));
        auto r = fdb_kv_get_blob(&db, name, fdb_blob_make(&blob, &result, sizeof(U)));
        rt_kprintf("actual size: %d\n", r);
        return result;
    }

    template<class U>
    inline void writeBasic(U& u, const char* name) {
        fdb_blob blob = {};
        auto r = fdb_kv_set_blob(&db, name, fdb_blob_make(&blob, &u, sizeof(U)));
        rt_kprintf("result: %d\n", r);
    }

private:
    static void lock() {
        mutex.lock();
    }
    static void unlock() {
        mutex.unlock();
    }

protected:
    fdb_kvdb db = {};
private:
    static rtthread::Mutex mutex;
};

template<class T>
rtthread::Mutex Kvdb<T>::mutex = {};