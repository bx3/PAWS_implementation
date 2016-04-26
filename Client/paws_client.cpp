/**
 * This file contains most functionality supported by PAWS. The implemented 
 * function contains: init_request, register_request, avail_spec_query and 
 * database notification.
 */
#include "paws_client.h"
#include "paws_struct_serializer.h"

/**
 * For testing purpose on one computer, we use the self-loop IP address default 
 * to most computer. The port has to been consistent to the server port.
 */
PAWS_client::PAWS_client()
{    
    server_ip = "127.0.0.1"; // 127.0.0.1 128.95.30.158
    port = 7002;
    notify_db = false;
}

/**
 * Initialize everything
 * @return 0 for success, otherwise fail.
 */
int PAWS_client::start_paws()
{           
    my_location.point.center.latitude = 47.6063889;                       
    my_location.point.center.longitude = -122.3308333;
    rulesetID = "FccTvBandWhiteSpace-2010";
    my_antenna.height = 10.2;
    my_antenna.heightType = AGL;    
    
    char choice;
    std::cout << "If use default setting (Y/n)" << std::endl;
    std::cin >> choice;
    if(choice == 'n' || choice == 'N')
    {
        std::cout << "please enter your location latitude" << std::endl;
        std::cin >> my_location.point.center.latitude;
        std::cout << "please enter your location longitude" << std::endl;
        std::cin >> my_location.point.center.longitude;  
        
    }    
    return 0;
}

/**
 * Giving upper layer an interface to enable database notification, if notify_db
 * is true, notification will be enabled.
 * @param choice
 * @return 
 */
int PAWS_client::is_enable_notification(bool choice)
{
    notify_db = choice;
    return 0;
}

/**
 * This is just a test function for RPC, one computer A is sending a and b, the 
 * remote computer B should compute the sum and return the result to the 
 * computer A in the end.
 * @param a
 * @param b
 * @return 
 */
double PAWS_client::add(double a, double b){
    cxxtools::json::HttpClient httpJsonClient(server_ip, port,"/jsonrpc");
    cxxtools::RemoteClient& theClient = static_cast<cxxtools::RemoteClient&>(httpJsonClient);
    cxxtools::RemoteProcedure<double, double, double> add(theClient, "add");   
    double result = 0;
    result = add(a,b);
    std::cout << result << std::endl;
    return result;
}

int PAWS_client::report_error(ErrorElement * error)
{
    printf("Error code %d\n", error->code);
    if(!error->message.empty())
        std::cout << "Error message: " << error->message << std::endl;        
    return 0;
}

/**
 * Function to be called by the upper layer. The sender will sends 
 * initialization to the database, format the same as PAWS documentation.
 * @return 
 */
int PAWS_client::init_request()
{
    int status = -1;
    // TX data    
    cxxtools::json::HttpClient httpJsonClient(server_ip, port,"/jsonrpc");
    cxxtools::RemoteClient& theClient = static_cast<cxxtools::RemoteClient&>(httpJsonClient);    
    cxxtools::RemoteProcedure<std::string, std::string> init_request(theClient, "init_req");      
    std::stringstream os;
    Init_req init_req; 
    if(create_init_req_struct(&init_req)!=0)
    {
        std::cout << "init request data creation" << std::endl;
        return E_DATA_CREATION;
    }            
    cxxtools::JsonSerializer serializer(os);   
    serializer.beautify(true);   // this makes it just nice to read   
    serializer.serialize(init_req).finish();   
    std::string init_req_s = os.str();
    //std::cout << init_req_s << std::endl;     
    try{
        std::string init_resp_s = init_request(init_req_s);  // this variable corresponds to remoteProcedure object 
        status = process_init_resp_string(&init_resp_s);            
    }
    catch (const std::exception e)
    {
        std::cout << "no server available, check server URL and retry\n" << std::endl;
        status = E_NOT_CONNECTTED; //connection refused
    }
    return status;
}

/**
 * Create a structure for initial request, the RPC library doesn't have the 
 * ability to make a default value for variables unspecified, so server should 
 * only checks for the parts, it expects to see.
 * @param init_req
 * @return 
 */
