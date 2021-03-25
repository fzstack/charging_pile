#ifndef DEVICES_HLW8112_HXX_
#define DEVICES_HLW8112_HXX_

#include "hlw8112_regs.hxx"
#include <utilities/observable.hxx>
#include <optional>
#include <utilities/nested.hxx>
#include <cstring>

class Hlw8112 {
public:
    Hlw8112();
    void init();
private:
    template<class T>
    class Session: public Nested<Hlw8112> {
    public:
        using reg_type = typename T::type;
        Session(Hlw8112* outer): Nested<Hlw8112>(outer), reg(read()), regInit(reg) { }

        ~Session() { commit(); }

        void commit() {
            if(T::readonly)
                return;
            if(compare(reg, regInit))
                return;
            outer->writeReg(T::addr, &reg, getSize());
        }

        reg_type* operator->() { return &reg; }
        reg_type& operator*() { return reg; }

    private:
        reg_type read() { //TODO: timeout
            reg_type reg;
            outer->readReg(T::addr, &reg, getSize());
            return reg;
        }

        bool compare(reg_type a, reg_type b) {
            return !memcmp(&a, &b, getSize());
        }

        constexpr int getSize() {
            return T::size == 0 ? sizeof(reg_type) : T::size;
        }

    private:
        reg_type reg, regInit;
        bool modified;
    };

public:
    template<class T>
    Session<T> makeSess() {
        return Session<T>(this);
    }

    void selectChannelA();
    void selectChannelB();

protected:
    virtual void cmd(int cmd, void* data, int len) = 0;
    virtual void readReg(char addr, void* data, int len, rt_int32_t timeout = 5) = 0;
    void writeEnable();
    void writeDisable();

private:
    void specCmd(char cmd);
    void writeReg(int addr, void* data, int len);

public:
    Observable<std::optional<bool>> writable = {};
private:
    Observable<bool> inited = {false};
};

class hlw8112_error: virtual public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif /* DEVICES_HLW8112_HXX_ */
