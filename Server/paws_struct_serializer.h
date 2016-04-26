/* 
 * File:   paws_struct_serializer.h
 * Author: bx
 *
 * Created on October 15, 2015, 1:05 PM
 * 
 * This function mainly concerns with how to convert a structure into a long
 * string which can be transmitted in the Internet. 
 */

#ifndef PAWS_STRUCT_SERIALIZER_H
#define	PAWS_STRUCT_SERIALIZER_H

#include "paws_struct.h"
#include <iostream>
#include <cxxtools/jsonserializer.h>
#include <cxxtools/serializationinfo.h>
#include <cxxtools/deserializer.h>
#include <cxxtools/jsondeserializer.h>
/* RulesetInfo*/
void operator<<= (cxxtools::SerializationInfo& rulesetInfo_s, const RulesetInfo& rulesetInfo)
{
    rulesetInfo_s.addMember("authority") <<= rulesetInfo.authority;
    rulesetInfo_s.addMember("rulesetId") <<= rulesetInfo.rulesetId; 
    rulesetInfo_s.addMember("maxLocationChange") <<= rulesetInfo.maxLocationChange; 
    rulesetInfo_s.addMember("maxPollingSecs") <<= rulesetInfo.maxPollingSecs; 
}
void operator>>= (const cxxtools::SerializationInfo& rulesetInfo_s, RulesetInfo& rulesetInfo)
{
    rulesetInfo_s.getMember("authority") >>= rulesetInfo.authority;
    rulesetInfo_s.getMember("rulesetId") >>= rulesetInfo.rulesetId;    
    rulesetInfo_s.getMember("maxLocationChange") >>= rulesetInfo.maxLocationChange; 
    rulesetInfo_s.getMember("maxPollingSecs") >>= rulesetInfo.maxPollingSecs; 
}
/* device descriptor */
void operator<<= (cxxtools::SerializationInfo& deviceDesc_s, const DeviceDescriptor& deviceDesc)
{
    deviceDesc_s.addMember("rulesetIds") <<= deviceDesc.rulesetIds;
    deviceDesc_s.addMember("serialNumber") <<= deviceDesc.serialNumber;
    deviceDesc_s.addMember("manufactureId") <<= deviceDesc.manufactureId;
    deviceDesc_s.addMember("modelId") <<= deviceDesc.modelId;    
}
void operator>>= (const cxxtools::SerializationInfo& deviceDesc_s, DeviceDescriptor& deviceDesc)
{
    deviceDesc_s.getMember("rulesetIds") >>= deviceDesc.rulesetIds;
    deviceDesc_s.getMember("serialNumber") >>= deviceDesc.serialNumber;
    deviceDesc_s.getMember("manufactureId") >>= deviceDesc.manufactureId;
    deviceDesc_s.getMember("modelId") >>= deviceDesc.modelId;    
}
/* Point */
void operator<<= (cxxtools::SerializationInfo& point_s, const Point& point)
{
    point_s.addMember("latitude") <<= point.latitude;
    point_s.addMember("longitude") <<= point.longitude;
}
void operator>>= (const cxxtools::SerializationInfo& point_s, Point& point)
{
    point_s.getMember("latitude") >>= point.latitude;
    point_s.getMember("longitude") >>= point.longitude;        
}
/* Polygon*/
void operator<<= (cxxtools::SerializationInfo& polygon_s, const Polygon& polygon)
{
    polygon_s.addMember("exterior") <<= polygon.vertices;
}
void operator>>= (const cxxtools::SerializationInfo& polygon_s, Polygon& polygon)
{
    polygon_s.getMember("exterior") >>= polygon.vertices;       
}
/* Ellipse*/
void operator<<= (cxxtools::SerializationInfo& ellipse_s, const Ellipse& ellipse)
{
    ellipse_s.addMember("center") <<= ellipse.center;
    ellipse_s.addMember("semiMajorAxis") <<= ellipse.semiMajorAxis;
    ellipse_s.addMember("semiMinorAxis") <<= ellipse.semiMinorAxis;
    ellipse_s.addMember("orientation") <<= ellipse.orientation;        
}
void operator>>= (const cxxtools::SerializationInfo& ellipse_s, Ellipse& ellipse)
{
    ellipse_s.getMember("center") >>= ellipse.center;
    ellipse_s.getMember("semiMajorAxis") >>= ellipse.semiMajorAxis;
    ellipse_s.getMember("semiMinorAxis") >>= ellipse.semiMinorAxis;
    ellipse_s.getMember("orientation") >>= ellipse.orientation;     
}
/* Geolocation*/
void operator<<= (cxxtools::SerializationInfo& geoLocation_s, const GeoLocation& geoLocation)
{
    geoLocation_s.addMember("ellipse")<<= geoLocation.point;
    geoLocation_s.addMember("polygon")<<= geoLocation.region;
    geoLocation_s.addMember("confidence")<<= geoLocation.confidence;
}
void operator>>= (const cxxtools::SerializationInfo& geoLocation_s, GeoLocation& geoLocation)
{
    geoLocation_s.getMember("ellipse")>>= geoLocation.point;
    geoLocation_s.getMember("polygon")>>= geoLocation.region;
    geoLocation_s.getMember("confidence")>>= geoLocation.confidence;      
}


