/* 
 * File:   main.cpp
 * Author: bx
 *
 * Created on October 22, 2015, 11:23 AM
 * 
 * This file contains all the functions can be processed at the database side. 
 */

#include <cstdlib>

#include "../PAWS_client/paws_client.h"
#include "../PAWS_client/paws_struct_serializer.h"

#include <iostream>
#include <cxxtools/arg.h>
#include <cxxtools/log.h>
#include <cxxtools/xmlrpc/service.h>
#include <cxxtools/http/server.h>
#include <cxxtools/bin/rpcserver.h>
#include <cxxtools/json/rpcserver.h>
#include <cxxtools/json/httpservice.h>
#include <cxxtools/eventloop.h>

/** echo whatever received by the client*/
std::string echo(const std::string& message)
{
    std::cout << message << std::endl;
    return message;
}

/** return sum to clients*/
double add(double a1, double a2)
{
  return a1 + a2;
}

/** return subtraction result to client*/
double subtract(double a1, double a2)
{
  std::cout << "a1 is " << a1 << " and a2 is " << a2 << std::endl;
  return (a1 - a2);
}

/** process initialization request at the database side*/
int init_req_processing(Init_req* init_req, Init_resp *init_resp)
{
    std::cout << "rulset id " << init_req->deviceDesc.rulesetIds.at(0)<< std::endl;
    if(init_req->location.point.center.latitude == DEFAULT_LONGITUDE || \
       init_req->location.point.center.latitude == DEFAULT_LATITUDE)
    {
        printf("request for re-transmit\n");
        return E_MISSING;
    }        
    else
    {    
        std::cout << "latitude " << init_req->location.point.center.latitude << std::endl;
        std::cout << "longitude " << init_req->location.point.center.longitude << std::endl;
    }
    
    std::cout << "semiMajorAxis " << init_req->location.point.semiMajorAxis << std::endl;
    std::cout << "semiMinorAxis " << init_req->location.point.semiMinorAxis << std::endl;
    
    RulesetInfo rulesetInfo_1;
    rulesetInfo_1.authority = "FCC";
    rulesetInfo_1.rulesetId = "FccTvBandWhiteSpace-2010";
    rulesetInfo_1.maxLocationChange = 100.0; //make up some number
    rulesetInfo_1.maxPollingSecs = 60; //make up some number
    init_resp->rulesetInfos.push_back(rulesetInfo_1);
    
    return 0;
}

/** process registration request*/
int registration_req_processing(Registration_req* reg_req, Registration_resp* reg_resp)
{
    std::cout << "deviceDesc rule " << reg_req->deviceDesc.rulesetIds.at(0)  << std::endl;
    std::cout << "latitude " << reg_req->location.point.center.latitude << std::endl;
    std::cout << "longitude " << reg_req->location.point.center.longitude << std::endl;
    std::cout << "deviceOwner " << reg_req->deviceOwner.change_later << std::endl;
    std::cout << "antenna " << reg_req->antenna.heightType << std::endl;
    //if(init_req.deviceDesc)std::string init_req_s
            
    DatabaseSpec dbSpec1, dbSpec2;
    RulesetInfo rulesetInfo_1;
    rulesetInfo_1.authority = "FCC";
    rulesetInfo_1.rulesetId = "FccTvBandWhiteSpace-2010";
    rulesetInfo_1.maxLocationChange = 100.0;
    rulesetInfo_1.maxPollingSecs = 60;
    reg_resp->rulesetInfos.push_back(rulesetInfo_1);
    
    dbSpec1.name = "Jun";
    dbSpec1.uri = "https://jun.com hihihi";  
    
    dbSpec2.name = "bowen";
    dbSpec2.uri = "https://bowen.com hihihi";  
    reg_resp->databasechange.databases.push_back(dbSpec1);
    reg_resp->databasechange.databases.push_back(dbSpec2);
    
    return 0;
}