int PAWS_client::create_init_req_struct(Init_req* init_req)
{
    init_req->deviceDesc.rulesetIds.push_back(rulesetID);
    init_req->deviceDesc.modelId = "";
    init_req->deviceDesc.serialNumber = "";
    init_req->deviceDesc.manufactureId = "";    
    init_req->location.confidence = DEFAULT_CONFIDENCE;
    init_req->location.point.center.latitude = my_location.point.center.latitude;//DEFAULT_LATITUDE;//
    init_req->location.point.center.longitude = my_location.point.center.longitude; //DEFAULT_LONGITUDE;//
    init_req->location.point.semiMajorAxis = DEFAULT_MAJOR_AXIS;
    init_req->location.point.semiMinorAxis = DEFAULT_MINOR_AXIS;
    init_req->location.point.orientation = DEFAULT_ORIENTATION;    
    return 0;
}

/**
 * After database responds to the init request, this function is used to 
 * process the string,
 * @param resp_s
 * @return 
 */
int PAWS_client::process_init_resp_string(std::string * resp_s)
{    
    std::stringstream resp_stream;
    resp_stream << *resp_s;
        
    Init_resp init_resp;    
    cxxtools::JsonDeserializer deserializer(resp_stream);
    deserializer.deserialize(init_resp);
    for(int i=0; i<init_resp.rulesetInfos.size(); i++)
    {
        std::cout << "authority is " << init_resp.rulesetInfos[i].authority << std::endl;
        std::cout << "rulesetId is " << init_resp.rulesetInfos[i].rulesetId << std::endl;
        std::cout << "maxLocationChange is " << init_resp.rulesetInfos[i].maxLocationChange << std::endl;
        std::cout << "maxPollingSecs is " << init_resp.rulesetInfos[i].maxPollingSecs << std::endl;
        std::cout << std::endl;    
    }
    return 0;
}

/**
 * In case the database requires registration, this function can be used.
 * @return 
 */

int PAWS_client::register_request()
{
    int status = -1;
    cxxtools::json::HttpClient httpJsonClient(server_ip, port,"/jsonrpc");
    cxxtools::RemoteClient& theClient = static_cast<cxxtools::RemoteClient&>(httpJsonClient);
    cxxtools::RemoteProcedure<std::string, std::string> registeration_request(theClient, "register_req");  
    std::stringstream req_stream;
    //Transit request
    Registration_req reg_req;
    if(create_register_req_struct(&reg_req)!=0)
    {
        std::cout << "request request data creation" << std::endl;
        return E_DATA_CREATION;
    }        
    cxxtools::JsonSerializer serializer(req_stream);
    serializer.beautify(true);   // this makes it just nice to read
    serializer.serialize(reg_req).finish();
    std::string reg_req_s = req_stream.str();   
        
    try{
        std::string reg_resp_s = registeration_request(reg_req_s);        
        //process response    
        status = process_register_resp_string(&reg_resp_s);   
    }
    catch (const std::exception e)
    {
        std::cout << "no server available, check server URL and retry\n" << std::endl;
        status = E_NOT_CONNECTTED; //connection refused
    }
    return status;
}

/**
 * This function is responsible for precessing the reply from the database at 
 * the computer side.
 * @param reg_resp_s
 * @return 
 */
int PAWS_client::process_register_resp_string(std::string* reg_resp_s)
{    
    std::stringstream resp_stream;
    resp_stream << *reg_resp_s;
    Registration_resp reg_resp;
    cxxtools::JsonDeserializer deserializer(resp_stream);
    deserializer.deserialize(reg_resp);
        
    for(int i=0; i<reg_resp.rulesetInfos.size(); i++)
    {
        std::cout << "authority is " << reg_resp.rulesetInfos[i].authority << std::endl;
        std::cout << "rulesetId is " << reg_resp.rulesetInfos[i].rulesetId << std::endl;        
        for (int j=0; j<reg_resp.databasechange.databases.size(); j++)
        {            
            std::cout << "database change is " << reg_resp.databasechange.databases.at(j).name << std::endl;
        }
        std::cout << std::endl;    
    }
    return 0;
}

/**
 * This function creates the registration structure, All the information is 
 * currently hard coded. It can later be designed with a terminal or 
 * graphic interface.
 * @param reg_req
 * @return 
 */
int PAWS_client::create_register_req_struct(Registration_req* reg_req)
{
    reg_req->deviceDesc.rulesetIds.push_back("FccTvBandWhiteSpace-2010");
    reg_req->deviceDesc.modelId= "";
    reg_req->deviceDesc.manufactureId= "";
    reg_req->deviceDesc.serialNumber= "";
    
    reg_req->location.confidence = DEFAULT_CONFIDENCE;
    reg_req->location.point.center.latitude = DEFAULT_LATITUDE;
    reg_req->location.point.center.longitude = DEFAULT_LONGITUDE; 
    reg_req->location.point.semiMajorAxis = DEFAULT_MAJOR_AXIS;
    reg_req->location.point.semiMinorAxis = DEFAULT_MINOR_AXIS;
    reg_req->location.point.orientation = DEFAULT_ORIENTATION;
    
    reg_req->antenna.height = my_antenna.height;
    reg_req->antenna.heightType = my_antenna.heightType;
    reg_req->antenna.heightUncertainty = DEFAULT_ANTENNA_HEIGHT_UNCERTAIN;            
    return 0;    
}

