#pragma once
#include "kvdb.hxx"
#include <utilities/inner_port.hxx>
#include <config/app.hxx>

class LowerBackup: public Kvdb<LowerBackup> {
public:
    LowerBackup();
    Backup read(InnerPort p);
    void write(InnerPort p, Backup& backup);
public:
    static const char* kPartName;
};

#include <utilities/singleton.hxx>
namespace Preset {
class LowerBackup: public Singleton<LowerBackup>, public ::LowerBackup {
    friend singleton_t;
    LowerBackup(): ::LowerBackup() { }
};
}