/** process spectrum query request sent from clients*/
int spectrum_req_processing(Avail_spectrum_req* spectrum_req, Avail_spectrum_resp* spectrum_resp)
{               
    spectrum_resp->deviceDesc = spectrum_req->deviceDesc;
    
    SpectrumSpec spec1;
    spec1.rulesetInfo.authority = "FCC";
    
    SpectrumSchedule schedule1;
    
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);
    
    schedule1.eventTime.startTime = str;
    schedule1.eventTime.stopTime = str+" + 1hour";
    
    Spectrum spectrum1;
    spectrum1.resolutionBwHz = 6000000;
    
    SpectrumProfile profile1;
    
    SpectrumProfilePoint point1, point2, point3, point4;
    point1.hz = 518000000; point1.dbm = 30.0; 
    point2.hz = 536000000; point2.dbm = 30.0; 
    point3.hz = 536000000; point3.dbm = 36.0; 
    point4.hz = 542000000; point4.dbm = 36.0;
    profile1.spectrumProfilePoints.push_back(point1);
    profile1.spectrumProfilePoints.push_back(point2);
    profile1.spectrumProfilePoints.push_back(point3);
    profile1.spectrumProfilePoints.push_back(point4);
    
    SpectrumProfile profile2;
    point1.hz = 536000000; point1.dbm = 27.0; 
    point2.hz = 542000000; point2.dbm = 27.0; 
    profile2.spectrumProfilePoints.push_back(point1);
    profile2.spectrumProfilePoints.push_back(point2);
    
    spectrum1.profiles.push_back(profile1);
    spectrum1.profiles.push_back(profile2);
    
    schedule1.spectra.push_back(spectrum1);
    
    
    spec1.specSchedules.push_back(schedule1);
    spectrum_resp->spectrumSpecs.push_back(spec1);
    return 0;
}

/** process notification, add more content when database is added to the system*/
int notification_processing(Spectrum_use_notify* spectrum_notify, Spectrum_use_resp * notify_resp)
{        
    return 0;
}

/**
 * convert serialized string initialization request to PAWS structure. After 
 * processing, sends response to clients.
 */
std::string init_req(std::string init_req_s)
{
    int status = 0;
    Init_req init_req;
    Init_resp init_resp;
    std::stringstream req_stream;
    std::stringstream resp_stream;
    req_stream << init_req_s;    
    cxxtools::JsonDeserializer deserializer(req_stream);
    deserializer.deserialize(init_req);
    
    if((status=init_req_processing(&init_req, &init_resp))==0)
    {        
        cxxtools::JsonSerializer serializer(resp_stream);
        serializer.beautify(true);   // this makes it just nice to read
        serializer.serialize(init_resp).finish();
        std::string init_resp_s = resp_stream.str();        
        return init_resp_s;
    }
    else if(status == E_MISSING)
    {
        ErrorElement error;
        error.code = E_MISSING;
        error.message = "";
        
        cxxtools::JsonSerializer serializer(resp_stream);
        serializer.beautify(true);   // this makes it just nice to read
        serializer.serialize(error).finish();
        std::string error_s = resp_stream.str();
        std::cout << error_s;
        return error_s;        
    }   
}

/**
 * Registration request will be processed.
 * @param registeration_req_s
 * @return 
 */
std::string register_req(std::string registeration_req_s)
{
    Registration_req registration_req;
    Registration_resp registration_resp;
    std::stringstream oos;
    oos << registeration_req_s;    
    cxxtools::JsonDeserializer deserializer(oos);
    deserializer.deserialize(registration_req);
    
    if(registration_req_processing(&registration_req,&registration_resp)!=0)
        printf("registration request processing error\n");
    
    std::stringstream os;
    cxxtools::JsonSerializer serializer(os);
    serializer.beautify(true);   // this makes it just nice to read
    serializer.serialize(registration_resp).finish();
    std::string registration_resp_s = os.str();
    std::cout << registration_resp_s;    
    return registration_resp_s;    
}

