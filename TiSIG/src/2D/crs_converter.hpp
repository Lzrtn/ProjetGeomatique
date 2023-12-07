#include <proj.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <iomanip>

std::vector<double> crs_converter(int epsg_entry, double northing_entry, double easting_entry, int epsg_exit) {

    PJ_CONTEXT *C;
    PJ *P;
    PJ *norm;
    PJ_COORD a, b;

    std::string epsg = "EPSG:";
    std::string str_epsg_entry = epsg + std::to_string(epsg_entry);
    std::string str_epsg_exit = epsg + std::to_string(epsg_exit);
    const char* char_epsg_entry = str_epsg_entry.c_str();
     const char* char_epsg_exit = str_epsg_exit.c_str();    
    
    C = proj_context_create();

    P = proj_create_crs_to_crs(
        C, char_epsg_entry, char_epsg_exit,
        NULL);

    if (0 == P) {
        fprintf(stderr, "Failed to create transformation object.\n");
        return {0};
    }

    /* This will ensure that the order of coordinates for the input CRS */
    /* will be longitude, latitude, whereas EPSG:4326 mandates latitude, */
    /* longitude */
    norm = proj_normalize_for_visualization(C, P);
    if (0 == norm) {
        fprintf(stderr, "Failed to normalize transformation object.\n");
        return {0};
    }
    proj_destroy(P);
    P = norm;
  
    /* 
     * Given that we have used proj_normalize_for_visualization(), the order 
     * of coordinates is longitude, latitude, and values are expressed in 
     * degrees. 
     */

    a = proj_coord(northing_entry, easting_entry, 0, 0);

    /* transform to UTM zone 32, then back to geographical */
    b = proj_trans(P, PJ_FWD, a);
    printf("easting: %.3f, northing: %.3f\n", b.enu.e, b.enu.n);

    /*b = proj_trans(P, PJ_INV, b);
    printf("longitude: %g, latitude: %g\n", b.lp.lam, b.lp.phi);*/

    /* Clean up */
    proj_destroy(P);
    proj_context_destroy(C); /* may be omitted in the single threaded case */
    return {b.enu.n, b.enu.e};
}
