#ifndef APPLICATIONS_APP_HXX_
#define APPLICATIONS_APP_HXX_

#ifdef __cplusplus
extern "C" {
#endif
void runApp();
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class App {
public:
    virtual void run() = 0;
};


#endif



#endif /* APPLICATIONS_APP_HXX_ */