/**
 * Spectrum query will be process here.
 * @param spec_req_s
 * @return 
 */
std::string spec_req(std::string spec_req_s)
{
    Avail_spectrum_req spectrum_req;
    Avail_spectrum_resp spectrum_resp;
    std::stringstream oos;
    oos << spec_req_s;    
    cxxtools::JsonDeserializer deserializer(oos);
    deserializer.deserialize(spectrum_req);
    
    if(spectrum_req_processing(&spectrum_req,&spectrum_resp)!=0)
        printf("registration request processing error\n");
    
    std::stringstream os;
    cxxtools::JsonSerializer serializer(os);
    serializer.beautify(true);   // this makes it just nice to read
    serializer.serialize(spectrum_resp).finish();
    std::string spectrum_resp_s = os.str();
    std::cout << spectrum_resp_s;    
    return spectrum_resp_s;    
}

/**
 * Notification will be processed here.
 * @param notification
 * @return 
 */
std::string notification(std::string notification)
{
    Spectrum_use_notify spectrum_notify;
    Spectrum_use_resp notify_resp;    
    std::stringstream oos;
    oos << notification;    
    cxxtools::JsonDeserializer deserializer(oos);
    deserializer.deserialize(spectrum_notify);
    
    if(notification_processing(&spectrum_notify, &notify_resp)!=0)
        printf("registration request processing error\n");
    
    std::stringstream os;
    cxxtools::JsonSerializer serializer(os);
    serializer.beautify(true);   // this makes it just nice to read
    serializer.serialize(notify_resp).finish();
    std::string notify_resp_s = os.str();
    std::cout << notify_resp_s;    
    return notify_resp_s;
}



////////////////////////////////////////////////////////////////////////
// main
//
int main(int argc, char* argv[])
{
    try
    {
        // initialize logging - this reads the file log.xml from the current directory
        log_init();

        // read the command line options

        // option -i <ip-address> defines the ip address of the interface, where the
        // server waits for connections. Default is empty, which tells the server to
        // listen on all local interfaces
        cxxtools::Arg<std::string> ip(argc, argv, 'i');

        // option -p <number> specifies the port, where http requests are expected.
        // This port is valid for xmlrpc and json over http. It defaults to 7002.
        cxxtools::Arg<unsigned short> port(argc, argv, 'p', 7002);

        // option -b <number> specifies the port, where the binary server waits for
        // requests. It defaults to port 7003.
        cxxtools::Arg<unsigned short> bport(argc, argv, 'b', 7003);

        // option -j <number> specifies the port, where the json server wait for
        // requests. It defaults to port 7004.
        cxxtools::Arg<unsigned short> jport(argc, argv, 'j', 7004);

        std::cout << "run rpcecho server\n"
                  << "http protocol on port "<< port.getValue() << "\n"
                  << "binary protocol on port " << bport.getValue() << "\n"
                  << "json protocol on port " << jport.getValue() << std::endl;

        // create an event loop
        cxxtools::EventLoop loop;

        // the http server is instantiated with an ip address and a port number
        // It will be used for xmlrpc and json over http on different urls.
        cxxtools::http::Server httpServer(loop, ip, port);

        ////////////////////////////////////////////////////////////////////////
        // Json rpc over http

        // for json over http we need a service object
        cxxtools::json::HttpService jsonhttpService;

        // we register our functions
        jsonhttpService.registerFunction("echo", echo);
        jsonhttpService.registerFunction("add", add);
        jsonhttpService.registerFunction("subtract",subtract);
        jsonhttpService.registerFunction("init_req",init_req);
        jsonhttpService.registerFunction("register_req",register_req);
        jsonhttpService.registerFunction("spec_req", spec_req);
        jsonhttpService.registerFunction("notification", notification);
                
        // ... and register the service under a url
        httpServer.addService("/jsonrpc", jsonhttpService);
        // now start the servers by running the event loop
        loop.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

