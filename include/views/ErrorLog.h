#ifndef HIKAGE_ERRORLOG_H
#define HIKAGE_ERRORLOG_H

#include "Viewable.h"

class ErrorLog : public Viewable{
public:
    ErrorLog() = default;

    void render() override;
    void setLog(char* log);

private:
    char* _log = nullptr;

};


#endif //HIKAGE_ERRORLOG_H