/**
 * This function sends spectrum query to the database, and process the received 
 * message.
 * @return 
 */
int PAWS_client::avail_spec_query()
{
    int status = -1;
    cxxtools::json::HttpClient httpJsonClient(server_ip, port,"/jsonrpc");
    cxxtools::RemoteClient& theClient = static_cast<cxxtools::RemoteClient&>(httpJsonClient);
    cxxtools::RemoteProcedure<std::string, std::string> avail_spec_query(theClient, "spec_req");  
    std::stringstream spec_stream;
    //Transit request
    Avail_spectrum_req spectrum_req;
    
    
    if(create_spec_req_struct(&spectrum_req)!=0)
    {
        std::cout << "request request data creation" << std::endl;
        return E_DATA_CREATION;;
    }        
    cxxtools::JsonSerializer serializer(spec_stream);
    serializer.beautify(true);   // this makes it just nice to read
    serializer.serialize(spectrum_req).finish();
    std::string spec_req_s = spec_stream.str();        
    std::cout << spec_req_s;
    
    try{
        std::string spec_resp_s = avail_spec_query(spec_req_s);            
        //Receive response
        //std::cout << spec_resp_s;
        status = process_spec_resp_string(&spec_resp_s);     
    }
    catch (const std::exception e)
    {
        std::cout << "no server available, check server URL and retry\n" << std::endl;
        status = E_NOT_CONNECTTED; //connection refused
    }
    return status;
}

/**
 * This function processes the spectrum query response from the database.
 * @param spec_resp_s
 * @return 
 */
int PAWS_client::process_spec_resp_string(std::string* spec_resp_s)
{   
    int status = -1;
    std::stringstream resp_stream;
    resp_stream << *spec_resp_s;
    Avail_spectrum_resp spec_resp;    
    cxxtools::JsonDeserializer deserializer(resp_stream);    
    deserializer.deserialize(spec_resp);                                    
    status = 0;
    decide_schedule(&spec_resp);
    
    if(notify_db)
    {        
        status = notify_database(); 
        std::cout << "databse ACKed" << std::endl;
    }                
    return status;
}

/**
 * This function sends and processes notification structure,
 * @return 
 */
int PAWS_client::notify_database()
{
    int status = -1;
    cxxtools::json::HttpClient httpJsonClient(server_ip, port,"/jsonrpc");
    cxxtools::RemoteClient& theClient = static_cast<cxxtools::RemoteClient&>(httpJsonClient);
    cxxtools::RemoteProcedure<std::string, std::string>  spectrum_use_notify(theClient, "notification");  
    std::stringstream notify_stream;
    //Transit request
    Spectrum_use_notify spectrum_notify;
        
    if(create_notify_struct(&spectrum_notify)!=0)
    {
        std::cout << "request request data creation" << std::endl;
        return E_DATA_CREATION;
    }        
    cxxtools::JsonSerializer serializer(notify_stream);
    serializer.beautify(true);   // this makes it just nice to read
    serializer.serialize(spectrum_notify).finish();
    std::string spectrum_notify_s = notify_stream.str();        
    std::cout << spectrum_notify_s;
        
    try{
        std::string notify_resp_s = spectrum_use_notify(spectrum_notify_s);
        //process response    
        status = process_use_notify_string(&notify_resp_s);   
    }
    catch (const std::exception e)
    {
        std::cout << "while sending notification to database, no response\n" << std::endl;
        status = E_CONNECTION_LOST; //connection refused
    }   
    return status;       
}

/**
 * This function creates notification message to the database.
 * @param spectrum_notify
 * @return 
 */
int PAWS_client::create_notify_struct(Spectrum_use_notify * spectrum_notify)
{
    spectrum_notify->deviceDesc.manufactureId = "notify producer";
    spectrum_notify->spectra.push_back(spectrum);
    return 0;
}

/**
 * This function processes the notification string.
 * @param notify_resp_s
 * @return 
 */
