#ifndef COMPONENTS_AIR_ESSENTIAL_HXX_
#define COMPONENTS_AIR_ESSENTIAL_HXX_

#include "air_component.hxx"
#include <string>

class Air724;
class AirEssential: public AirComponent<AirEssential> {
    friend class Air724;
private:
    AirEssential(std::shared_ptr<Air724> owner);
public:
    std::string getImei();
    std::string getIccid();
    int getCsq();

private:
    void closeEcho();
    void attachGprs();
    void activatePdp();

};



#endif /* COMPONENTS_AIR_ESSENTIAL_HXX_ */
