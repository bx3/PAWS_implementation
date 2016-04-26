/* 
 * File:   paws_struct.h
 * Author: bx
 *
 * Created on October 14, 2015, 8:30 PM
 */

#ifndef PAWS_STRUCT_H
#define	PAWS_STRUCT_H
#include <string>
#include <vector>
#define AGL  0
#define AMSL 1

/** Error Code **/
#define E_VERSION -101 
#define E_UNSUPPORTED -102
#define E_UNIMPLEMENTED -103 
#define E_OUTSIDE_COVERAGE -104 
#define E_DATABASE_CHANGE -105
#define E_MISSING -201 
#define E_INVALID_VALUE -202
#define E_UNAUTHORIZED -301 
#define E_NOT_REGISTERED -302 
#define E_NOT_CONNECTTED -1 //constom
#define E_DATA_CREATION -2
#define E_CONNECTION_LOST -3

/**     Geo-Location **/
struct Point{
    float latitude;
    float longitude;
};

struct Ellipse {
    Point center;
    float semiMajorAxis;
    float semiMinorAxis;
    float orientation;
};

struct Polygon {
    //Point vertices[16];
    std::vector<Point> vertices;
};

struct GeoLocation {
    Ellipse point;
    Polygon region;
    int confidence;
};

/** Device descriptor  **/
struct DeviceDescriptor {
    std::string serialNumber;
    std::string manufactureId;
    std::string modelId;
    std::vector<std::string> rulesetIds;    
};

struct AntennaCharacteristics {
    float height;
    int heightType;
    float heightUncertainty;
};

/** Device Capability **/
struct FrequencyRange {
    float startHz;
    float stopHz;
};

struct DeviceCapabilities {
    std::vector<FrequencyRange> frequencyRanges;    
};

/** Device Owner **/
struct DeviceOwner
{
    std::string change_later;
    //vcard owner    
};

/** Rulesetinfo**/
struct RulesetInfo
{
    std::string authority;
    std::string rulesetId;
    float maxLocationChange;
    int maxPollingSecs;    
};

/**   DBupdate **/
struct DatabaseSpec {
    std::string name;
    std::string uri;
};
struct DbUpdateSpec {
    std::vector<DatabaseSpec> databases;
};

/** Spectrum spec**/
struct EventTime {
    std::string startTime;
    std::string stopTime;
};

/** Spectrum Schedule**/
struct SpectrumProfilePoint {
    float hz;
    float dbm;
};
struct SpectrumProfile {
    std::vector<SpectrumProfilePoint> spectrumProfilePoints;
};
struct Spectrum {
    float resolutionBwHz;
    std::vector<SpectrumProfile> profiles;
};

struct SpectrumSchedule
{
    EventTime eventTime;
    std::vector<Spectrum> spectra;
};

/** Spectrumm spec**/
struct SpectrumSpec {
    RulesetInfo rulesetInfo;
    std::vector<SpectrumSchedule> specSchedules;
};

/** Geo Spectrum spec**/
struct GeoSpectrumSpec {
    GeoLocation location;
    std::vector<SpectrumSpec> spectrumSpecList;
};

/** Device Validity */
struct DeviceValidity {
    DeviceDescriptor deviceDesc;
    bool isValid;
    std::string reason;
};

/** Error element **/
struct ErrorElement {
    int code;
    std::string message;
};

struct Init_req {
    DeviceDescriptor deviceDesc;
    GeoLocation location;
};

struct Init_resp {
    std::vector<RulesetInfo> rulesetInfos;
};

struct Registration_req{
    DeviceDescriptor deviceDesc;
    GeoLocation location;
    DeviceOwner deviceOwner;
    AntennaCharacteristics antenna;
};

struct Registration_resp{
    std::vector<RulesetInfo> rulesetInfos;
    DbUpdateSpec databasechange;
};

struct Avail_spectrum_req{
    DeviceDescriptor deviceDesc;
    GeoLocation location;
    DeviceOwner deviceOwner;
    AntennaCharacteristics antenna;
    DeviceCapabilities capabilities;
    DeviceDescriptor masterDeviceDesc;
    GeoLocation masterGeoLocation;
    std::string requestType;
};

struct Avail_spectrum_resp{
    std::string timestamp;
    DeviceDescriptor deviceDesc;
    std::vector<SpectrumSpec> spectrumSpecs;
    DbUpdateSpec databseChange;
};

struct Spectrum_use_notify {
    DeviceDescriptor deviceDesc;
    GeoLocation location;
    DeviceDescriptor masteDeviceDesc;
    GeoLocation masterDeviceLocation;
    std::vector<Spectrum> spectra;
};

struct Spectrum_use_resp {
    DbUpdateSpec databaseChange;
};

struct Dev_valid_req {
    std::vector<DeviceDescriptor> deviceDescs;
    DeviceDescriptor masterDeviceDesc;
};

struct Dev_valid_resp {
    std::vector<DeviceValidity> deviceValidities;
    DbUpdateSpec databaseChange;
};
#endif	/* PAWS_STRUCT_H */