int PAWS_client::process_use_notify_string(std::string * notify_resp_s)
{
    std::stringstream resp_stream;
    resp_stream << *notify_resp_s;
    Spectrum_use_resp use_resp;    
    cxxtools::JsonDeserializer deserializer(resp_stream);    
    deserializer.deserialize(use_resp);
    return 0;
    
}

/**
 * After sending a spectrum query message, and receiving a response from 
 * database, this function asks human user to select spectrum information.
 * Currently, the information selected isn't much important, but later when 
 * a real database is used. This function needs to revise, now it is used for 
 * a demonstration purpose.  
 * @param spec_resp
 * @return 
 */
int PAWS_client::decide_schedule(Avail_spectrum_resp * spec_resp)
{
    for (int spec = 0; spec < spec_resp->spectrumSpecs.size(); spec++)
    {
        SpectrumSpec curr_spec = spec_resp->spectrumSpecs.at(spec);
        for (int schedule = 0; schedule< curr_spec.specSchedules.size(); schedule++)
        {            
            SpectrumSchedule curr_schedule = curr_spec.specSchedules.at(schedule);
            for (int spectrum = 0; spectrum < curr_schedule.spectra.size(); spectrum++)
            {                
                Spectrum curr_spectrum = curr_schedule.spectra.at(spectrum);
                std::cout << spectrum << ". " <<"Time start at " << \
                            curr_schedule.eventTime.startTime << " and stop at " << \
                            curr_schedule.eventTime.stopTime << std::endl;
                for (int profile = 0; profile < curr_spectrum.profiles.size(); profile++)
                {
                    
                    SpectrumProfile curr_profile = curr_spectrum.profiles.at(profile);
                    std::cout << "  resolutionBwHz " << curr_spectrum.resolutionBwHz << std::endl;
                    for (int point = 0; point < curr_profile.spectrumProfilePoints.size(); point++)
                    {                        
                        SpectrumProfilePoint curr_point = curr_profile.spectrumProfilePoints.at(point);
                        std::cout << "          " << curr_point.hz << " hz with " << curr_point.dbm << " dbm" << std::endl;
                    }            
                    std::cout << "----------------------------------------------" << std::endl;
                }
            }
        }    
    }

    std::cout << "choose a start time" << std::endl;
    std::cin >> my_event.startTime;
    std::cout << "choose a stop time" << std::endl;
    std::cin >> my_event.stopTime;
                
    std::cout << "choose the desired resolution bandwidth" << std::endl;
    std::cin >> spectrum.resolutionBwHz;
    SpectrumProfile profile; SpectrumProfilePoint point;
    std::cout << "choose center frequency" << std::endl;
    std::cin >> point.hz;
    std::cout << "choose power" << std::endl;
    std::cin >> point.dbm;       
    profile.spectrumProfilePoints.push_back(point);
    spectrum.profiles.push_back(profile);
    return 0;
}

/**
 * This function creates a data structure for querying spectrum information.
 * @param spec_req
 * @return 
 */
int PAWS_client::create_spec_req_struct(Avail_spectrum_req* spec_req)
{
    spec_req->deviceDesc.rulesetIds.push_back("FccTvBandWhiteSpace-2010");
    spec_req->deviceDesc.modelId= "";
    spec_req->deviceDesc.manufactureId= "";
    spec_req->deviceDesc.serialNumber= "";
    
    
    spec_req->location.confidence = DEFAULT_CONFIDENCE;
    spec_req->location.point.center.latitude = my_location.point.center.latitude;
    spec_req->location.point.center.longitude = my_location.point.center.longitude; 
    spec_req->location.point.semiMajorAxis = DEFAULT_MAJOR_AXIS;
    spec_req->location.point.semiMinorAxis = DEFAULT_MINOR_AXIS;
    spec_req->location.point.orientation = DEFAULT_ORIENTATION;
    
    spec_req->antenna.height = my_antenna.height;;
    spec_req->antenna.heightType = my_antenna.heightType;
    spec_req->antenna.heightUncertainty = DEFAULT_ANTENNA_HEIGHT_UNCERTAIN;            
    
    //FrequencyRange fr1, fr2;
    //fr1.startHz = 1000000000.0;
    //fr1.stopHz  = 1200000000.0;
    //fr2.startHz = 1300000000.0;
    //fr2.stopHz  = 1400000000.0;
    
    //spec_req->capabilities.frequencyRanges.push_back(fr1);
    //spec_req->capabilities.frequencyRanges.push_back(fr2);
   
    return 0;
}
