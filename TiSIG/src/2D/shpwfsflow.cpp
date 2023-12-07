#include "shpwfsflow.h"


#include <iostream>
#include <string>

#include <gdal/ogrsf_frmts.h>
#include <postgresql/libpq-fe.h>

#include <gdal/gdal.h>
#include <gdal/ogr_api.h>



shpWFSflow::shpWFSflow(DbManager db_manager_, WFSFlow *wfsflow_)
    :Shapefile(wfsflow_->GetfilePath(), db_manager_), wfsflow(wfsflow_)
{
    path.replace(path.size() - 4, 4, ".shp");
    idType = 3000;
}

