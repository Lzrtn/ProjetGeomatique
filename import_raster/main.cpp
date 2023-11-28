#include <iostream>
#include <fstream>
#include <curl/curl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include "wms.h"
#include "wmts.h"

using namespace std;

int main() {
    //Initialize
    WMS wmsInstance(4326, 45.98624572753906, 45.63631057739258, 4.98760461807251, 4.668561363220215, 1080, 1080);
    WMTS wmtsInstance((string)"LAMB93", 18, 536225.222852, 5739265.686868);

    //Convert into char*
    const char* url = wmsInstance.getUrl();
    const char* url2 = wmtsInstance.getUrl();
    wmsInstance.getImage();
    wmtsInstance.getImage();
    
    /*
    // Initialize libcurl
    CURL* curl = curl_easy_init();

    if (curl) {

        // Set the URL to download
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set up the callback to write the data to a file
        std::ofstream imageFile("downloaded_image.png", std::ios::binary);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &imageFile);

        // Perform the HTTP request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Cleanup
        curl_easy_cleanup(curl);
        imageFile.close();
    }

    // Use stb_image to load the downloaded image
    int width, height, channels;
    unsigned char* image = stbi_load("downloaded_image.png", &width, &height, &channels, 0);
    */

    /*if (image) {
        // Create an OpenCV Mat from the loaded image data
        cv::Mat imgMat(height, width, channels == 4 ? CV_8UC4 : CV_8UC3, image);

        // Display the image using OpenCV
        cv::imshow("Downloaded Image", imgMat);
        cv::waitKey(0);        

        // Free the image data
        stbi_image_free(image);
        
    } else {
        std::cerr << "Error loading image" << std::endl;
    }*/
    return 0;
}