/* Device Owner*/
void operator<<= (cxxtools::SerializationInfo& deviceOwner_s, const DeviceOwner& deviceOwner)
{
    deviceOwner_s.addMember("change_later")<<= deviceOwner.change_later;    
}
void operator>>= (const cxxtools::SerializationInfo& deviceOwner_s, DeviceOwner& deviceOwner)
{
    deviceOwner_s.getMember("change_later")>>= deviceOwner.change_later;    
}


/*   Antenna characteristics*/
void operator<<= (cxxtools::SerializationInfo& antenna_s, const AntennaCharacteristics& antenna)
{
    antenna_s.addMember("height") <<= antenna.height;
    antenna_s.addMember("heightType") <<= antenna.heightType;
    antenna_s.addMember("heightUncertainty") <<= antenna.heightUncertainty;    
}
void operator>>= (const cxxtools::SerializationInfo& antenna_s, AntennaCharacteristics& antenna)
{
    antenna_s.getMember("height") >>= antenna.height;
    antenna_s.getMember("heightType") >>= antenna.heightType;
    antenna_s.getMember("heightUncertainty") >>= antenna.heightUncertainty;    
}

/*      DatabseSpec*/
void operator<<= (cxxtools::SerializationInfo& databaseSpec_s, const DatabaseSpec& databaseSpec)
{
    databaseSpec_s.addMember("name") <<= databaseSpec.name;
    databaseSpec_s.addMember("uri") <<= databaseSpec.uri;    
}
void operator>>= (const cxxtools::SerializationInfo& databaseSpec_s, DatabaseSpec& databaseSpec)
{
    databaseSpec_s.getMember("name") >>= databaseSpec.name;
    databaseSpec_s.getMember("uri") >>= databaseSpec.uri;    
}

/*      DBupdateSpec*/
void operator<<= (cxxtools::SerializationInfo& dbUpdateSpec_s, const DbUpdateSpec& dbUpdateSpec)
{
    dbUpdateSpec_s.addMember("databases") <<= dbUpdateSpec.databases;
}
void operator>>= (const cxxtools::SerializationInfo& dbUpdateSpec_s, DbUpdateSpec& dbUpdateSpec)
{
    dbUpdateSpec_s.getMember("databases") >>= dbUpdateSpec.databases;
}

