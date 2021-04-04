#ifdef TEST_USER_INPUT

#include <components/user_input.hxx>
#include <rtthread.h>
#include <utilities/f.hxx>
#include <string>

using namespace std;

int init_test_user_input() {
    auto user = Preset::UserInput::get();
    user->onConfirm += [](NatPort port, string cardId) {
        F{} << "mock start @"_r << port.get() << ", cardId"_r << cardId << endln;
    };

    F{} << "user test init ok, please test"_r << endln;
    return RT_EOK;
}

INIT_APP_EXPORT(init_test_user_input);

#endif
