#include "lower_backup.hxx"

static Backup defaultBackup = {

};

#define ITEM(x) { \
    key: #x, \
    value: &defaultBackup, \
    value_len: sizeof(defaultBackup), \
}

static fdb_default_kv_node defaultConfTab[] = {
    ITEM(0), ITEM(1), ITEM(2), ITEM(3), ITEM(4), ITEM(5), ITEM(6), ITEM(7), ITEM(8), ITEM(9),
};

#undef ITEM

static fdb_default_kv defaultConfKv = {
    kvs: defaultConfTab,
    num: sizeof(defaultConfTab) / sizeof(defaultConfTab[0]),
};

LowerBackup::LowerBackup(): Base(kPartName, &defaultConfKv) {

}

Backup LowerBackup::read(InnerPort p) {
    fdb_blob blob;
    Backup backup = {};
    char name[2] = {'\0'};
    name[0] = '0' + p.get();
    fdb_kv_get_blob(&db, name, fdb_blob_make(&blob, &backup, sizeof(backup)));
    return backup;
}

void LowerBackup::write(InnerPort p, Backup& backup) {
    fdb_blob blob;
    char name[2] = {'\0'};
    name[0] = '0' + p.get();
    rt_kprintf("conf name: [%s]\n", name);
    auto r = fdb_kv_set_blob(&db, name, fdb_blob_make(&blob, &backup, sizeof(backup)));
    rt_kprintf("result: %d\n", r);
}

const char* LowerBackup::kPartName = "backup";