/*      ErrorElement*/
void operator<<= (cxxtools::SerializationInfo& errorElement_s, const ErrorElement& errorElement)
{    
    errorElement_s.addMember("code") <<= errorElement.code;
    errorElement_s.addMember("message") <<= errorElement.message;
}
void operator>>= (const cxxtools::SerializationInfo& errorElement_s, ErrorElement& errorElement)
{
    errorElement_s.getMember("code") >>= errorElement.code;
    errorElement_s.getMember("message") >>= errorElement.message;
}

/* FrequencyRange       */
void operator<<= (cxxtools::SerializationInfo& frequency_range_s, const FrequencyRange& frequency_range)
{    
    frequency_range_s.addMember("startHz") <<= frequency_range.startHz;    
    frequency_range_s.addMember("stopHz") <<= frequency_range.stopHz; 
}
void operator>>= (const cxxtools::SerializationInfo& frequency_range_s, FrequencyRange& frequency_range)
{    
    frequency_range_s.getMember("startHz") >>= frequency_range.startHz;    
    frequency_range_s.getMember("stopHz") >>= frequency_range.stopHz; 
}

/* DeviceCapabilities   */
void operator<<= (cxxtools::SerializationInfo& device_capabilities_s, const DeviceCapabilities& device_capabilities)
{    
    device_capabilities_s.addMember("frequencyRanges") <<= device_capabilities.frequencyRanges;    
}
void operator>>= (const cxxtools::SerializationInfo& device_capabilities_s, DeviceCapabilities& device_capabilities)
{    
    device_capabilities_s.getMember("frequencyRanges") >>= device_capabilities.frequencyRanges;    
}

/* Event time                     */
void operator<<= (cxxtools::SerializationInfo& event_time_s, const EventTime& event_time)
{
    event_time_s.addMember("startTime")<<= event_time.startTime;
    event_time_s.addMember("stopTime")<<= event_time.stopTime;    
}
void operator>>= (const cxxtools::SerializationInfo& event_time_s, EventTime& event_time)
{
    event_time_s.getMember("startTime")>>= event_time.startTime;
    event_time_s.getMember("stopTime")>>= event_time.stopTime;    
}

/* SpectrumProfilePoint         */
void operator<<= (cxxtools::SerializationInfo& spectrum_profile_point_s, const SpectrumProfilePoint& spectrum_profile_point)
{
    spectrum_profile_point_s.addMember("dbm")<<= spectrum_profile_point.dbm;
    spectrum_profile_point_s.addMember("hz")<<= spectrum_profile_point.hz;    
}
void operator>>= (const cxxtools::SerializationInfo& spectrum_profile_point_s, SpectrumProfilePoint& spectrum_profile_point)
{
    spectrum_profile_point_s.getMember("dbm")>>= spectrum_profile_point.dbm;
    spectrum_profile_point_s.getMember("hz")>>= spectrum_profile_point.hz;    
}

/* SpectrumProfile*/
void operator<<= (cxxtools::SerializationInfo& spectrum_profile_s, const SpectrumProfile& spectrum_profile)
{
    spectrum_profile_s.addMember("spectrumProfilePoints")<<= spectrum_profile.spectrumProfilePoints;    
}
void operator>>= (const cxxtools::SerializationInfo& spectrum_profile_s, SpectrumProfile& spectrum_profile)
{
    spectrum_profile_s.getMember("spectrumProfilePoints")>>= spectrum_profile.spectrumProfilePoints;    
}

/* Spectrum     */
void operator<<= (cxxtools::SerializationInfo& spectrum_s, const Spectrum& spectrum)
{
    spectrum_s.addMember("resolutionBwHz")<<= spectrum.resolutionBwHz;
    spectrum_s.addMember("profiles")<<= spectrum.profiles;    
}
void operator>>= (const cxxtools::SerializationInfo& spectrum_s, Spectrum& spectrum)
{
    spectrum_s.getMember("resolutionBwHz")>>= spectrum.resolutionBwHz;
    spectrum_s.getMember("profiles")>>= spectrum.profiles;    
}

