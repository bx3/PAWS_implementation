/* 
 * File:   paws_client.h
 * Author: bx
 *
 * Created on October 14, 2015, 6:20 PM
 */

#ifndef PAWS_CLIENT_H
#define	PAWS_CLIENT_H
#include <iostream>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>
#include <cxxtools/remoteprocedure.h>
#include <cxxtools/xmlrpc/httpclient.h>
#include <cxxtools/bin/rpcclient.h>
#include <cxxtools/json/rpcclient.h>
#include <cxxtools/json/httpclient.h>
#include <stdio.h>
#include <string.h>
#include <ctime>

#include "paws_struct.h"

#define DEFAULT_ORIENTATION              -1000
#define DEFAULT_MINOR_AXIS               -1000
#define DEFAULT_MAJOR_AXIS               -1000
#define DEFAULT_CONFIDENCE               -1000
#define DEFAULT_LONGITUDE                -1000
#define DEFAULT_LATITUDE                 -1000
#define DEFAULT_ANTENNA_HEIGHT_UNCERTAIN -1000
#define DEFAULT_ANTENNA_HEIGHT           -1000
#define DEFAULT_ANTENNA_HEIGHT_TYPE      -1000
#define DEFAULT_ERROR                    100

class PAWS_client{
public:
    PAWS_client();
    double add(double a, double b);   
    int init_request();
    int register_request();
    int avail_spec_query();
    int start_paws();
    int is_enable_notification(bool choice);
private:
    int create_init_req_struct(Init_req* init_req);
    int process_init_resp_string(std::string * resp_s);
    int create_register_req_struct(Registration_req* reg_req);
    int process_register_resp_string(std::string* resp_s);    
    int create_spec_req_struct(Avail_spectrum_req* spec_req);
    int process_spec_resp_string(std::string* resp_s);
    int decide_schedule(Avail_spectrum_resp * spec_resp);
    int notify_database();
    int create_notify_struct(Spectrum_use_notify * spectrum_notify);
    int process_use_notify_string(std::string * notify_resp_s);   
    
    int report_error(ErrorElement * error);
            
    std::string server_ip;
    short port;
    GeoLocation my_location; 
    AntennaCharacteristics my_antenna;
    std::string rulesetID;
    bool notify_db;
    EventTime my_event;
    Spectrum spectrum;        
};

#endif	/* PAWS_CLIENT_H */

