#pragma once
#include <utilities/observer.hxx>

class Rc522Base {
    using card_id_t = std::optional<std::string>;
protected:
    Observable<card_id_t> cardId;
public:
    Observer<card_id_t> oCardId = cardId;
};