/* SpectrumSchedule */
void operator<<= (cxxtools::SerializationInfo& spectrum_schedule_s, const SpectrumSchedule& spectrum_schedule)
{
    spectrum_schedule_s.addMember("eventTime")<<= spectrum_schedule.eventTime;
    spectrum_schedule_s.addMember("spectra")<<= spectrum_schedule.spectra;    
}

void operator>>= (const cxxtools::SerializationInfo& spectrum_schedule_s, SpectrumSchedule& spectrum_schedule)
{
    spectrum_schedule_s.getMember("eventTime")>>= spectrum_schedule.eventTime;
    spectrum_schedule_s.getMember("spectra")>>= spectrum_schedule.spectra;    
}

/* spectrum spec*/
void operator<<= (cxxtools::SerializationInfo& spectrum_spec_s, const SpectrumSpec& spectrum_spec)
{
    spectrum_spec_s.addMember("rulesetInfo")<<= spectrum_spec.rulesetInfo;
    spectrum_spec_s.addMember("specSchedules")<<= spectrum_spec.specSchedules;    
}

void operator>>= (const cxxtools::SerializationInfo& spectrum_spec_s, SpectrumSpec& spectrum_spec)
{
    spectrum_spec_s.getMember("rulesetInfo")>>= spectrum_spec.rulesetInfo;
    spectrum_spec_s.getMember("specSchedules")>>= spectrum_spec.specSchedules;    
}

/**             Initialization request               **/
void operator<<= (cxxtools::SerializationInfo& init_req_s, const Init_req& init_req)
{        
    init_req_s.addMember("deviceDesc") <<= init_req.deviceDesc;    
    init_req_s.addMember("geoLocation") <<= init_req.location;
}

void operator>>= (const cxxtools::SerializationInfo& init_req_s, Init_req& init_req)
{
    init_req_s.getMember("deviceDesc") >>= init_req.deviceDesc;    
    init_req_s.getMember("geoLocation") >>= init_req.location;
}

void operator<<= (cxxtools::SerializationInfo& init_resp_s, const Init_resp& init_resp)
{ 
    init_resp_s.addMember("rulesetInfos") <<= init_resp.rulesetInfos;
}
void operator>>= (const cxxtools::SerializationInfo& init_resp_s, Init_resp& init_resp)
{
    init_resp_s.getMember("rulesetInfos") >>= init_resp.rulesetInfos;
}

/**             Registration request    */
void operator<<= (cxxtools::SerializationInfo& registration_req_s, const Registration_req& registration_req)
{ 
    registration_req_s.addMember("deviceDesc") <<= registration_req.deviceDesc;
    registration_req_s.addMember("geoLocation") <<= registration_req.location;
    registration_req_s.addMember("deviceOwner") <<= registration_req.deviceOwner;
    registration_req_s.addMember("antenna") <<= registration_req.antenna;    
}
void operator>>= (const cxxtools::SerializationInfo& registration_req_s, Registration_req& registration_req)
{
    registration_req_s.getMember("deviceDesc") >>= registration_req.deviceDesc;
    registration_req_s.getMember("geoLocation") >>= registration_req.location;
    registration_req_s.getMember("deviceOwner") >>= registration_req.deviceOwner;
    registration_req_s.getMember("antenna") >>= registration_req.antenna;
}

/**             Registration response    */
void operator<<= (cxxtools::SerializationInfo& registration_resp_s, const Registration_resp& registration_resp)
{ 
    registration_resp_s.addMember("rulesetInfos") <<= registration_resp.rulesetInfos;
    registration_resp_s.addMember("databases") <<= registration_resp.databasechange;   
}
void operator>>= (const cxxtools::SerializationInfo& registration_resp_s, Registration_resp& registration_resp)
{
    registration_resp_s.getMember("rulesetInfos") >>= registration_resp.rulesetInfos;
    registration_resp_s.getMember("databases") >>= registration_resp.databasechange;
}

