#ifndef SHPWFSFLOW_H
#define SHPWFSFLOW_H
#include "shapefile.h"
#include "wfsflow.h"

class shpWFSflow : public Shapefile
{
public:
    shpWFSflow(DbManager db_manager, WFSFlow *wfsflow);

protected:
    WFSFlow *wfsflow;

};

#endif // SHPWFSFLOW_H
