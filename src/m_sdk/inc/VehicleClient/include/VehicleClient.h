/******************************************************************
 *FILE: VehicleClient.h
 *SW-COMPONENT: vehcile client
 *DESCRIPTION: Declare VehicleClient class
 *COPYRIGHT: © 2020 Robert Bosch GmbH
 *
 *The reproduction, distribution and utilization of this file as
 *well as the communication of its contents to others without express
 *authorization is prohibited. Offenders will be held liable for the
 *payment of damages. All rights reserved in the event of the grant
 *of a patent, utility model or design.
 ******************************************************************/
#ifndef _VEHICLE_CLIENT_H_
#define _VEHICLE_CLIENT_H_
#include <vector>
#include <string>
#include "type.h"


namespace gwm {
namespace platform {
namespace vehicle {

/**
* callback function, user call subscribe() to register this callback.
* If the value corresponding to propId change, this function will be called.
* @param VehicleCallback VehiclePropValue.
* @return none;
*/
typedef void (*vehicle_callback)(VehiclePropValue value, PropertyType prop_type);

/**
* callback function, user call subscribe() to register this callback.
* If the signal corresponding to propId lost, this function will be called.
* @param int prop id.
* @param VehiclePropertyStatus status define in type.h. If signal lost, status will equal to VehiclePropertyStatus::ERROR
* @return none;
*/
typedef void (*vehicle_error_callback)(int propId, VehiclePropertyStatus status);

class VehicleClientImpl;
class SyncMutex;
class VehicleClient
{
public:

    static VehicleClient* getInstance();

    /**
    * Destructor
    */
    virtual ~VehicleClient();

    /**
    * Subscribes Callback to receive vehicle property events.
    *
    * Clients can be able to subscribe to property by propId.
    *
    * @param callback when subscribe prop change, callback will be called.
    * @param propId subscribe propId.
    * @return true: valid propId;
    *         false: invalid propId
    */
    bool subscribe(vehicle_callback callback, std::vector<int32_t> subscribe_prop, vehicle_error_callback error_callback = nullptr);

    /**
    * Unsubscribes to property events.
    *
    * Clients can be able to unsubscribe to property by propId.
    *
    * @param callback when subscribe prop change, callback will be called.
    * @param propId unsubscribe propId.
    * @return true: valid propId;
    *         false: invalid propId
    */
    bool unsubscribe(vehicle_callback callback, std::vector<int32_t> unsubscribe_prop);

    /**
    * Get a vehicle property value.
    *
    * @param propId propId.
    * @param prop_value get prop value of propId
    * @return true: valid propId;
    *         false: invalid propId
    */
    bool getPropValue(int32_t propId, VehiclePropValue* prop_value);

    /**
    * Set a vehicle property value.
    *
    * @param propId propId.
    * @param prop_value get prop value of propId
    * @return true: valid propId;
    *         false: invalid propId
    */
    bool setPropValue(int32_t propId, VehiclePropValue* val);

    /**
    * get property type according to propId.
    *
    * @return PropertyType: property type
    */
    PropertyType getPropertyType(int32_t propId);

    /**
    * get values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_FLOAT, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool getFloatProperty(int32_t propId, std::vector<float>& values);

    /**
    * get values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is PROPERTY_TYPE_BOOL, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool getBoolProperty(int32_t propId, bool& value);

    /**
    * get values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is PROPERTY_TYPE_INT32, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool getInt32Property(int32_t propId, std::vector<int32_t>& values);

    /**
    * get values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is PROPERTY_TYPE_INT64, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool getInt64Property(int32_t propId, std::vector<int64_t>& values);

    /**
    * get values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is PROPERTY_TYPE_BYTE, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool getByteProperty(int32_t propId, std::vector<uint8_t>& values);

    /**
    * get values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is PROPERTY_TYPE_STRING, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool getStringProperty(int32_t propId, string& values);

    /**
    * set values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_FLOAT, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool setFloatProperty(int32_t propId, std::vector<float>& values);

    /**
    * set values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_BOOL, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool setBoolProperty(int32_t propId, bool& value);

    /**
    * set values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_INT32, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool setInt32Property(int32_t propId, std::vector<int32_t>& values);

    /**
    * set values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_INT64, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool setInt64Property(int32_t propId, std::vector<int64_t>& values);

    /**
    * set values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_BYTE, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool setByteProperty(int32_t propId, std::vector<uint8_t>& values);

    /**
    * set values according to propId.
    * first call the function of getPropertyType to get PropertyType,
    * if value type is VALUE_TYPE_STRING, then call this funtion.
    * @param propId prop id difined in type.h.
    * @param values the values that corresponding propId
    * @return true: get the values that corresponding propId success
    *         false: get the values that corresponding propId falied
    */
    bool setStringProperty(int32_t propId, string& values);
	
    VehicleClient(const VehicleClient&) = delete;
    VehicleClient& operator = (const VehicleClient&) = delete;
    
private:
    /**
    * Constructor
    */
    VehicleClient();

    VehicleClientImpl* mVehicleClientImp;
    void onPorpertyChanged(int propId, const VehiclePropValue & value, int count);
    void onPropSetError(int propId, VehiclePropertyStatus status);
    void onClientOnLine(bool on_line);
    vehicle_callback mCallback;
    vehicle_error_callback mErrorCallback;
    bool mClientOnLine;
    static VehicleClient* mVehicleClient;
    static SyncMutex mMutex;
};

} // namespace vehicle
} // namespace platform
} // namespace bosch

#endif // _VEHICLE_CLIENT_H_