/**             Spectrum request    */
void operator<<= (cxxtools::SerializationInfo& spectrum_req_s, const Avail_spectrum_req& spectrum_req)
{ 
    spectrum_req_s.addMember("deviceDesc") <<= spectrum_req.deviceDesc;
    spectrum_req_s.addMember("geoLocation") <<= spectrum_req.location;
    spectrum_req_s.addMember("deviceOwner") <<= spectrum_req.deviceOwner;
    spectrum_req_s.addMember("antenna") <<= spectrum_req.antenna;
    spectrum_req_s.addMember("capabilities") <<= spectrum_req.capabilities;             
}
void operator>>= (const cxxtools::SerializationInfo& spectrum_req_s, Avail_spectrum_req& spectrum_req)
{ 
    spectrum_req_s.getMember("deviceDesc") >>= spectrum_req.deviceDesc;
    spectrum_req_s.getMember("geoLocation") >>= spectrum_req.location;
    spectrum_req_s.getMember("deviceOwner") >>= spectrum_req.deviceOwner;
    spectrum_req_s.getMember("antenna") >>= spectrum_req.antenna;
    spectrum_req_s.getMember("capabilities") >>= spectrum_req.capabilities;             
}

/*            Spectrum response    */
void operator<<= (cxxtools::SerializationInfo& spectrum_resp_s, const Avail_spectrum_resp& spectrum_resp)
{ 
    spectrum_resp_s.addMember("deviceDesc") <<= spectrum_resp.deviceDesc;
    spectrum_resp_s.addMember("databases") <<= spectrum_resp.databseChange;   
    spectrum_resp_s.addMember("timestamp") <<= spectrum_resp.timestamp;   
    spectrum_resp_s.addMember("spectrumSpecs") <<= spectrum_resp.spectrumSpecs;   
}
void operator>>= (const cxxtools::SerializationInfo& spectrum_resp_s, Avail_spectrum_resp& spectrum_resp)
{ 
    spectrum_resp_s.getMember("deviceDesc") >>= spectrum_resp.deviceDesc;
    spectrum_resp_s.getMember("databases") >>= spectrum_resp.databseChange;   
    spectrum_resp_s.getMember("timestamp") >>= spectrum_resp.timestamp;   
    spectrum_resp_s.getMember("spectrumSpecs") >>= spectrum_resp.spectrumSpecs;   
}

/*            notification     */
void operator<<= (cxxtools::SerializationInfo& notify_s, const Spectrum_use_notify& notify)
{ 
    notify_s.addMember("deviceDesc") <<= notify.deviceDesc;
    notify_s.addMember("location") <<= notify.location;   
    notify_s.addMember("masteDeviceDesc") <<= notify.masteDeviceDesc;   
    notify_s.addMember("masterDeviceLocation") <<= notify.masterDeviceLocation;   
    notify_s.addMember("spectra") <<= notify.spectra;       
}
void operator>>= (const cxxtools::SerializationInfo& notify_s, Spectrum_use_notify& notify)
{ 
    notify_s.getMember("deviceDesc") >>= notify.deviceDesc;
    notify_s.getMember("location") >>= notify.location;   
    notify_s.getMember("masteDeviceDesc") >>= notify.masteDeviceDesc;   
    notify_s.getMember("masterDeviceLocation") >>= notify.masterDeviceLocation;   
    notify_s.getMember("spectra") >>= notify.spectra;       
}

/* notification response        */
void operator<<= (cxxtools::SerializationInfo& use_response_s, const Spectrum_use_resp& use_response)
{ 
    use_response_s.addMember("databaseChange") <<= use_response.databaseChange;    
}

void operator>>= (const cxxtools::SerializationInfo& use_response_s, Spectrum_use_resp& use_response)
{ 
    use_response_s.getMember("databaseChange") >>= use_response.databaseChange;    
}

#endif	/* PAWS_STRUCT_SERIALIZER_H */