/******************************************************************
 *FILE: types.h
 *SW-COMPONENT: enum and struct declare
 *DESCRIPTION: Declare enum and struct used by VehicleService
 *COPYRIGHT: © 2020 Robert Bosch GmbH
 *
 *The reproduction, distribution and utilization of this file as
 *well as the communication of its contents to others without express
 *authorization is prohibited. Offenders will be held liable for the
 *payment of damages. All rights reserved in the event of the grant
 *of a patent, utility model or design.
 ******************************************************************/
#ifndef _TYPE_H_
#define _TYPE_H_
#include <vector>
#include <string>

using namespace::std;
namespace gwm
{
namespace platform
{
namespace vehicle
{

enum PropertyType {
    PROPERTY_TYPE_STRING = 1,  // the value type is string
    PROPERTY_TYPE_BOOL,    // the value type is bool
    PROPERTY_TYPE_INT32,   // the value type is int32_t
    PROPERTY_TYPE_INT64,   // the value type is int64_t
    PROPERTY_TYPE_FLOAT,   // the value type is float
    PROPERTY_TYPE_BYTE,    // the value type is int8_t
    PROPERTY_TYPE_INVALID,  // the value type is invalid
};

#define VNAME(name) (#name)
#define countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))

enum class VehiclePropMsgType : int32_t
{
    GETPROP          = 0x00000010,
    SETPROP          = 0x00000020,
    REPORTPROP       = 0x00000040,
    LOSTPROP         = 0x00000080,
    MASK = 0x0000ffff
};

enum class StatusCode : int32_t
{
    OK = 0,

    /** Try again. */
    TRY_AGAIN = 1,

    /** Invalid argument provided. */
    INVALID_ARG = 2,

    /**
     * This code must be returned when device that associated with the vehicle
     * property is not available. For example, when client tries to set HVAC
     * temperature when the whole HVAC unit is turned OFF.
     */
    NOT_AVAILABLE = 3,

    /** Access denied */
    ACCESS_DENIED = 4,

    /** Something unexpected has happened in Vehicle HAL */
    INTERNAL_ERROR = 5,
    /** car signal lost in Vehicle HAL */
    SIGNAL_LOST = 6,
};



/**
 * Property status is a dynamic value that may change based on the vehicle state.
 */
enum class VehiclePropertyStatus : int32_t
{
    /** Property is available and behaving normally */
    AVAILABLE   = 0x00,
    /**
     * A property in this state is not available for reading and writing.  This
     * is a transient state that depends on the availability of the underlying
     * implementation (e.g. hardware or driver). It MUST NOT be used to
     * represent features that this vehicle is always incapable of.  A get() of
     * a property in this state MAY return an undefined value, but MUST
     * correctly describe its status as UNAVAILABLE A set() of a property in
     * this state MAY return NOT_AVAILABLE. The HAL implementation MUST ignore
     * the value of the status field when writing a property value coming from
     * Android.
     */
    UNAVAILABLE = 0x01,
    /** There is an error with this property. Such as siganl missing or can node lost*/
    ERROR       = 0x02,
};

/**
 * Property config defines the capabilities of it. User of the API
 * must first get the property config to understand the output from get()
 * commands and also to ensure that set() or events commands are in sync with
 * the expected output.
 */
enum class VehiclePropertyAccess : int32_t
{
    NONE = 0x00,

    READ = 0x01,
    WRITE = 0x02,
    READ_WRITE = 0x03,
};


/**
 * This describes how value of property can change.
 */
enum class VehiclePropertyChangeMode : int32_t
{
    /**
     * Property of this type must never be changed. Subscription is not supported
     * for these properties.
     */
    STATIC = 0x00,

    /**
     * Properties of this type must report when there is a change.
     * IVehicle#get call must return the current value.
     * Set operation for this property is assumed to be asynchronous. When the
     * property is read (using IVehicle#get) after IVehicle#set, it may still
     * return old value until underlying H/W backing this property has actually
     * changed the state. Once state is changed, the property must dispatch
     * changed value as event.
     */
    ON_CHANGE = 0x01,

    /**
     * Properties of this type change continuously and require a fixed rate of
     * sampling to retrieve the data.  Implementers may choose to send extra
     * notifications on significant value changes.
     */
    CONTINUOUS = 0x02,
};

struct VehicleAreaConfig
{
    /**
     * Area id is ignored for VehiclePropertyGroup:GLOBAL properties.
     */
    int32_t areaId;

    int32_t minInt32Value;
    int32_t maxInt32Value;

    int64_t minInt64Value;
    int64_t maxInt64Value;

    float minFloatValue;
    float maxFloatValue;
};


struct VehiclePropConfig
{
    /** Property identifier */
    int32_t prop;

    /**
     * Defines if the property is read or write or both.
     */
    VehiclePropertyAccess access;

    /**
     * Defines the change mode of the property.
     */
    VehiclePropertyChangeMode changeMode;

    /**
     * Min sample rate in Hz.
     * Must be defined for VehiclePropertyChangeMode::CONTINUOUS
     */
    float minSampleRate;

    /**
     * Must be defined for VehiclePropertyChangeMode::CONTINUOUS
     * Max sample rate in Hz.
     */
    float maxSampleRate;

    /**
     * Contains per-area configuration.
     */
    // vector<VehicleAreaConfig> areaConfigs;

    /** Contains additional configuration parameters */
    // vector<int32_t> configArray;

    /**
     * Some properties may require additional information passed over this
     * string. Most properties do not need to set this.
     */
    // string configString;

};

/**
 * Enumerates supported data type for VehicleProperty.
 *
 * Used to create property ID in VehicleProperty enum.
 */

enum class VehiclePropertyType : int32_t
{
    STRING          = 0x00100000,
    BOOLEAN         = 0x00200000,
    INT32           = 0x00400000,
    INT32_VEC       = 0x00410000,
    INT64           = 0x00500000,
    INT64_VEC       = 0x00510000,
    FLOAT           = 0x00600000,
    FLOAT_VEC       = 0x00610000,
    BYTES           = 0x00700000,

    /**
     * Any combination of scalar or vector types. The exact format must be
     * provided in the description of the property.
     */
    MIXED           = 0x00e00000,

    MASK            = 0x00ff0000
};

/**
 * Encapsulates the property name and the associated value. It
 * is used across various API calls to set values, get values or to register for
 * events.
 */
struct VehiclePropValue
{
    /** Time is elapsed nanoseconds since boot */
    int64_t timestamp;

    /**
     * Area type(s) for non-global property it must be one of the value from
     * VehicleArea* enums or 0 for global properties.
     */
    int32_t areaId;

    /** Property identifier */
    int32_t prop;

    /** Status of the property */
    VehiclePropertyStatus status;

    /**
     * Contains value for a single property. Depending on property data type of
     * this property (VehiclePropetyType) one field of this structure must be filled in.
     */
    struct RawValue
    {
        /**
         * This is used for properties of types VehiclePropertyType#INT
         * and VehiclePropertyType#INT_VEC
         */
        vector<int32_t> int32Values;

        /**
         * This is used for properties of types VehiclePropertyType#FLOAT
         * and VehiclePropertyType#FLOAT_VEC
         */
        vector<float> floatValues;

        /** This is used for properties of type VehiclePropertyType#INT64 */
        vector<int64_t> int64Values;

        /** This is used for properties of type VehiclePropertyType#BYTES */
        vector<uint8_t> bytes;

        /** This is used for properties of type VehiclePropertyType#STRING */
        string stringValue;
    };

    RawValue value;
};

enum  class VehicleArea : int32_t
{
    GLOBAL      = 0x01000000,
    /** WINDOW maps to enum VehicleAreaWindow */
    WINDOW      = 0x03000000,
    /** MIRROR maps to enum VehicleAreaMirror */
    MIRROR      = 0x04000000,
    /** SEAT maps to enum VehicleAreaSeat */
    SEAT        = 0x05000000,
    /** DOOR maps to enum VehicleAreaDoor */
    DOOR        = 0x06000000,
    /** WHEEL maps to enum VehicleAreaWheel */
    WHEEL       = 0x07000000,

    MASK        = 0x0f000000,
};

/**
 * Enumerates property groups.
 *
 * Used to create property ID in VehicleProperty enum.
 */
enum class VehiclePropertyGroup : int32_t
{
    /**
     * Properties declared in AOSP must use this flag.
     */
    SYSTEM      = 0x10000000,

    /**
     * Properties declared by vendors must use this flag.
     */
    VENDOR      = 0x20000000,
    VENDOR_HOST = 0x40000000,

    MASK        = 0x70000000,
};

/**
 * Declares all vehicle properties. VehicleProperty has a bitwise structure.
 * Each property must have:
 *  - a unique id from range 0x0100 - 0xffff
 *  - associated data type using VehiclePropertyType
 *  - property group (VehiclePropertyGroup)
 *  - vehicle area (VehicleArea)
 *
 * Vendors are allowed to extend this enum with their own properties. In this
 * case they must use VehiclePropertyGroup:VENDOR flag when property is
 * declared.
 *
 * When a property's status field is not set to AVAILABLE:
 *  - IVehicle#set may return StatusCode::NOT_AVAILABLE.
 *  - IVehicle#get is not guaranteed to work.
 */
enum class VehicleProperty : int32_t
{
    /**
     * Undefined property.  */
    INVALID = 0,
    /**
     * VIN of vehicle
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    INFO_VIN = 286261504, // (((0x0100 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:STRING) | VehicleArea:GLOBAL)
    /**
     * Manufacturer of vehicle
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    INFO_MAKE = 286261505, // (((0x0101 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:STRING) | VehicleArea:GLOBAL)
    /**
     * Model of vehicle
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    INFO_MODEL = 286261506, // (((0x0102 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:STRING) | VehicleArea:GLOBAL)
    /**
     * Model year of vehicle.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:YEAR
     */
    INFO_MODEL_YEAR = 289407235, // (((0x0103 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Fuel capacity of the vehicle in milliliters
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MILLILITER
     */
    INFO_FUEL_CAPACITY = 291504388, // (((0x0104 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * List of fuels the vehicle may use
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @data_enum FuelType
     */
    INFO_FUEL_TYPE = 289472773, // (((0x0105 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32_VEC) | VehicleArea:GLOBAL)
    /**
     * Battery capacity of the vehicle, if EV or hybrid.  This is the nominal
     * battery capacity when the vehicle is new.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:WH
     */
    INFO_EV_BATTERY_CAPACITY = 291504390, // (((0x0106 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * List of connectors this EV may use
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @data_enum EvConnectorType
     * @access VehiclePropertyAccess:READ
     */
    INFO_EV_CONNECTOR_TYPE = 289472775, // (((0x0107 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32_VEC) | VehicleArea:GLOBAL)
    /**
     * Fuel door location
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @data_enum FuelDoorLocationType
     * @access VehiclePropertyAccess:READ
     */
    INFO_FUEL_DOOR_LOCATION = 289407240, // (((0x0108 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * EV port location
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     * @data_enum PortLocationType
     */
    INFO_EV_PORT_LOCATION = 289407241, // (((0x0109 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Driver's seat location
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @data_enum VehicleAreaSeat
     * @access VehiclePropertyAccess:READ
     */
    INFO_DRIVER_SEAT = 356516106, // (((0x010A | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Current odometer value of the vehicle
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:KILOMETER
     */
    PERF_ODOMETER = 291504644, // (((0x0204 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Speed of the vehicle
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:METER_PER_SEC
     */
    PERF_VEHICLE_SPEED = 291504647, // (((0x0207 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Temperature of engine coolant
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    ENGINE_COOLANT_TEMP = 291504897, // (((0x0301 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Engine oil level
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleOilLevel
     */
    ENGINE_OIL_LEVEL = 289407747, // (((0x0303 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Temperature of engine oil
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    ENGINE_OIL_TEMP = 291504900, // (((0x0304 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Engine rpm
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:RPM
     */
    ENGINE_RPM = 291504901, // (((0x0305 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Reports wheel ticks
     *
     * The first element in the vector is a reset count.  A reset indicates
     * previous tick counts are not comparable with this and future ones.  Some
     * sort of discontinuity in tick counting has occurred.
     *
     * The next four elements represent ticks for individual wheels in the
     * following order: front left, front right, rear right, rear left.  All
     * tick counts are cumulative.  Tick counts increment when the vehicle
     * moves forward, and decrement when vehicles moves in reverse.  The ticks
     * should be reset to 0 when the vehicle is started by the user.
     *
     *  int64Values[0] = reset count
     *  int64Values[1] = front left ticks
     *  int64Values[2] = front right ticks
     *  int64Values[3] = rear right ticks
     *  int64Values[4] = rear left ticks
     *
     * configArray is used to indicate the micrometers-per-wheel-tick value and
     * which wheels are supported.  configArray is set as follows:
     *
     *  configArray[0], bits [0:3] = supported wheels.  Uses enum Wheel.
     *  configArray[1] = micrometers per front left wheel tick
     *  configArray[2] = micrometers per front right wheel tick
     *  configArray[3] = micrometers per rear right wheel tick
     *  configArray[4] = micrometers per rear left wheel tick
     *
     * NOTE:  If a wheel is not supported, its value shall always be set to 0.
     *
     * VehiclePropValue.timestamp must be correctly filled in.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     */
    WHEEL_TICK = 290521862, // (((0x0306 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT64_VEC) | VehicleArea:GLOBAL)
    /**
     * Fuel remaining in the the vehicle, in milliliters
     *
     * Value may not exceed INFO_FUEL_CAPACITY
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MILLILITER
     */
    FUEL_LEVEL = 291504903, // (((0x0307 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Fuel door open
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    FUEL_DOOR_OPEN = 287310600, // (((0x0308 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * EV battery level in WH, if EV or hybrid
     *
     * Value may not exceed INFO_EV_BATTERY_CAPACITY
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:WH
     */
    EV_BATTERY_LEVEL = 291504905, // (((0x0309 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * EV charge port open
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    EV_CHARGE_PORT_OPEN = 287310602, // (((0x030A | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * EV charge port connected
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    EV_CHARGE_PORT_CONNECTED = 287310603, // (((0x030B | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * EV instantaneous charge rate in milliwatts
     *
     * Positive value indicates battery is being charged.
     * Negative value indicates battery being discharged.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:MW
     */
    EV_BATTERY_INSTANTANEOUS_CHARGE_RATE = 291504908, // (((0x030C | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Range remaining
     *
     * Meters remaining of fuel and charge.  Range remaining shall account for
     * all energy sources in a vehicle.  For example, a hybrid car's range will
     * be the sum of the ranges based on fuel and battery.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ_WRITE
     * @unit VehicleUnit:METER
     */
    RANGE_REMAINING = 291504904, // (((0x0308 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Tire pressure
     *
     * min/max value indicates tire pressure sensor range.  Each tire will have a separate min/max
     * value denoted by its areaConfig.areaId.
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:KILOPASCAL
     */
    TIRE_PRESSURE = 392168201, // (((0x0309 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:WHEEL)
    /**
     * Currently selected gear
     *
     * This is the gear selected by the user.
     *
     * Values in the config data must represent the list of supported gears
     * for this vehicle.  For example, config data for an automatic transmission
     * must contain {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_PARK, GEAR_DRIVE,
     * GEAR_1, GEAR_2,...} and for manual transmission the list must be
     * {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_1, GEAR_2,...}
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleGear
     */
    GEAR_SELECTION = 289408000, // (((0x0400 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Current gear. In non-manual case, selected gear may not
     * match the current gear. For example, if the selected gear is GEAR_DRIVE,
     * the current gear will be one of GEAR_1, GEAR_2 etc, which reflects
     * the actual gear the transmission is currently running in.
     *
     * Values in the config data must represent the list of supported gears
     * for this vehicle.  For example, config data for an automatic transmission
     * must contain {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_PARK, GEAR_1, GEAR_2,...}
     * and for manual transmission the list must be
     * {GEAR_NEUTRAL, GEAR_REVERSE, GEAR_1, GEAR_2,...}. This list need not be the
     * same as that of the supported gears reported in GEAR_SELECTION.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleGear
     */
    CURRENT_GEAR = 289408001, // (((0x0401 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Parking brake state.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    PARKING_BRAKE_ON = 287310850, // (((0x0402 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Auto-apply parking brake.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    PARKING_BRAKE_AUTO_APPLY = 287310851, // (((0x0403 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Warning for fuel low level.
     *
     * This property corresponds to the low fuel warning on the dashboard.
     * Once FUEL_LEVEL_LOW is set, it should not be cleared until more fuel is
     * added to the vehicle.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    FUEL_LEVEL_LOW = 287310853, // (((0x0405 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Night mode
     *
     * True indicates that night mode is currently enabled.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    NIGHT_MODE = 287310855, // (((0x0407 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * State of the vehicles turn signals
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleTurnSignal
     */
    TURN_SIGNAL_STATE = 289408008, // (((0x0408 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Represents ignition state
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleIgnitionState
     */
    IGNITION_STATE = 289408009, // (((0x0409 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * ABS is active
     *
     * Set to true when ABS is active.  Reset to false when ABS is off.  This
     * property may be intermittently set (pulsing) based on the real-time
     * state of the ABS system.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    ABS_ACTIVE = 287310858, // (((0x040A | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Traction Control is active
     *
     * Set to true when traction control (TC) is active.  Reset to false when
     * TC is off.  This property may be intermittently set (pulsing) based on
     * the real-time state of the TC system.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    TRACTION_CONTROL_ACTIVE = 287310859, // (((0x040B | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Fan speed setting
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_FAN_SPEED = 356517120, // (((0x0500 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Fan direction setting
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleHvacFanDirection
     */
    HVAC_FAN_DIRECTION = 356517121, // (((0x0501 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * HVAC current temperature.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    HVAC_TEMPERATURE_CURRENT = 358614274, // (((0x0502 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:SEAT)
    /**
     * HVAC, target temperature set.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @unit VehicleUnit:CELSIUS
     */
    HVAC_TEMPERATURE_SET = 358614275, // (((0x0503 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:SEAT)
    /**
     * On/off defrost for designated window
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_DEFROSTER = 320865540, // (((0x0504 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:WINDOW)
    /**
     * On/off AC for designated areaId
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @config_flags Supported areaIds
     */
    HVAC_AC_ON = 354419973, // (((0x0505 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * On/off max AC
     *
     * When MAX AC is on, the ECU may adjust the vent position, fan speed,
     * temperature, etc as necessary to cool the vehicle as quickly as possible.
     * Any parameters modified as a side effect of turning on/off the MAX AC
     * parameter shall generate onPropertyEvent() callbacks to the VHAL.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_MAX_AC_ON = 354419974, // (((0x0506 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * On/off max defrost
     *
     * When MAX DEFROST is on, the ECU may adjust the vent position, fan speed,
     * temperature, etc as necessary to defrost the windows as quickly as
     * possible.  Any parameters modified as a side effect of turning on/off
     * the MAX DEFROST parameter shall generate onPropertyEvent() callbacks to
     * the VHAL.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_MAX_DEFROST_ON = 354419975, // (((0x0507 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * Recirculation on/off
     *
     * Controls the supply of exterior air to the cabin.  Recirc “on” means the
     * majority of the airflow into the cabin is originating in the cabin.
     * Recirc “off” means the majority of the airflow into the cabin is coming
     * from outside the car.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_RECIRC_ON = 354419976, // (((0x0508 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * Enable temperature coupling between areas.
     *
     * The AreaIDs for HVAC_DUAL_ON property shall contain a combination of
     * HVAC_TEMPERATURE_SET AreaIDs that can be coupled together. If
     * HVAC_TEMPERATURE_SET is mapped to AreaIDs [a_1, a_2, ..., a_n], and if
     * HVAC_DUAL_ON can be enabled to couple a_i and a_j, then HVAC_DUAL_ON
     * property must be mapped to [a_i | a_j]. Further, if a_k and a_l can also
     * be coupled together separately then HVAC_DUAL_ON must be mapped to
     * [a_i | a_j, a_k | a_l].
     *
     * Example: A car has two front seats (ROW_1_LEFT, ROW_1_RIGHT) and three
     *  back seats (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT). There are two
     *  temperature control units -- driver side and passenger side -- which can
     *  be optionally synchronized. This may be expressed in the AreaIDs this way:
     *  - HVAC_TEMPERATURE_SET->[ROW_1_LEFT | ROW_2_LEFT, ROW_1_RIGHT | ROW_2_CENTER | ROW_2_RIGHT]
     *  - HVAC_DUAL_ON->[ROW_1_LEFT | ROW_2_LEFT | ROW_1_RIGHT | ROW_2_CENTER | ROW_2_RIGHT]
     *
     * When the property is enabled, the ECU must synchronize the temperature
     * for the affected areas. Any parameters modified as a side effect
     * of turning on/off the DUAL_ON parameter shall generate
     * onPropertyEvent() callbacks to the VHAL. In addition, if setting
     * a temperature (i.e. driver's temperature) changes another temperature
     * (i.e. front passenger's temperature), then the appropriate
     * onPropertyEvent() callbacks must be generated.  If a user changes a
     * temperature that breaks the coupling (e.g. setting the passenger
     * temperature independently) then the VHAL must send the appropriate
     * onPropertyEvent() callbacks (i.e. HVAC_DUAL_ON = false,
     * HVAC_TEMPERATURE_SET[AreaID] = xxx, etc).
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_DUAL_ON = 354419977, // (((0x0509 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * On/off automatic mode
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_AUTO_ON = 354419978, // (((0x050A | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * Seat heating/cooling
     *
     * Negative values indicate cooling.
     * 0 indicates off.
     * Positive values indicate heating.
     *
     * Some vehicles may have multiple levels of heating and cooling. The
     * min/max range defines the allowable range and number of steps in each
     * direction.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_SEAT_TEMPERATURE = 356517131, // (((0x050B | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Side Mirror Heat
     *
     * Increasing values denote higher heating levels for side mirrors.
     * The Max value in the config data represents the highest heating level.
     * The Min value in the config data MUST be zero and indicates no heating.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_SIDE_MIRROR_HEAT = 339739916, // (((0x050C | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:MIRROR)
    /**
     * Steering Wheel Heating/Cooling
     *
     * Sets the amount of heating/cooling for the steering wheel
     * config data Min and Max MUST be set appropriately.
     * Positive value indicates heating.
     * Negative value indicates cooling.
     * 0 indicates temperature control is off.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_STEERING_WHEEL_HEAT = 289408269, // (((0x050D | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Temperature units for display
     *
     * Indicates whether the vehicle is displaying temperature to the user as
     * Celsius or Fahrenheit.
     * This parameter MAY be used for displaying any HVAC temperature in the system.
     * Values must be one of VehicleUnit::CELSIUS or VehicleUnit::FAHRENHEIT
     * Note that internally, all temperatures are represented in floating point Celsius.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_TEMPERATURE_DISPLAY_UNITS = 289408270, // (((0x050E | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Actual fan speed
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    HVAC_ACTUAL_FAN_SPEED_RPM = 356517135, // (((0x050F | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Represents global power state for HVAC. Setting this property to false
     * MAY mark some properties that control individual HVAC features/subsystems
     * to UNAVAILABLE state. Setting this property to true MAY mark some
     * properties that control individual HVAC features/subsystems to AVAILABLE
     * state (unless any/all of them are UNAVAILABLE on their own individual
     * merits).
     *
     * [Definition] HvacPower_DependentProperties: Properties that need HVAC to be
     *   powered on in order to enable their functionality. For example, in some cars,
     *   in order to turn on the AC, HVAC must be powered on first.
     *
     * HvacPower_DependentProperties list must be set in the
     * VehiclePropConfig.configArray. HvacPower_DependentProperties must only contain
     * properties that are associated with VehicleArea:SEAT. Properties that are not
     * associated with VehicleArea:SEAT, for example, HVAC_DEFROSTER, must never
     * depend on HVAC_POWER_ON property and must never be part of
     * HvacPower_DependentProperties list.
     *
     * AreaID mapping for HVAC_POWER_ON property must contain all AreaIDs that
     * HvacPower_DependentProperties are mapped to.
     *
     * Example 1: A car has two front seats (ROW_1_LEFT, ROW_1_RIGHT) and three back
     *  seats (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT). If the HVAC features (AC,
     *  Temperature etc.) throughout the car are dependent on a single HVAC power
     *  controller then HVAC_POWER_ON must be mapped to
     *  [ROW_1_LEFT | ROW_1_RIGHT | ROW_2_LEFT | ROW_2_CENTER | ROW_2_RIGHT].
     *
     * Example 2: A car has two seats in the front row (ROW_1_LEFT, ROW_1_RIGHT) and
     *   three seats in the second (ROW_2_LEFT, ROW_2_CENTER, ROW_2_RIGHT) and third
     *   rows (ROW_3_LEFT, ROW_3_CENTER, ROW_3_RIGHT). If the car has temperature
     *   controllers in the front row which can operate entirely independently of
     *   temperature controllers in the back of the vehicle, then HVAC_POWER_ON
     *   must be mapped to a two element array:
     *   - ROW_1_LEFT | ROW_1_RIGHT
     *   - ROW_2_LEFT | ROW_2_CENTER | ROW_2_RIGHT | ROW_3_LEFT | ROW_3_CENTER | ROW_3_RIGHT
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_POWER_ON = 354419984, // (((0x0510 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * Fan Positions Available
     *
     * This is a bit mask of fan positions available for the zone.  Each
     * available fan direction is denoted by a separate entry in the vector.  A
     * fan direction may have multiple bits from vehicle_hvac_fan_direction set.
     * For instance, a typical car may have the following fan positions:
     *   - FAN_DIRECTION_FACE (0x1)
     *   - FAN_DIRECTION_FLOOR (0x2)
     *   - FAN_DIRECTION_FACE | FAN_DIRECTION_FLOOR (0x3)
     *   - FAN_DIRECTION_DEFROST (0x4)
     *   - FAN_DIRECTION_FLOOR | FAN_DIRECTION_DEFROST (0x6)
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    HVAC_FAN_DIRECTION_AVAILABLE = 356582673, // (((0x0511 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32_VEC) | VehicleArea:SEAT)
    /**
     * Automatic recirculation on/off
     *
     * When automatic recirculation is ON, the HVAC system may automatically
     * switch to recirculation mode if the vehicle detects poor incoming air
     * quality.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_AUTO_RECIRC_ON = 354419986, // (((0x0512 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * Seat ventilation
     *
     * 0 indicates off.
     * Positive values indicates ventilation level.
     *
     * Used by HVAC apps and Assistant to enable, change, or read state of seat
     * ventilation.  This is different than seating cooling. It can be on at the
     * same time as cooling, or not.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    HVAC_SEAT_VENTILATION = 356517139, // (((0x0513 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Outside temperature
     *
     * @change_mode VehiclePropertyChangeMode:CONTINUOUS
     * @access VehiclePropertyAccess:READ
     * @unit VehicleUnit:CELSIUS
     */
    ENV_OUTSIDE_TEMPERATURE = 291505923, // (((0x0703 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:FLOAT) | VehicleArea:GLOBAL)
    /**
     * Property to control power state of application processor
     *
     * It is assumed that AP's power state is controller by separate power
     * controller.
     *
     * For configuration information, VehiclePropConfig.configArray can have bit flag combining
     * values in VehicleApPowerStateConfigFlag.
     *
     *   int32Values[0] : VehicleApPowerStateReq enum value
     *   int32Values[1] : additional parameter relevant for each state,
     *                    0 if not used.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VEHICLE_PROP_ACCESS_READ
     */
    //AP_POWER_STATE_REQ = 289475072, // (((0x0A00 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32_VEC) | VehicleArea:GLOBAL)
    /**
     * Property to report power state of application processor
     *
     * It is assumed that AP's power state is controller by separate power
     * controller.
     *
     *   int32Values[0] : VehicleApPowerStateReport enum value
     *   int32Values[1] : Time in ms to wake up, if necessary.  Otherwise 0.
     *
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VEHICLE_PROP_ACCESS_WRITE
     */
    //AP_POWER_STATE_REPORT = 289475073, // (((0x0A01 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32_VEC) | VehicleArea:GLOBAL)
    /**
     * Property to report bootup reason for the current power on. This is a
     * static property that will not change for the whole duration until power
     * off. For example, even if user presses power on button after automatic
     * power on with door unlock, bootup reason must stay with
     * VehicleApPowerBootupReason#USER_UNLOCK.
     *
     * int32Values[0] must be VehicleApPowerBootupReason.
     *
     * @change_mode VehiclePropertyChangeMode:STATIC
     * @access VehiclePropertyAccess:READ
     */
    AP_POWER_BOOTUP_REASON = 289409538, // (((0x0A02 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Property to represent brightness of the display. Some cars have single
     * control for the brightness of all displays and this property is to share
     * change in that control.
     *
     * If this is writable, android side can set this value when user changes
     * display brightness from Settings. If this is read only, user may still
     * change display brightness from Settings, but that must not be reflected
     * to other displays.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DISPLAY_BRIGHTNESS = 289409539, // (((0x0A03 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Property to feed H/W input events to android
     *
     * int32Values[0] : action defined by VehicleHwKeyInputAction
     * int32Values[1] : key code, must use standard android key code
     * int32Values[2] : target display defined in VehicleDisplay. Events not
     *                  tied to specific display must be sent to
     *                  VehicleDisplay#MAIN.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @config_flags
     */
    //HW_KEY_INPUT = 289475088, // (((0x0A10 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32_VEC) | VehicleArea:GLOBAL)
    /**
     * ************************************************************************
     * Most Car Cabin properties have both a POSition and MOVE parameter.  These
     * are used to control the various movements for seats, doors, and windows
     * in a vehicle.
     *
     * A POS parameter allows the user to set the absolution position.  For
     * instance, for a door, 0 indicates fully closed and max value indicates
     * fully open.  Thus, a value halfway between min and max must indicate
     * the door is halfway open.
     *
     * A MOVE parameter moves the device in a particular direction.  The sign
     * indicates direction, and the magnitude indicates speed (if multiple
     * speeds are available).  For a door, a move of -1 will close the door, and
     * a move of +1 will open it.  Once a door reaches the limit of open/close,
     * the door should automatically stop moving.  The user must NOT need to
     * send a MOVE(0) command to stop the door at the end of its range.
     * ************************************************************************
     *
     * Door position
     *
     * This is an integer in case a door may be set to a particular position.
     * Max value indicates fully open, min value (0) indicates fully closed.
     *
     * Some vehicles (minivans) can open the door electronically.  Hence, the
     * ability to write this property.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DOOR_POS = 373295872, // (((0x0B00 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:DOOR)
    /**
     * Door move
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DOOR_MOVE = 373295873, // (((0x0B01 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:DOOR)
    /**
     * Door lock
     *
     * 'true' indicates door is locked
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    DOOR_LOCK = 371198722, // (((0x0B02 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:DOOR)
    /**
     * Mirror Z Position
     *
     * Positive value indicates tilt upwards, negative value is downwards
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Z_POS = 339741504, // (((0x0B40 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:MIRROR)
    /**
     * Mirror Z Move
     *
     * Positive value indicates tilt upwards, negative value is downwards
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Z_MOVE = 339741505, // (((0x0B41 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:MIRROR)
    /**
     * Mirror Y Position
     *
     * Positive value indicate tilt right, negative value is left
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Y_POS = 339741506, // (((0x0B42 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:MIRROR)
    /**
     * Mirror Y Move
     *
     * Positive value indicate tilt right, negative value is left
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_Y_MOVE = 339741507, // (((0x0B43 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:MIRROR)
    /**
     * Mirror Lock
     *
     * True indicates mirror positions are locked and not changeable
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_LOCK = 287312708, // (((0x0B44 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Mirror Fold
     *
     * True indicates mirrors are folded
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    MIRROR_FOLD = 287312709, // (((0x0B45 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:GLOBAL)
    /**
     * Seat memory select
     *
     * This parameter selects the memory preset to use to select the seat
     * position. The minValue is always 0, and the maxValue determines the
     * number of seat positions available.
     *
     * For instance, if the driver's seat has 3 memory presets, the maxValue
     * will be 3. When the user wants to select a preset, the desired preset
     * number (1, 2, or 3) is set.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:WRITE
     */
    SEAT_MEMORY_SELECT = 356518784, // (((0x0B80 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat memory set
     *
     * This setting allows the user to save the current seat position settings
     * into the selected preset slot.  The maxValue for each seat position
     * must match the maxValue for SEAT_MEMORY_SELECT.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:WRITE
     */
    SEAT_MEMORY_SET = 356518785, // (((0x0B81 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seatbelt buckled
     *
     * True indicates belt is buckled.
     *
     * Write access indicates automatic seat buckling capabilities.  There are
     * no known cars at this time, but you never know...
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BELT_BUCKLED = 354421634, // (((0x0B82 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:SEAT)
    /**
     * Seatbelt height position
     *
     * Adjusts the shoulder belt anchor point.
     * Max value indicates highest position
     * Min value indicates lowest position
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BELT_HEIGHT_POS = 356518787, // (((0x0B83 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seatbelt height move
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BELT_HEIGHT_MOVE = 356518788, // (((0x0B84 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat fore/aft position
     *
     * Sets the seat position forward (closer to steering wheel) and backwards.
     * Max value indicates closest to wheel, min value indicates most rearward
     * position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_FORE_AFT_POS = 356518789, // (((0x0B85 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat fore/aft move
     *
     * Moves the seat position forward and aft.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_FORE_AFT_MOVE = 356518790, // (((0x0B86 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat backrest angle 1 position
     *
     * Backrest angle 1 is the actuator closest to the bottom of the seat.
     * Max value indicates angling forward towards the steering wheel.
     * Min value indicates full recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_1_POS = 356518791, // (((0x0B87 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat backrest angle 1 move
     *
     * Moves the backrest forward or recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_1_MOVE = 356518792, // (((0x0B88 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat backrest angle 2 position
     *
     * Backrest angle 2 is the next actuator up from the bottom of the seat.
     * Max value indicates angling forward towards the steering wheel.
     * Min value indicates full recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_2_POS = 356518793, // (((0x0B89 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat backrest angle 2 move
     *
     * Moves the backrest forward or recline.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_BACKREST_ANGLE_2_MOVE = 356518794, // (((0x0B8A | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat height position
     *
     * Sets the seat height.
     * Max value indicates highest position.
     * Min value indicates lowest position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEIGHT_POS = 356518795, // (((0x0B8B | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat height move
     *
     * Moves the seat height.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEIGHT_MOVE = 356518796, // (((0x0B8C | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat depth position
     *
     * Sets the seat depth, distance from back rest to front edge of seat.
     * Max value indicates longest depth position.
     * Min value indicates shortest position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_DEPTH_POS = 356518797, // (((0x0B8D | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat depth move
     *
     * Adjusts the seat depth.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_DEPTH_MOVE = 356518798, // (((0x0B8E | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat tilt position
     *
     * Sets the seat tilt.
     * Max value indicates front edge of seat higher than back edge.
     * Min value indicates front edge of seat lower than back edge.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_TILT_POS = 356518799, // (((0x0B8F | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Seat tilt move
     *
     * Tilts the seat.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_TILT_MOVE = 356518800, // (((0x0B90 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Lumber fore/aft position
     *
     * Pushes the lumbar support forward and backwards
     * Max value indicates most forward position.
     * Min value indicates most rearward position.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_FORE_AFT_POS = 356518801, // (((0x0B91 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Lumbar fore/aft move
     *
     * Adjusts the lumbar support.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_FORE_AFT_MOVE = 356518802, // (((0x0B92 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Lumbar side support position
     *
     * Sets the amount of lateral lumbar support.
     * Max value indicates widest lumbar setting (i.e. least support)
     * Min value indicates thinnest lumbar setting.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_SIDE_SUPPORT_POS = 356518803, // (((0x0B93 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Lumbar side support move
     *
     * Adjusts the amount of lateral lumbar support.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_LUMBAR_SIDE_SUPPORT_MOVE = 356518804, // (((0x0B94 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Headrest height position
     *
     * Sets the headrest height.
     * Max value indicates tallest setting.
     * Min value indicates shortest setting.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_HEIGHT_POS = 289409941, // (((0x0B95 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Headrest height move
     *
     * Moves the headrest up and down.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_HEIGHT_MOVE = 356518806, // (((0x0B96 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Headrest angle position
     *
     * Sets the angle of the headrest.
     * Max value indicates most upright angle.
     * Min value indicates shallowest headrest angle.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_ANGLE_POS = 356518807, // (((0x0B97 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Headrest angle move
     *
     * Adjusts the angle of the headrest
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_ANGLE_MOVE = 356518808, // (((0x0B98 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Headrest fore/aft position
     *
     * Adjusts the headrest forwards and backwards.
     * Max value indicates position closest to front of car.
     * Min value indicates position closest to rear of car.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_FORE_AFT_POS = 356518809, // (((0x0B99 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Headrest fore/aft move
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    SEAT_HEADREST_FORE_AFT_MOVE = 356518810, // (((0x0B9A | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:SEAT)
    /**
     * Window Position
     *
     * Min = window up / closed
     * Max = window down / open
     *
     * For a window that may open out of plane (i.e. vent mode of sunroof) this
     * parameter will work with negative values as follows:
     *  Max = sunroof completely open
     *  0 = sunroof closed.
     *  Min = sunroof vent completely open
     *
     *  Note that in this mode, 0 indicates the window is closed.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    WINDOW_POS = 322964416, // (((0x0BC0 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:WINDOW)
    /**
     * Window Move
     *
     * Max = Open the window as fast as possible
     * Min = Close the window as fast as possible
     * Magnitude denotes relative speed.  I.e. +2 is faster than +1 in closing
     * the window.
     *
     * For a window that may open out of plane (i.e. vent mode of sunroof) this
     * parameter will work as follows:
     *
     * If sunroof is open:
     *   Max = open the sunroof further, automatically stop when fully open.
     *   Min = close the sunroof, automatically stop when sunroof is closed.
     *
     * If vent is open:
     *   Max = close the vent, automatically stop when vent is closed.
     *   Min = open the vent further, automatically stop when vent is fully open.
     *
     * If sunroof is in the closed position:
     *   Max = open the sunroof, automatically stop when sunroof is fully open.
     *   Min = open the vent, automatically stop when vent is fully open.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    WINDOW_MOVE = 322964417, // (((0x0BC1 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:WINDOW)
    /**
     * Window Lock
     *
     * True indicates windows are locked and can't be moved.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    WINDOW_LOCK = 320867268, // (((0x0BC4 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:BOOLEAN) | VehicleArea:WINDOW)
    /**
     * Vehicle Maps Service (VMS) message
     *
     * This property uses MIXED data to communicate vms messages.
     *
     * Its contents are to be interpreted as follows:
     * the indices defined in VmsMessageIntegerValuesIndex are to be used to
     * read from int32Values;
     * bytes is a serialized VMS message as defined in the vms protocol
     * which is opaque to the framework;
     *
     * IVehicle#get must always return StatusCode::NOT_AVAILABLE.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     */
    VEHICLE_MAP_SERVICE = 299895808, // (((0x0C00 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:MIXED) | VehicleArea:GLOBAL)
    /**
     * OBD2 Live Sensor Data
     *
     * Reports a snapshot of the current (live) values of the OBD2 sensors available.
     *
     * The configArray is set as follows:
     *   configArray[0] = number of vendor-specific integer-valued sensors
     *   configArray[1] = number of vendor-specific float-valued sensors
     *
     * The values of this property are to be interpreted as in the following example.
     * Considering a configArray = {2,3}
     * int32Values must be a vector containing Obd2IntegerSensorIndex.LAST_SYSTEM_INDEX + 2
     * elements (that is, 33 elements);
     * floatValues must be a vector containing Obd2FloatSensorIndex.LAST_SYSTEM_INDEX + 3
     * elements (that is, 73 elements);
     *
     * It is possible for each frame to contain a different subset of sensor values, both system
     * provided sensors, and vendor-specific ones. In order to support that, the bytes element
     * of the property value is used as a bitmask,.
     *
     * bytes must have a sufficient number of bytes to represent the total number of possible
     * sensors (in this case, 14 bytes to represent 106 possible values); it is to be read as
     * a contiguous bitmask such that each bit indicates the presence or absence of a sensor
     * from the frame, starting with as many bits as the size of int32Values, immediately
     * followed by as many bits as the size of floatValues.
     *
     * For example, should bytes[0] = 0x4C (0b01001100) it would mean that:
     *   int32Values[0 and 1] are not valid sensor values
     *   int32Values[2 and 3] are valid sensor values
     *   int32Values[4 and 5] are not valid sensor values
     *   int32Values[6] is a valid sensor value
     *   int32Values[7] is not a valid sensor value
     * Should bytes[5] = 0x61 (0b01100001) it would mean that:
     *   int32Values[32] is a valid sensor value
     *   floatValues[0 thru 3] are not valid sensor values
     *   floatValues[4 and 5] are valid sensor values
     *   floatValues[6] is not a valid sensor value
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    OBD2_LIVE_FRAME = 299896064, // (((0x0D00 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:MIXED) | VehicleArea:GLOBAL)
    /**
     * OBD2 Freeze Frame Sensor Data
     *
     * Reports a snapshot of the value of the OBD2 sensors available at the time that a fault
     * occurred and was detected.
     *
     * A configArray must be provided with the same meaning as defined for OBD2_LIVE_FRAME.
     *
     * The values of this property are to be interpreted in a similar fashion as those for
     * OBD2_LIVE_FRAME, with the exception that the stringValue field may contain a non-empty
     * diagnostic troubleshooting code (DTC).
     *
     * A IVehicle#get request of this property must provide a value for int64Values[0].
     * This will be interpreted as the timestamp of the freeze frame to retrieve. A list of
     * timestamps can be obtained by a IVehicle#get of OBD2_FREEZE_FRAME_INFO.
     *
     * Should no freeze frame be available at the given timestamp, a response of NOT_AVAILABLE
     * must be returned by the implementation. Because vehicles may have limited storage for
     * freeze frames, it is possible for a frame request to respond with NOT_AVAILABLE even if
     * the associated timestamp has been recently obtained via OBD2_FREEZE_FRAME_INFO.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    OBD2_FREEZE_FRAME = 299896065, // (((0x0D01 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:MIXED) | VehicleArea:GLOBAL)
    /**
     * OBD2 Freeze Frame Information
     *
     * This property describes the current freeze frames stored in vehicle
     * memory and available for retrieval via OBD2_FREEZE_FRAME.
     *
     * The values are to be interpreted as follows:
     * each element of int64Values must be the timestamp at which a a fault code
     * has been detected and the corresponding freeze frame stored, and each
     * such element can be used as the key to OBD2_FREEZE_FRAME to retrieve
     * the corresponding freeze frame.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     */
    OBD2_FREEZE_FRAME_INFO = 299896066, // (((0x0D02 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:MIXED) | VehicleArea:GLOBAL)
    /**
     * OBD2 Freeze Frame Clear
     *
     * This property allows deletion of any of the freeze frames stored in
     * vehicle memory, as described by OBD2_FREEZE_FRAME_INFO.
     *
     * The configArray is set as follows:
     *  configArray[0] = 1 if the implementation is able to clear individual freeze frames
     *                   by timestamp, 0 otherwise
     *
     * IVehicle#set of this property is to be interpreted as follows:
     *   if int64Values contains no elements, then all frames stored must be cleared;
     *   if int64Values contains one or more elements, then frames at the timestamps
     *   stored in int64Values must be cleared, and the others not cleared. Should the
     *   vehicle not support selective clearing of freeze frames, this latter mode must
     *   return NOT_AVAILABLE.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:WRITE
     */
    OBD2_FREEZE_FRAME_CLEAR = 299896067, // (((0x0D03 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:MIXED) | VehicleArea:GLOBAL)
    /**
     * Headlights State
     *
     * Return the current state of headlights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    HEADLIGHTS_STATE = 289410560, // (((0x0E00 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * High beam lights state
     *
     * Return the current state of high beam lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    HIGH_BEAM_LIGHTS_STATE = 289410561, // (((0x0E01 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Fog light state
     *
     * Return the current state of fog lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    FOG_LIGHTS_STATE = 289410562, // (((0x0E02 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Hazard light status
     *
     * Return the current status of hazard lights.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ
     * @data_enum VehicleLightState
     */
    HAZARD_LIGHTS_STATE = 289410563, // (((0x0E03 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Headlight switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    HEADLIGHTS_SWITCH = 289410576, // (((0x0E10 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * High beam light switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    HIGH_BEAM_LIGHTS_SWITCH = 289410577, // (((0x0E11 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Fog light switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    FOG_LIGHTS_SWITCH = 289410578, // (((0x0E12 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    /**
     * Hazard light switch
     *
     * The setting that the user wants.
     *
     * @change_mode VehiclePropertyChangeMode:ON_CHANGE
     * @access VehiclePropertyAccess:READ_WRITE
     * @data_enum VehicleLightSwitch
     */
    HAZARD_LIGHTS_SWITCH = 289410579, // (((0x0E13 | VehiclePropertyGroup:SYSTEM) | VehiclePropertyType:INT32) | VehicleArea:GLOBAL)
    VEHICLE_POWER_START = 285215232, // ((0x0A00 | VehiclePropertyGroup:SYSTEM) | VehicleArea:GLOBAL)
    AP_POWER_STATE_REQ = 289475072, // ((VEHICLE_POWER_START + 0X0000) | VehiclePropertyType:INT32_VEC)
    AP_POWER_STATE_REPORT = 289475073, // ((VEHICLE_POWER_START + 0X0001) | VehiclePropertyType:INT32_VEC)
    VEHICLE_POWER_END = 285215247, // (VEHICLE_POWER_START + 0X000F)
    VEHICLE_HARDKEY_START = 285215248, // ((0x0A10 | VehiclePropertyGroup:SYSTEM) | VehicleArea:GLOBAL)
    HW_KEY_INPUT = 289475088, // ((VEHICLE_HARDKEY_START + 0X0000) | VehiclePropertyType:INT32_VEC)
    VEHICLE_HARDKEY_END = 285215263, // (VEHICLE_HARDKEY_START + 0X000F)
    VEHICLE_BCM_START = 553651741, // ((0X0E1D | VehiclePropertyGroup:VENDOR) | VehicleArea:GLOBAL)
    DCT_FD4_DRVMODDISP = 557846046, // ((VEHICLE_BCM_START + 0X0001) | VehiclePropertyType:INT32)
    ESP_FD2_VEHSPD = 559943199, // ((VEHICLE_BCM_START + 0X0002) | VehiclePropertyType:FLOAT)
    BCM1_DRVDOORSTS = 557846048, // ((VEHICLE_BCM_START + 0X0003) | VehiclePropertyType:INT32)
    BCM1_PASSENGERDOORSTS = 557846049, // ((VEHICLE_BCM_START + 0X0004) | VehiclePropertyType:INT32)
    BCM1_LRDOORSTS = 557846050, // ((VEHICLE_BCM_START + 0X0005) | VehiclePropertyType:INT32)
    BCM1_RRDOORSTS = 557846051, // ((VEHICLE_BCM_START + 0X0006) | VehiclePropertyType:INT32)
    CSA2_STEERWHEELANG = 559943204, // ((VEHICLE_BCM_START + 0X0007) | VehiclePropertyType:FLOAT)
    CSA2_STEERWHEELANGSIGN = 557846053, // ((VEHICLE_BCM_START + 0X0008) | VehiclePropertyType:INT32)
    VEHICLE_BCM_END = 553652252, // (VEHICLE_BCM_START + 0X01FF)
    VEHICLE_HVAC_START = 553652253, // (VEHICLE_BCM_END + 0X0001)
    HUT1_ACOFFREQ_TC = 557846558, // ((VEHICLE_HVAC_START + 0X0001) | VehiclePropertyType:INT32)
    HUT1_ACAUTOMODREQ_TC = 557846559, // ((VEHICLE_HVAC_START + 0X0002) | VehiclePropertyType:INT32)
    HUT1_ACCMPRREQ_TC = 557846560, // ((VEHICLE_HVAC_START + 0X0003) | VehiclePropertyType:INT32)
    HUT1_ACAIRINLETREQ_TC = 557846561, // ((VEHICLE_HVAC_START + 0X0004) | VehiclePropertyType:INT32)
    HUT1_ACDUALREQ_TC = 557846562, // ((VEHICLE_HVAC_START + 0X0005) | VehiclePropertyType:INT32)
    HUT1_ACDRVAIRDISTRIBMODREQ_TC = 557846563, // ((VEHICLE_HVAC_START + 0X0006) | VehiclePropertyType:INT32)
    HUT1_ACFRNTDEFRSTREQ_TC = 557846564, // ((VEHICLE_HVAC_START + 0X0007) | VehiclePropertyType:INT32)
    HUT2_REARWINHEATCMD_TC = 557846565, // ((VEHICLE_HVAC_START + 0X0008) | VehiclePropertyType:INT32)
    HUT1_ACDRVTEMPINCREQ_TC = 559943718, // ((VEHICLE_HVAC_START + 0X0009) | VehiclePropertyType:FLOAT)
    HUT1_ACDRVTEMPDECREQ_TC = 559943719, // ((VEHICLE_HVAC_START + 0X000A) | VehiclePropertyType:FLOAT)
    HUT1_ACFRNTPASSTEMPINCREQ_TC = 559943720, // ((VEHICLE_HVAC_START + 0X000B) | VehiclePropertyType:FLOAT)
    HUT1_ACFRNTPASSTEMPDECREQ_TC = 559943721, // ((VEHICLE_HVAC_START + 0X000C) | VehiclePropertyType:FLOAT)
    HUT1_ACFRNTBLWRSPDINCREQ_TC = 557846570, // ((VEHICLE_HVAC_START + 0X000D) | VehiclePropertyType:INT32)
    HUT1_ACFRNTBLWRSPDDECREQ_TC = 557846571, // ((VEHICLE_HVAC_START + 0X000E) | VehiclePropertyType:INT32)
    AC6_L_PM25INAIRQLTYLVL = 557846572, // ((VEHICLE_HVAC_START + 0X000F) | VehiclePropertyType:INT32)
    AC6_L_PM25INDENS = 557846573, // ((VEHICLE_HVAC_START + 0X0010) | VehiclePropertyType:INT32)
    HUT1_ACAQSREQ_TC = 557846574, // ((VEHICLE_HVAC_START + 0X0011) | VehiclePropertyType:INT32)
    HUT1_ACAIUREQ_TC = 557846575, // ((VEHICLE_HVAC_START + 0X0012) | VehiclePropertyType:INT32)
    HUT6_AUTODEFROST_TC = 557846576, // ((VEHICLE_HVAC_START + 0X0013) | VehiclePropertyType:INT32)
    HUT15_ACOPERMOD_TC = 557846577, // ((VEHICLE_HVAC_START + 0X0014) | VehiclePropertyType:INT32)
    AC1_ACEEMSTS = 557846578, // ((VEHICLE_HVAC_START + 0X0015) | VehiclePropertyType:INT32)
    AC2_ACHMIDISPCMD = 557846579, // ((VEHICLE_HVAC_START + 0X0016) | VehiclePropertyType:INT32)
    AC2_ACAMBTEMP = 559943732, // ((VEHICLE_HVAC_START + 0X0017) | VehiclePropertyType:FLOAT)
    HUT1_ACBLWRSTEPLSSPDREQ_TC = 557846581, // ((VEHICLE_HVAC_START + 0X0018) | VehiclePropertyType:INT32)
    HUT15_ACDRVSETTEMPSTEPLSREQ_TC = 559943734, // ((VEHICLE_HVAC_START + 0X0019) | VehiclePropertyType:FLOAT)
    HUT15_ACPASSSETTEMPSTEPLSREQ_TC = 559943735, // ((VEHICLE_HVAC_START + 0X001A) | VehiclePropertyType:FLOAT)
    HUT4_ACMODREQ_HUT = 557846584, // ((VEHICLE_HVAC_START + 0X001B) | VehiclePropertyType:INT32)
    R_PBOX1_RWINHEATGFAILSTS = 557846585, // ((VEHICLE_HVAC_START + 0X001C) | VehiclePropertyType:INT32)
    HUT35_ACMAXREQ_TC = 557846586, // ((VEHICLE_HVAC_START + 0X001D) | VehiclePropertyType:INT32)
    AC4_ACMAXENASTS = 557846587, // ((VEHICLE_HVAC_START + 0X001E) | VehiclePropertyType:INT32)
    VEHICLE_HVAC_END = 553652764, // (VEHICLE_HVAC_START + 0X01FF)
    VEHICLE_ADAS_START = 553652765, // (VEHICLE_HVAC_END + 0X0001)
    HUT_FD1_ADAS_POSN_MSGTYE = 557847070, // ((VEHICLE_ADAS_START + 0X0001) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_OFFSET = 557847071, // ((VEHICLE_ADAS_START + 0X0002) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_CYCCNT = 557847072, // ((VEHICLE_ADAS_START + 0X0003) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_PATHLDX = 557847073, // ((VEHICLE_ADAS_START + 0X0004) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_IDX = 557847074, // ((VEHICLE_ADAS_START + 0X0005) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_POSPROBB = 559944227, // ((VEHICLE_ADAS_START + 0X0006) | VehiclePropertyType:FLOAT)
    HUT_FD1_ADAS_POSN_AGE = 557847076, // ((VEHICLE_ADAS_START + 0X0007) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_POSCONFDC = 557847077, // ((VEHICLE_ADAS_START + 0X0008) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_CURLANE = 557847078, // ((VEHICLE_ADAS_START + 0X0009) | VehiclePropertyType:INT32)
    HUT_FD1_ADAS_POSN_SPD = 559944231, // ((VEHICLE_ADAS_START + 0X000A) | VehiclePropertyType:FLOAT)
    HUT_FD1_ADAS_POSN_REHEAD = 559944232, // ((VEHICLE_ADAS_START + 0X000B) | VehiclePropertyType:FLOAT)
    HUT19_ADAS_META_MSGTYP = 557847081, // ((VEHICLE_ADAS_START + 0X000C) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_MAPPROVIDER = 557847082, // ((VEHICLE_ADAS_START + 0X000D) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_COUNTRYCODE = 557847083, // ((VEHICLE_ADAS_START + 0X000E) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_CYCCNT = 557847084, // ((VEHICLE_ADAS_START + 0X000F) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_PROTVER_MAJOR = 557847085, // ((VEHICLE_ADAS_START + 0X0010) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_PROTVER_MINORSUB = 557847086, // ((VEHICLE_ADAS_START + 0X0011) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_HWVER = 557847087, // ((VEHICLE_ADAS_START + 0X0012) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_DRVSIDE = 557847088, // ((VEHICLE_ADAS_START + 0X0013) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_REGIONCODE = 557847089, // ((VEHICLE_ADAS_START + 0X0014) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_MAPVERQTR = 557847090, // ((VEHICLE_ADAS_START + 0X0015) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_MAPVERYEAR = 557847091, // ((VEHICLE_ADAS_START + 0X0016) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_SPDUNITS = 557847092, // ((VEHICLE_ADAS_START + 0X0017) | VehiclePropertyType:INT32)
    HUT19_ADAS_META_PROTVER_MINOR = 557847093, // ((VEHICLE_ADAS_START + 0X0018) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_MSGTYP = 557847094, // ((VEHICLE_ADAS_START + 0X0019) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_OFFSET = 557847095, // ((VEHICLE_ADAS_START + 0X001A) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_CYCCNT = 557847096, // ((VEHICLE_ADAS_START + 0X001B) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_PATHIDX = 557847097, // ((VEHICLE_ADAS_START + 0X001C) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_PROFTYP = 557847098, // ((VEHICLE_ADAS_START + 0X001D) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_CTRLPOINT = 557847099, // ((VEHICLE_ADAS_START + 0X001E) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_RETR = 557847100, // ((VEHICLE_ADAS_START + 0X001F) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_UPDATE = 557847101, // ((VEHICLE_ADAS_START + 0X0020) | VehiclePropertyType:INT32)
    HUT20_ADAS_PROFLONG_VALUE = 557847102, // ((VEHICLE_ADAS_START + 0X0021) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_MSGTYP = 557847103, // ((VEHICLE_ADAS_START + 0X0022) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_OFFSET = 557847104, // ((VEHICLE_ADAS_START + 0X0023) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_CYCCNT = 557847105, // ((VEHICLE_ADAS_START + 0X0024) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_PATHIDX = 557847106, // ((VEHICLE_ADAS_START + 0X0025) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_PROFTYP = 557847107, // ((VEHICLE_ADAS_START + 0X0026) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_CTRLPOINT = 557847108, // ((VEHICLE_ADAS_START + 0X0027) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_RETR = 557847109, // ((VEHICLE_ADAS_START + 0X0028) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_UPDATE = 557847110, // ((VEHICLE_ADAS_START + 0X0029) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_ACCURCLASS = 557847111, // ((VEHICLE_ADAS_START + 0X002A) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_DIST1 = 557847112, // ((VEHICLE_ADAS_START + 0X002B) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_VALUE0 = 557847113, // ((VEHICLE_ADAS_START + 0X002C) | VehiclePropertyType:INT32)
    HUT21_ADAS_PROFSHORT_VALUE1 = 557847114, // ((VEHICLE_ADAS_START + 0X002D) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_MSGTYP = 557847115, // ((VEHICLE_ADAS_START + 0X002E) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_OFFSET = 557847116, // ((VEHICLE_ADAS_START + 0X002F) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_CYCCNT = 557847117, // ((VEHICLE_ADAS_START + 0X0030) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_PATHIDX = 557847118, // ((VEHICLE_ADAS_START + 0X0031) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_TUNNEL = 557847119, // ((VEHICLE_ADAS_START + 0X0032) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_BRDG = 557847120, // ((VEHICLE_ADAS_START + 0X0033) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_BUILDUPAREA = 557847121, // ((VEHICLE_ADAS_START + 0X0034) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_RETR = 557847122, // ((VEHICLE_ADAS_START + 0X0035) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_UPDATE = 557847123, // ((VEHICLE_ADAS_START + 0X0036) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_RELPROBB = 559944276, // ((VEHICLE_ADAS_START + 0X0037) | VehiclePropertyType:FLOAT)
    HUT22_ADAS_SEG_FUNCROADCLASS = 557847125, // ((VEHICLE_ADAS_START + 0X0038) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_PARTOFCALCROUTE = 557847126, // ((VEHICLE_ADAS_START + 0X0039) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_CMPLXINSCT = 557847127, // ((VEHICLE_ADAS_START + 0X003A) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_FORMOFWAY = 557847128, // ((VEHICLE_ADAS_START + 0X003B) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_EFFSPDLMT = 557847129, // ((VEHICLE_ADAS_START + 0X003C) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_EFFSPDLMTTYP = 557847130, // ((VEHICLE_ADAS_START + 0X003D) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_DIVIDEROAD = 557847131, // ((VEHICLE_ADAS_START + 0X003E) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_NUMOFLANEOPPDIR = 557847132, // ((VEHICLE_ADAS_START + 0X003F) | VehiclePropertyType:INT32)
    HUT22_ADAS_SEG_NUMOFLANEDRVDIR = 557847133, // ((VEHICLE_ADAS_START + 0X0040) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_MSGTYP = 557847134, // ((VEHICLE_ADAS_START + 0X0041) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_OFFSET = 557847135, // ((VEHICLE_ADAS_START + 0X0042) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_CYCCNT = 557847136, // ((VEHICLE_ADAS_START + 0X0043) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_PATHIDX = 557847137, // ((VEHICLE_ADAS_START + 0X0044) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_STUBPATHIDX = 557847138, // ((VEHICLE_ADAS_START + 0X0045) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_RETR = 557847139, // ((VEHICLE_ADAS_START + 0X0046) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_UPDATE = 557847140, // ((VEHICLE_ADAS_START + 0X0047) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_RELPROBB = 559944293, // ((VEHICLE_ADAS_START + 0X0048) | VehiclePropertyType:FLOAT)
    HUT23_ADAS_STUB_FUNCROADCLASS = 557847142, // ((VEHICLE_ADAS_START + 0X0049) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_PARTOFCALCROUTE = 557847143, // ((VEHICLE_ADAS_START + 0X004A) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_CMPLXINSCT = 557847144, // ((VEHICLE_ADAS_START + 0X004B) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_FORMOFWAY = 557847145, // ((VEHICLE_ADAS_START + 0X004C) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_TURNANGL = 559944298, // ((VEHICLE_ADAS_START + 0X004D) | VehiclePropertyType:FLOAT)
    HUT23_ADAS_STUB_LASTSTUB = 557847147, // ((VEHICLE_ADAS_START + 0X004E) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_RTOFWAY = 557847148, // ((VEHICLE_ADAS_START + 0X004F) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_NUMOFLANEOPPDIR = 557847149, // ((VEHICLE_ADAS_START + 0X0050) | VehiclePropertyType:INT32)
    HUT23_ADAS_STUB_NUMOFLANEDRVDIR = 557847150, // ((VEHICLE_ADAS_START + 0X0051) | VehiclePropertyType:INT32)
    ACC_FD2_TJA_ICA_MODDISP = 557847151, // ((VEHICLE_ADAS_START + 0X0052) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_MODDISP = 557847152, // ((VEHICLE_ADAS_START + 0X0053) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_MODDISP = 557847153, // ((VEHICLE_ADAS_START + 0X0054) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_SPDSETVALUE = 557847154, // ((VEHICLE_ADAS_START + 0X0055) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_LINE01_CURV = 559944307, // ((VEHICLE_ADAS_START + 0X0056) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE01_CURVCHANGE = 559944308, // ((VEHICLE_ADAS_START + 0X0057) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE02_CURV = 559944309, // ((VEHICLE_ADAS_START + 0X0058) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE02_CURVCHANGE = 559944310, // ((VEHICLE_ADAS_START + 0X0059) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE03_CURV = 559944311, // ((VEHICLE_ADAS_START + 0X005A) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE03_CURVCHANGE = 559944312, // ((VEHICLE_ADAS_START + 0X005B) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_POTENTIALTGTDETN = 557847161, // ((VEHICLE_ADAS_START + 0X005C) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_POTENTIALTGTTYP = 557847162, // ((VEHICLE_ADAS_START + 0X005D) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RLVTGTDETN = 557847163, // ((VEHICLE_ADAS_START + 0X005E) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RLVTGTTYP = 557847164, // ((VEHICLE_ADAS_START + 0X005F) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETARGRTDECTION = 557847165, // ((VEHICLE_ADAS_START + 0X0060) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETGTTYP = 557847166, // ((VEHICLE_ADAS_START + 0X0061) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RITARGRTDETN = 557847167, // ((VEHICLE_ADAS_START + 0X0062) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RITGTTYP = 557847168, // ((VEHICLE_ADAS_START + 0X0063) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_POTENTIALTGTDY = 559944321, // ((VEHICLE_ADAS_START + 0X0064) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_POTENTIALTGTDX = 559944322, // ((VEHICLE_ADAS_START + 0X0065) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RLVTGTDY = 559944323, // ((VEHICLE_ADAS_START + 0X0066) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RLVTGTDX = 559944324, // ((VEHICLE_ADAS_START + 0X0067) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_LETGTDY = 559944325, // ((VEHICLE_ADAS_START + 0X0068) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_LETGTDX = 559944326, // ((VEHICLE_ADAS_START + 0X0069) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RITGTDY = 559944327, // ((VEHICLE_ADAS_START + 0X006A) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RITGTDX = 559944328, // ((VEHICLE_ADAS_START + 0X006B) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE01_TYPE = 557847177, // ((VEHICLE_ADAS_START + 0X006C) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_LINE02_TYPE = 557847178, // ((VEHICLE_ADAS_START + 0X006D) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_LINE03_TYPE = 557847179, // ((VEHICLE_ADAS_START + 0X006E) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_LINE04_TYPE = 557847180, // ((VEHICLE_ADAS_START + 0X006F) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_LINE01_DY = 559944333, // ((VEHICLE_ADAS_START + 0X0070) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE02_DY = 559944334, // ((VEHICLE_ADAS_START + 0X0071) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE03_DY = 559944335, // ((VEHICLE_ADAS_START + 0X0072) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE04_DY = 559944336, // ((VEHICLE_ADAS_START + 0X0073) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE01_HEADINGANGLE = 559944337, // ((VEHICLE_ADAS_START + 0X0074) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE02_HEADINGANGLE = 559944338, // ((VEHICLE_ADAS_START + 0X0075) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE03_HEADINGANGLE = 559944339, // ((VEHICLE_ADAS_START + 0X0076) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE04_CURV = 559944340, // ((VEHICLE_ADAS_START + 0X0077) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_LINE04_CURVCHANGE = 559944341, // ((VEHICLE_ADAS_START + 0X0078) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE04_HEADINGANGLE = 559944342, // ((VEHICLE_ADAS_START + 0X0079) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_ROADEDGE01_TYPE = 557847191, // ((VEHICLE_ADAS_START + 0X007A) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_ROADEDGE01_DY = 559944344, // ((VEHICLE_ADAS_START + 0X007B) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_ROADEDGE01_CURV = 559944345, // ((VEHICLE_ADAS_START + 0X007C) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_ROADEDGE01_CURVCHANGE = 559944346, // ((VEHICLE_ADAS_START + 0X007D) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_ROADEDGE01_HEADINGANGLE = 559944347, // ((VEHICLE_ADAS_START + 0X007E) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_ROADEDGE02_TYPE = 557847196, // ((VEHICLE_ADAS_START + 0X007F) | VehiclePropertyType:INT32)
    IFC_FD5_IFC_ROADEDGE02_DY = 559944349, // ((VEHICLE_ADAS_START + 0X0080) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_ROADEDGE02_CURV = 559944350, // ((VEHICLE_ADAS_START + 0X0081) | VehiclePropertyType:FLOAT)
    IFC_FD5_IFC_ROADEDGE02_CURVCHANGE = 559944351, // ((VEHICLE_ADAS_START + 0X0082) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_ROADEDGE02_HEADINGANGLE = 559944352, // ((VEHICLE_ADAS_START + 0X0083) | VehiclePropertyType:FLOAT)
    IFC_FD2_LSSINTERVENTION = 557847201, // ((VEHICLE_ADAS_START + 0X0084) | VehiclePropertyType:INT32)
    RSDS_FD1_BSD_LCA_WARNINGREQRIGHT = 557847202, // ((VEHICLE_ADAS_START + 0X0085) | VehiclePropertyType:INT32)
    RSDS_FD1_BSD_LCA_WARNINGREQLEFT = 557847203, // ((VEHICLE_ADAS_START + 0X0086) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RLVTGTHEADING = 559944356, // ((VEHICLE_ADAS_START + 0X0087) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_LETGTHEADING = 559944357, // ((VEHICLE_ADAS_START + 0X0088) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RITGTHEADING = 559944358, // ((VEHICLE_ADAS_START + 0X0089) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_POTENTIALTGTHEADING = 559944359, // ((VEHICLE_ADAS_START + 0X008A) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_TGTOBJBARDISP = 557847208, // ((VEHICLE_ADAS_START + 0X008B) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETGTOBJBARDISP = 557847209, // ((VEHICLE_ADAS_START + 0X008C) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RITGTOBJBARDISP = 557847210, // ((VEHICLE_ADAS_START + 0X008D) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_LETGT_01 = 557847211, // ((VEHICLE_ADAS_START + 0X008E) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_LETGT_01_DX = 559944364, // ((VEHICLE_ADAS_START + 0X008F) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_LETGT_01_DY = 559944365, // ((VEHICLE_ADAS_START + 0X0090) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_LETGT_02 = 557847214, // ((VEHICLE_ADAS_START + 0X0091) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_LETGT_02_DX = 559944367, // ((VEHICLE_ADAS_START + 0X0092) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_LETGT_02_DY = 559944368, // ((VEHICLE_ADAS_START + 0X0093) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_RITGT_01 = 557847217, // ((VEHICLE_ADAS_START + 0X0094) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_RITGT_01_DX = 559944370, // ((VEHICLE_ADAS_START + 0X0095) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_RITGT_01_DY = 559944371, // ((VEHICLE_ADAS_START + 0X0096) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_RITGT_02 = 557847220, // ((VEHICLE_ADAS_START + 0X0097) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_RITGT_02_DX = 559944373, // ((VEHICLE_ADAS_START + 0X0098) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_RITGT_02_DY = 559944374, // ((VEHICLE_ADAS_START + 0X0099) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_MIDTGT_01 = 557847223, // ((VEHICLE_ADAS_START + 0X009A) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_MIDTGT_01_DX = 559944376, // ((VEHICLE_ADAS_START + 0X009B) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_MIDTGT_01_DY = 559944377, // ((VEHICLE_ADAS_START + 0X009C) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_MIDTGT_02 = 557847226, // ((VEHICLE_ADAS_START + 0X009D) | VehiclePropertyType:INT32)
    RSDS_FD2_RSDS_MIDTGT_02_DX = 559944379, // ((VEHICLE_ADAS_START + 0X009E) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RSDS_MIDTGT_02_DY = 559944380, // ((VEHICLE_ADAS_START + 0X009F) | VehiclePropertyType:FLOAT)
    AEB_FD2_AEB_PEDRESP = 557847229, // ((VEHICLE_ADAS_START + 0X00A0) | VehiclePropertyType:INT32)
    AEB_FD2_AEB_VEHRESP = 557847230, // ((VEHICLE_ADAS_START + 0X00A1) | VehiclePropertyType:INT32)
    AEB_FD2_FCW_SNVTY = 557847231, // ((VEHICLE_ADAS_START + 0X00A2) | VehiclePropertyType:INT32)
    CR_FD1_CR_FCTA_RESP = 557847232, // ((VEHICLE_ADAS_START + 0X00A3) | VehiclePropertyType:INT32)
    CR_FD1_CR_FCTB_RESP = 557847233, // ((VEHICLE_ADAS_START + 0X00A4) | VehiclePropertyType:INT32)
    AEB_FD2_AEB_JA_RESP = 557847234, // ((VEHICLE_ADAS_START + 0X00A5) | VehiclePropertyType:INT32)
    IFC_FD2_ESS_PEDRESP = 557847235, // ((VEHICLE_ADAS_START + 0X00A6) | VehiclePropertyType:INT32)
    IFC_FD2_ESS_VEHRESP = 557847236, // ((VEHICLE_ADAS_START + 0X00A7) | VehiclePropertyType:INT32)
    IFC_FD2_AESRESP = 557847237, // ((VEHICLE_ADAS_START + 0X00A8) | VehiclePropertyType:INT32)
    AEB_FD2_FCW_AEB_VEHFCTSTS = 557847238, // ((VEHICLE_ADAS_START + 0X00A9) | VehiclePropertyType:INT32)
    AEB_FD2_FCW_WARN = 557847239, // ((VEHICLE_ADAS_START + 0X00AA) | VehiclePropertyType:INT32)
    AEB_FD2_AEB_VEHTRIG = 557847240, // ((VEHICLE_ADAS_START + 0X00AB) | VehiclePropertyType:INT32)
    AEB_FD2_AEB_PEDTRIG = 557847241, // ((VEHICLE_ADAS_START + 0X00AC) | VehiclePropertyType:INT32)
    AEB_FD2_AEB_JABRKTRIG = 557847242, // ((VEHICLE_ADAS_START + 0X00AD) | VehiclePropertyType:INT32)
    CR_FD1_FCTA_WARN = 557847243, // ((VEHICLE_ADAS_START + 0X00AE) | VehiclePropertyType:INT32)
    CR_FD1_FCTBTRIG = 557847244, // ((VEHICLE_ADAS_START + 0X00AF) | VehiclePropertyType:INT32)
    CR_FD1_FCTA_B_FUNCSTS = 557847245, // ((VEHICLE_ADAS_START + 0X00B0) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_INTERSYSINFODISP = 557847246, // ((VEHICLE_ADAS_START + 0X00B1) | VehiclePropertyType:INT32)
    CR_FD1_CR_BLISTS = 557847247, // ((VEHICLE_ADAS_START + 0X00B2) | VehiclePropertyType:INT32)
    IFC_FD2_AESSTS = 557847248, // ((VEHICLE_ADAS_START + 0X00B3) | VehiclePropertyType:INT32)
    IFC_FD2_AESERRSTS = 557847249, // ((VEHICLE_ADAS_START + 0X00B4) | VehiclePropertyType:INT32)
    IFC_FD2_AESINTERVENTION = 557847250, // ((VEHICLE_ADAS_START + 0X00B5) | VehiclePropertyType:INT32)
    IFC_FD2_ESSSTS = 557847251, // ((VEHICLE_ADAS_START + 0X00B6) | VehiclePropertyType:INT32)
    IFC_FD2_ESSERRSTS = 557847252, // ((VEHICLE_ADAS_START + 0X00B7) | VehiclePropertyType:INT32)
    IFC_FD2_ESSINTERVENTION = 557847253, // ((VEHICLE_ADAS_START + 0X00B8) | VehiclePropertyType:INT32)
    IFC_FD2_LDWRESP = 557847254, // ((VEHICLE_ADAS_START + 0X00B9) | VehiclePropertyType:INT32)
    IFC_FD2_LKARESP = 557847255, // ((VEHICLE_ADAS_START + 0X00BA) | VehiclePropertyType:INT32)
    IFC_FD2_LCKRESP = 557847256, // ((VEHICLE_ADAS_START + 0X00BB) | VehiclePropertyType:INT32)
    IFC_FD2_ELKRESP = 557847257, // ((VEHICLE_ADAS_START + 0X00BC) | VehiclePropertyType:INT32)
    IFC_FD2_LDW_LKA_SNVTY = 557847258, // ((VEHICLE_ADAS_START + 0X00BD) | VehiclePropertyType:INT32)
    IFC_FD2_LSSRESP = 557847259, // ((VEHICLE_ADAS_START + 0X00BE) | VehiclePropertyType:INT32)
    IFC_FD2_LSSWARNFORMRESP = 557847260, // ((VEHICLE_ADAS_START + 0X00BF) | VehiclePropertyType:INT32)
    IFC_FD2_INTELLIGENTEVARESP = 557847261, // ((VEHICLE_ADAS_START + 0X00C0) | VehiclePropertyType:INT32)
    IFC_FD2_LSSFUNCSTS = 557847262, // ((VEHICLE_ADAS_START + 0X00C1) | VehiclePropertyType:INT32)
    IFC_FD2_LSSERRSTS = 557847263, // ((VEHICLE_ADAS_START + 0X00C2) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_LCARESP = 557847264, // ((VEHICLE_ADAS_START + 0X00C3) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_RCTARESP = 557847265, // ((VEHICLE_ADAS_START + 0X00C4) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_RCTABRKRESP = 557847266, // ((VEHICLE_ADAS_START + 0X00C5) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_DOWRESP = 557847267, // ((VEHICLE_ADAS_START + 0X00C6) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_RCWRESP = 557847268, // ((VEHICLE_ADAS_START + 0X00C7) | VehiclePropertyType:INT32)
    RSDS_FD1_RCTA_WARNINGREQRIGHT = 557847269, // ((VEHICLE_ADAS_START + 0X00C8) | VehiclePropertyType:INT32)
    RSDS_FD1_RCTA_WARNINGREQLEFT = 557847270, // ((VEHICLE_ADAS_START + 0X00C9) | VehiclePropertyType:INT32)
    RSDS_FD1_DOW_WARNINGREQRIGHT = 557847271, // ((VEHICLE_ADAS_START + 0X00CA) | VehiclePropertyType:INT32)
    RSDS_FD1_DOW_WARNINGREQLEFT = 557847272, // ((VEHICLE_ADAS_START + 0X00CB) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_IPSOUNDREQ = 557847273, // ((VEHICLE_ADAS_START + 0X00CC) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_BRKGTRIG = 557847274, // ((VEHICLE_ADAS_START + 0X00CD) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_RCW_TRIGGER = 557847275, // ((VEHICLE_ADAS_START + 0X00CE) | VehiclePropertyType:INT32)
    IFC_FD3_TSRRESP = 557847276, // ((VEHICLE_ADAS_START + 0X00CF) | VehiclePropertyType:INT32)
    IFC_FD3_TSRWARNRESP = 557847277, // ((VEHICLE_ADAS_START + 0X00D0) | VehiclePropertyType:INT32)
    IFC_FD3_TSR_SNVTY = 557847278, // ((VEHICLE_ADAS_START + 0X00D1) | VehiclePropertyType:INT32)
    ACC_FD2_ISL_RESP = 557847279, // ((VEHICLE_ADAS_START + 0X00D2) | VehiclePropertyType:INT32)
    IFC_FD3_TSRTRFCSIGNVAL = 557847280, // ((VEHICLE_ADAS_START + 0X00D3) | VehiclePropertyType:INT32)
    IFC_FD3_TSRSPDLIM = 557847281, // ((VEHICLE_ADAS_START + 0X00D4) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_RESP = 557847282, // ((VEHICLE_ADAS_START + 0X00D5) | VehiclePropertyType:INT32)
    ACC_FD2_ALC_RESP = 557847283, // ((VEHICLE_ADAS_START + 0X00D6) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_SNVTYSET_RESP = 557847284, // ((VEHICLE_ADAS_START + 0X00D7) | VehiclePropertyType:INT32)
    VEHICLE_ADAS_END = 553653276, // (VEHICLE_ADAS_START + 0X01FF)
    VEHICLE_DRVCTRL_START = 553653277, // (VEHICLE_ADAS_END + 0X0001)
    HUT_FD1_HUT_ESPFUNCOFFSWTSTS = 557847582, // ((VEHICLE_DRVCTRL_START + 0X0001) | VehiclePropertyType:INT32)
    HUT_FD1_SSSSET_HUT = 557847583, // ((VEHICLE_DRVCTRL_START + 0X0002) | VehiclePropertyType:INT32)
    HUT_FD1_DSTSWTREQ = 557847584, // ((VEHICLE_DRVCTRL_START + 0X0003) | VehiclePropertyType:INT32)
    HUT_FD1_EPSSTEERMODSWTREQ = 557847585, // ((VEHICLE_DRVCTRL_START + 0X0004) | VehiclePropertyType:INT32)
    HUT1_DRVDROWSNSDETNSET = 557847586, // ((VEHICLE_DRVCTRL_START + 0X0005) | VehiclePropertyType:INT32)
    HUT_FD1_DRIVINGMODREQ_HUT = 557847587, // ((VEHICLE_DRVCTRL_START + 0X0006) | VehiclePropertyType:INT32)
    HUT_FD1_HUT_DRVMODREQ = 557847588, // ((VEHICLE_DRVCTRL_START + 0X0007) | VehiclePropertyType:INT32)
    HUT_FD1_HUT_BATTSOCLIMSAVEMOD = 557847589, // ((VEHICLE_DRVCTRL_START + 0X0008) | VehiclePropertyType:INT32)
    HUT13_VSGSWTSET = 557847590, // ((VEHICLE_DRVCTRL_START + 0X0009) | VehiclePropertyType:INT32)
    HUT17_OPDSWTSTS = 557847591, // ((VEHICLE_DRVCTRL_START + 0X000A) | VehiclePropertyType:INT32)
    HUT25_CARWASHMODE_REQ = 557847592, // ((VEHICLE_DRVCTRL_START + 0X000B) | VehiclePropertyType:INT32)
    ESP_FD2_ESPFAILSTS = 557847593, // ((VEHICLE_DRVCTRL_START + 0X000C) | VehiclePropertyType:INT32)
    HCU_PT4_OPDPARKWARNREQ = 557847594, // ((VEHICLE_DRVCTRL_START + 0X000D) | VehiclePropertyType:INT32)
    BCM1_DROWSYDRVDETWARN = 557847595, // ((VEHICLE_DRVCTRL_START + 0X000E) | VehiclePropertyType:INT32)
    VEHICLE_DRVCTRL_END = 553653788, // (VEHICLE_DRVCTRL_START + 0X01FF)
    VEHICLE_INNERCTRL_START = 553653789, // (VEHICLE_DRVCTRL_END + 0X0001)
    HUT10_ALCMSTATSWT = 557848094, // ((VEHICLE_INNERCTRL_START + 0X0001) | VehiclePropertyType:INT32)
    HUT_FD1_ALCMLIGHTLVLSET = 557848095, // ((VEHICLE_INNERCTRL_START + 0X0002) | VehiclePropertyType:INT32)
    HUT10_ALCMCLRSET = 557848096, // ((VEHICLE_INNERCTRL_START + 0X0003) | VehiclePropertyType:INT32)
    HUT10_ALCMRHMSWT = 557848097, // ((VEHICLE_INNERCTRL_START + 0X0004) | VehiclePropertyType:INT32)
    HUT17_WPC_SWTREQ = 557848098, // ((VEHICLE_INNERCTRL_START + 0X0005) | VehiclePropertyType:INT32)
    WPC1_WPC_SWTSTS = 557848099, // ((VEHICLE_INNERCTRL_START + 0X0006) | VehiclePropertyType:INT32)
    WPC1_WPC_CHRGSTS = 557848100, // ((VEHICLE_INNERCTRL_START + 0X0007) | VehiclePropertyType:INT32)
    HUT25_HUD_SWTREQ_VR = 557848101, // ((VEHICLE_INNERCTRL_START + 0X0008) | VehiclePropertyType:INT32)
    HUT25_HUD_HEIGHTLVLSWT_VR = 557848102, // ((VEHICLE_INNERCTRL_START + 0X0009) | VehiclePropertyType:INT32)
    HUT25_HUD_BRIGHTNESSLVLSWT_VR = 557848103, // ((VEHICLE_INNERCTRL_START + 0X000A) | VehiclePropertyType:INT32)
    HUT30_HUD_ROTATEDIRCFG = 557848104, // ((VEHICLE_INNERCTRL_START + 0X000B) | VehiclePropertyType:INT32)
    HUT30_HUD_ROTATEANGLECFG = 557848105, // ((VEHICLE_INNERCTRL_START + 0X000C) | VehiclePropertyType:INT32)
    HUT30_HUD_DISPLAYMODECFG = 557848106, // ((VEHICLE_INNERCTRL_START + 0X000D) | VehiclePropertyType:INT32)
    HUT27_HUT_DISTANCETOTURN = 557848107, // ((VEHICLE_INNERCTRL_START + 0X000E) | VehiclePropertyType:INT32)
    HUT27_HUT_ETA = 557848108, // ((VEHICLE_INNERCTRL_START + 0X000F) | VehiclePropertyType:INT32)
    HUT28_HUT_DTD = 557848109, // ((VEHICLE_INNERCTRL_START + 0X0010) | VehiclePropertyType:INT32)
    HUT28_HUT_TURNARROW = 557848110, // ((VEHICLE_INNERCTRL_START + 0X0011) | VehiclePropertyType:INT32)
    HUT30_BTPHONEDISPSWT = 557848111, // ((VEHICLE_INNERCTRL_START + 0X0012) | VehiclePropertyType:INT32)
    HUT33_ADASDISPSWT = 557848112, // ((VEHICLE_INNERCTRL_START + 0X0013) | VehiclePropertyType:INT32)
    HUT30_PPMIBSRSWTSET = 557848113, // ((VEHICLE_INNERCTRL_START + 0X0014) | VehiclePropertyType:INT32)
    PPMI1_BSR_ENASTS = 557848114, // ((VEHICLE_INNERCTRL_START + 0X0015) | VehiclePropertyType:INT32)
    HUT30_PPMIHWSWTSET = 557848115, // ((VEHICLE_INNERCTRL_START + 0X0016) | VehiclePropertyType:INT32)
    PPMI1_HW_ENASTS = 557848116, // ((VEHICLE_INNERCTRL_START + 0X0017) | VehiclePropertyType:INT32)
    HUT13_DRVSEATADJMT_HUT = 557848117, // ((VEHICLE_INNERCTRL_START + 0X0018) | VehiclePropertyType:INT32)
    HUT1_CHAIRMEMPOSNENA = 557848118, // ((VEHICLE_INNERCTRL_START + 0X0019) | VehiclePropertyType:INT32)
    HUT35_WPC2_SWTREQ = 557848119, // ((VEHICLE_INNERCTRL_START + 0X001A) | VehiclePropertyType:INT32)
    HUT35_NFCINFOREQ = 558896696, // ((VEHICLE_INNERCTRL_START + 0X001B) | VehiclePropertyType:INT64)
    WPC2_1_WPC2_SWTSTS = 557848121, // ((VEHICLE_INNERCTRL_START + 0X001C) | VehiclePropertyType:INT32)
    WPC2_1_WPC2_FAULTSTS = 557848122, // ((VEHICLE_INNERCTRL_START + 0X001D) | VehiclePropertyType:INT32)
    WPC2_1_WPC2_PHONEREMINDER = 557848123, // ((VEHICLE_INNERCTRL_START + 0X001E) | VehiclePropertyType:INT32)
    HUT33_ALCMDYNMODSET = 557848124, // ((VEHICLE_INNERCTRL_START + 0X001F) | VehiclePropertyType:INT32)
    HUT_FD3_ALCMMASTERSWT = 557848125, // ((VEHICLE_INNERCTRL_START + 0X0020) | VehiclePropertyType:INT32)
    HUT10_ALCMDYNSWT = 557848126, // ((VEHICLE_INNERCTRL_START + 0X0021) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_SWTSTS = 557848134, // ((VEHICLE_INNERCTRL_START + 0X0029) | VehiclePropertyType:INT32)
    HUT33_CHAIRMEMPOSNSETSWT = 557848135, // ((VEHICLE_INNERCTRL_START + 0X002A) | VehiclePropertyType:INT32)
    HUT_FD1_CHAIRMEMPOSNSET = 557848136, // ((VEHICLE_INNERCTRL_START + 0X002B) | VehiclePropertyType:INT32)
    HUT_FD1_CHAIRMEMPOSNSETRESULT = 557848137, // ((VEHICLE_INNERCTRL_START + 0X002C) | VehiclePropertyType:INT32)
    VEHICLE_INNERCTRL_END = 553654300, // (VEHICLE_INNERCTRL_START + 0X01FF)
    VEHICLE_BODYCTRL_START = 553654301, // (VEHICLE_INNERCTRL_END + 0X0001)
    HUT15_DOORUNLOCKMODSET = 557848606, // ((VEHICLE_BODYCTRL_START + 0X0001) | VehiclePropertyType:INT32)
    HUT15_SPDAUTOLOCKMODSET = 557848607, // ((VEHICLE_BODYCTRL_START + 0X0002) | VehiclePropertyType:INT32)
    HUT33_FLAOUTUNLOCKSET = 557848608, // ((VEHICLE_BODYCTRL_START + 0X0003) | VehiclePropertyType:INT32)
    HUT33_SUPERLOCKSET = 557848609, // ((VEHICLE_BODYCTRL_START + 0X0004) | VehiclePropertyType:INT32)
    HUT15_SUNROOFSWTREQ_VR = 557848610, // ((VEHICLE_BODYCTRL_START + 0X0005) | VehiclePropertyType:INT32)
    HUT15_SUNSHADESWTREQ_VR = 557848611, // ((VEHICLE_BODYCTRL_START + 0X0006) | VehiclePropertyType:INT32)
    HUT_FD1_HFASWTSTS = 557848612, // ((VEHICLE_BODYCTRL_START + 0X0007) | VehiclePropertyType:INT32)
    HUT1_REARVIEWFOLDMODSET = 557848613, // ((VEHICLE_BODYCTRL_START + 0X0008) | VehiclePropertyType:INT32)
    HUT13_ORVMLENSADJMT_HUT = 557848614, // ((VEHICLE_BODYCTRL_START + 0X0009) | VehiclePropertyType:INT32)
    HUT15_AUTOWSHWIPSET = 557848615, // ((VEHICLE_BODYCTRL_START + 0X000A) | VehiclePropertyType:INT32)
    HUT4_ELECSIDESTEPPINGSYSSET = 557848616, // ((VEHICLE_BODYCTRL_START + 0X000B) | VehiclePropertyType:INT32)
    HUT4_ALLTERRAINDISLSET = 557848617, // ((VEHICLE_BODYCTRL_START + 0X000C) | VehiclePropertyType:INT32)
    HUT4_ROOFMODSET = 557848618, // ((VEHICLE_BODYCTRL_START + 0X000D) | VehiclePropertyType:INT32)
    BCM8_RVIEWMIRRORSTS = 557848619, // ((VEHICLE_BODYCTRL_START + 0X000E) | VehiclePropertyType:INT32)
    BCM3_DOORLOCKSTS = 557848620, // ((VEHICLE_BODYCTRL_START + 0X000F) | VehiclePropertyType:INT32)
    BCM3_DRVDOORLOCKSTS = 557848621, // ((VEHICLE_BODYCTRL_START + 0X0010) | VehiclePropertyType:INT32)
    BCM12_SSPOSN_VR_APP = 557848622, // ((VEHICLE_BODYCTRL_START + 0X0011) | VehiclePropertyType:INT32)
    HUT13_ORVMPOSNENASTS = 557848623, // ((VEHICLE_BODYCTRL_START + 0X0012) | VehiclePropertyType:INT32)
    VEHICLE_BODYCTRL_END = 553654812, // (VEHICLE_BODYCTRL_START + 0X01FF)
    VEHICLE_SETLIGHT_START = 553654813, // (VEHICLE_BODYCTRL_END + 0X0001)
    HUT1_BATTSAVEDELAYTIMESET = 557849118, // ((VEHICLE_SETLIGHT_START + 0X0001) | VehiclePropertyType:INT32)
    HUT1_DOMELMPDLYTIMSET = 557849119, // ((VEHICLE_SETLIGHT_START + 0X0002) | VehiclePropertyType:INT32)
    HUT1_FOLWMEHOMEDLYTIMSET = 557849120, // ((VEHICLE_SETLIGHT_START + 0X0003) | VehiclePropertyType:INT32)
    HUT15_SPORTMODLIGHTSET = 557849121, // ((VEHICLE_SETLIGHT_START + 0X0004) | VehiclePropertyType:INT32)
    HUT15_PARKLMPSET = 557849122, // ((VEHICLE_SETLIGHT_START + 0X0005) | VehiclePropertyType:INT32)
    HUT15_SEEKVEHSET = 557849123, // ((VEHICLE_SETLIGHT_START + 0X0006) | VehiclePropertyType:INT32)
    HUT33_APPROACHLAMPSET = 557849124, // ((VEHICLE_SETLIGHT_START + 0X0007) | VehiclePropertyType:INT32)
    HUT30_DISTRCTNDETNSET = 557849125, // ((VEHICLE_SETLIGHT_START + 0X0008) | VehiclePropertyType:INT32)
    HUT25_GAZEAREAINTERACTIONSETUP = 557849126, // ((VEHICLE_SETLIGHT_START + 0X0009) | VehiclePropertyType:INT32)
    HUT25_OCCPTEXPRSNSETUP = 557849127, // ((VEHICLE_SETLIGHT_START + 0X000A) | VehiclePropertyType:INT32)
    HUT25_OCCPTAGESETUP = 557849128, // ((VEHICLE_SETLIGHT_START + 0X000B) | VehiclePropertyType:INT32)
    HUT25_OCCPTGENDERSETUP = 557849129, // ((VEHICLE_SETLIGHT_START + 0X000C) | VehiclePropertyType:INT32)
    VEHICLE_SETLIGHT_END = 553655324, // (VEHICLE_SETLIGHT_START + 0X01FF)
    VEHICLE_SETPARK_START = 553655325, // (VEHICLE_SETLIGHT_END + 0X0001)
    HUT6_FPAS_AUTOMODSWT = 557849630, // ((VEHICLE_SETPARK_START + 0X0001) | VehiclePropertyType:INT32)
    HUT6_MEBSWTSET = 557849631, // ((VEHICLE_SETPARK_START + 0X0002) | VehiclePropertyType:INT32)
    HUT15_SDWSWTSET = 557849632, // ((VEHICLE_SETPARK_START + 0X0003) | VehiclePropertyType:INT32)
    HUT6_AUTOAVMSWSET_CMD = 557849633, // ((VEHICLE_SETPARK_START + 0X0004) | VehiclePropertyType:INT32)
    HUT15_PASEXITSPDSWT = 557849634, // ((VEHICLE_SETPARK_START + 0X0005) | VehiclePropertyType:INT32)
    HUT6_LANECALACTVTCMD = 557849635, // ((VEHICLE_SETPARK_START + 0X0006) | VehiclePropertyType:INT32)
    HUT6_SWTOFIELDCALRSTCMD = 557849636, // ((VEHICLE_SETPARK_START + 0X0007) | VehiclePropertyType:INT32)
    HAP_FD2_LANECALSTS = 557849637, // ((VEHICLE_SETPARK_START + 0X0008) | VehiclePropertyType:INT32)
    HAP_FD2_LANECALFLTSTS = 557849638, // ((VEHICLE_SETPARK_START + 0X0009) | VehiclePropertyType:INT32)
    VEHICLE_SETPARK_END = 553655836, // (VEHICLE_SETPARK_START + 0X01FF)
    VEHICLE_INTELDRV_START = 553655837, // (VEHICLE_SETPARK_END + 0X0001)
    BCM8_DRVASSTSWTREQ = 557850142, // ((VEHICLE_INTELDRV_START + 0X0001) | VehiclePropertyType:INT32)
    BCM8_DRVASSTSWTERRSTS = 557850143, // ((VEHICLE_INTELDRV_START + 0X0002) | VehiclePropertyType:INT32)
    HUT_FD1_AEB_PEDSWTREQ = 557850144, // ((VEHICLE_INTELDRV_START + 0X0003) | VehiclePropertyType:INT32)
    HUT_FD1_AEB_VEHSWTREQ = 557850145, // ((VEHICLE_INTELDRV_START + 0X0004) | VehiclePropertyType:INT32)
    HUT_FD1_FCW_SNVTYSET = 557850146, // ((VEHICLE_INTELDRV_START + 0X0005) | VehiclePropertyType:INT32)
    HUT_FD1_FCTASWTREQ = 557850147, // ((VEHICLE_INTELDRV_START + 0X0006) | VehiclePropertyType:INT32)
    HUT_FD1_FCTABRKSWTREQ = 557850148, // ((VEHICLE_INTELDRV_START + 0X0007) | VehiclePropertyType:INT32)
    HUT_FD1_AEB_JASWTREQ = 557850149, // ((VEHICLE_INTELDRV_START + 0X0008) | VehiclePropertyType:INT32)
    HUT_FD1_ESS_PEDSWTREQ = 557850150, // ((VEHICLE_INTELDRV_START + 0X0009) | VehiclePropertyType:INT32)
    HUT_FD1_ESS_VEHSWTREQ = 557850151, // ((VEHICLE_INTELDRV_START + 0X000A) | VehiclePropertyType:INT32)
    HUT_FD1_AESSWTREQ = 557850152, // ((VEHICLE_INTELDRV_START + 0X000B) | VehiclePropertyType:INT32)
    HUT_FD1_LDWSWTREQ = 557850153, // ((VEHICLE_INTELDRV_START + 0X000C) | VehiclePropertyType:INT32)
    HUT_FD1_LKASWTREQ = 557850154, // ((VEHICLE_INTELDRV_START + 0X000D) | VehiclePropertyType:INT32)
    HUT_FD1_LCKSWTREQ = 557850155, // ((VEHICLE_INTELDRV_START + 0X000E) | VehiclePropertyType:INT32)
    HUT_FD1_ELKSWTREQ = 557850156, // ((VEHICLE_INTELDRV_START + 0X000F) | VehiclePropertyType:INT32)
    HUT_FD1_IFC_SNVTYSET = 557850157, // ((VEHICLE_INTELDRV_START + 0X0010) | VehiclePropertyType:INT32)
    HUT_FD1_LSSSWTREQ = 557850158, // ((VEHICLE_INTELDRV_START + 0X0011) | VehiclePropertyType:INT32)
    HUT_FD1_LSSWARNFORMSWTREQ = 557850159, // ((VEHICLE_INTELDRV_START + 0X0012) | VehiclePropertyType:INT32)
    HUT_FD1_INTELLIGENTEVASWTREQ = 557850160, // ((VEHICLE_INTELDRV_START + 0X0013) | VehiclePropertyType:INT32)
    HUT_FD1_LCASWTREQ = 557850161, // ((VEHICLE_INTELDRV_START + 0X0014) | VehiclePropertyType:INT32)
    HUT_FD1_RCTASWTREQ = 557850162, // ((VEHICLE_INTELDRV_START + 0X0015) | VehiclePropertyType:INT32)
    HUT_FD1_RCTABRKSWTREQ = 557850163, // ((VEHICLE_INTELDRV_START + 0X0016) | VehiclePropertyType:INT32)
    HUT_FD1_DOWSWTREQ = 557850164, // ((VEHICLE_INTELDRV_START + 0X0017) | VehiclePropertyType:INT32)
    HUT_FD1_RCWSWTREQ = 557850165, // ((VEHICLE_INTELDRV_START + 0X0018) | VehiclePropertyType:INT32)
    HUT_FD1_TSRSWTREQ = 557850166, // ((VEHICLE_INTELDRV_START + 0X0019) | VehiclePropertyType:INT32)
    HUT_FD1_TSRWARNSWTREQ = 557850167, // ((VEHICLE_INTELDRV_START + 0X001A) | VehiclePropertyType:INT32)
    HUT_FD1_TSR_SNVTYSET = 557850168, // ((VEHICLE_INTELDRV_START + 0X001B) | VehiclePropertyType:INT32)
    HUT_FD1_ISLSWTREQ = 557850169, // ((VEHICLE_INTELDRV_START + 0X001C) | VehiclePropertyType:INT32)
    HUT_FD1_HWA_SWTREQ = 557850170, // ((VEHICLE_INTELDRV_START + 0X001D) | VehiclePropertyType:INT32)
    HUT_FD1_ALC_SWTREQ = 557850171, // ((VEHICLE_INTELDRV_START + 0X001E) | VehiclePropertyType:INT32)
    HUT_FD1_HWA_SNVTYSET = 557850172, // ((VEHICLE_INTELDRV_START + 0X001F) | VehiclePropertyType:INT32)
    HUT_FD1_HWA_WARNFORMSWTREQ = 557850173, // ((VEHICLE_INTELDRV_START + 0X0020) | VehiclePropertyType:INT32)
    VEHICLE_INTELDRV_END = 553656348, // (VEHICLE_INTELDRV_START + 0X01FF)
    VEHICLE_BASICINFO_START = 553656349, // (VEHICLE_INTELDRV_END + 0X0001)
    ESP_FD2_VEHSPDVLD = 557850654, // ((VEHICLE_BASICINFO_START + 0X0001) | VehiclePropertyType:INT32)
    IP1_IP_VEHSPDDISP = 559947807, // ((VEHICLE_BASICINFO_START + 0X0002) | VehiclePropertyType:FLOAT)
    ECM1_ENGSPD = 559947808, // ((VEHICLE_BASICINFO_START + 0X0003) | VehiclePropertyType:FLOAT)
    ECM1_ENGSPDVLDTY = 557850657, // ((VEHICLE_BASICINFO_START + 0X0004) | VehiclePropertyType:INT32)
    DCT3_TGTENGSPDIPVALID = 557850658, // ((VEHICLE_BASICINFO_START + 0X0005) | VehiclePropertyType:INT32)
    DCT3_TGTENGSPDIP = 557850659, // ((VEHICLE_BASICINFO_START + 0X0006) | VehiclePropertyType:INT32)
    ECM11_ENGSPD_FORIP = 559947812, // ((VEHICLE_BASICINFO_START + 0X0007) | VehiclePropertyType:FLOAT)
    HCU_HC16_HCU_TOTDRVREQPOWER = 557850661, // ((VEHICLE_BASICINFO_START + 0X0008) | VehiclePropertyType:INT32)
    HCU_HC7_HCU_RECUPPWR = 557850662, // ((VEHICLE_BASICINFO_START + 0X0009) | VehiclePropertyType:INT32)
    BMS65_BMS_BATTSOC_P0 = 559947815, // ((VEHICLE_BASICINFO_START + 0X000A) | VehiclePropertyType:FLOAT)
    ECM2_FUELCONSUMPTION = 557850664, // ((VEHICLE_BASICINFO_START + 0X000B) | VehiclePropertyType:INT32)
    IP2_IP_FUELLVLINFO = 557850665, // ((VEHICLE_BASICINFO_START + 0X000C) | VehiclePropertyType:INT32)
    IP3_IP_REMAINDISTANCE = 557850666, // ((VEHICLE_BASICINFO_START + 0X000D) | VehiclePropertyType:INT32)
    DCT_FD4_DRVMODDISPSTS = 557850667, // ((VEHICLE_BASICINFO_START + 0X000E) | VehiclePropertyType:INT32)
    DCT_FD4_DRVGEARDISPSTS = 557850668, // ((VEHICLE_BASICINFO_START + 0X000F) | VehiclePropertyType:INT32)
    DCT_FD4_DCT_SHFTRECOM_M = 557850669, // ((VEHICLE_BASICINFO_START + 0X0010) | VehiclePropertyType:INT32)
    HUT34_TIMEMOD = 557850670, // ((VEHICLE_BASICINFO_START + 0X0011) | VehiclePropertyType:INT32)
    HUT34_TIMEINDCN = 557850671, // ((VEHICLE_BASICINFO_START + 0X0012) | VehiclePropertyType:INT32)
    HUT34_TIME_HOUR = 557850672, // ((VEHICLE_BASICINFO_START + 0X0013) | VehiclePropertyType:INT32)
    HUT34_TIME_MINUTES = 557850673, // ((VEHICLE_BASICINFO_START + 0X0014) | VehiclePropertyType:INT32)
    HUT34_TIME_SECOND = 557850674, // ((VEHICLE_BASICINFO_START + 0X0015) | VehiclePropertyType:INT32)
    HUT34_TIME_YEAR_LEFT = 557850675, // ((VEHICLE_BASICINFO_START + 0X0016) | VehiclePropertyType:INT32)
    HUT34_TIME_YEAR_RIGHT = 557850676, // ((VEHICLE_BASICINFO_START + 0X0017) | VehiclePropertyType:INT32)
    HUT34_TIME_MONTH = 557850677, // ((VEHICLE_BASICINFO_START + 0X0018) | VehiclePropertyType:INT32)
    HUT34_TIME_DAY = 557850678, // ((VEHICLE_BASICINFO_START + 0X0019) | VehiclePropertyType:INT32)
    IFC_FD3_TSRSTS = 557850679, // ((VEHICLE_BASICINFO_START + 0X001A) | VehiclePropertyType:INT32)
    IFC_FD3_TSRSPDLIMSTS = 557850680, // ((VEHICLE_BASICINFO_START + 0X001B) | VehiclePropertyType:INT32)
    IFC_FD3_TSRSPDLIMUNIT = 557850681, // ((VEHICLE_BASICINFO_START + 0X001C) | VehiclePropertyType:INT32)
    IFC_FD3_TSRSPDLIMWARN = 557850682, // ((VEHICLE_BASICINFO_START + 0X001D) | VehiclePropertyType:INT32)
    IFC_FD3_TSRTRFCSIGNVALCFDC = 557850683, // ((VEHICLE_BASICINFO_START + 0X001E) | VehiclePropertyType:INT32)
    BMS35_BMS_BATTSOC = 559947836, // ((VEHICLE_BASICINFO_START + 0X001F) | VehiclePropertyType:FLOAT)
    HCU_HC7_HCU_EVCONTNSDISTANCE = 557850685, // ((VEHICLE_BASICINFO_START + 0X0020) | VehiclePropertyType:INT32)
    HCU_HC7_HCU_EVCONTNSDISTANCEVLD = 557850686, // ((VEHICLE_BASICINFO_START + 0X0021) | VehiclePropertyType:INT32)
    HCU_HP5_DRVGEARDISP = 557850687, // ((VEHICLE_BASICINFO_START + 0X0022) | VehiclePropertyType:INT32)
    EEM1_STAT_PWRSV_LVL = 557850688, // ((VEHICLE_BASICINFO_START + 0X0023) | VehiclePropertyType:INT32)
    EEM1_DYN_PWRSV_LVL = 557850689, // ((VEHICLE_BASICINFO_START + 0X0024) | VehiclePropertyType:INT32)
    EEM1_LVPWRSYSERR = 557850690, // ((VEHICLE_BASICINFO_START + 0X0025) | VehiclePropertyType:INT32)
    VEHICLE_BASICINFO_END = 553656860, // (VEHICLE_BASICINFO_START + 0X01FF)
    VEHICLE_CARLAP_START = 553656861, // (VEHICLE_BASICINFO_END + 0X0001)
    IP3_IP_INSTFUELCONSUNIT = 557851166, // ((VEHICLE_CARLAP_START + 0X0001) | VehiclePropertyType:INT32)
    IP3_IP_INSTFUELCONS = 559948319, // ((VEHICLE_CARLAP_START + 0X0002) | VehiclePropertyType:FLOAT)
    IP2_IP_VEHTOTDISTANCE = 559948320, // ((VEHICLE_CARLAP_START + 0X0003) | VehiclePropertyType:FLOAT)
    IP2_IP_VEHTOTDISTANCEVALID = 557851169, // ((VEHICLE_CARLAP_START + 0X0004) | VehiclePropertyType:INT32)
    IP3_IP_AVGFUELCONS = 559948322, // ((VEHICLE_CARLAP_START + 0X0005) | VehiclePropertyType:FLOAT)
    TPMS1_FLTIREPRESSINDSTS = 557851171, // ((VEHICLE_CARLAP_START + 0X0006) | VehiclePropertyType:INT32)
    TPMS1_FRTIREPRESSINDSTS = 557851172, // ((VEHICLE_CARLAP_START + 0X0007) | VehiclePropertyType:INT32)
    TPMS1_RLTIREPRESSINDSTS = 557851173, // ((VEHICLE_CARLAP_START + 0X0008) | VehiclePropertyType:INT32)
    TPMS1_RRTIREPRESSINDSTS = 557851174, // ((VEHICLE_CARLAP_START + 0X0009) | VehiclePropertyType:INT32)
    TPMS2_FLTIREPRESS = 559948327, // ((VEHICLE_CARLAP_START + 0X000A) | VehiclePropertyType:FLOAT)
    TPMS2_FRTIREPRESS = 559948328, // ((VEHICLE_CARLAP_START + 0X000B) | VehiclePropertyType:FLOAT)
    TPMS2_RLTIREPRESS = 559948329, // ((VEHICLE_CARLAP_START + 0X000C) | VehiclePropertyType:FLOAT)
    TPMS2_RRTIREPRESS = 559948330, // ((VEHICLE_CARLAP_START + 0X000D) | VehiclePropertyType:FLOAT)
    TPMS1_FLTIRETEMPSTS = 557851179, // ((VEHICLE_CARLAP_START + 0X000E) | VehiclePropertyType:INT32)
    TPMS1_FRTIRETEMPSTS = 557851180, // ((VEHICLE_CARLAP_START + 0X000F) | VehiclePropertyType:INT32)
    TPMS1_RLTIRETEMPSTS = 557851181, // ((VEHICLE_CARLAP_START + 0X0010) | VehiclePropertyType:INT32)
    TPMS1_RRTIRETEMPSTS = 557851182, // ((VEHICLE_CARLAP_START + 0X0011) | VehiclePropertyType:INT32)
    TPMS2_FLTIRETEMP = 557851183, // ((VEHICLE_CARLAP_START + 0X0012) | VehiclePropertyType:INT32)
    TPMS2_FRTIRETEMP = 557851184, // ((VEHICLE_CARLAP_START + 0X0013) | VehiclePropertyType:INT32)
    TPMS2_RLTIRETEMP = 557851185, // ((VEHICLE_CARLAP_START + 0X0014) | VehiclePropertyType:INT32)
    TPMS2_RRTIRETEMP = 557851186, // ((VEHICLE_CARLAP_START + 0X0015) | VehiclePropertyType:INT32)
    TPMS1_TIREPRESSSYSFAILRINDCN = 557851187, // ((VEHICLE_CARLAP_START + 0X0016) | VehiclePropertyType:INT32)
    ECM3_ENGCOOLANTTEMP = 559948340, // ((VEHICLE_CARLAP_START + 0X0017) | VehiclePropertyType:FLOAT)
    BMS4_BMS_PACKCURR = 559948341, // ((VEHICLE_CARLAP_START + 0X0018) | VehiclePropertyType:FLOAT)
    BMS4_BMS_PACKVOLT = 557851190, // ((VEHICLE_CARLAP_START + 0X0019) | VehiclePropertyType:INT32)
    P2M3_P2M_ACTSPD = 557851191, // ((VEHICLE_CARLAP_START + 0X001A) | VehiclePropertyType:INT32)
    P4M5_P4M_ACTROTSPD = 557851192, // ((VEHICLE_CARLAP_START + 0X001B) | VehiclePropertyType:INT32)
    VEHICLE_CARLAP_END = 553657372, // (VEHICLE_CARLAP_START + 0X01FF)
    VEHICLE_CHARGE_START = 553657373, // (VEHICLE_CARLAP_END + 0X0001)
    GW_FD1_SOC = 557851678, // ((VEHICLE_CHARGE_START + 0X0001) | VehiclePropertyType:INT32)
    OBC1_OBC_MODSTS = 557851679, // ((VEHICLE_CHARGE_START + 0X0002) | VehiclePropertyType:INT32)
    HCU_HC6_HCU_CHARGSTS = 557851680, // ((VEHICLE_CHARGE_START + 0X0003) | VehiclePropertyType:INT32)
    BMS22_BMS_CHRGDURATIONTIME = 557851681, // ((VEHICLE_CHARGE_START + 0X0004) | VehiclePropertyType:INT32)
    VEHICLE_CHARGE_END = 553657884, // (VEHICLE_CHARGE_START + 0X01FF)
    VEHICLE_DMS_START = 553657885, // (VEHICLE_CHARGE_END + 0X0001)
    DMS_FD1_DMSIDNRESULT = 557852190, // ((VEHICLE_DMS_START + 0X0001) | VehiclePropertyType:INT32)
    DMS_FD1_DMSPROC = 557852191, // ((VEHICLE_DMS_START + 0X0002) | VehiclePropertyType:INT32)
    DMS_FD1_DMSPROCRESULT = 557852192, // ((VEHICLE_DMS_START + 0X0003) | VehiclePropertyType:INT32)
    DMS_FD1_DMSPROCSTS = 557852193, // ((VEHICLE_DMS_START + 0X0004) | VehiclePropertyType:INT32)
    DCT5_TGS_LEVER = 557852194, // ((VEHICLE_DMS_START + 0X0005) | VehiclePropertyType:INT32)
    HUT30_ENROLLFACEREQ = 557852195, // ((VEHICLE_DMS_START + 0X0006) | VehiclePropertyType:INT32)
    VEHICLE_DMS_END = 553658396, // (VEHICLE_DMS_START + 0X01FF)
    VEHICLE_SEATHV_START = 553658397, // (VEHICLE_DMS_END + 0X0001)
    HUT4_DRVSEATHEATGLVLSET = 557852702, // ((VEHICLE_SEATHV_START + 0X0001) | VehiclePropertyType:INT32)
    HUT4_DRVSEATVENTNLVLSET = 557852703, // ((VEHICLE_SEATHV_START + 0X0002) | VehiclePropertyType:INT32)
    HUT4_PASSSEATHEATGLVLSET = 557852704, // ((VEHICLE_SEATHV_START + 0X0003) | VehiclePropertyType:INT32)
    HUT4_PASSSEATVENTNLVLSET = 557852705, // ((VEHICLE_SEATHV_START + 0X0004) | VehiclePropertyType:INT32)
    HUT4_RLSEATHEATGLVLSET = 557852706, // ((VEHICLE_SEATHV_START + 0X0005) | VehiclePropertyType:INT32)
    HUT4_RRSEATHEATGLVLSET = 557852707, // ((VEHICLE_SEATHV_START + 0X0006) | VehiclePropertyType:INT32)
    VEHICLE_SEATHV_END = 553658908, // (VEHICLE_SEATHV_START + 0X01FF)
    VEHICLE_FRAGRANCE_START = 553658909, // (VEHICLE_SEATHV_END + 0X0001)
    HUT33_ACFGACHANTYP_TC = 557853214, // ((VEHICLE_FRAGRANCE_START + 0X0001) | VehiclePropertyType:INT32)
    AC6_L_FGACHANTYPSTS = 557853215, // ((VEHICLE_FRAGRANCE_START + 0X0002) | VehiclePropertyType:INT32)
    AC7_FG_CHAN1TYP = 557853216, // ((VEHICLE_FRAGRANCE_START + 0X0003) | VehiclePropertyType:INT32)
    AC7_FG_CHAN2TYP = 557853217, // ((VEHICLE_FRAGRANCE_START + 0X0004) | VehiclePropertyType:INT32)
    AC7_FG_CHAN3TYP = 557853218, // ((VEHICLE_FRAGRANCE_START + 0X0005) | VehiclePropertyType:INT32)
    HUT33_ACFGALVLREQ_TC = 557853219, // ((VEHICLE_FRAGRANCE_START + 0X0006) | VehiclePropertyType:INT32)
    AC6_L_FGALVLRESP = 557853220, // ((VEHICLE_FRAGRANCE_START + 0X0007) | VehiclePropertyType:INT32)
    AC4_L_FGACHAN1TIMEOUTALARM = 557853221, // ((VEHICLE_FRAGRANCE_START + 0X0008) | VehiclePropertyType:INT32)
    AC4_L_FGACHAN2TIMEOUTALARM = 557853222, // ((VEHICLE_FRAGRANCE_START + 0X0009) | VehiclePropertyType:INT32)
    AC4_L_FGACHAN3TIMEOUTALARM = 557853223, // ((VEHICLE_FRAGRANCE_START + 0X000A) | VehiclePropertyType:INT32)
    HUT33_ACFGAENAREQ_TC = 557853224, // ((VEHICLE_FRAGRANCE_START + 0X000B) | VehiclePropertyType:INT32)
    VEHICLE_FRAGRANCE_END = 553659420, // (VEHICLE_FRAGRANCE_START + 0X01FF)
    VEHICLE_STEERHEAT_START = 553659421, // (VEHICLE_FRAGRANCE_END + 0X0001)
    HUT15_STEERWHEELHEATSWTREQ = 557853726, // ((VEHICLE_STEERHEAT_START + 0X0001) | VehiclePropertyType:INT32)
    CSA3_STEERWHEELHEATDSTS = 557853727, // ((VEHICLE_STEERHEAT_START + 0X0002) | VehiclePropertyType:INT32)
    VEHICLE_STEERHEAT_END = 553659932, // (VEHICLE_STEERHEAT_START + 0X01FF)
    VEHICLE_AMP_START = 553659933, // (VEHICLE_STEERHEAT_END + 0X0001)
    HUT16_POWERSET = 557854238, // ((VEHICLE_AMP_START + 0X0001) | VehiclePropertyType:INT32)
    HUT2_AMP_MEDIACALLSOURCESET = 557854239, // ((VEHICLE_AMP_START + 0X0002) | VehiclePropertyType:INT32)
    HUT2_AMP_NAVISOURCESET = 557854240, // ((VEHICLE_AMP_START + 0X0003) | VehiclePropertyType:INT32)
    HUT2_AMP_BEEPSOURCESET = 557854241, // ((VEHICLE_AMP_START + 0X0004) | VehiclePropertyType:INT32)
    HUT2_AMP_LRAUDIOBALANCESET = 557854242, // ((VEHICLE_AMP_START + 0X0005) | VehiclePropertyType:INT32)
    HUT2_AMP_AUDIOVOLVSCMODREQ = 557854243, // ((VEHICLE_AMP_START + 0X0006) | VehiclePropertyType:INT32)
    HUT2_AMP_FRAUDIOFADERSET = 557854244, // ((VEHICLE_AMP_START + 0X0007) | VehiclePropertyType:INT32)
    HUT2_AMP_LRDRVSIDESET = 557854245, // ((VEHICLE_AMP_START + 0X0008) | VehiclePropertyType:INT32)
    HUT2_AMP_LOWFRQAUDIOSET = 557854246, // ((VEHICLE_AMP_START + 0X0009) | VehiclePropertyType:INT32)
    HUT2_AMP_MIDFRQAUDIOSET = 557854247, // ((VEHICLE_AMP_START + 0X000A) | VehiclePropertyType:INT32)
    HUT2_AMP_HIGHFRQAUDIOSET = 557854248, // ((VEHICLE_AMP_START + 0X000B) | VehiclePropertyType:INT32)
    HUT2_T_BOX_ECALLSET = 557854249, // ((VEHICLE_AMP_START + 0X000C) | VehiclePropertyType:INT32)
    HUT2_QLISURROUNDSET = 557854250, // ((VEHICLE_AMP_START + 0X000D) | VehiclePropertyType:INT32)
    HUT2_ANCSWTSET = 557854251, // ((VEHICLE_AMP_START + 0X000E) | VehiclePropertyType:INT32)
    HUT3_AMP_HFMVOLSET = 557854252, // ((VEHICLE_AMP_START + 0X000F) | VehiclePropertyType:INT32)
    HUT3_AMP_TTSVOLSET_VR = 557854253, // ((VEHICLE_AMP_START + 0X0010) | VehiclePropertyType:INT32)
    HUT3_AMP_NAVIVOLSET = 557854254, // ((VEHICLE_AMP_START + 0X0011) | VehiclePropertyType:INT32)
    HUT3_AMP_GETSWINFOREQ = 557854255, // ((VEHICLE_AMP_START + 0X0012) | VehiclePropertyType:INT32)
    HUT3_AMP_GETHWINFOREQ = 557854256, // ((VEHICLE_AMP_START + 0X0013) | VehiclePropertyType:INT32)
    HUT3_AMP_GETTEMPINFOREQ = 557854257, // ((VEHICLE_AMP_START + 0X0014) | VehiclePropertyType:INT32)
    AMP1_AMP_TEMPVALUE = 557854258, // ((VEHICLE_AMP_START + 0X0015) | VehiclePropertyType:INT32)
    HUT3_AMP_GETINTVOLTINFOREQ = 559951411, // ((VEHICLE_AMP_START + 0X0016) | VehiclePropertyType:FLOAT)
    HUT3_IESS_MDLSWTREQ = 557854260, // ((VEHICLE_AMP_START + 0X0017) | VehiclePropertyType:INT32)
    HUT3_IESS_MDLINFOREQ = 557854261, // ((VEHICLE_AMP_START + 0X0018) | VehiclePropertyType:INT32)
    HUT3_AMP_BESTLISTEGPOSNREQ = 557854262, // ((VEHICLE_AMP_START + 0X0019) | VehiclePropertyType:INT32)
    HUT3_AMP_CLARI_FIREQ = 557854263, // ((VEHICLE_AMP_START + 0X001A) | VehiclePropertyType:INT32)
    HUT10_IPVOLSET = 557854264, // ((VEHICLE_AMP_START + 0X001B) | VehiclePropertyType:INT32)
    HUT17_AVMSWTREQ = 557854265, // ((VEHICLE_AMP_START + 0X001C) | VehiclePropertyType:INT32)
    HUT17_NAVIMEDIAVOLLVLREQ = 557854266, // ((VEHICLE_AMP_START + 0X001D) | VehiclePropertyType:INT32)
    HUT17_AVMMEDIAVOLLVLREQ = 557854267, // ((VEHICLE_AMP_START + 0X001E) | VehiclePropertyType:INT32)
    HUT17_FPASCHANSWTREQ = 557854268, // ((VEHICLE_AMP_START + 0X001F) | VehiclePropertyType:INT32)
    HUT17_RPASCHANSWTREQ = 557854269, // ((VEHICLE_AMP_START + 0X0020) | VehiclePropertyType:INT32)
    HUT3_AMP_MAINVOLSET = 557854270, // ((VEHICLE_AMP_START + 0X0021) | VehiclePropertyType:INT32)
    HUT2_AMP_MUTESET = 557854271, // ((VEHICLE_AMP_START + 0X0022) | VehiclePropertyType:INT32)
    AMP2_GETHUTINFOSTS = 557854272, // ((VEHICLE_AMP_START + 0X0023) | VehiclePropertyType:INT32)
    AMP2_FRESHNESSVALUE_AMP2 = 557854273, // ((VEHICLE_AMP_START + 0X0024) | VehiclePropertyType:INT32)
    AMP2_AVMSTRESP = 557854274, // ((VEHICLE_AMP_START + 0X0025) | VehiclePropertyType:INT32)
    AMP2_MAC_CHECK_AMP2 = 558902851, // ((VEHICLE_AMP_START + 0X0026) | VehiclePropertyType:INT64)
    VEHICLE_AMP_END = 553660444, // (VEHICLE_AMP_START + 0X01FF)
    VEHICLE_LCM_START = 553660445, // (VEHICLE_AMP_END + 0X0001)
    AP_POWER_SHUTDOWN_REQUEST = 557854750, // ((VEHICLE_LCM_START + 0X0001) | VehiclePropertyType:INT32)
    AP_POWER_MODE_REQUEST = 557920287, // ((VEHICLE_LCM_START + 0X0002) | VehiclePropertyType:INT32_VEC)
    AP_POWER_BCALL_STATUS_REQ = 557854752, // ((VEHICLE_LCM_START + 0X0003) | VehiclePropertyType:INT32)
    AP_POWER_AUDIO_STATUS_REQ = 557920289, // ((VEHICLE_LCM_START + 0X0004) | VehiclePropertyType:INT32_VEC)
    AP_POWER_AM_FREQ_REQUEST = 557854754, // ((VEHICLE_LCM_START + 0X0005) | VehiclePropertyType:INT32)
    VEHICLE_LCM_END = 553660460, // (VEHICLE_LCM_START + 0X000F)
    VEHICLE_PARK_START = 553660461, // (VEHICLE_LCM_END + 0X0001)
    BCM1_TRUNKSTS = 557854766, // ((VEHICLE_PARK_START + 0X0001) | VehiclePropertyType:INT32)
    DDCM1_DRVMIRRFOLD_UNFOLDSTS = 557854767, // ((VEHICLE_PARK_START + 0X0002) | VehiclePropertyType:INT32)
    PDCM1_PASSMIRRFOLD_UNFOLDSTS = 557854768, // ((VEHICLE_PARK_START + 0X0003) | VehiclePropertyType:INT32)
    BCM1_POSNLIGHTREQ = 557854769, // ((VEHICLE_PARK_START + 0X0004) | VehiclePropertyType:INT32)
    HUT_FD1_AUTOPWRONHDLIGHT = 557854770, // ((VEHICLE_PARK_START + 0X0005) | VehiclePropertyType:INT32)
    HUT6_CARMDLDISPCMD = 557854771, // ((VEHICLE_PARK_START + 0X0006) | VehiclePropertyType:INT32)
    HUT6_GUID_OVL_DISPLAY_CMD = 557854772, // ((VEHICLE_PARK_START + 0X0007) | VehiclePropertyType:INT32)
    HUT6_VIEW_SOFTSWITCHCMD = 557854773, // ((VEHICLE_PARK_START + 0X0008) | VehiclePropertyType:INT32)
    HUT6_MODCHGREQ = 557854774, // ((VEHICLE_PARK_START + 0X0009) | VehiclePropertyType:INT32)
    HAP_FD2_MOD_WORKSTS = 557854775, // ((VEHICLE_PARK_START + 0X000A) | VehiclePropertyType:INT32)
    HAP_FD2_MOD_ALARMSTS = 557854776, // ((VEHICLE_PARK_START + 0X000B) | VehiclePropertyType:INT32)
    RPAS1_MEBWORKSTS = 557854777, // ((VEHICLE_PARK_START + 0X000C) | VehiclePropertyType:INT32)
    RPAS1_MEB_ENABLESTS = 557854778, // ((VEHICLE_PARK_START + 0X000D) | VehiclePropertyType:INT32)
    HUT6_CARMDLTRSPRCYSWTCMD = 557854779, // ((VEHICLE_PARK_START + 0X000E) | VehiclePropertyType:INT32)
    HAP_FD2_HAP_HMI_INDEX = 557854780, // ((VEHICLE_PARK_START + 0X000F) | VehiclePropertyType:INT32)
    HAP_FD1_APS_WORKSTS = 557854781, // ((VEHICLE_PARK_START + 0X0010) | VehiclePropertyType:INT32)
    HUT33_SELPRKGFCTNCMD = 557854782, // ((VEHICLE_PARK_START + 0X0011) | VehiclePropertyType:INT32)
    HAP_FD3_FADS_FUNCSTS = 557854783, // ((VEHICLE_PARK_START + 0X0012) | VehiclePropertyType:INT32)
    HAP_FD3_APA_MENUDISPCTRLCMD = 557854784, // ((VEHICLE_PARK_START + 0X0013) | VehiclePropertyType:INT32)
    HUT32_DETVIDEOLOST = 557854785, // ((VEHICLE_PARK_START + 0X0014) | VehiclePropertyType:INT32)
    HAP_FD1_APS_TEXTDISP = 557854786, // ((VEHICLE_PARK_START + 0X0015) | VehiclePropertyType:INT32)
    HAP_FD1_P2P_PAHDELSTS = 557854787, // ((VEHICLE_PARK_START + 0X0016) | VehiclePropertyType:INT32)
    HAP_FD1_P2P_SELFDETERRCODE = 557854788, // ((VEHICLE_PARK_START + 0X0017) | VehiclePropertyType:INT32)
    HAP_FD1_P2P_SELFDETSTS = 557854789, // ((VEHICLE_PARK_START + 0X0018) | VehiclePropertyType:INT32)
    HAP_FD1_P2P_PAH1AVAIL = 557854790, // ((VEHICLE_PARK_START + 0X0019) | VehiclePropertyType:INT32)
    HAP_FD1_P2P_PAH2AVAIL = 557854791, // ((VEHICLE_PARK_START + 0X001A) | VehiclePropertyType:INT32)
    HCU_PT4_CARWASHMODE_STSDISP = 557854792, // ((VEHICLE_PARK_START + 0X001B) | VehiclePropertyType:INT32)
    HUT32_P2P_PRKGDIRSWTREQ = 557854793, // ((VEHICLE_PARK_START + 0X001C) | VehiclePropertyType:INT32)
    HUT6_AUTOVIEWCHGCMD = 557854794, // ((VEHICLE_PARK_START + 0X001D) | VehiclePropertyType:INT32)
    HAP_FD3_MEBWORKSTS = 557854795, // ((VEHICLE_PARK_START + 0X001E) | VehiclePropertyType:INT32)
    HAP_FD3_MEB_ENABLESTS = 557854796, // ((VEHICLE_PARK_START + 0X001F) | VehiclePropertyType:INT32)
    HUT32_PRKGCTRLMODREQVALID = 557854797, // ((VEHICLE_PARK_START + 0X0020) | VehiclePropertyType:INT32)
    HUT32_PRKGCTRLMODREQ = 557854798, // ((VEHICLE_PARK_START + 0X0021) | VehiclePropertyType:INT32)
    HAP_FD1_HAP_PRKGMODCURRSTS = 557854799, // ((VEHICLE_PARK_START + 0X0022) | VehiclePropertyType:INT32)
    HUT6_RADAR_DISPCMD = 557854800, // ((VEHICLE_PARK_START + 0X0023) | VehiclePropertyType:INT32)
    RPAS1_RPAS_RLCORNRSNSRFAILSTS = 557854801, // ((VEHICLE_PARK_START + 0X0024) | VehiclePropertyType:INT32)
    RPAS1_RPAS_RLMIDDLSNSRFAILSTS = 557854802, // ((VEHICLE_PARK_START + 0X0025) | VehiclePropertyType:INT32)
    RPAS1_RPAS_RRMIDDLSNSRFAILSTS = 557854803, // ((VEHICLE_PARK_START + 0X0026) | VehiclePropertyType:INT32)
    RPAS1_RPAS_RRCORNRSNSRFAILSTS = 557854804, // ((VEHICLE_PARK_START + 0X0027) | VehiclePropertyType:INT32)
    HAP_FD3_APS_FRSIDESNSRFAILSTS = 557854805, // ((VEHICLE_PARK_START + 0X0028) | VehiclePropertyType:INT32)
    HAP_FD3_APS_FLSIDESNSRFAILSTS = 557854806, // ((VEHICLE_PARK_START + 0X0029) | VehiclePropertyType:INT32)
    HAP_FD3_APS_RRSIDESNSRFAILSTS = 557854807, // ((VEHICLE_PARK_START + 0X002A) | VehiclePropertyType:INT32)
    HAP_FD3_APS_RLSIDESNSRFAILSTS = 557854808, // ((VEHICLE_PARK_START + 0X002B) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_RLCORNRSNSRFAILSTS = 557854809, // ((VEHICLE_PARK_START + 0X002C) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_RLMIDDLSNSRFAILSTS = 557854810, // ((VEHICLE_PARK_START + 0X002D) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_RRMIDDLSNSRFAILSTS = 557854811, // ((VEHICLE_PARK_START + 0X002E) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_RRCORNRSNSRFAILSTS = 557854812, // ((VEHICLE_PARK_START + 0X002F) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_FLCORNRSNSRFAILSTS = 557854813, // ((VEHICLE_PARK_START + 0X0030) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_FLMIDDLSNSRFAILSTS = 557854814, // ((VEHICLE_PARK_START + 0X0031) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_FRMIDDLSNSRFAILSTS = 557854815, // ((VEHICLE_PARK_START + 0X0032) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_FRCORNRSNSRFAILSTS = 557854816, // ((VEHICLE_PARK_START + 0X0033) | VehiclePropertyType:INT32)
    ABS1_FLWHEELSPDVLD = 557854817, // ((VEHICLE_PARK_START + 0X0034) | VehiclePropertyType:INT32)
    ABS1_FLWHEELSPD = 559951970, // ((VEHICLE_PARK_START + 0X0035) | VehiclePropertyType:FLOAT)
    ABS1_FRWHEELSPDVLD = 557854819, // ((VEHICLE_PARK_START + 0X0036) | VehiclePropertyType:INT32)
    ABS1_FRWHEELSPD = 559951972, // ((VEHICLE_PARK_START + 0X0037) | VehiclePropertyType:FLOAT)
    ABS2_RLWHEELSPDVLD = 557854821, // ((VEHICLE_PARK_START + 0X0038) | VehiclePropertyType:INT32)
    ABS2_RLWHEELSPD = 559951974, // ((VEHICLE_PARK_START + 0X0039) | VehiclePropertyType:FLOAT)
    ABS2_RRWHEELSPDVLD = 557854823, // ((VEHICLE_PARK_START + 0X003A) | VehiclePropertyType:INT32)
    ABS2_RRWHEELSPD = 559951976, // ((VEHICLE_PARK_START + 0X003B) | VehiclePropertyType:FLOAT)
    BCM3_LASHDLMPSTS = 557854825, // ((VEHICLE_PARK_START + 0X003C) | VehiclePropertyType:INT32)
    HCM_L1_LLASHDLMPSTS = 557854826, // ((VEHICLE_PARK_START + 0X003D) | VehiclePropertyType:INT32)
    HCM_L1_LLASHDLMPFAILSTS = 557854827, // ((VEHICLE_PARK_START + 0X003E) | VehiclePropertyType:INT32)
    HCM_R1_RLASHDLMPSTS = 557854828, // ((VEHICLE_PARK_START + 0X003F) | VehiclePropertyType:INT32)
    HCM_R1_RLASHDLMPFAILSTS = 557854829, // ((VEHICLE_PARK_START + 0X0040) | VehiclePropertyType:INT32)
    PLG_1_PLG_STS = 557854830, // ((VEHICLE_PARK_START + 0X0041) | VehiclePropertyType:INT32)
    HAP_FD3_RADS_FUNCSTS = 557854831, // ((VEHICLE_PARK_START + 0X0042) | VehiclePropertyType:INT32)
    HAP_FD2_FRNTCAMINPFAILSTS = 557854832, // ((VEHICLE_PARK_START + 0X0043) | VehiclePropertyType:INT32)
    HAP_FD2_REARCAMINPFAILSTS = 557854833, // ((VEHICLE_PARK_START + 0X0044) | VehiclePropertyType:INT32)
    HAP_FD2_LECAMINPFAILSTS = 557854834, // ((VEHICLE_PARK_START + 0X0045) | VehiclePropertyType:INT32)
    HAP_FD2_RICAMINPFAILSTS = 557854835, // ((VEHICLE_PARK_START + 0X0046) | VehiclePropertyType:INT32)
    HAP_FD2_OVLOPFAILSTS = 557854836, // ((VEHICLE_PARK_START + 0X0047) | VehiclePropertyType:INT32)
    HAP_FD1_HAP_SWTDISPCTRLCMD = 557854837, // ((VEHICLE_PARK_START + 0X0048) | VehiclePropertyType:INT32)
    FPAS1_FPAS_FLCORNRSNSRFLTSTS = 557854838, // ((VEHICLE_PARK_START + 0X0049) | VehiclePropertyType:INT32)
    FPAS1_FPAS_FLMIDDLSNSRFLTSTS = 557854839, // ((VEHICLE_PARK_START + 0X004A) | VehiclePropertyType:INT32)
    FPAS1_FPAS_FRMIDDLSNSRFLTSTS = 557854840, // ((VEHICLE_PARK_START + 0X004B) | VehiclePropertyType:INT32)
    FPAS1_FPAS_FRCORNRSNSRFLTSTS = 557854841, // ((VEHICLE_PARK_START + 0X004C) | VehiclePropertyType:INT32)
    BCM8_PASWORKCMD = 557854842, // ((VEHICLE_PARK_START + 0X004D) | VehiclePropertyType:INT32)
    HAP_FD2_AVM_CURRSTS = 557854843, // ((VEHICLE_PARK_START + 0X004E) | VehiclePropertyType:INT32)
    HAP_FD2_HAP_DISPCMD = 557854844, // ((VEHICLE_PARK_START + 0X004F) | VehiclePropertyType:INT32)
    HUT6_SGL_VIEW_SEL = 557854845, // ((VEHICLE_PARK_START + 0X0050) | VehiclePropertyType:INT32)
    HAP_FD3_P2P_FUNCSTS = 557854846, // ((VEHICLE_PARK_START + 0X0051) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRRCORNRAR1 = 557854847, // ((VEHICLE_PARK_START + 0X0052) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRRCORNRAR2 = 557854848, // ((VEHICLE_PARK_START + 0X0053) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRRMIDLAR1 = 557854849, // ((VEHICLE_PARK_START + 0X0054) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRRMIDLAR2 = 557854850, // ((VEHICLE_PARK_START + 0X0055) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRLMIDLAR1 = 557854851, // ((VEHICLE_PARK_START + 0X0056) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRLMIDLAR2 = 557854852, // ((VEHICLE_PARK_START + 0X0057) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRLCORNRAR1 = 557854853, // ((VEHICLE_PARK_START + 0X0058) | VehiclePropertyType:INT32)
    RPAS1_RPAS_OBJRLCORNRAR2 = 557854854, // ((VEHICLE_PARK_START + 0X0059) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJRISIDEAR1 = 557854855, // ((VEHICLE_PARK_START + 0X005A) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJRISIDEAR2 = 557854856, // ((VEHICLE_PARK_START + 0X005B) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJRISIDEAR3 = 557854857, // ((VEHICLE_PARK_START + 0X005C) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJRISIDEAR4 = 557854858, // ((VEHICLE_PARK_START + 0X005D) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJRISIDEAR5 = 557854859, // ((VEHICLE_PARK_START + 0X005E) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJLESIDEAR1 = 557854860, // ((VEHICLE_PARK_START + 0X005F) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJLESIDEAR2 = 557854861, // ((VEHICLE_PARK_START + 0X0060) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJLESIDEAR3 = 557854862, // ((VEHICLE_PARK_START + 0X0061) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJLESIDEAR4 = 557854863, // ((VEHICLE_PARK_START + 0X0062) | VehiclePropertyType:INT32)
    HAP_FD3_APS_OBJLESIDEAR5 = 557854864, // ((VEHICLE_PARK_START + 0X0063) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRRCORNRAR1 = 557854865, // ((VEHICLE_PARK_START + 0X0064) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRRCORNRAR2 = 557854866, // ((VEHICLE_PARK_START + 0X0065) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRRMIDLAR1 = 557854867, // ((VEHICLE_PARK_START + 0X0066) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRRMIDLAR2 = 557854868, // ((VEHICLE_PARK_START + 0X0067) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRLMIDLAR1 = 557854869, // ((VEHICLE_PARK_START + 0X0068) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRLMIDLAR2 = 557854870, // ((VEHICLE_PARK_START + 0X0069) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRLCORNRAR1 = 557854871, // ((VEHICLE_PARK_START + 0X006A) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_OBJRLCORNRAR2 = 557854872, // ((VEHICLE_PARK_START + 0X006B) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFLCORNRAR1 = 557854873, // ((VEHICLE_PARK_START + 0X006C) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFLCORNRAR2 = 557854874, // ((VEHICLE_PARK_START + 0X006D) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFLMIDLAR1 = 557854875, // ((VEHICLE_PARK_START + 0X006E) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFLMIDLAR2 = 557854876, // ((VEHICLE_PARK_START + 0X006F) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFRMIDLAR1 = 557854877, // ((VEHICLE_PARK_START + 0X0070) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFRMIDLAR2 = 557854878, // ((VEHICLE_PARK_START + 0X0071) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFRCORNRAR1 = 557854879, // ((VEHICLE_PARK_START + 0X0072) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_OBJFRCORNRAR2 = 557854880, // ((VEHICLE_PARK_START + 0X0073) | VehiclePropertyType:INT32)
    VEHICLE_PARK_END = 553660972, // (VEHICLE_PARK_START + 0X01FF)
    VEHICLE_ENERGY_START = 553660973, // (VEHICLE_PARK_END + 0X0001)
    HCU_HC3_HCU_ENRGFLOWIND_DHT = 557855278, // ((VEHICLE_ENERGY_START + 0X0001) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_ENRGFLOWIND_P2 = 557855279, // ((VEHICLE_ENERGY_START + 0X0002) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_ENRGFLOWIND = 557855280, // ((VEHICLE_ENERGY_START + 0X0003) | VehiclePropertyType:INT32)
    HCU_HC7_HCU_EGYUSED = 559952433, // ((VEHICLE_ENERGY_START + 0X0004) | VehiclePropertyType:FLOAT)
    ESP_FD2_VEHODOINFOSTS = 557855282, // ((VEHICLE_ENERGY_START + 0X0005) | VehiclePropertyType:INT32)
    ESP_FD2_VEHODOINFO = 557855283, // ((VEHICLE_ENERGY_START + 0X0006) | VehiclePropertyType:INT32)
    OBC2_OBC_CONNECTSTS = 557855284, // ((VEHICLE_ENERGY_START + 0X0007) | VehiclePropertyType:INT32)
    HUT25_HUT_CHRGNMODE = 557855285, // ((VEHICLE_ENERGY_START + 0X0008) | VehiclePropertyType:INT32)
    HUT25_HUT_BATTSOCLIM = 557855286, // ((VEHICLE_ENERGY_START + 0X0009) | VehiclePropertyType:INT32)
    HUT25_HUT_BATTKEEPTEMP = 557855288, // ((VEHICLE_ENERGY_START + 0X000B) | VehiclePropertyType:INT32)
    HUT25_HUT_INTELBATTTEMPMAGSET = 557855289, // ((VEHICLE_ENERGY_START + 0X000C) | VehiclePropertyType:INT32)
    BMS8_BMS_BATTHEATRUNAWAY = 557855290, // ((VEHICLE_ENERGY_START + 0X000D) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_DRVMODDISP = 557855291, // ((VEHICLE_ENERGY_START + 0X000E) | VehiclePropertyType:INT32)
    HCU_PT5_HCU_DRVMOD = 557855292, // ((VEHICLE_ENERGY_START + 0X000F) | VehiclePropertyType:INT32)
    HUT10_HUT_EGYRECVRYSET = 557855293, // ((VEHICLE_ENERGY_START + 0X0010) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_1 = 557855294, // ((VEHICLE_ENERGY_START + 0X0011) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_2 = 557855295, // ((VEHICLE_ENERGY_START + 0X0012) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_3 = 557855296, // ((VEHICLE_ENERGY_START + 0X0013) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_4 = 557855297, // ((VEHICLE_ENERGY_START + 0X0014) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_5 = 557855298, // ((VEHICLE_ENERGY_START + 0X0015) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_6 = 557855299, // ((VEHICLE_ENERGY_START + 0X0016) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_7 = 557855300, // ((VEHICLE_ENERGY_START + 0X0017) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSERR_8 = 557855301, // ((VEHICLE_ENERGY_START + 0X0018) | VehiclePropertyType:INT32)
    GW_FD1_U_BATT = 559952454, // ((VEHICLE_ENERGY_START + 0X0019) | VehiclePropertyType:FLOAT)
    VCU_FD3_VCU_EGYFLOWINDCR = 557855304, // ((VEHICLE_ENERGY_START + 0X001B) | VehiclePropertyType:INT32)
    VCU5_VCU_AVRGEGYCNS = 559952457, // ((VEHICLE_ENERGY_START + 0X001C) | VehiclePropertyType:FLOAT)
    VCU5_VCU_INSTANTEGYCNS = 559952458, // ((VEHICLE_ENERGY_START + 0X001D) | VehiclePropertyType:FLOAT)
    MCU_FD2_MCU_MOTORPOWER = 557855307, // ((VEHICLE_ENERGY_START + 0X001E) | VehiclePropertyType:INT32)
    BMS_FD1_BMS_SOC = 559952460, // ((VEHICLE_ENERGY_START + 0X001F) | VehiclePropertyType:FLOAT)
    VCU_FD4_VCU_RESRDRVRNG = 557855309, // ((VEHICLE_ENERGY_START + 0X0020) | VehiclePropertyType:INT32)
    HUT10_HUT_IMDCHRGSET = 557855310, // ((VEHICLE_ENERGY_START + 0X0021) | VehiclePropertyType:INT32)
    VCU_FD3_VCU_OPDSTS = 557855311, // ((VEHICLE_ENERGY_START + 0X0022) | VehiclePropertyType:INT32)
    HUT10_HUT_ECOACLIMNSET = 557855312, // ((VEHICLE_ENERGY_START + 0X0023) | VehiclePropertyType:INT32)
    HUT10_VSGMUTESWSET = 557855313, // ((VEHICLE_ENERGY_START + 0X0024) | VehiclePropertyType:INT32)
    VSG1_VSG_HUTSOUNDRESP = 557855314, // ((VEHICLE_ENERGY_START + 0X0025) | VehiclePropertyType:INT32)
    HUT6_HUT_VCUBATTKEEPTEMP = 557855315, // ((VEHICLE_ENERGY_START + 0X0026) | VehiclePropertyType:INT32)
    HUT17_HUT_V2VSTART = 557855316, // ((VEHICLE_ENERGY_START + 0X0027) | VehiclePropertyType:INT32)
    HUT10_HUT_ECODRVMODREQ = 557855319, // ((VEHICLE_ENERGY_START + 0X002A) | VehiclePropertyType:INT32)
    VCU_FD5_VCU_TOTALSCORE = 559952472, // ((VEHICLE_ENERGY_START + 0X002B) | VehiclePropertyType:FLOAT)
    VCU_FD5_VCU_ACCELTIMES = 557855321, // ((VEHICLE_ENERGY_START + 0X002C) | VehiclePropertyType:INT32)
    VCU_FD5_VCU_DECELTIMES = 557855322, // ((VEHICLE_ENERGY_START + 0X002D) | VehiclePropertyType:INT32)
    VEHICLE_ENERGY_END = 553661484, // (VEHICLE_ENERGY_START + 0X01FF)
    VEHICLE_INTELCTRL_START = 553661485, // (VEHICLE_ENERGY_END + 0X0001)
    AC1_INCARTEMP = 559952942, // ((VEHICLE_INTELCTRL_START + 0X0001) | VehiclePropertyType:FLOAT)
    HUT33_DRVWINSWTREQ_VR = 557855791, // ((VEHICLE_INTELCTRL_START + 0X0002) | VehiclePropertyType:INT32)
    HUT33_PASSWINSWTREQ_VR = 557855792, // ((VEHICLE_INTELCTRL_START + 0X0003) | VehiclePropertyType:INT32)
    HUT33_RLWINSWTREQ_VR = 557855793, // ((VEHICLE_INTELCTRL_START + 0X0004) | VehiclePropertyType:INT32)
    HUT33_RRWINSWTREQ_VR = 557855794, // ((VEHICLE_INTELCTRL_START + 0X0005) | VehiclePropertyType:INT32)
    AMP2_MEDIACALLSOURCESTATE = 557855795, // ((VEHICLE_INTELCTRL_START + 0X0006) | VehiclePropertyType:INT32)
    AMP3_MAINVOLUMESETSTATE = 557855796, // ((VEHICLE_INTELCTRL_START + 0X0007) | VehiclePropertyType:INT32)
    HUT13_ORVMFOLDSWT_HUT = 557855797, // ((VEHICLE_INTELCTRL_START + 0X0008) | VehiclePropertyType:INT32)
    AMP3_VR_TTSVOLSETSTS = 557855798, // ((VEHICLE_INTELCTRL_START + 0X0009) | VehiclePropertyType:INT32)
    VEHICLE_INTELCTRL_END = 553661996, // (VEHICLE_INTELCTRL_START + 0X01FF)
    VEHICLE_STEREO_START = 553661997, // (VEHICLE_INTELCTRL_END + 0X0001)
    AEB_FD2_AEB_JA_WARN = 557856302, // ((VEHICLE_STEREO_START + 0X0001) | VehiclePropertyType:INT32)
    VEHICLE_STEREO_END = 553662508, // (VEHICLE_STEREO_START + 0X01FF)
    VEHICLE_RHTADAS_START = 553662509, // (VEHICLE_STEREO_END + 0X0001)
    R_PBOX1_RTURNLMPSTS_R_PBOX = 557856814, // ((VEHICLE_RHTADAS_START + 0X0001) | VehiclePropertyType:INT32)
    RSDS_FD2_BSD_LCA_RIGHT_TTC = 559953967, // ((VEHICLE_RHTADAS_START + 0X0002) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RCTA_B_TTC = 559953968, // ((VEHICLE_RHTADAS_START + 0X0003) | VehiclePropertyType:FLOAT)
    RSDS_FD2_BSD_LCA_LEFT_TTC = 559953969, // ((VEHICLE_RHTADAS_START + 0X0004) | VehiclePropertyType:FLOAT)
    RSDS_FD2_RCW_TTC = 559953970, // ((VEHICLE_RHTADAS_START + 0X0005) | VehiclePropertyType:FLOAT)
    BCM1_BRKPEDALSTS_BCM = 557856819, // ((VEHICLE_RHTADAS_START + 0X0006) | VehiclePropertyType:INT32)
    ECM9_ACCELPEDLPOSNDIAGC = 559953972, // ((VEHICLE_RHTADAS_START + 0X0007) | VehiclePropertyType:FLOAT)
    ECM2_ACCELPEDALPOSNVLDTY = 557856821, // ((VEHICLE_RHTADAS_START + 0X0008) | VehiclePropertyType:INT32)
    DCT_FD4_DRVGEARDISP = 557856822, // ((VEHICLE_RHTADAS_START + 0X0009) | VehiclePropertyType:INT32)
    PEPS2_SYSPOWERMOD = 557856823, // ((VEHICLE_RHTADAS_START + 0X000A) | VehiclePropertyType:INT32)
    PEPS2_SYSPOWERMODVLD = 557856824, // ((VEHICLE_RHTADAS_START + 0X000B) | VehiclePropertyType:INT32)
    VEHICLE_RHTADAS_END = 553663020, // (VEHICLE_RHTADAS_START + 0X01FF)
    VEHICLE_RESETWORLD_START = 553663021, // (VEHICLE_RHTADAS_END + 0X0001)
    ACC_FD2_ACC_RLVTGTFUSION = 557857326, // ((VEHICLE_RESETWORLD_START + 0X0001) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETGTFUSION = 557857327, // ((VEHICLE_RESETWORLD_START + 0X0002) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RITGTFUSION = 557857328, // ((VEHICLE_RESETWORLD_START + 0X0003) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_POTENTIALTGTFUSION = 557857329, // ((VEHICLE_RESETWORLD_START + 0X0004) | VehiclePropertyType:INT32)
    VEHICLE_RESETWORLD_END = 553663532, // (VEHICLE_RESETWORLD_START + 0X01FF)
    VEHICLE_FACEREGIS_START = 553663533, // (VEHICLE_RESETWORLD_END + 0X0001)
    HUT30_ENROLLUSERIDRESULT = 557857838, // ((VEHICLE_FACEREGIS_START + 0X0001) | VehiclePropertyType:INT32)
    HUT30_DELETEUSERIDFACEDATA = 557857839, // ((VEHICLE_FACEREGIS_START + 0X0002) | VehiclePropertyType:INT32)
    HUT30_DELETEUSERIDVOICEDATA = 557857840, // ((VEHICLE_FACEREGIS_START + 0X0003) | VehiclePropertyType:INT32)
    HUT30_FACEIDNSET = 557857841, // ((VEHICLE_FACEREGIS_START + 0X0004) | VehiclePropertyType:INT32)
    DMS_FD1_DISTRCTNLVL = 557857842, // ((VEHICLE_FACEREGIS_START + 0X0005) | VehiclePropertyType:INT32)
    DMS_FD1_DISTRCTNLVLQLTY = 557857843, // ((VEHICLE_FACEREGIS_START + 0X0006) | VehiclePropertyType:INT32)
    DMS_FD1_DROWSNSLVL = 557857844, // ((VEHICLE_FACEREGIS_START + 0X0007) | VehiclePropertyType:INT32)
    DMS_FD1_DROWSNSLVLQLTY = 557857845, // ((VEHICLE_FACEREGIS_START + 0X0008) | VehiclePropertyType:INT32)
    HUT30_EXPRSNSETUP = 557857846, // ((VEHICLE_FACEREGIS_START + 0X0009) | VehiclePropertyType:INT32)
    DMS_FD1_EXPRSNSTS = 557857847, // ((VEHICLE_FACEREGIS_START + 0X000A) | VehiclePropertyType:INT32)
    DMS_FD1_EXPRSNSTSQLTY = 557857848, // ((VEHICLE_FACEREGIS_START + 0X000B) | VehiclePropertyType:INT32)
    HUT25_DRVAGESETUP = 557857849, // ((VEHICLE_FACEREGIS_START + 0X000C) | VehiclePropertyType:INT32)
    DMS_FD1_DRVAGESTS = 557857850, // ((VEHICLE_FACEREGIS_START + 0X000D) | VehiclePropertyType:INT32)
    HUT25_DRVGENDERSETUP = 557857851, // ((VEHICLE_FACEREGIS_START + 0X000E) | VehiclePropertyType:INT32)
    DMS_FD1_GENDERSTS = 557857852, // ((VEHICLE_FACEREGIS_START + 0X000F) | VehiclePropertyType:INT32)
    DMS_FD1_DRVGAZEAREASTS = 557857853, // ((VEHICLE_FACEREGIS_START + 0X0010) | VehiclePropertyType:INT32)
    DMS_FD1_FRNTOCCPTEXPRSNSTS = 557857854, // ((VEHICLE_FACEREGIS_START + 0X0011) | VehiclePropertyType:INT32)
    DMS_FD1_RLOCCPTEXPRSNSTS = 557857855, // ((VEHICLE_FACEREGIS_START + 0X0012) | VehiclePropertyType:INT32)
    DMS_FD1_RMOCCPTEXPRSNSTS = 557857856, // ((VEHICLE_FACEREGIS_START + 0X0013) | VehiclePropertyType:INT32)
    DMS_FD1_RROCCPTEXPRSNSTS = 557857857, // ((VEHICLE_FACEREGIS_START + 0X0014) | VehiclePropertyType:INT32)
    DMS_FD1_FRNTOCCPTAGESTS = 557857858, // ((VEHICLE_FACEREGIS_START + 0X0015) | VehiclePropertyType:INT32)
    DMS_FD1_RLOCCPTAGESTS = 557857859, // ((VEHICLE_FACEREGIS_START + 0X0016) | VehiclePropertyType:INT32)
    DMS_FD1_RMOCCPTAGESTS = 557857860, // ((VEHICLE_FACEREGIS_START + 0X0017) | VehiclePropertyType:INT32)
    DMS_FD1_RROCCPTAGESTS = 557857861, // ((VEHICLE_FACEREGIS_START + 0X0018) | VehiclePropertyType:INT32)
    DMS_FD1_FRNTOCCPTGENDERSTS = 557857862, // ((VEHICLE_FACEREGIS_START + 0X0019) | VehiclePropertyType:INT32)
    DMS_FD1_RLOCCPTGENDERSTS = 557857863, // ((VEHICLE_FACEREGIS_START + 0X001A) | VehiclePropertyType:INT32)
    DMS_FD1_RMOCCPTGENDERSTS = 557857864, // ((VEHICLE_FACEREGIS_START + 0X001B) | VehiclePropertyType:INT32)
    DMS_FD1_RROCCPTGENDERSTS = 557857865, // ((VEHICLE_FACEREGIS_START + 0X001C) | VehiclePropertyType:INT32)
    HUT30_IDNRESULTREQV3_5_HUT = 557857866, // ((VEHICLE_FACEREGIS_START + 0X001D) | VehiclePropertyType:INT32)
    VEHICLE_FACEREGIS_END = 553664044, // (VEHICLE_FACEREGIS_START + 0X01FF)
    VEHICLE_ARHUD_START = 553664045, // (VEHICLE_FACEREGIS_END + 0X0001)
    HUT30_HUD_SWTREQ = 557858350, // ((VEHICLE_ARHUD_START + 0X0001) | VehiclePropertyType:INT32)
    HUT30_HUD_HEIGHTLVLSWT = 557858351, // ((VEHICLE_ARHUD_START + 0X0002) | VehiclePropertyType:INT32)
    HUT30_HUD_BRIGHTNESSLVLSWT = 557858352, // ((VEHICLE_ARHUD_START + 0X0003) | VehiclePropertyType:INT32)
    HUT25_HUD_ROTATEDIRCFG_VR = 557858353, // ((VEHICLE_ARHUD_START + 0X0004) | VehiclePropertyType:INT32)
    HUT25_HUD_ROTATEANGLECFG_VR = 557858354, // ((VEHICLE_ARHUD_START + 0X0005) | VehiclePropertyType:INT32)
    HUT25_HUD_DISPLAYMODECFG_VR = 557858355, // ((VEHICLE_ARHUD_START + 0X0006) | VehiclePropertyType:INT32)
    HUT33_ADASDISPSWT_VR = 557858356, // ((VEHICLE_ARHUD_START + 0X0007) | VehiclePropertyType:INT32)
    HUT33_NAVIDISPSWT = 557858357, // ((VEHICLE_ARHUD_START + 0X0008) | VehiclePropertyType:INT32)
    HUT33_NAVIDISPSWT_VR = 557858358, // ((VEHICLE_ARHUD_START + 0X0009) | VehiclePropertyType:INT32)
    HUT25_BTPHONEDISPSWT_VR = 557858359, // ((VEHICLE_ARHUD_START + 0X000A) | VehiclePropertyType:INT32)
    HUT16_HUT_CALLSTS_DIALING = 557858360, // ((VEHICLE_ARHUD_START + 0X000B) | VehiclePropertyType:INT32)
    HUT16_HUT_CALLSTS_INCOMINGCALL = 557858361, // ((VEHICLE_ARHUD_START + 0X000C) | VehiclePropertyType:INT32)
    HUT16_HUT_CALLSTS_ONGOING = 557858362, // ((VEHICLE_ARHUD_START + 0X000D) | VehiclePropertyType:INT32)
    HUT16_HUT_CALLTIME = 557858363, // ((VEHICLE_ARHUD_START + 0X000E) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_FAULT = 557858364, // ((VEHICLE_ARHUD_START + 0X000F) | VehiclePropertyType:INT32)
    AR_HUD1_FRESHNESSVALUE_HUD1 = 557858365, // ((VEHICLE_ARHUD_START + 0X0010) | VehiclePropertyType:INT32)
    AR_HUD1_MAC_CHECK_HUD1 = 558906942, // ((VEHICLE_ARHUD_START + 0X0011) | VehiclePropertyType:INT64)
    HUT31_HUT_PHONENEXTROAD = 561004095, // ((VEHICLE_ARHUD_START + 0X0012) | VehiclePropertyType:BYTES)
    HUT12_WEATHERCDN = 557858368, // ((VEHICLE_ARHUD_START + 0X0013) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYENA = 557858369, // ((VEHICLE_ARHUD_START + 0X0014) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYSIZE = 557858370, // ((VEHICLE_ARHUD_START + 0X0015) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER1 = 557858371, // ((VEHICLE_ARHUD_START + 0X0016) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON1 = 557858372, // ((VEHICLE_ARHUD_START + 0X0017) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER2 = 557858373, // ((VEHICLE_ARHUD_START + 0X0018) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON2 = 557858374, // ((VEHICLE_ARHUD_START + 0X0019) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER3 = 557858375, // ((VEHICLE_ARHUD_START + 0X001A) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON3 = 557858376, // ((VEHICLE_ARHUD_START + 0X001B) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON4 = 557858377, // ((VEHICLE_ARHUD_START + 0X001C) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER4 = 557858378, // ((VEHICLE_ARHUD_START + 0X001D) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER5 = 557858379, // ((VEHICLE_ARHUD_START + 0X001E) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON5 = 557858380, // ((VEHICLE_ARHUD_START + 0X001F) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER6 = 557858381, // ((VEHICLE_ARHUD_START + 0X0020) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON6 = 557858382, // ((VEHICLE_ARHUD_START + 0X0021) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER7 = 557858383, // ((VEHICLE_ARHUD_START + 0X0022) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON7 = 557858384, // ((VEHICLE_ARHUD_START + 0X0023) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYNUMBER8 = 557858385, // ((VEHICLE_ARHUD_START + 0X0024) | VehiclePropertyType:INT32)
    HUT_FD1_DRVWAYLANEBACKICON8 = 557858386, // ((VEHICLE_ARHUD_START + 0X0025) | VehiclePropertyType:INT32)
    HUT33_HUD_VIBRATIONCORRN = 557858387, // ((VEHICLE_ARHUD_START + 0X0026) | VehiclePropertyType:INT32)
    HUD1_HUD_UIMOD = 557858388, // ((VEHICLE_ARHUD_START + 0X0027) | VehiclePropertyType:INT32)
    HUD1_HUD_UIMODVLD = 557858389, // ((VEHICLE_ARHUD_START + 0X0028) | VehiclePropertyType:INT32)
    HUD1_HUD_SWTSTS = 557858390, // ((VEHICLE_ARHUD_START + 0X0029) | VehiclePropertyType:INT32)
    HUD1_HUD_BRIGHTNESSLVL = 557858391, // ((VEHICLE_ARHUD_START + 0X002A) | VehiclePropertyType:INT32)
    HUD1_HUD_HEIGHTLVL = 557858392, // ((VEHICLE_ARHUD_START + 0X002B) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_HEIGHTLVL = 557858393, // ((VEHICLE_ARHUD_START + 0X002C) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_DISPLAYMODE = 557858394, // ((VEHICLE_ARHUD_START + 0X002D) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_BRIGHTNESSLVL = 557858395, // ((VEHICLE_ARHUD_START + 0X002E) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_ADASDISPSWTRESP = 557858396, // ((VEHICLE_ARHUD_START + 0X002F) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_NAVIDISPSWTRESP = 557858397, // ((VEHICLE_ARHUD_START + 0X0030) | VehiclePropertyType:INT32)
    AR_HUD1_ARHUD_BTPHONEDISPSWTRESP = 557858398, // ((VEHICLE_ARHUD_START + 0X0031) | VehiclePropertyType:INT32)
    VEHICLE_ARHUD_END = 553664556, // (VEHICLE_ARHUD_START + 0X01FF)
    VEHICLE_REMOTEPARK_START = 553664557, // (VEHICLE_ARHUD_END + 0X0001)
    HUT25_HUT_REMTENGCTRL = 557858862, // ((VEHICLE_REMOTEPARK_START + 0X0001) | VehiclePropertyType:INT32)
    ECM2_ENGSTATE = 557858863, // ((VEHICLE_REMOTEPARK_START + 0X0002) | VehiclePropertyType:INT32)
    R_PBOX1_LTURNLMPSTS_R_PBOX = 557858864, // ((VEHICLE_REMOTEPARK_START + 0X0003) | VehiclePropertyType:INT32)
    HUT32_CHECKSUM_HUT32 = 557858865, // ((VEHICLE_REMOTEPARK_START + 0X0004) | VehiclePropertyType:INT32)
    HUT32_ROLLINGCOUNTER_HUT32 = 557858866, // ((VEHICLE_REMOTEPARK_START + 0X0005) | VehiclePropertyType:INT32)
    HUT32_PRKSTRAIGHTSWTREQ_HUT = 557858867, // ((VEHICLE_REMOTEPARK_START + 0X0006) | VehiclePropertyType:INT32)
    HUT32_PRKSTRAIGHTMODSEL_HUT = 557858868, // ((VEHICLE_REMOTEPARK_START + 0X0007) | VehiclePropertyType:INT32)
    HUT32_PRKSTRAIGHTSTS_HUT = 557858869, // ((VEHICLE_REMOTEPARK_START + 0X0008) | VehiclePropertyType:INT32)
    HUT32_PRKSTRAIGHTSTRTREQ_HUT = 557858870, // ((VEHICLE_REMOTEPARK_START + 0X0009) | VehiclePropertyType:INT32)
    HUT32_PRKSTRAIGHTVEHSTRTREQ_HUT_VALID = 557858871, // ((VEHICLE_REMOTEPARK_START + 0X000A) | VehiclePropertyType:INT32)
    HUT32_PRKSTRAIGHTCONTINUEREQ_HUT = 557858872, // ((VEHICLE_REMOTEPARK_START + 0X000B) | VehiclePropertyType:INT32)
    HUT_FD3_HUT_OD_WORKSTS = 557858873, // ((VEHICLE_REMOTEPARK_START + 0X000C) | VehiclePropertyType:INT32)
    HUT_FD3_HUT_OD_OBJ_TIMESTAMP = 557858874, // ((VEHICLE_REMOTEPARK_START + 0X000D) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ0_ID = 557858875, // ((VEHICLE_REMOTEPARK_START + 0X000E) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ0_TYPE = 557858876, // ((VEHICLE_REMOTEPARK_START + 0X000F) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ0_CONFIDENCE = 559956029, // ((VEHICLE_REMOTEPARK_START + 0X0010) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ0_WIDTH = 559956030, // ((VEHICLE_REMOTEPARK_START + 0X0011) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ0_GROUND_POS_X = 559956031, // ((VEHICLE_REMOTEPARK_START + 0X0012) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ0_GROUND_POS_Y = 559956032, // ((VEHICLE_REMOTEPARK_START + 0X0013) | VehiclePropertyType:FLOAT)
    HUT_FD2_HTU_OD_OBJ1_ID = 557858881, // ((VEHICLE_REMOTEPARK_START + 0X0014) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ1_TYPE = 557858882, // ((VEHICLE_REMOTEPARK_START + 0X0015) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ1_CONFIDENCE = 559956035, // ((VEHICLE_REMOTEPARK_START + 0X0016) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ1_WIDTH = 559956036, // ((VEHICLE_REMOTEPARK_START + 0X0017) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ1_GROUND_POS_X = 559956037, // ((VEHICLE_REMOTEPARK_START + 0X0018) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ1_GROUND_POS_Y = 559956038, // ((VEHICLE_REMOTEPARK_START + 0X0019) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ2_ID = 557858887, // ((VEHICLE_REMOTEPARK_START + 0X001A) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ2_TYPE = 557858888, // ((VEHICLE_REMOTEPARK_START + 0X001B) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ2_CONFIDENCE = 559956041, // ((VEHICLE_REMOTEPARK_START + 0X001C) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ2_WIDTH = 559956042, // ((VEHICLE_REMOTEPARK_START + 0X001D) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ2_GROUND_POS_X = 559956043, // ((VEHICLE_REMOTEPARK_START + 0X001E) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ2_GROUND_POS_Y = 559956044, // ((VEHICLE_REMOTEPARK_START + 0X001F) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ3_ID = 557858893, // ((VEHICLE_REMOTEPARK_START + 0X0020) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ3_TYPE = 557858894, // ((VEHICLE_REMOTEPARK_START + 0X0021) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ3_CONFIDENCE = 559956047, // ((VEHICLE_REMOTEPARK_START + 0X0022) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ3_WIDTH = 559956048, // ((VEHICLE_REMOTEPARK_START + 0X0023) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ3_GROUND_POS_X = 559956049, // ((VEHICLE_REMOTEPARK_START + 0X0024) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ3_GROUND_POS_Y = 559956050, // ((VEHICLE_REMOTEPARK_START + 0X0025) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ4_ID = 557858899, // ((VEHICLE_REMOTEPARK_START + 0X0026) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ4_TYPE = 557858900, // ((VEHICLE_REMOTEPARK_START + 0X0027) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ4_CONFIDENCE = 559956053, // ((VEHICLE_REMOTEPARK_START + 0X0028) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ4_WIDTH = 559956054, // ((VEHICLE_REMOTEPARK_START + 0X0029) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ4_GROUND_POS_X = 559956055, // ((VEHICLE_REMOTEPARK_START + 0X002A) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ4_GROUND_POS_Y = 559956056, // ((VEHICLE_REMOTEPARK_START + 0X002B) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ5_ID = 557858905, // ((VEHICLE_REMOTEPARK_START + 0X002C) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ5_TYPE = 557858906, // ((VEHICLE_REMOTEPARK_START + 0X002D) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ5_CONFIDENCE = 559956059, // ((VEHICLE_REMOTEPARK_START + 0X002E) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ5_WIDTH = 559956060, // ((VEHICLE_REMOTEPARK_START + 0X002F) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ5_GROUND_POS_X = 559956061, // ((VEHICLE_REMOTEPARK_START + 0X0030) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ5_GROUND_POS_Y = 559956062, // ((VEHICLE_REMOTEPARK_START + 0X0031) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ6_ID = 557858911, // ((VEHICLE_REMOTEPARK_START + 0X0032) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ6_TYPE = 557858912, // ((VEHICLE_REMOTEPARK_START + 0X0033) | VehiclePropertyType:INT32)
    HUT_FD2_HUT_OD_OBJ6_CONFIDENCE = 559956065, // ((VEHICLE_REMOTEPARK_START + 0X0034) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ6_WIDTH = 559956066, // ((VEHICLE_REMOTEPARK_START + 0X0035) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ6_GROUND_POS_X = 559956067, // ((VEHICLE_REMOTEPARK_START + 0X0036) | VehiclePropertyType:FLOAT)
    HUT_FD2_HUT_OD_OBJ6_GROUND_POS_Y = 559956068, // ((VEHICLE_REMOTEPARK_START + 0X0037) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_OD_OBJ7_ID = 557858917, // ((VEHICLE_REMOTEPARK_START + 0X0038) | VehiclePropertyType:INT32)
    HUT_FD3_HUT_OD_OBJ7_TYPE = 557858918, // ((VEHICLE_REMOTEPARK_START + 0X0039) | VehiclePropertyType:INT32)
    HUT_FD3_HUT_OD_OBJ7_CONFIDENCE = 559956071, // ((VEHICLE_REMOTEPARK_START + 0X003A) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_OD_OBJ7_WIDTH = 559956072, // ((VEHICLE_REMOTEPARK_START + 0X003B) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_OD_OBJ7_GROUND_POS_X = 559956073, // ((VEHICLE_REMOTEPARK_START + 0X003C) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_OD_OBJ7_GROUND_POS_Y = 559956074, // ((VEHICLE_REMOTEPARK_START + 0X003D) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD1_DISTANCE = 559956075, // ((VEHICLE_REMOTEPARK_START + 0X003E) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD2_DISTANCE = 559956076, // ((VEHICLE_REMOTEPARK_START + 0X003F) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD3_DISTANCE = 559956077, // ((VEHICLE_REMOTEPARK_START + 0X0040) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD4_DISTANCE = 559956078, // ((VEHICLE_REMOTEPARK_START + 0X0041) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD5_DISTANCE = 559956079, // ((VEHICLE_REMOTEPARK_START + 0X0042) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD6_DISTANCE = 559956080, // ((VEHICLE_REMOTEPARK_START + 0X0043) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD7_DISTANCE = 559956081, // ((VEHICLE_REMOTEPARK_START + 0X0044) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD8_DISTANCE = 559956082, // ((VEHICLE_REMOTEPARK_START + 0X0045) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD9_DISTANCE = 559956083, // ((VEHICLE_REMOTEPARK_START + 0X0046) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD10_DISTANCE = 559956084, // ((VEHICLE_REMOTEPARK_START + 0X0047) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD11_DISTANCE = 559956085, // ((VEHICLE_REMOTEPARK_START + 0X0048) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD12_DISTANCE = 559956086, // ((VEHICLE_REMOTEPARK_START + 0X0049) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD13_DISTANCE = 559956087, // ((VEHICLE_REMOTEPARK_START + 0X004A) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD14_DISTANCE = 559956088, // ((VEHICLE_REMOTEPARK_START + 0X004B) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD15_DISTANCE = 559956089, // ((VEHICLE_REMOTEPARK_START + 0X004C) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD16_DISTANCE = 559956090, // ((VEHICLE_REMOTEPARK_START + 0X004D) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD17_DISTANCE = 559956091, // ((VEHICLE_REMOTEPARK_START + 0X004E) | VehiclePropertyType:FLOAT)
    HUT_FD3_HUT_FSD18_DISTANCE = 559956092, // ((VEHICLE_REMOTEPARK_START + 0X004F) | VehiclePropertyType:FLOAT)
    HUT32_PRKSTRAIGHTSUSPENDORFINISHREQ_HUT = 557858941, // ((VEHICLE_REMOTEPARK_START + 0X0050) | VehiclePropertyType:INT32)
    VEHICLE_REMOTEPARK_END = 553665068, // (VEHICLE_REMOTEPARK_START + 0X01FF)
    VEHICLE_VR_START = 553665069, // (VEHICLE_REMOTEPARK_END + 0X0001)
    HUT4_ACAUTOMODREQ_VR = 557859374, // ((VEHICLE_VR_START + 0X0001) | VehiclePropertyType:INT32)
    HUT4_ACCMPRREQ_VR = 557859375, // ((VEHICLE_VR_START + 0X0002) | VehiclePropertyType:INT32)
    HUT4_ACPOWERREQ_VR = 557859376, // ((VEHICLE_VR_START + 0X0003) | VehiclePropertyType:INT32)
    HUT4_ACBLWRSPDREQ_VR = 557859377, // ((VEHICLE_VR_START + 0X0004) | VehiclePropertyType:INT32)
    HUT4_ACDRVTEMPREQ_VR = 559956530, // ((VEHICLE_VR_START + 0X0005) | VehiclePropertyType:FLOAT)
    HUT4_ACFRNTDEFROSTREQ_VR = 557859379, // ((VEHICLE_VR_START + 0X0006) | VehiclePropertyType:INT32)
    HUT4_ACDRVAIRDISTRIBMODREQ_VR = 557859380, // ((VEHICLE_VR_START + 0X0007) | VehiclePropertyType:INT32)
    HUT4_ACAIRINLETREQ_VR = 557859381, // ((VEHICLE_VR_START + 0X0008) | VehiclePropertyType:INT32)
    HUT6_AUTODEFROST_VR = 557859382, // ((VEHICLE_VR_START + 0X0009) | VehiclePropertyType:INT32)
    HUT10_ACDUALREQ_VR = 557859383, // ((VEHICLE_VR_START + 0X000A) | VehiclePropertyType:INT32)
    HUT10_ACAQSREQ_VR = 557859384, // ((VEHICLE_VR_START + 0X000B) | VehiclePropertyType:INT32)
    HUT10_ACREARDEFRSTREQ_VR = 557859385, // ((VEHICLE_VR_START + 0X000C) | VehiclePropertyType:INT32)
    HUT10_ACPASSTEMPREQ_VR = 559956538, // ((VEHICLE_VR_START + 0X000D) | VehiclePropertyType:FLOAT)
    HUT10_ACAIUREQ_VR = 557859387, // ((VEHICLE_VR_START + 0X000E) | VehiclePropertyType:INT32)
    HUT10_PASSWTREQ_VR = 557859388, // ((VEHICLE_VR_START + 0X000F) | VehiclePropertyType:INT32)
    HUT15_ACOPERMOD_VR = 557859389, // ((VEHICLE_VR_START + 0X0010) | VehiclePropertyType:INT32)
    HUT15_APSSWTREQ_VR = 557859390, // ((VEHICLE_VR_START + 0X0011) | VehiclePropertyType:INT32)
    HUT15_ACEAHREQ_VR = 557859391, // ((VEHICLE_VR_START + 0X0012) | VehiclePropertyType:INT32)
    HUT17_PLGREQ_VR = 557859392, // ((VEHICLE_VR_START + 0X0013) | VehiclePropertyType:INT32)
    HUT35_ACMAXREQ_VR = 557859393, // ((VEHICLE_VR_START + 0X0014) | VehiclePropertyType:INT32)
    VEHICLE_VR_END = 553665580, // (VEHICLE_VR_START + 0X01FF)
    VEHICLE_AMBLIGHT_START = 553665581, // (VEHICLE_VR_END + 0X0001)
    HUT38_DRVDOORALCMFLASHFREQ = 557859887, // ((VEHICLE_AMBLIGHT_START + 0X0002) | VehiclePropertyType:INT32)
    HUT38_PASSDOORALCMFLASHFREQ = 557859888, // ((VEHICLE_AMBLIGHT_START + 0X0003) | VehiclePropertyType:INT32)
    HUT38_LRDOORALCMFLASHFREQ = 557859889, // ((VEHICLE_AMBLIGHT_START + 0X0004) | VehiclePropertyType:INT32)
    HUT38_RRDOORALCMFLASHFREQ = 557859890, // ((VEHICLE_AMBLIGHT_START + 0X0005) | VehiclePropertyType:INT32)
    HUT38_FOOTALCMFLASHFREQ = 557859891, // ((VEHICLE_AMBLIGHT_START + 0X0006) | VehiclePropertyType:INT32)
    HUT38_LEFTPANELALCMFLASHFREQ = 557859892, // ((VEHICLE_AMBLIGHT_START + 0X0007) | VehiclePropertyType:INT32)
    HUT38_RIGHTPANELALCMFLASHFREQ = 557859893, // ((VEHICLE_AMBLIGHT_START + 0X0008) | VehiclePropertyType:INT32)
    HUT39_DRVDOORALCMCLRSET = 557859894, // ((VEHICLE_AMBLIGHT_START + 0X0009) | VehiclePropertyType:INT32)
    HUT39_PASSDOORALCMCLRSET = 557859895, // ((VEHICLE_AMBLIGHT_START + 0X000A) | VehiclePropertyType:INT32)
    HUT39_LRDOORALCMCLRSET = 557859896, // ((VEHICLE_AMBLIGHT_START + 0X000B) | VehiclePropertyType:INT32)
    HUT41_RRDOORALCMCLRSET = 557859897, // ((VEHICLE_AMBLIGHT_START + 0X000C) | VehiclePropertyType:INT32)
    HUT41_LEFTPANELALCMCLRSET = 557859898, // ((VEHICLE_AMBLIGHT_START + 0X000D) | VehiclePropertyType:INT32)
    HUT41_RIGHTPANELALCMCLRSET = 557859899, // ((VEHICLE_AMBLIGHT_START + 0X000E) | VehiclePropertyType:INT32)
    HUT41_FOOTALCMCLRSET = 557859900, // ((VEHICLE_AMBLIGHT_START + 0X000F) | VehiclePropertyType:INT32)
    HUT41_DRVDOORALCMLIGHTLVLSET = 557859901, // ((VEHICLE_AMBLIGHT_START + 0X0010) | VehiclePropertyType:INT32)
    HUT41_PASSDOORALCMLIGHTLVLSET = 557859902, // ((VEHICLE_AMBLIGHT_START + 0X0011) | VehiclePropertyType:INT32)
    HUT41_LRDOORALCMLIGHTLVLSET = 557859903, // ((VEHICLE_AMBLIGHT_START + 0X0012) | VehiclePropertyType:INT32)
    HUT41_RRDOORALCMLIGHTLVLSET = 557859904, // ((VEHICLE_AMBLIGHT_START + 0X0013) | VehiclePropertyType:INT32)
    HUT41_LEFTPANELALCMLIGHTLVLSET = 557859905, // ((VEHICLE_AMBLIGHT_START + 0X0014) | VehiclePropertyType:INT32)
    HUT41_RIGHTPANELALCMLIGHTLVLSET = 557859906, // ((VEHICLE_AMBLIGHT_START + 0X0015) | VehiclePropertyType:INT32)
    HUT41_FOOTALCMLIGHTLVLSET = 557859907, // ((VEHICLE_AMBLIGHT_START + 0X0016) | VehiclePropertyType:INT32)
    VEHICLE_AMBLIGHT_END = 553666092, // (VEHICLE_AMBLIGHT_START + 0X01FF)
    VEHICLE_HELPDRV_START = 553666093, // (VEHICLE_AMBLIGHT_END + 0X0001)
    IFC_FD2_TJA_FOLLOWSTS = 557860398, // ((VEHICLE_HELPDRV_START + 0X0001) | VehiclePropertyType:INT32)
    VEHICLE_HELPDRV_END = 553666604, // (VEHICLE_HELPDRV_START + 0X01FF)
    VEHICLE_RTCTIME_START = 553666605, // (VEHICLE_HELPDRV_END + 0X0001)
    HW_RTCTIME = 557926446, // ((VEHICLE_RTCTIME_START + 0X0001) | VehiclePropertyType:INT32_VEC)
    VEHICLE_RTCTIME_END = 553667116, // (VEHICLE_RTCTIME_START + 0X01FF)
    VEHICLE_WHUD_START = 553667117, // (VEHICLE_RTCTIME_END + 0X0001)
    BCM8_REMOTEMODSTS = 557861422, // ((VEHICLE_WHUD_START + 0X0001) | VehiclePropertyType:INT32)
    IFC_FD2_CAMERABLOCKAGESTS = 557861423, // ((VEHICLE_WHUD_START + 0X0002) | VehiclePropertyType:INT32)
    IFC_FD2_IFCCALIBRATIONSTS = 557861424, // ((VEHICLE_WHUD_START + 0X0003) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_HANDSOFFWARN = 557861425, // ((VEHICLE_WHUD_START + 0X0004) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_TAKEOVERREQ = 557861426, // ((VEHICLE_WHUD_START + 0X0005) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETGT_02_TYP = 557861427, // ((VEHICLE_WHUD_START + 0X0006) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETARGRT_02_DETN = 557861428, // ((VEHICLE_WHUD_START + 0X0007) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RITARGRT_02_DETN = 557861429, // ((VEHICLE_WHUD_START + 0X0008) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_RITGT_02_TYP = 557861430, // ((VEHICLE_WHUD_START + 0X0009) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_LETGT_02_DY = 559958583, // ((VEHICLE_WHUD_START + 0X000A) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_LETGT_02_DX = 559958584, // ((VEHICLE_WHUD_START + 0X000B) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RITGT_02_DY = 559958585, // ((VEHICLE_WHUD_START + 0X000C) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_RITGT_02_DX = 559958586, // ((VEHICLE_WHUD_START + 0X000D) | VehiclePropertyType:FLOAT)
    ACC_FD2_ACC_TIMEGAPSET = 557861435, // ((VEHICLE_WHUD_START + 0X000E) | VehiclePropertyType:INT32)
    F_PBOX1_HOODSTS_F_PBOX = 557861436, // ((VEHICLE_WHUD_START + 0X000F) | VehiclePropertyType:INT32)
    _IP_FUELLVLLOWLMPSTS = 557861437, // ((VEHICLE_WHUD_START + 0X0010) | VehiclePropertyType:INT32)
    VEHICLE_WHUD_END = 553667628, // (VEHICLE_WHUD_START + 0X01FF)
    VEHICLE_LAMP_START = 553667629, // (VEHICLE_WHUD_END + 0X0001)
    IP2_IP_DRVSEATBELTWARNLMPFAILSTS = 557861934, // ((VEHICLE_LAMP_START + 0X0001) | VehiclePropertyType:INT32)
    IP2_IP_PASSSEATBELTWARNLMPFAILSTS = 557861935, // ((VEHICLE_LAMP_START + 0X0002) | VehiclePropertyType:INT32)
    IP2_IP_FUELLVLLOWLMPSTS = 557861936, // ((VEHICLE_LAMP_START + 0X0003) | VehiclePropertyType:INT32)
    IP2_IP_BATTCHRGLMPSTS = 557861937, // ((VEHICLE_LAMP_START + 0X0004) | VehiclePropertyType:INT32)
    IP2_IP_ENGOILPRESSLOWLMPSTS = 557861938, // ((VEHICLE_LAMP_START + 0X0005) | VehiclePropertyType:INT32)
    IP2_IP_ABMWARNLMPFAILRSTS = 557861939, // ((VEHICLE_LAMP_START + 0X0006) | VehiclePropertyType:INT32)
    HUT_FD1_FLASHLMPSWTREQ = 557861940, // ((VEHICLE_LAMP_START + 0X0007) | VehiclePropertyType:INT32)
    HUT_FD1_PIXELHDLMPFCTMSTSWT = 557861941, // ((VEHICLE_LAMP_START + 0X0008) | VehiclePropertyType:INT32)
    HUT15_ABSLMPSET = 557861942, // ((VEHICLE_LAMP_START + 0X0009) | VehiclePropertyType:INT32)
    EDC1_WARNLMPREQ = 557861943, // ((VEHICLE_LAMP_START + 0X000A) | VehiclePropertyType:INT32)
    BCM1_IPBACKGROUNDLMPCMD = 557861944, // ((VEHICLE_LAMP_START + 0X000B) | VehiclePropertyType:INT32)
    F_PBOX1_FFOGLMPSTS_F_PBOX = 557861945, // ((VEHICLE_LAMP_START + 0X000C) | VehiclePropertyType:INT32)
    F_PBOX1_LASHDLMPSTS_F_PBOX = 557861946, // ((VEHICLE_LAMP_START + 0X000D) | VehiclePropertyType:INT32)
    F_PBOX1_LHEADLMPFAILSTS = 557861947, // ((VEHICLE_LAMP_START + 0X000E) | VehiclePropertyType:INT32)
    F_PBOX1_RHEADLMPFAILSTS = 557861948, // ((VEHICLE_LAMP_START + 0X000F) | VehiclePropertyType:INT32)
    F_PBOX1_RLASHDLMPDIAGSIG = 557861949, // ((VEHICLE_LAMP_START + 0X0010) | VehiclePropertyType:INT32)
    F_PBOX1_LLASHDLMPDIAGSIG = 557861950, // ((VEHICLE_LAMP_START + 0X0011) | VehiclePropertyType:INT32)
    CSA1_AUTOLMPSWTSTS = 557861951, // ((VEHICLE_LAMP_START + 0X0012) | VehiclePropertyType:INT32)
    HALDEX1_DIAGLMPREQ = 557861952, // ((VEHICLE_LAMP_START + 0X0013) | VehiclePropertyType:INT32)
    R_PBOX1_HIPOSNBRKLMPFAILSTS = 557861953, // ((VEHICLE_LAMP_START + 0X0014) | VehiclePropertyType:INT32)
    R_PBOX1_LBRKLMPFAILSTS = 557861954, // ((VEHICLE_LAMP_START + 0X0015) | VehiclePropertyType:INT32)
    R_PBOX1_RFOGLMPFAILSTS_R_PBOX = 557861955, // ((VEHICLE_LAMP_START + 0X0016) | VehiclePropertyType:INT32)
    R_PBOX1_RFOGLMPSTS_R_PBOX = 557861956, // ((VEHICLE_LAMP_START + 0X0017) | VehiclePropertyType:INT32)
    R_PBOX1_PARKLMPSTS_R_PBOX = 557861957, // ((VEHICLE_LAMP_START + 0X0018) | VehiclePropertyType:INT32)
    R_PBOX1_RBRKLMPFAILSTS = 557861958, // ((VEHICLE_LAMP_START + 0X0019) | VehiclePropertyType:INT32)
    R_PBOX1_POSNLMPOUTPSTS_R_PBOX = 557861959, // ((VEHICLE_LAMP_START + 0X001A) | VehiclePropertyType:INT32)
    R_PBOX1_RRPOSNLMPFAILSTS = 557861960, // ((VEHICLE_LAMP_START + 0X001B) | VehiclePropertyType:INT32)
    R_PBOX1_RVSLMPFAILSTS = 557861961, // ((VEHICLE_LAMP_START + 0X001C) | VehiclePropertyType:INT32)
    R_PBOX1_LRPOSNLMPFAILSTS = 557861962, // ((VEHICLE_LAMP_START + 0X001D) | VehiclePropertyType:INT32)
    R_PBOX1_LICPLATELMPFAILSTS = 557861963, // ((VEHICLE_LAMP_START + 0X001E) | VehiclePropertyType:INT32)
    ABM1_AIRBFAILLMPCMD = 557861964, // ((VEHICLE_LAMP_START + 0X001F) | VehiclePropertyType:INT32)
    ECM3_GENTRWARNLMPSTS = 557861965, // ((VEHICLE_LAMP_START + 0X0020) | VehiclePropertyType:INT32)
    ESP_FD2_ESPACTVINFOLMP = 557861966, // ((VEHICLE_LAMP_START + 0X0021) | VehiclePropertyType:INT32)
    EPB1_EPBREDLMPSTS = 557861967, // ((VEHICLE_LAMP_START + 0X0022) | VehiclePropertyType:INT32)
    HCU_PT5_HCU_EVLMPSTS = 557861968, // ((VEHICLE_LAMP_START + 0X0023) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_12VSYSERRLMPSTS = 557861969, // ((VEHICLE_LAMP_START + 0X0024) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERRLMPSTS = 557861970, // ((VEHICLE_LAMP_START + 0X0025) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_POWERREDWARNLMPCMD = 557861971, // ((VEHICLE_LAMP_START + 0X0026) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_RDYLMPSTS = 557861972, // ((VEHICLE_LAMP_START + 0X0027) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_OPDLMPSTS = 557861973, // ((VEHICLE_LAMP_START + 0X0028) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_CHRGSYSLMPSTS = 557861974, // ((VEHICLE_LAMP_START + 0X0029) | VehiclePropertyType:INT32)
    BCM3_HILOWBEAMSTS = 557861975, // ((VEHICLE_LAMP_START + 0X002A) | VehiclePropertyType:INT32)
    HCM_L1_LHIBEAMSTS = 557861976, // ((VEHICLE_LAMP_START + 0X002B) | VehiclePropertyType:INT32)
    HCM_L1_LHIBEAMFAILSTS_HCM = 557861977, // ((VEHICLE_LAMP_START + 0X002C) | VehiclePropertyType:INT32)
    HCM_L1_LLOWBEAMSTS = 557861978, // ((VEHICLE_LAMP_START + 0X002D) | VehiclePropertyType:INT32)
    HCM_L1_LLOWBEAMFAILSTS_HCM = 557861979, // ((VEHICLE_LAMP_START + 0X002E) | VehiclePropertyType:INT32)
    HCM_R1_RHIBEAMSTS = 557861980, // ((VEHICLE_LAMP_START + 0X002F) | VehiclePropertyType:INT32)
    HCM_R1_RHIBEAMFAILSTS_HCM = 557861981, // ((VEHICLE_LAMP_START + 0X0030) | VehiclePropertyType:INT32)
    HCM_R1_RLOWBEAMSTS = 557861982, // ((VEHICLE_LAMP_START + 0X0031) | VehiclePropertyType:INT32)
    HCM_R1_RLOWBEAMFAILSTS_HCM = 557861983, // ((VEHICLE_LAMP_START + 0X0032) | VehiclePropertyType:INT32)
    F_PBOX1_HIBEAMSTS_F_PBOX = 557861984, // ((VEHICLE_LAMP_START + 0X0033) | VehiclePropertyType:INT32)
    F_PBOX1_LHIBEAMFAILSTS = 557861985, // ((VEHICLE_LAMP_START + 0X0034) | VehiclePropertyType:INT32)
    F_PBOX1_RHIBEAMFAILSTS = 557861986, // ((VEHICLE_LAMP_START + 0X0035) | VehiclePropertyType:INT32)
    F_PBOX1_LLOWBEAMFAILSTS = 557861987, // ((VEHICLE_LAMP_START + 0X0036) | VehiclePropertyType:INT32)
    F_PBOX1_RLOWBEAMFAILSTS = 557861988, // ((VEHICLE_LAMP_START + 0X0037) | VehiclePropertyType:INT32)
    HC1_HIGHBEAMSTS_HC = 557861989, // ((VEHICLE_LAMP_START + 0X0038) | VehiclePropertyType:INT32)
    HC1_LEFTLOWBEAMFAILSTS = 557861990, // ((VEHICLE_LAMP_START + 0X0039) | VehiclePropertyType:INT32)
    HC1_RIGHTLOWBEAMFAILSTS = 557861991, // ((VEHICLE_LAMP_START + 0X003A) | VehiclePropertyType:INT32)
    HC1_LEFTHIGHBEAMFAILSTS = 557861992, // ((VEHICLE_LAMP_START + 0X003B) | VehiclePropertyType:INT32)
    HC1_RIGHTHIGHBEAMFAILSTS = 557861993, // ((VEHICLE_LAMP_START + 0X003C) | VehiclePropertyType:INT32)
    FCM1_HIGHBEAMOUTPUTSTS_FCM = 557861994, // ((VEHICLE_LAMP_START + 0X003D) | VehiclePropertyType:INT32)
    VEHICLE_LAMP_END = 553668140, // (VEHICLE_LAMP_START + 0X01FF)
    VEHICLE_NFC_START = 553668141, // (VEHICLE_LAMP_END + 0X0001)
    WPC1_WPC_FAULTSTS = 557862446, // ((VEHICLE_NFC_START + 0X0001) | VehiclePropertyType:INT32)
    WPC1_WPC_PHONEREMINDER = 557862447, // ((VEHICLE_NFC_START + 0X0002) | VehiclePropertyType:INT32)
    WPC2_1_WPC2_CHRGSTS = 557862448, // ((VEHICLE_NFC_START + 0X0003) | VehiclePropertyType:INT32)
    WPC2_2_NFCINFOREPLY = 558911025, // ((VEHICLE_NFC_START + 0X0004) | VehiclePropertyType:INT64)
    BLE1_BLECONNSTS = 557862450, // ((VEHICLE_NFC_START + 0X0005) | VehiclePropertyType:INT32)
    BLE1_BLESECURITYSTS = 557862451, // ((VEHICLE_NFC_START + 0X0006) | VehiclePropertyType:INT32)
    BLE5_BLE_BEANID = 558911028, // ((VEHICLE_NFC_START + 0X0007) | VehiclePropertyType:INT64)
    VEHICLE_NFC_END = 553668652, // (VEHICLE_NFC_START + 0X01FF)
    VEHICLE_AVAS_START = 553668653, // (VEHICLE_NFC_END + 0X0001)
    HCU_HP6_HCU_GEARSTS = 557862958, // ((VEHICLE_AVAS_START + 0X0001) | VehiclePropertyType:INT32)
    ECM_PT7_ENGSTATE_PHEV = 557862959, // ((VEHICLE_AVAS_START + 0X0002) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_ACCELPEDALPOSN_DIAG = 559960112, // ((VEHICLE_AVAS_START + 0X0003) | VehiclePropertyType:FLOAT)
    VSG1_VSG_STS = 557862961, // ((VEHICLE_AVAS_START + 0X0004) | VehiclePropertyType:INT32)
    VEHICLE_AVAS_END = 553669164, // (VEHICLE_AVAS_START + 0X01FF)
    VEHICLE_V71_START = 553669165, // (VEHICLE_AVAS_END + 0X0001)
    IP1_IP_CURRTHEME = 557863470, // ((VEHICLE_V71_START + 0X0001) | VehiclePropertyType:INT32)
    IP1_IP_VELSPDDISP_MILE = 557863471, // ((VEHICLE_V71_START + 0X0002) | VehiclePropertyType:INT32)
    IP1_IP_LAGUESET = 557863472, // ((VEHICLE_V71_START + 0X0003) | VehiclePropertyType:INT32)
    IP1_IP_FUELMAINTANKR = 557863473, // ((VEHICLE_V71_START + 0X0004) | VehiclePropertyType:INT32)
    IP1_IP_FUELAUXTANKR = 557863474, // ((VEHICLE_V71_START + 0X0005) | VehiclePropertyType:INT32)
    IP2_CHECKSUM_IP2 = 557863475, // ((VEHICLE_V71_START + 0X0006) | VehiclePropertyType:INT32)
    IP2_IP_ERR = 557863476, // ((VEHICLE_V71_START + 0X0007) | VehiclePropertyType:INT32)
    IP2_IP_ACCERR = 557863477, // ((VEHICLE_V71_START + 0X0008) | VehiclePropertyType:INT32)
    IP2_IP_VEHSPDUNIT = 557863478, // ((VEHICLE_V71_START + 0X0009) | VehiclePropertyType:INT32)
    IP2_IP_VINCOMPR = 557863479, // ((VEHICLE_V71_START + 0X000A) | VehiclePropertyType:INT32)
    IP2_ROLLINGCOUNTER_IP2 = 557863480, // ((VEHICLE_V71_START + 0X000B) | VehiclePropertyType:INT32)
    IP2_FRESHNESSVALUE_IP2 = 557863481, // ((VEHICLE_V71_START + 0X000C) | VehiclePropertyType:INT32)
    IP2_MAC_CHECK_IP2 = 558912058, // ((VEHICLE_V71_START + 0X000D) | VehiclePropertyType:INT64)
    IP3_IP_EBDFAILSTS = 557863483, // ((VEHICLE_V71_START + 0X000E) | VehiclePropertyType:INT32)
    HUT_FD1_ICA_HANDSOFFSWTREQ = 557863484, // ((VEHICLE_V71_START + 0X000F) | VehiclePropertyType:INT32)
    HUT_FD1_HUTRADIOFRQ = 557863485, // ((VEHICLE_V71_START + 0X0010) | VehiclePropertyType:INT32)
    HUT_FD1_MEDIAPLAYSRC = 557863486, // ((VEHICLE_V71_START + 0X0011) | VehiclePropertyType:INT32)
    HUT_FD1_USB_AUDIOSTS = 557863487, // ((VEHICLE_V71_START + 0X0012) | VehiclePropertyType:INT32)
    HUT_FD1_USB_VIDEOSTS = 557863488, // ((VEHICLE_V71_START + 0X0013) | VehiclePropertyType:INT32)
    HUT_FD1_BT_AUDIOSTS = 557863489, // ((VEHICLE_V71_START + 0X0014) | VehiclePropertyType:INT32)
    HUT_FD1_IPOD_STS = 557863490, // ((VEHICLE_V71_START + 0X0015) | VehiclePropertyType:INT32)
    HUT_FD1_AM_STS = 557863491, // ((VEHICLE_V71_START + 0X0016) | VehiclePropertyType:INT32)
    HUT_FD1_FM_STS = 557863492, // ((VEHICLE_V71_START + 0X0017) | VehiclePropertyType:INT32)
    HUT_FD1_APPLINK_STS = 557863493, // ((VEHICLE_V71_START + 0X0018) | VehiclePropertyType:INT32)
    HUT_FD1_QQMUSIC_STS = 557863494, // ((VEHICLE_V71_START + 0X0019) | VehiclePropertyType:INT32)
    HUT_FD1_KAOLAFM_STS = 557863495, // ((VEHICLE_V71_START + 0X001A) | VehiclePropertyType:INT32)
    HUT_FD1_ONLINENEWS_STS = 557863496, // ((VEHICLE_V71_START + 0X001B) | VehiclePropertyType:INT32)
    HUT_FD1_TABSWTREQ_HUT = 557863497, // ((VEHICLE_V71_START + 0X001C) | VehiclePropertyType:INT32)
    HUT_FD1_BENDADPVLIGHTG = 557863498, // ((VEHICLE_V71_START + 0X001D) | VehiclePropertyType:INT32)
    HUT_FD1_GLAREFREEFORPED = 557863499, // ((VEHICLE_V71_START + 0X001E) | VehiclePropertyType:INT32)
    HUT_FD1_GLAREFREEFORDRVR = 557863500, // ((VEHICLE_V71_START + 0X001F) | VehiclePropertyType:INT32)
    HUT_FD1_VIRTINSTMTBRD = 557863501, // ((VEHICLE_V71_START + 0X0020) | VehiclePropertyType:INT32)
    HUT_FD1_SPDLIMGUDBRDIDNPRJTN = 557863502, // ((VEHICLE_V71_START + 0X0021) | VehiclePropertyType:INT32)
    HUT_FD1_DSTVEHIDNSPAPRJTN = 557863503, // ((VEHICLE_V71_START + 0X0022) | VehiclePropertyType:INT32)
    HUT_FD1_VEHWIDEPRJTN = 557863504, // ((VEHICLE_V71_START + 0X0023) | VehiclePropertyType:INT32)
    HUT_FD1_DYNZEBRAPRJTN = 557863505, // ((VEHICLE_V71_START + 0X0024) | VehiclePropertyType:INT32)
    HUT_FD1_STEERMKRPRJTN = 557863506, // ((VEHICLE_V71_START + 0X0025) | VehiclePropertyType:INT32)
    HUT_FD1_VEHLAUNCHLOGOPRJTN = 557863507, // ((VEHICLE_V71_START + 0X0026) | VehiclePropertyType:INT32)
    HUT_FD1_CUSTSETTGSPRJTN = 557863508, // ((VEHICLE_V71_START + 0X0027) | VehiclePropertyType:INT32)
    HUT_FD1_NAVVEHTOTRAFEYEDIST = 557863509, // ((VEHICLE_V71_START + 0X0028) | VehiclePropertyType:INT32)
    HUT_FD1_NAVROADTYPE = 557863510, // ((VEHICLE_V71_START + 0X0029) | VehiclePropertyType:INT32)
    HUT_FD1_NAVSPDLIMTYPE = 557863511, // ((VEHICLE_V71_START + 0X002A) | VehiclePropertyType:INT32)
    HUT_FD1_NAVSPDLIMVALUE = 557863512, // ((VEHICLE_V71_START + 0X002B) | VehiclePropertyType:INT32)
    HUT_FD1_NAVSPDLIMUNIT_HUT = 557863513, // ((VEHICLE_V71_START + 0X002C) | VehiclePropertyType:INT32)
    HUT_FD1_NAVCTRYTYPE = 557863514, // ((VEHICLE_V71_START + 0X002D) | VehiclePropertyType:INT32)
    HUT_FD1_NAVSPDLIMSIGNSTS = 557863515, // ((VEHICLE_V71_START + 0X002E) | VehiclePropertyType:INT32)
    HUT_FD1_NAVTSRTRAFSIGN = 557863516, // ((VEHICLE_V71_START + 0X002F) | VehiclePropertyType:INT32)
    HUT_FD1_STEERCORRNREQ = 557863517, // ((VEHICLE_V71_START + 0X0030) | VehiclePropertyType:INT32)
    HUT_FD1_FRESHNESSVALUE_HUT_FD1 = 557863518, // ((VEHICLE_V71_START + 0X0031) | VehiclePropertyType:INT32)
    HUT_FD1_MAC_CHECK_HUT_FD1 = 558912095, // ((VEHICLE_V71_START + 0X0032) | VehiclePropertyType:INT64)
    HUT13_AUDIOMUTESTS = 557863520, // ((VEHICLE_V71_START + 0X0033) | VehiclePropertyType:INT32)
    HUT1_TRANPMODE_REQ = 557863521, // ((VEHICLE_V71_START + 0X0034) | VehiclePropertyType:INT32)
    HUT3_AMP_GETDTCINFOREQ = 557863522, // ((VEHICLE_V71_START + 0X0035) | VehiclePropertyType:INT32)
    HUT4_REMUPGRDSTS = 557863523, // ((VEHICLE_V71_START + 0X0036) | VehiclePropertyType:INT32)
    HUT6_MDLCOLRCHGCMD = 557863524, // ((VEHICLE_V71_START + 0X0037) | VehiclePropertyType:INT32)
    HUT6_WSHSOFTSWT = 557863525, // ((VEHICLE_V71_START + 0X0038) | VehiclePropertyType:INT32)
    HUT10_HUT_BEANIDREQ = 557863526, // ((VEHICLE_V71_START + 0X0039) | VehiclePropertyType:INT32)
    HUT15_APSPRKGTYPSELN = 557863527, // ((VEHICLE_V71_START + 0X003A) | VehiclePropertyType:INT32)
    HUT15_ACEAHREQ_TC = 557863528, // ((VEHICLE_V71_START + 0X003B) | VehiclePropertyType:INT32)
    HUT15_FRESHNESSVALUE_HUT15 = 557863529, // ((VEHICLE_V71_START + 0X003C) | VehiclePropertyType:INT32)
    HUT15_MAC_CHECK_HUT15 = 558912106, // ((VEHICLE_V71_START + 0X003D) | VehiclePropertyType:INT64)
    HUT17_BACKGROUNDLIGHTLVLSET = 557863531, // ((VEHICLE_V71_START + 0X003E) | VehiclePropertyType:INT32)
    HUT17_MANCMPSET = 557863532, // ((VEHICLE_V71_START + 0X003F) | VehiclePropertyType:INT32)
    HUT17_DISPMODSET = 557863533, // ((VEHICLE_V71_START + 0X0040) | VehiclePropertyType:INT32)
    HUT25_HUT_INTELENGIDLCHRGNSET = 557863534, // ((VEHICLE_V71_START + 0X0041) | VehiclePropertyType:INT32)
    HUT30_DROWSNSDETNSET = 557863535, // ((VEHICLE_V71_START + 0X0042) | VehiclePropertyType:INT32)
    HUT30_VOICEIDNSET = 557863536, // ((VEHICLE_V71_START + 0X0043) | VehiclePropertyType:INT32)
    HUT30_ERASEALLFACEUSERSREQ = 557863537, // ((VEHICLE_V71_START + 0X0044) | VehiclePropertyType:INT32)
    HUT30_ERASEALLVOICEUSERSREQ = 557863538, // ((VEHICLE_V71_START + 0X0045) | VehiclePropertyType:INT32)
    HUT30_CANCELVOICEIDN = 557863539, // ((VEHICLE_V71_START + 0X0046) | VehiclePropertyType:INT32)
    HUT30_BACKREQ_AVM = 557863540, // ((VEHICLE_V71_START + 0X0047) | VehiclePropertyType:INT32)
    HUT32_SCRNOPSTS = 557863541, // ((VEHICLE_V71_START + 0X0048) | VehiclePropertyType:INT32)
    HUT32_XLVL = 557863542, // ((VEHICLE_V71_START + 0X0049) | VehiclePropertyType:INT32)
    HUT32_YLVL = 557863543, // ((VEHICLE_V71_START + 0X004A) | VehiclePropertyType:INT32)
    HUT32_CONTNPRKGREQVALID = 557863544, // ((VEHICLE_V71_START + 0X004B) | VehiclePropertyType:INT32)
    HUT32_CONTNPRKGREQ = 557863545, // ((VEHICLE_V71_START + 0X004C) | VehiclePropertyType:INT32)
    HUT33_PRKINDIRCHOICE = 557863546, // ((VEHICLE_V71_START + 0X004D) | VehiclePropertyType:INT32)
    HUT33_PATHLRNGFINSHCMD = 557863547, // ((VEHICLE_V71_START + 0X004E) | VehiclePropertyType:INT32)
    HUT33_PATHLRNGSTARTCMD = 557863548, // ((VEHICLE_V71_START + 0X004F) | VehiclePropertyType:INT32)
    HUT33_SELPRKOUTDIRREQ = 557863549, // ((VEHICLE_V71_START + 0X0050) | VehiclePropertyType:INT32)
    HUT33_STARTPRKGPATH2CMD = 557863550, // ((VEHICLE_V71_START + 0X0051) | VehiclePropertyType:INT32)
    HUT33_DELETEPATH2CMD = 557863551, // ((VEHICLE_V71_START + 0X0052) | VehiclePropertyType:INT32)
    HUT33_STARTPRKGPATH1CMD = 557863552, // ((VEHICLE_V71_START + 0X0053) | VehiclePropertyType:INT32)
    HUT33_DELETEPATH1CMD = 557863553, // ((VEHICLE_V71_START + 0X0054) | VehiclePropertyType:INT32)
    HUT33_PARKMDLCMD = 557863554, // ((VEHICLE_V71_START + 0X0055) | VehiclePropertyType:INT32)
    HUT33_BACKREQ_APS = 557863555, // ((VEHICLE_V71_START + 0X0056) | VehiclePropertyType:INT32)
    HUT35_ACTVENTERSET = 557863556, // ((VEHICLE_V71_START + 0X0057) | VehiclePropertyType:INT32)
    HUT35_ACTVLEAVESET = 557863557, // ((VEHICLE_V71_START + 0X0058) | VehiclePropertyType:INT32)
    T_BOX_FD1_T_BOX_REMTDOORLOCKCTRL = 557863558, // ((VEHICLE_V71_START + 0X0059) | VehiclePropertyType:INT32)
    T_BOX_FD1_T_BOX_AUDIOMUTEREQ = 557863559, // ((VEHICLE_V71_START + 0X005A) | VehiclePropertyType:INT32)
    T_BOX_FD1_T_BOX_PM25AIRQLVL = 557863560, // ((VEHICLE_V71_START + 0X005B) | VehiclePropertyType:INT32)
    T_BOX_FD1_T_BOX_PM25DENS = 557863561, // ((VEHICLE_V71_START + 0X005C) | VehiclePropertyType:INT32)
    T_BOX_FD1_FRESHNESSVALUE_T_BOX_FD1 = 557863562, // ((VEHICLE_V71_START + 0X005D) | VehiclePropertyType:INT32)
    T_BOX_FD1_MAC_CHECK_T_BOX_FD1 = 558912139, // ((VEHICLE_V71_START + 0X005E) | VehiclePropertyType:INT64)
    T_BOX_FD3_T_BOX_SYSERRSTS = 557863564, // ((VEHICLE_V71_START + 0X005F) | VehiclePropertyType:INT32)
    T_BOX_FD3_T_BOX_CHRGCMPLTTIME_HOUR = 557863565, // ((VEHICLE_V71_START + 0X0060) | VehiclePropertyType:INT32)
    T_BOX_FD3_T_BOX_SWUPDSTS = 557863566, // ((VEHICLE_V71_START + 0X0061) | VehiclePropertyType:INT32)
    T_BOX_FD3_T_BOX_CHRGCMPLTTIME_MINUTE = 557863567, // ((VEHICLE_V71_START + 0X0062) | VehiclePropertyType:INT32)
    T_BOX_FD3_FRESHNESSVALUE_T_BOX_FD3 = 557863568, // ((VEHICLE_V71_START + 0X0063) | VehiclePropertyType:INT32)
    T_BOX_FD3_MAC_CHECK_T_BOX_FD3 = 558912145, // ((VEHICLE_V71_START + 0X0064) | VehiclePropertyType:INT64)
    T_BOX_FD4_PHONE_CHRGNMODE = 557863570, // ((VEHICLE_V71_START + 0X0065) | VehiclePropertyType:INT32)
    T_BOX_FD4_T_BOX_BATTKEEPTEMP = 557863571, // ((VEHICLE_V71_START + 0X0066) | VehiclePropertyType:INT32)
    T_BOX_FD4_T_BOX_INTELBATTTEMPMAGSW = 557863572, // ((VEHICLE_V71_START + 0X0067) | VehiclePropertyType:INT32)
    T_BOX_FD4_PHONE_BATTSOCLIM = 557863573, // ((VEHICLE_V71_START + 0X0068) | VehiclePropertyType:INT32)
    T_BOX_FD4_T_BOX_AVAILINPUTCUR = 557863574, // ((VEHICLE_V71_START + 0X0069) | VehiclePropertyType:INT32)
    DMS_FD1_GENDERSTSQLTY = 557863575, // ((VEHICLE_V71_START + 0X006A) | VehiclePropertyType:INT32)
    DMS_FD1_FRESHNESSVALUE_DMS_FD1 = 557863576, // ((VEHICLE_V71_START + 0X006B) | VehiclePropertyType:INT32)
    DMS_FD1_MAC_CHECK_DMS_FD1 = 558912153, // ((VEHICLE_V71_START + 0X006C) | VehiclePropertyType:INT64)
    EEM1_TRANPMODE_STS = 557863578, // ((VEHICLE_V71_START + 0X006D) | VehiclePropertyType:INT32)
    GW_FD1_I_RANGE = 557863579, // ((VEHICLE_V71_START + 0X006E) | VehiclePropertyType:INT32)
    GW_FD1_I_BATT = 557863580, // ((VEHICLE_V71_START + 0X006F) | VehiclePropertyType:INT32)
    GW_FD1_SOH_SUL = 557863581, // ((VEHICLE_V71_START + 0X0070) | VehiclePropertyType:INT32)
    GW_FD1_SOC_STATE = 557863582, // ((VEHICLE_V71_START + 0X0071) | VehiclePropertyType:INT32)
    GW_FD1_SOH_SUL_STATE = 557863583, // ((VEHICLE_V71_START + 0X0072) | VehiclePropertyType:INT32)
    EDC1_CURRDAMPRMODE = 557863584, // ((VEHICLE_V71_START + 0X0073) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_ERRSTS = 557863585, // ((VEHICLE_V71_START + 0X0074) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_BLISTS = 557863586, // ((VEHICLE_V71_START + 0X0075) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_TRAILERSTS = 557863587, // ((VEHICLE_V71_START + 0X0076) | VehiclePropertyType:INT32)
    RSDS_FD1_RSDS_CTA_ACTV = 557863588, // ((VEHICLE_V71_START + 0X0077) | VehiclePropertyType:INT32)
    IFC_FD2_CHECKSUM_IFC3 = 557863589, // ((VEHICLE_V71_START + 0X0078) | VehiclePropertyType:INT32)
    IFC_FD2_IFCHUTINTERFACE = 557863590, // ((VEHICLE_V71_START + 0X0079) | VehiclePropertyType:INT32)
    IFC_FD2_ROLLINGCOUNTER_IFC3 = 557863591, // ((VEHICLE_V71_START + 0X007A) | VehiclePropertyType:INT32)
    IFC_FD2_CHECKSUM_IFC4 = 557863592, // ((VEHICLE_V71_START + 0X007B) | VehiclePropertyType:INT32)
    IFC_FD2_LDW_LKAWARN = 557863593, // ((VEHICLE_V71_START + 0X007C) | VehiclePropertyType:INT32)
    IFC_FD2_ELKMAINSTATE = 557863594, // ((VEHICLE_V71_START + 0X007D) | VehiclePropertyType:INT32)
    IFC_FD2_LANEAVAILABILITY = 557863595, // ((VEHICLE_V71_START + 0X007E) | VehiclePropertyType:INT32)
    IFC_FD2_ROLLINGCOUNTER_IFC4 = 557863596, // ((VEHICLE_V71_START + 0X007F) | VehiclePropertyType:INT32)
    IFC_FD2_CHECKSUM_IFC5 = 557863597, // ((VEHICLE_V71_START + 0X0080) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_LANECURVE = 559960750, // ((VEHICLE_V71_START + 0X0081) | VehiclePropertyType:FLOAT)
    IFC_FD2_IFC_LELANETYP = 557863599, // ((VEHICLE_V71_START + 0X0082) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_RILANETYP = 557863600, // ((VEHICLE_V71_START + 0X0083) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_NEXTLELANETYP = 557863601, // ((VEHICLE_V71_START + 0X0084) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_NEXTRILANETYP = 557863602, // ((VEHICLE_V71_START + 0X0085) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_LELANE_DY = 559960755, // ((VEHICLE_V71_START + 0X0086) | VehiclePropertyType:FLOAT)
    IFC_FD2_IFC_RILANE_DY = 559960756, // ((VEHICLE_V71_START + 0X0087) | VehiclePropertyType:FLOAT)
    IFC_FD2_ROLLINGCOUNTER_IFC5 = 557863605, // ((VEHICLE_V71_START + 0X0088) | VehiclePropertyType:INT32)
    IFC_FD2_CHECKSUM_IFC6 = 557863606, // ((VEHICLE_V71_START + 0X0089) | VehiclePropertyType:INT32)
    IFC_FD2_IFC_NEXTLELANE_DY = 559960759, // ((VEHICLE_V71_START + 0X008A) | VehiclePropertyType:FLOAT)
    IFC_FD2_IFC_NEXTRILANE_DY = 559960760, // ((VEHICLE_V71_START + 0X008B) | VehiclePropertyType:FLOAT)
    IFC_FD2_ROLLINGCOUNTER_IFC6 = 557863609, // ((VEHICLE_V71_START + 0X008C) | VehiclePropertyType:INT32)
    IFC_FD2_FRESHNESSVALUE_IFC_FD2 = 557863610, // ((VEHICLE_V71_START + 0X008D) | VehiclePropertyType:INT32)
    IFC_FD2_MAC_CHECK_IFC_FD2 = 558912187, // ((VEHICLE_V71_START + 0X008E) | VehiclePropertyType:INT64)
    IFC_FD3_TSRSPDLIMCFDC = 557863612, // ((VEHICLE_V71_START + 0X008F) | VehiclePropertyType:INT32)
    IFC_FD6_IFC_LINE01_DXSTART = 559960765, // ((VEHICLE_V71_START + 0X0090) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE01_DXEND = 559960766, // ((VEHICLE_V71_START + 0X0091) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE02_DXSTART = 559960767, // ((VEHICLE_V71_START + 0X0092) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE02_DXEND = 559960768, // ((VEHICLE_V71_START + 0X0093) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE03_DXSTART = 559960769, // ((VEHICLE_V71_START + 0X0094) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE03_DXEND = 559960770, // ((VEHICLE_V71_START + 0X0095) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE04_DXSTART = 559960771, // ((VEHICLE_V71_START + 0X0096) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_LINE04_DXEND = 559960772, // ((VEHICLE_V71_START + 0X0097) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_ROADEDGE01_DXSTART = 559960773, // ((VEHICLE_V71_START + 0X0098) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_ROADEDGE01_DXEND = 559960774, // ((VEHICLE_V71_START + 0X0099) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_ROADEDGE02_DXSTART = 559960775, // ((VEHICLE_V71_START + 0X009A) | VehiclePropertyType:FLOAT)
    IFC_FD6_IFC_ROADEDGE02_DXEND = 559960776, // ((VEHICLE_V71_START + 0X009B) | VehiclePropertyType:FLOAT)
    ACC_FD2_CHECKSUM_ACC3 = 557863625, // ((VEHICLE_V71_START + 0X009C) | VehiclePropertyType:INT32)
    ACC_FD2_ICA_HANDSOFFRESP = 557863626, // ((VEHICLE_V71_START + 0X009D) | VehiclePropertyType:INT32)
    ACC_FD2_ALC_INTERSYSINFODISP = 557863627, // ((VEHICLE_V71_START + 0X009E) | VehiclePropertyType:INT32)
    ACC_FD2_ROLLINGCOUNTER_ACC3 = 557863628, // ((VEHICLE_V71_START + 0X009F) | VehiclePropertyType:INT32)
    ACC_FD2_CHECKSUM_ACC4 = 557863629, // ((VEHICLE_V71_START + 0X00A0) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_OBJDETECTE = 557863630, // ((VEHICLE_V71_START + 0X00A1) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_FCTSTS = 557863631, // ((VEHICLE_V71_START + 0X00A2) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_HAZACTV = 557863632, // ((VEHICLE_V71_START + 0X00A3) | VehiclePropertyType:INT32)
    ACC_FD2_TJA_ACC_SELSTS = 557863633, // ((VEHICLE_V71_START + 0X00A4) | VehiclePropertyType:INT32)
    ACC_FD2_CRUCHA_MODDISP = 557863634, // ((VEHICLE_V71_START + 0X00A5) | VehiclePropertyType:INT32)
    ACC_FD2_TJA_ICA_INTERSYSINFODISP = 557863635, // ((VEHICLE_V71_START + 0X00A6) | VehiclePropertyType:INT32)
    ACC_FD2_ISL_INTERSYSINFODISP = 557863636, // ((VEHICLE_V71_START + 0X00A7) | VehiclePropertyType:INT32)
    ACC_FD2_CRUCHAR_INTERSYSINFODISP = 557863637, // ((VEHICLE_V71_START + 0X00A8) | VehiclePropertyType:INT32)
    ACC_FD2_ROLLINGCOUNTER_ACC4 = 557863638, // ((VEHICLE_V71_START + 0X00A9) | VehiclePropertyType:INT32)
    ACC_FD2_CHECKSUM_ACC8 = 557863639, // ((VEHICLE_V71_START + 0X00AA) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_INTERSYSINFODISP = 557863640, // ((VEHICLE_V71_START + 0X00AB) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_LANECHANGE = 557863641, // ((VEHICLE_V71_START + 0X00AC) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_LANECHANGEREQ = 557863642, // ((VEHICLE_V71_START + 0X00AD) | VehiclePropertyType:INT32)
    ACC_FD2_HWA_WARNING = 557863643, // ((VEHICLE_V71_START + 0X00AE) | VehiclePropertyType:INT32)
    ACC_FD2_ROLLINGCOUNTER_ACC8 = 557863644, // ((VEHICLE_V71_START + 0X00AF) | VehiclePropertyType:INT32)
    ACC_FD2_CHECKSUM_ACC6 = 557863645, // ((VEHICLE_V71_START + 0X00B0) | VehiclePropertyType:INT32)
    ACC_FD2_ACC_EGOSTATUS = 557863646, // ((VEHICLE_V71_START + 0X00B1) | VehiclePropertyType:INT32)
    ACC_FD2_ROLLINGCOUNTER_ACC6 = 557863647, // ((VEHICLE_V71_START + 0X00B2) | VehiclePropertyType:INT32)
    ACC_FD2_CHECKSUM_ACC7 = 557863648, // ((VEHICLE_V71_START + 0X00B3) | VehiclePropertyType:INT32)
    ACC_FD2_REQUEST_DRIVEOFF = 557863649, // ((VEHICLE_V71_START + 0X00B4) | VehiclePropertyType:INT32)
    ACC_FD2_ROLLINGCOUNTER_ACC7 = 557863650, // ((VEHICLE_V71_START + 0X00B5) | VehiclePropertyType:INT32)
    ACC_FD2_FRESHNESSVALUE_ACC_FD2 = 557863651, // ((VEHICLE_V71_START + 0X00B6) | VehiclePropertyType:INT32)
    ACC_FD2_MAC_CHECK_ACC_FD2 = 558912228, // ((VEHICLE_V71_START + 0X00B7) | VehiclePropertyType:INT64)
    ACC_FD2_TARGETCUTINPRO = 559960805, // ((VEHICLE_V71_START + 0X00B8) | VehiclePropertyType:FLOAT)
    ACC_FD2_TARGETCUTOUTPRO = 559960806, // ((VEHICLE_V71_START + 0X00B9) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LEPEDDETN = 557863655, // ((VEHICLE_V71_START + 0X00BA) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_LEPEDDY = 559960808, // ((VEHICLE_V71_START + 0X00BB) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LEPEDTYP = 557863657, // ((VEHICLE_V71_START + 0X00BC) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_LEPED_COLLISIONPROB = 557863658, // ((VEHICLE_V71_START + 0X00BD) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_RIPEDDETN = 557863659, // ((VEHICLE_V71_START + 0X00BE) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_RIPEDDY = 559960812, // ((VEHICLE_V71_START + 0X00BF) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_RIPEDTYP = 557863661, // ((VEHICLE_V71_START + 0X00C0) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_RIPED_COLLISIONPROB = 557863662, // ((VEHICLE_V71_START + 0X00C1) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_LECYCLISTDY = 559960815, // ((VEHICLE_V71_START + 0X00C2) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LECYCLIST_COLLISIONPROB = 557863664, // ((VEHICLE_V71_START + 0X00C3) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_RICYCLISTDY = 559960817, // ((VEHICLE_V71_START + 0X00C4) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_RICYCLIST_COLLISIONPROB = 557863666, // ((VEHICLE_V71_START + 0X00C5) | VehiclePropertyType:INT32)
    ACC_FD3_FCWAEB_JATTC = 559960819, // ((VEHICLE_V71_START + 0X00C6) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LEPEDHEADING = 559960820, // ((VEHICLE_V71_START + 0X00C7) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_RIPEDHEADING = 559960821, // ((VEHICLE_V71_START + 0X00C8) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LECYCLISTHEADING = 559960822, // ((VEHICLE_V71_START + 0X00C9) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_RICYCLISTHEADING = 559960823, // ((VEHICLE_V71_START + 0X00CA) | VehiclePropertyType:FLOAT)
    ACC_FD3_FCWAEB_LONGTTC = 559960824, // ((VEHICLE_V71_START + 0X00CB) | VehiclePropertyType:FLOAT)
    ACC_FD3_FCWAEB_CROSSINGTTC = 559960825, // ((VEHICLE_V71_START + 0X00CC) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LEPEDDX = 559960826, // ((VEHICLE_V71_START + 0X00CD) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_RIPEDDX = 559960827, // ((VEHICLE_V71_START + 0X00CE) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LECYCLISTDETN = 557863676, // ((VEHICLE_V71_START + 0X00CF) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_LECYCLISTDX = 559960829, // ((VEHICLE_V71_START + 0X00D0) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_LECYCLISTTYP = 557863678, // ((VEHICLE_V71_START + 0X00D1) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_RICYCLISTDETN = 557863679, // ((VEHICLE_V71_START + 0X00D2) | VehiclePropertyType:INT32)
    ACC_FD3_AEB_RICYCLISTDX = 559960832, // ((VEHICLE_V71_START + 0X00D3) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEB_RICYCLISTTYP = 557863681, // ((VEHICLE_V71_START + 0X00D4) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJID = 557863682, // ((VEHICLE_V71_START + 0X00D5) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJDX = 559960835, // ((VEHICLE_V71_START + 0X00D6) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJDX = 559960836, // ((VEHICLE_V71_START + 0X00D7) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJID = 557863685, // ((VEHICLE_V71_START + 0X00D8) | VehiclePropertyType:INT32)
    ACC_FD4_PEDOBJID = 557863686, // ((VEHICLE_V71_START + 0X00D9) | VehiclePropertyType:INT32)
    ACC_FD4_PEDOBJDX = 559960839, // ((VEHICLE_V71_START + 0X00DA) | VehiclePropertyType:FLOAT)
    AEB_FD1_AEB_OBJSTY = 557863688, // ((VEHICLE_V71_START + 0X00DB) | VehiclePropertyType:INT32)
    AEB_FD2_CHECKSUM_AEB2 = 557863689, // ((VEHICLE_V71_START + 0X00DC) | VehiclePropertyType:INT32)
    AEB_FD2_ROLLINGCOUNTER_AEB2 = 557863690, // ((VEHICLE_V71_START + 0X00DD) | VehiclePropertyType:INT32)
    AEB_FD2_CHECKSUM_AEB3 = 557863691, // ((VEHICLE_V71_START + 0X00DE) | VehiclePropertyType:INT32)
    AEB_FD2_FCW_AEB_PEDFCTSTS = 557863692, // ((VEHICLE_V71_START + 0X00DF) | VehiclePropertyType:INT32)
    AEB_FD2_AEB_JAFUNCSTS = 557863693, // ((VEHICLE_V71_START + 0X00E0) | VehiclePropertyType:INT32)
    AEB_FD2_ROLLINGCOUNTER_AEB3 = 557863694, // ((VEHICLE_V71_START + 0X00E1) | VehiclePropertyType:INT32)
    AEB_FD2_FRESHNESSVALUE_AEB_FD2 = 557863695, // ((VEHICLE_V71_START + 0X00E2) | VehiclePropertyType:INT32)
    AEB_FD2_MAC_CHECK_AEB_FD2 = 558912272, // ((VEHICLE_V71_START + 0X00E3) | VehiclePropertyType:INT64)
    TRAILER1_TRAILERSTS = 557863697, // ((VEHICLE_V71_START + 0X00E4) | VehiclePropertyType:INT32)
    GLO_NASS1_QUIETVOICEREQ = 557863698, // ((VEHICLE_V71_START + 0X00E5) | VehiclePropertyType:INT32)
    CP1_ROOFMODFB = 557863699, // ((VEHICLE_V71_START + 0X00E6) | VehiclePropertyType:INT32)
    CP1_ALLTERRAINDISLFB = 557863700, // ((VEHICLE_V71_START + 0X00E7) | VehiclePropertyType:INT32)
    CP1_ELECSIDESTEPSYSFB = 557863701, // ((VEHICLE_V71_START + 0X00E8) | VehiclePropertyType:INT32)
    RHVSM1_RLSEATHEATSTS = 557863702, // ((VEHICLE_V71_START + 0X00E9) | VehiclePropertyType:INT32)
    RHVSM1_RRSEATHEATSTS = 557863703, // ((VEHICLE_V71_START + 0X00EA) | VehiclePropertyType:INT32)
    BCM1_BACKGROUNDLIGHTLVL = 557863704, // ((VEHICLE_V71_START + 0X00EB) | VehiclePropertyType:INT32)
    BCM1_RLSFAILSTS = 557863705, // ((VEHICLE_V71_START + 0X00EC) | VehiclePropertyType:INT32)
    BCM1_FRESHNESSVALUE_BCM1 = 557863706, // ((VEHICLE_V71_START + 0X00ED) | VehiclePropertyType:INT32)
    BCM1_MAC_CHECK_BCM1 = 558912283, // ((VEHICLE_V71_START + 0X00EE) | VehiclePropertyType:INT64)
    BCM3_FRESHNESSVALUE_BCM3 = 557863708, // ((VEHICLE_V71_START + 0X00EF) | VehiclePropertyType:INT32)
    BCM3_MAC_CHECK_BCM3 = 558912285, // ((VEHICLE_V71_START + 0X00F0) | VehiclePropertyType:INT64)
    BCM6_VOLSWTREQ = 557863710, // ((VEHICLE_V71_START + 0X00F1) | VehiclePropertyType:INT32)
    BCM6_VOLSWTERRSTS = 557863711, // ((VEHICLE_V71_START + 0X00F2) | VehiclePropertyType:INT32)
    BCM8_ADJVOLSWTSTS = 557863712, // ((VEHICLE_V71_START + 0X00F3) | VehiclePropertyType:INT32)
    BCM8_ADJVOLSWTSYNC = 557863713, // ((VEHICLE_V71_START + 0X00F4) | VehiclePropertyType:INT32)
    BCM8_ADJVOLSWTERRSTS = 557863714, // ((VEHICLE_V71_START + 0X00F5) | VehiclePropertyType:INT32)
    BCM8_KL30BRELAYSTS = 557863715, // ((VEHICLE_V71_START + 0X00F6) | VehiclePropertyType:INT32)
    BCM8_WIPRREQ_F_PBOX = 557863716, // ((VEHICLE_V71_START + 0X00F7) | VehiclePropertyType:INT32)
    BCM12_SRANTIPINCH = 557863717, // ((VEHICLE_V71_START + 0X00F8) | VehiclePropertyType:INT32)
    BCM12_SSANTIPINCH = 557863718, // ((VEHICLE_V71_START + 0X00F9) | VehiclePropertyType:INT32)
    BCM12_SRPOSN_VR_APP = 557863719, // ((VEHICLE_V71_START + 0X00FA) | VehiclePropertyType:INT32)
    PEPS2_REMUPGRDMODSYSPOWER = 557863720, // ((VEHICLE_V71_START + 0X00FB) | VehiclePropertyType:INT32)
    PEPS4_WARNPOWERNOTINOFFPOSN = 557863721, // ((VEHICLE_V71_START + 0X00FC) | VehiclePropertyType:INT32)
    PEPS4_WARNKEYINCARWHENEXIT = 557863722, // ((VEHICLE_V71_START + 0X00FD) | VehiclePropertyType:INT32)
    PEPS4_WARNVLDKEYNOTFOUND = 557863723, // ((VEHICLE_V71_START + 0X00FE) | VehiclePropertyType:INT32)
    PEPS4_WARNENGSTRTNOTMEET = 557863724, // ((VEHICLE_V71_START + 0X00FF) | VehiclePropertyType:INT32)
    PEPS4_POWERRLYFAULT = 557863725, // ((VEHICLE_V71_START + 0X0100) | VehiclePropertyType:INT32)
    HCM_L1_LADBSTATUS = 557863727, // ((VEHICLE_V71_START + 0X0102) | VehiclePropertyType:INT32)
    HCM_L1_LAFSSTATUS = 557863728, // ((VEHICLE_V71_START + 0X0103) | VehiclePropertyType:INT32)
    HCM_R1_RADBSTATUS = 557863730, // ((VEHICLE_V71_START + 0X0105) | VehiclePropertyType:INT32)
    HCM_R1_RAFSSTATUS = 557863731, // ((VEHICLE_V71_START + 0X0106) | VehiclePropertyType:INT32)
    RPAS1_RPAS_WORKSTS = 557863732, // ((VEHICLE_V71_START + 0X0107) | VehiclePropertyType:INT32)
    RPAS1_RPAS_SOUNDINDCN = 557863733, // ((VEHICLE_V71_START + 0X0108) | VehiclePropertyType:INT32)
    RPAS2_RPAS_NEARBRRDSTN = 557863734, // ((VEHICLE_V71_START + 0X0109) | VehiclePropertyType:INT32)
    AC1_ACAQSENASTS = 557863735, // ((VEHICLE_V71_START + 0X010A) | VehiclePropertyType:INT32)
    AC1_ACOPERMOD = 557863736, // ((VEHICLE_V71_START + 0X010B) | VehiclePropertyType:INT32)
    AC1_ACAIUENASTS = 557863737, // ((VEHICLE_V71_START + 0X010C) | VehiclePropertyType:INT32)
    AC1_ACDRVAIRDISTRIMOD = 557863738, // ((VEHICLE_V71_START + 0X010D) | VehiclePropertyType:INT32)
    AC1_ACSTEPLESSSPDSETENASTS = 557863739, // ((VEHICLE_V71_START + 0X010E) | VehiclePropertyType:INT32)
    AC1_ACFRNTPASSTEMPSTEPLESSSETENASTS = 557863740, // ((VEHICLE_V71_START + 0X010F) | VehiclePropertyType:INT32)
    AC1_ACDRVTEMPSTEPLESSSETENASTS = 557863741, // ((VEHICLE_V71_START + 0X0110) | VehiclePropertyType:INT32)
    AC2_ACFRNTDEFRSTSTS = 557863742, // ((VEHICLE_V71_START + 0X0111) | VehiclePropertyType:INT32)
    AC2_ACFRNTBLWRSPD = 557863743, // ((VEHICLE_V71_START + 0X0112) | VehiclePropertyType:INT32)
    AC2_ACOPENSTS = 557863744, // ((VEHICLE_V71_START + 0X0113) | VehiclePropertyType:INT32)
    AC2_ACDUALMODENASTS = 557863745, // ((VEHICLE_V71_START + 0X0114) | VehiclePropertyType:INT32)
    AC2_ACDRVTEMP_ATC = 559960898, // ((VEHICLE_V71_START + 0X0115) | VehiclePropertyType:FLOAT)
    AC2_ACAIRINLETSTS = 557863747, // ((VEHICLE_V71_START + 0X0116) | VehiclePropertyType:INT32)
    AC2_ACFRNTPASSTTEMP_ATC = 559960900, // ((VEHICLE_V71_START + 0X0117) | VehiclePropertyType:FLOAT)
    AC2_ACAUTOMODENASTS = 557863749, // ((VEHICLE_V71_START + 0X0118) | VehiclePropertyType:INT32)
    AC2_ACCMPRENASTS = 557863750, // ((VEHICLE_V71_START + 0X0119) | VehiclePropertyType:INT32)
    AC2_ACAUTODEFRSTMODENASTS = 557863751, // ((VEHICLE_V71_START + 0X011A) | VehiclePropertyType:INT32)
    AC6_L_FGAENASTS = 557863752, // ((VEHICLE_V71_START + 0X011B) | VehiclePropertyType:INT32)
    AC7_L_FGACHAN1ILLEGALITYSTS = 557863753, // ((VEHICLE_V71_START + 0X011C) | VehiclePropertyType:INT32)
    AC7_L_FGACHAN2ILLEGALITYSTS = 557863754, // ((VEHICLE_V71_START + 0X011D) | VehiclePropertyType:INT32)
    AC7_L_FGACHAN3ILLEGALITYSTS = 557863755, // ((VEHICLE_V71_START + 0X011E) | VehiclePropertyType:INT32)
    DSM1_CHAIRMEMPOSNSETSWTFB = 557863756, // ((VEHICLE_V71_START + 0X011F) | VehiclePropertyType:INT32)
    ALCM1_ALCMSWTSTS = 557863757, // ((VEHICLE_V71_START + 0X0120) | VehiclePropertyType:INT32)
    SCM1_DRVSEATHEATSTS = 557863758, // ((VEHICLE_V71_START + 0X0121) | VehiclePropertyType:INT32)
    SCM1_PASSSEATHEATSTS = 557863759, // ((VEHICLE_V71_START + 0X0122) | VehiclePropertyType:INT32)
    SCM1_DRVSEATVENTNSTS = 557863760, // ((VEHICLE_V71_START + 0X0123) | VehiclePropertyType:INT32)
    SCM1_PASSSEATVENTNSTS = 557863761, // ((VEHICLE_V71_START + 0X0124) | VehiclePropertyType:INT32)
    CSA3_MENURETURNSWTSTS = 557863762, // ((VEHICLE_V71_START + 0X0125) | VehiclePropertyType:INT32)
    CSA3_HOMESWTSTS = 557863763, // ((VEHICLE_V71_START + 0X0126) | VehiclePropertyType:INT32)
    CSA3_MUTESWTSTS = 557863764, // ((VEHICLE_V71_START + 0X0127) | VehiclePropertyType:INT32)
    CSA3_CUSTOMSWTSTS = 557863765, // ((VEHICLE_V71_START + 0X0128) | VehiclePropertyType:INT32)
    CSA3_PAGEUPSWTSTS = 557863766, // ((VEHICLE_V71_START + 0X0129) | VehiclePropertyType:INT32)
    CSA3_PAGEDWNSWTSTS = 557863767, // ((VEHICLE_V71_START + 0X012A) | VehiclePropertyType:INT32)
    CSA3_PAGELSWTSTS = 557863768, // ((VEHICLE_V71_START + 0X012B) | VehiclePropertyType:INT32)
    CSA3_PAGERSWTSTS = 557863769, // ((VEHICLE_V71_START + 0X012C) | VehiclePropertyType:INT32)
    CSA3_SEEKUPSWTSTS = 557863770, // ((VEHICLE_V71_START + 0X012D) | VehiclePropertyType:INT32)
    CSA3_SEEKDWNSWTSTS = 557863771, // ((VEHICLE_V71_START + 0X012E) | VehiclePropertyType:INT32)
    CSA3_ADJVOLUPSWTSTS = 557863772, // ((VEHICLE_V71_START + 0X012F) | VehiclePropertyType:INT32)
    CSA3_ADJVOLDWNSWTSTS = 557863773, // ((VEHICLE_V71_START + 0X0130) | VehiclePropertyType:INT32)
    CSA3_VRSWTSTS = 557863774, // ((VEHICLE_V71_START + 0X0131) | VehiclePropertyType:INT32)
    CSA3_ENTERSWTSTS = 557863775, // ((VEHICLE_V71_START + 0X0132) | VehiclePropertyType:INT32)
    CSA3_ENTERSWTSTS_MMED = 557863776, // ((VEHICLE_V71_START + 0X0133) | VehiclePropertyType:INT32)
    CSA3_CUSTOMSWTSTS_MMED = 557863777, // ((VEHICLE_V71_START + 0X0134) | VehiclePropertyType:INT32)
    HALDEX1_COUPLINGTHERMWARNING = 557863778, // ((VEHICLE_V71_START + 0X0135) | VehiclePropertyType:INT32)
    HALDEX1_COUPLINGFAULTSTS = 557863779, // ((VEHICLE_V71_START + 0X0136) | VehiclePropertyType:INT32)
    AMP1_AMP_SWVERSION = 557863780, // ((VEHICLE_V71_START + 0X0137) | VehiclePropertyType:INT32)
    AMP1_AMP_HWVERSION = 557863781, // ((VEHICLE_V71_START + 0X0138) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN1DTCINFO = 557863782, // ((VEHICLE_V71_START + 0X0139) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN2DTCINFO = 557863783, // ((VEHICLE_V71_START + 0X013A) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN3DTCINFO = 557863784, // ((VEHICLE_V71_START + 0X013B) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN4DTCINFO = 557863785, // ((VEHICLE_V71_START + 0X013C) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN5DTCINFO = 557863786, // ((VEHICLE_V71_START + 0X013D) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN6DTCINFO = 557863787, // ((VEHICLE_V71_START + 0X013E) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN7DTCINFO = 557863788, // ((VEHICLE_V71_START + 0X013F) | VehiclePropertyType:INT32)
    AMP1_AMP_CHAN8DTCINFO = 557863789, // ((VEHICLE_V71_START + 0X0140) | VehiclePropertyType:INT32)
    AMP1_AMP_TEMPSTS = 557863790, // ((VEHICLE_V71_START + 0X0141) | VehiclePropertyType:INT32)
    AMP1_AMP_VOLVALUE = 559960943, // ((VEHICLE_V71_START + 0X0142) | VehiclePropertyType:FLOAT)
    AMP1_BESTLISTEGPOSNRESP = 557863792, // ((VEHICLE_V71_START + 0X0143) | VehiclePropertyType:INT32)
    AMP2_BEEPSOURCESTS = 557863793, // ((VEHICLE_V71_START + 0X0144) | VehiclePropertyType:INT32)
    AMP2_BALANCESETSTATE = 557863794, // ((VEHICLE_V71_START + 0X0145) | VehiclePropertyType:INT32)
    AMP2_VSCMODESTATE = 557863795, // ((VEHICLE_V71_START + 0X0146) | VehiclePropertyType:INT32)
    AMP2_FADESETSTATE = 557863796, // ((VEHICLE_V71_START + 0X0147) | VehiclePropertyType:INT32)
    AMP2_DRIVESIDESTS = 557863797, // ((VEHICLE_V71_START + 0X0148) | VehiclePropertyType:INT32)
    AMP2_BASSSETSTATE = 557863798, // ((VEHICLE_V71_START + 0X0149) | VehiclePropertyType:INT32)
    AMP2_MUTESTATE = 557863799, // ((VEHICLE_V71_START + 0X014A) | VehiclePropertyType:INT32)
    AMP2_NAVISOURCESTATE = 557863800, // ((VEHICLE_V71_START + 0X014B) | VehiclePropertyType:INT32)
    AMP2_MIDRANGESETSTATE = 557863801, // ((VEHICLE_V71_START + 0X014C) | VehiclePropertyType:INT32)
    AMP2_T_BOX_ECALLSTS = 557863802, // ((VEHICLE_V71_START + 0X014D) | VehiclePropertyType:INT32)
    AMP2_TREBLESETSTATE = 557863803, // ((VEHICLE_V71_START + 0X014E) | VehiclePropertyType:INT32)
    AMP2_QLISURROUNDSETST = 557863804, // ((VEHICLE_V71_START + 0X014F) | VehiclePropertyType:INT32)
    AMP2_NAVIMEDIAVOLLVLRESP = 557863805, // ((VEHICLE_V71_START + 0X0150) | VehiclePropertyType:INT32)
    AMP2_AVMMEDIAVOLLVLRESP = 557863806, // ((VEHICLE_V71_START + 0X0151) | VehiclePropertyType:INT32)
    AMP2_FPASCHSTRESP = 557863807, // ((VEHICLE_V71_START + 0X0152) | VehiclePropertyType:INT32)
    AMP2_RPASCHSTRESP = 557863808, // ((VEHICLE_V71_START + 0X0153) | VehiclePropertyType:INT32)
    AMP3_HFMVOLSETSTS = 557863809, // ((VEHICLE_V71_START + 0X0154) | VehiclePropertyType:INT32)
    AMP3_NAVIVOLUMESETSTATE = 557863810, // ((VEHICLE_V71_START + 0X0155) | VehiclePropertyType:INT32)
    AMP3_AMPPOWERSTS = 557863811, // ((VEHICLE_V71_START + 0X0156) | VehiclePropertyType:INT32)
    AMP3_IPVOLSETSTS = 557863812, // ((VEHICLE_V71_START + 0X0157) | VehiclePropertyType:INT32)
    AMP4_ANC1_MICROPHONE_CHA1DTCINFO = 557863813, // ((VEHICLE_V71_START + 0X0158) | VehiclePropertyType:INT32)
    AMP4_ANC1_MICROPHONE_CHA2DTCINFO = 557863814, // ((VEHICLE_V71_START + 0X0159) | VehiclePropertyType:INT32)
    AMP4_ANC1_MICROPHONE_CHA3DTCINFO = 557863815, // ((VEHICLE_V71_START + 0X015A) | VehiclePropertyType:INT32)
    AMP4_ANC1_MICROPHONE_CHA4DTCINFO = 557863816, // ((VEHICLE_V71_START + 0X015B) | VehiclePropertyType:INT32)
    AMP4_ANC1_ANCSWTSTATUS = 557863817, // ((VEHICLE_V71_START + 0X015C) | VehiclePropertyType:INT32)
    AMP5_IESS1_IESS_SWITCHMODEL = 557863818, // ((VEHICLE_V71_START + 0X015D) | VehiclePropertyType:INT32)
    R_PBOX1_RWINHEATGICONRLYCMD = 557863819, // ((VEHICLE_V71_START + 0X015E) | VehiclePropertyType:INT32)
    R_PBOX1_FRESHNESSVALUE_R_PBOX1 = 557863820, // ((VEHICLE_V71_START + 0X015F) | VehiclePropertyType:INT32)
    R_PBOX1_MAC_CHECK_R_PBOX1 = 558912397, // ((VEHICLE_V71_START + 0X0160) | VehiclePropertyType:INT64)
    ABM1_CHECKSUM_ABM1 = 557863822, // ((VEHICLE_V71_START + 0X0161) | VehiclePropertyType:INT32)
    ABM1_SECROWLSBR = 557863823, // ((VEHICLE_V71_START + 0X0162) | VehiclePropertyType:INT32)
    ABM1_SECROWMIDSBR = 557863824, // ((VEHICLE_V71_START + 0X0163) | VehiclePropertyType:INT32)
    ABM1_DRVSBR = 557863825, // ((VEHICLE_V71_START + 0X0164) | VehiclePropertyType:INT32)
    ABM1_SECROWRSBR = 557863826, // ((VEHICLE_V71_START + 0X0165) | VehiclePropertyType:INT32)
    ABM1_PASSSBR = 557863827, // ((VEHICLE_V71_START + 0X0166) | VehiclePropertyType:INT32)
    ABM1_PASSSBR_VISUAL = 557863828, // ((VEHICLE_V71_START + 0X0167) | VehiclePropertyType:INT32)
    ABM1_DRVSBR_VISUAL = 557863829, // ((VEHICLE_V71_START + 0X0168) | VehiclePropertyType:INT32)
    ABM1_SECROWLSBR_VISUAL = 557863830, // ((VEHICLE_V71_START + 0X0169) | VehiclePropertyType:INT32)
    ABM1_SECROWMIDSBR_VISUAL = 557863831, // ((VEHICLE_V71_START + 0X016A) | VehiclePropertyType:INT32)
    ABM1_SECROWRSBR_VISUAL = 557863832, // ((VEHICLE_V71_START + 0X016B) | VehiclePropertyType:INT32)
    ABM1_ROLLINGCOUNTER_ABM1 = 557863833, // ((VEHICLE_V71_START + 0X016C) | VehiclePropertyType:INT32)
    ABM1_FRESHNESSVALUE_ABM1 = 557863834, // ((VEHICLE_V71_START + 0X016D) | VehiclePropertyType:INT32)
    ABM1_MAC_CHECK_ABM1 = 558912411, // ((VEHICLE_V71_START + 0X016E) | VehiclePropertyType:INT64)
    TPMS1_FRESHNESSVALUE_TPMS1 = 557863836, // ((VEHICLE_V71_START + 0X016F) | VehiclePropertyType:INT32)
    TPMS1_MAC_CHECK_TPMS1 = 558912413, // ((VEHICLE_V71_START + 0X0170) | VehiclePropertyType:INT64)
    TPMS2_FRESHNESSVALUE_TPMS2 = 557863838, // ((VEHICLE_V71_START + 0X0171) | VehiclePropertyType:INT32)
    TPMS2_MAC_CHECK_TPMS2 = 558912415, // ((VEHICLE_V71_START + 0X0172) | VehiclePropertyType:INT64)
    HAP_FD1_APS_PRKGMOD = 557863840, // ((VEHICLE_V71_START + 0X0173) | VehiclePropertyType:INT32)
    HAP_FD1_APS_PROCBAR = 557863841, // ((VEHICLE_V71_START + 0X0174) | VehiclePropertyType:INT32)
    HAP_FD1_APS_SWTSTS = 557863842, // ((VEHICLE_V71_START + 0X0175) | VehiclePropertyType:INT32)
    HAP_FD1_APS_SLOTDISP = 557863843, // ((VEHICLE_V71_START + 0X0176) | VehiclePropertyType:INT32)
    HAP_FD1_APS_SYSSOUNDINDCN = 557863844, // ((VEHICLE_V71_START + 0X0177) | VehiclePropertyType:INT32)
    HAP_FD1_APS_PRKGTYP = 557863845, // ((VEHICLE_V71_START + 0X0178) | VehiclePropertyType:INT32)
    HAP_FD1_P2P_PRKGDIRECTSTS = 557863846, // ((VEHICLE_V71_START + 0X0179) | VehiclePropertyType:INT32)
    HAP_FD1_LPARAPRKGOUTVALD = 557863847, // ((VEHICLE_V71_START + 0X017A) | VehiclePropertyType:INT32)
    HAP_FD1_RPARAPRKGOUTVALD = 557863848, // ((VEHICLE_V71_START + 0X017B) | VehiclePropertyType:INT32)
    HAP_FD1_HEADVERTPRKGOUTVALD = 557863849, // ((VEHICLE_V71_START + 0X017C) | VehiclePropertyType:INT32)
    HAP_FD1_TAILVERTPRKGOUTVALD = 557863850, // ((VEHICLE_V71_START + 0X017D) | VehiclePropertyType:INT32)
    HAP_FD1_APS_MOVGDIRC = 557863851, // ((VEHICLE_V71_START + 0X017E) | VehiclePropertyType:INT32)
    HAP_FD2_AUTOAVMSWTSET_STS = 557863852, // ((VEHICLE_V71_START + 0X017F) | VehiclePropertyType:INT32)
    HAP_FD2_GUIDOVLSTS = 557863853, // ((VEHICLE_V71_START + 0X0180) | VehiclePropertyType:INT32)
    HAP_FD2_EOLNOTCMPLTD = 557863854, // ((VEHICLE_V71_START + 0X0181) | VehiclePropertyType:INT32)
    HAP_FD2_RADARDISPSTS = 557863855, // ((VEHICLE_V71_START + 0X0182) | VehiclePropertyType:INT32)
    HAP_FD2_AUTOVIEWCHGFUNCSTS = 557863856, // ((VEHICLE_V71_START + 0X0183) | VehiclePropertyType:INT32)
    HAP_FD2_CARMDLTRSPRCYSTS = 557863857, // ((VEHICLE_V71_START + 0X0184) | VehiclePropertyType:INT32)
    HAP_FD2_CARMDLDISPSTS = 557863858, // ((VEHICLE_V71_START + 0X0185) | VehiclePropertyType:INT32)
    HAP_FD2_CURRCALRESULTSTS = 557863859, // ((VEHICLE_V71_START + 0X0186) | VehiclePropertyType:INT32)
    HAP_FD2_MDLCOLRSTS = 557863860, // ((VEHICLE_V71_START + 0X0187) | VehiclePropertyType:INT32)
    HAP_FD2_SIGVIEWINDCN = 557863861, // ((VEHICLE_V71_START + 0X0188) | VehiclePropertyType:INT32)
    HAP_FD2_AVM_3DANGSTS = 557863862, // ((VEHICLE_V71_START + 0X0189) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_NEARBRRDSTN = 557863863, // ((VEHICLE_V71_START + 0X018A) | VehiclePropertyType:INT32)
    HAP_FD3_SDW_SOUNDINDCN = 557863864, // ((VEHICLE_V71_START + 0X018B) | VehiclePropertyType:INT32)
    HAP_FD3_SDWACTIVESTS = 557863865, // ((VEHICLE_V71_START + 0X018C) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_WORKSTS = 557863866, // ((VEHICLE_V71_START + 0X018D) | VehiclePropertyType:INT32)
    HAP_FD3_RPAS_SOUNDINDCN = 557863867, // ((VEHICLE_V71_START + 0X018E) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_WORKSTS = 557863868, // ((VEHICLE_V71_START + 0X018F) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_AUTOMODSTS = 557863869, // ((VEHICLE_V71_START + 0X0190) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_SOUNDINDCN = 557863870, // ((VEHICLE_V71_START + 0X0191) | VehiclePropertyType:INT32)
    HAP_FD3_FPAS_NEARBRRDSTN = 557863871, // ((VEHICLE_V71_START + 0X0192) | VehiclePropertyType:INT32)
    HAP_FD3_APA_FUNCSTS = 557863872, // ((VEHICLE_V71_START + 0X0193) | VehiclePropertyType:INT32)
    HAP_FD3_HAP_TEXTICONDISPCTRL = 557863873, // ((VEHICLE_V71_START + 0X0194) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTCAM_WORKSTS = 557863874, // ((VEHICLE_V71_START + 0X0195) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ1_ID = 557863875, // ((VEHICLE_V71_START + 0X0196) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ1_TYPE = 557863876, // ((VEHICLE_V71_START + 0X0197) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ1_CONFIDENCE = 559961029, // ((VEHICLE_V71_START + 0X0198) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ1_GROUND_POS_X = 559961030, // ((VEHICLE_V71_START + 0X0199) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ1_GROUND_POS_Y = 559961031, // ((VEHICLE_V71_START + 0X019A) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ2_ID = 557863880, // ((VEHICLE_V71_START + 0X019B) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ2_TYPE = 557863881, // ((VEHICLE_V71_START + 0X019C) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ2_CONFIDENCE = 559961034, // ((VEHICLE_V71_START + 0X019D) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ2_GROUND_POS_X = 559961035, // ((VEHICLE_V71_START + 0X019E) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ2_GROUND_POS_Y = 559961036, // ((VEHICLE_V71_START + 0X019F) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ3_ID = 557863885, // ((VEHICLE_V71_START + 0X01A0) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ3_TYPE = 557863886, // ((VEHICLE_V71_START + 0X01A1) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ3_CONFIDENCE = 559961039, // ((VEHICLE_V71_START + 0X01A2) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ3_GROUND_POS_X = 559961040, // ((VEHICLE_V71_START + 0X01A3) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ3_GROUND_POS_Y = 559961041, // ((VEHICLE_V71_START + 0X01A4) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ4_ID = 557863890, // ((VEHICLE_V71_START + 0X01A5) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ4_TYPE = 557863891, // ((VEHICLE_V71_START + 0X01A6) | VehiclePropertyType:INT32)
    HAP_FD6_HAP_RTSIDE_OBJ4_CONFIDENCE = 559961044, // ((VEHICLE_V71_START + 0X01A7) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ4_GROUND_POS_X = 559961045, // ((VEHICLE_V71_START + 0X01A8) | VehiclePropertyType:FLOAT)
    HAP_FD6_HAP_RTSIDE_OBJ4_GROUND_POS_Y = 559961046, // ((VEHICLE_V71_START + 0X01A9) | VehiclePropertyType:FLOAT)
    HAP_FD6_USS_FLSIDE1_STATUS = 557863895, // ((VEHICLE_V71_START + 0X01AA) | VehiclePropertyType:INT32)
    HAP_FD6_USS_FLSIDE1_DIST = 559961048, // ((VEHICLE_V71_START + 0X01AB) | VehiclePropertyType:FLOAT)
    HAP_FD6_USS_RLSIDE2_STATUS = 557863897, // ((VEHICLE_V71_START + 0X01AC) | VehiclePropertyType:INT32)
    HAP_FD6_USS_RLSIDE2_DIST = 559961050, // ((VEHICLE_V71_START + 0X01AD) | VehiclePropertyType:FLOAT)
    HAP_FD6_USS_FRSIDE1_STATUS = 557863899, // ((VEHICLE_V71_START + 0X01AE) | VehiclePropertyType:INT32)
    HAP_FD6_USS_FRSIDE1_DIST = 559961052, // ((VEHICLE_V71_START + 0X01AF) | VehiclePropertyType:FLOAT)
    HAP_FD6_USS_RRSIDE2_STATUS = 557863901, // ((VEHICLE_V71_START + 0X01B0) | VehiclePropertyType:INT32)
    HAP_FD6_USS_RRSIDE2_DIST = 559961054, // ((VEHICLE_V71_START + 0X01B1) | VehiclePropertyType:FLOAT)
    HC1_CHECKSUM_HC1 = 557863903, // ((VEHICLE_V71_START + 0X01B2) | VehiclePropertyType:INT32)
    HC1_ADBSTATUS = 557863904, // ((VEHICLE_V71_START + 0X01B3) | VehiclePropertyType:INT32)
    HC1_ALS_FAILSTS = 557863905, // ((VEHICLE_V71_START + 0X01B4) | VehiclePropertyType:INT32)
    HC1_ALSDRVSIDE_FB = 557863906, // ((VEHICLE_V71_START + 0X01B5) | VehiclePropertyType:INT32)
    HC1_AFSSTS = 557863907, // ((VEHICLE_V71_START + 0X01B6) | VehiclePropertyType:INT32)
    HC1_DBLSTS = 557863908, // ((VEHICLE_V71_START + 0X01B7) | VehiclePropertyType:INT32)
    HC1_ROLLINGCOUNTER_HC1 = 557863909, // ((VEHICLE_V71_START + 0X01B8) | VehiclePropertyType:INT32)
    DDCM1_POWERCUTSWT = 557863910, // ((VEHICLE_V71_START + 0X01B9) | VehiclePropertyType:INT32)
    DDCM1_DRVWINPOSNSTS = 557863911, // ((VEHICLE_V71_START + 0X01BA) | VehiclePropertyType:INT32)
    DDCM1_DRVWINLRNSTS = 557863912, // ((VEHICLE_V71_START + 0X01BB) | VehiclePropertyType:INT32)
    DDCM1_DRVWINHOTPROTSTS = 557863913, // ((VEHICLE_V71_START + 0X01BC) | VehiclePropertyType:INT32)
    DDCM1_DRVSIDEREARWINPOSNSTS = 557863914, // ((VEHICLE_V71_START + 0X01BD) | VehiclePropertyType:INT32)
    DDCM1_DRVSIDEREARWINLRNSTS = 557863915, // ((VEHICLE_V71_START + 0X01BE) | VehiclePropertyType:INT32)
    DDCM1_DRVSIDEREARWINHOTPROTSTS = 557863916, // ((VEHICLE_V71_START + 0X01BF) | VehiclePropertyType:INT32)
    PDCM1_PASSWINPOSNSTS = 557863917, // ((VEHICLE_V71_START + 0X01C0) | VehiclePropertyType:INT32)
    PDCM1_PASSWINLRNSTS = 557863918, // ((VEHICLE_V71_START + 0X01C1) | VehiclePropertyType:INT32)
    PDCM1_PASSWINHOTPROTSTS = 557863919, // ((VEHICLE_V71_START + 0X01C2) | VehiclePropertyType:INT32)
    PDCM1_PASSSIDEREARWINPOSNSTS = 557863920, // ((VEHICLE_V71_START + 0X01C3) | VehiclePropertyType:INT32)
    PDCM1_PASSSIDEREARWINLRNSTS = 557863921, // ((VEHICLE_V71_START + 0X01C4) | VehiclePropertyType:INT32)
    PDCM1_PASSSIDEREARWINHOTPROTSTS = 557863922, // ((VEHICLE_V71_START + 0X01C5) | VehiclePropertyType:INT32)
    CR_FD1_FCTA_B_TTC = 559961075, // ((VEHICLE_V71_START + 0X01C6) | VehiclePropertyType:FLOAT)
    ECM4_NETENGTRQ = 559961076, // ((VEHICLE_V71_START + 0X01C7) | VehiclePropertyType:FLOAT)
    ECM1_EMS_AUTHENRESULT = 557863925, // ((VEHICLE_V71_START + 0X01C8) | VehiclePropertyType:INT32)
    ECM7_HCU_RACELAUNCHAVAIL = 557863926, // ((VEHICLE_V71_START + 0X01C9) | VehiclePropertyType:INT32)
    ECM7_HCU_RACELAUNCHSTS = 557863927, // ((VEHICLE_V71_START + 0X01CA) | VehiclePropertyType:INT32)
    ECM2_TGTCRSVEHSPD = 557863928, // ((VEHICLE_V71_START + 0X01CB) | VehiclePropertyType:INT32)
    ECM2_CRUISECTRLSTS = 557863929, // ((VEHICLE_V71_START + 0X01CC) | VehiclePropertyType:INT32)
    ECM2_ENGSVSSIG = 557863930, // ((VEHICLE_V71_START + 0X01CD) | VehiclePropertyType:INT32)
    ECM2_MILSTS = 557863931, // ((VEHICLE_V71_START + 0X01CE) | VehiclePropertyType:INT32)
    ECM_PT6_PRESSRELIPROGS = 557863932, // ((VEHICLE_V71_START + 0X01CF) | VehiclePropertyType:INT32)
    ECM_PT6_FTIVFAULT = 557863933, // ((VEHICLE_V71_START + 0X01D0) | VehiclePropertyType:INT32)
    ECM_PT6_FUELTANKCAPWARNING = 557863934, // ((VEHICLE_V71_START + 0X01D1) | VehiclePropertyType:INT32)
    ECM_PT7_CRUISECTRLSTS_PHEV = 557863935, // ((VEHICLE_V71_START + 0X01D2) | VehiclePropertyType:INT32)
    ECM3_BAROPRESSURE = 559961088, // ((VEHICLE_V71_START + 0X01D3) | VehiclePropertyType:FLOAT)
    ECM3_AGS_FAILURE = 557863937, // ((VEHICLE_V71_START + 0X01D4) | VehiclePropertyType:INT32)
    ECM3_GPF_WARNING = 557863938, // ((VEHICLE_V71_START + 0X01D5) | VehiclePropertyType:INT32)
    ECM3_ENGCLNTTEMPWARN = 557863939, // ((VEHICLE_V71_START + 0X01D6) | VehiclePropertyType:INT32)
    ECM3_ENGCLNTTEMPVLDTY = 557863940, // ((VEHICLE_V71_START + 0X01D7) | VehiclePropertyType:INT32)
    ECM3_SS_SCREEN_TIP = 557863941, // ((VEHICLE_V71_START + 0X01D8) | VehiclePropertyType:INT32)
    ECM3_SSSSETSTS = 557863942, // ((VEHICLE_V71_START + 0X01D9) | VehiclePropertyType:INT32)
    ECM3_SS_ENABLE_LAMP = 557863943, // ((VEHICLE_V71_START + 0X01DA) | VehiclePropertyType:INT32)
    ECM3_SS_FAULT_LAMP = 557863944, // ((VEHICLE_V71_START + 0X01DB) | VehiclePropertyType:INT32)
    ECM11_EWP_ERR = 557863945, // ((VEHICLE_V71_START + 0X01DC) | VehiclePropertyType:INT32)
    ECM19_ECM_VIN = 558912522, // ((VEHICLE_V71_START + 0X01DD) | VehiclePropertyType:INT64)
    ECM20_ECM_VEHDSTBACKUP = 559961099, // ((VEHICLE_V71_START + 0X01DE) | VehiclePropertyType:FLOAT)
    DCT_FD4_CHECKSUM_DCT7 = 557863948, // ((VEHICLE_V71_START + 0X01DF) | VehiclePropertyType:INT32)
    DCT_FD4_DCT_RACEMODESTS = 557863949, // ((VEHICLE_V71_START + 0X01E0) | VehiclePropertyType:INT32)
    DCT_FD4_DCT_PRSBRK_WARN = 557863950, // ((VEHICLE_V71_START + 0X01E1) | VehiclePropertyType:INT32)
    DCT_FD4_DCT_SHFTNOTALWD_WARN = 557863951, // ((VEHICLE_V71_START + 0X01E2) | VehiclePropertyType:INT32)
    DCT_FD4_DRVWARNMSG = 557863952, // ((VEHICLE_V71_START + 0X01E3) | VehiclePropertyType:INT32)
    DCT_FD4_DRVFAIMSG = 557863953, // ((VEHICLE_V71_START + 0X01E4) | VehiclePropertyType:INT32)
    DCT_FD4_DRVAUTOGEARDISP = 557863954, // ((VEHICLE_V71_START + 0X01E5) | VehiclePropertyType:INT32)
    DCT_FD4_ROLLINGCOUNTER_DCT7 = 557863955, // ((VEHICLE_V71_START + 0X01E6) | VehiclePropertyType:INT32)
    DCT_FD4_FRESHNESSVALUE_DCT_FD4 = 557863956, // ((VEHICLE_V71_START + 0X01E7) | VehiclePropertyType:INT32)
    DCT_FD4_MAC_CHECK_DCT_FD4 = 558912533, // ((VEHICLE_V71_START + 0X01E8) | VehiclePropertyType:INT64)
    EPS_FD1_CHECKSUM_EPS1 = 557863958, // ((VEHICLE_V71_START + 0X01E9) | VehiclePropertyType:INT32)
    EPS_FD1_EPS_FAILSTS = 557863959, // ((VEHICLE_V71_START + 0X01EA) | VehiclePropertyType:INT32)
    EPS_FD1_EPS_STEERMOD = 557863960, // ((VEHICLE_V71_START + 0X01EB) | VehiclePropertyType:INT32)
    EPS_FD1_STEERCORRNRMN = 557863961, // ((VEHICLE_V71_START + 0X01EC) | VehiclePropertyType:INT32)
    EPS_FD1_STEERCORRNRESP = 557863962, // ((VEHICLE_V71_START + 0X01ED) | VehiclePropertyType:INT32)
    EPS_FD1_ROLLINGCOUNTER_EPS1 = 557863963, // ((VEHICLE_V71_START + 0X01EE) | VehiclePropertyType:INT32)
    EPS_FD1_FRESHNESSVALUE_EPS_FD1 = 557863964, // ((VEHICLE_V71_START + 0X01EF) | VehiclePropertyType:INT32)
    EPS_FD1_MAC_CHECK_EPS_FD1 = 558912541, // ((VEHICLE_V71_START + 0X01F0) | VehiclePropertyType:INT64)
    ESP_FD2_CHECKSUM_ESP2 = 557863966, // ((VEHICLE_V71_START + 0X01F1) | VehiclePropertyType:INT32)
    ESP_FD2_HDCFAULT = 557863967, // ((VEHICLE_V71_START + 0X01F2) | VehiclePropertyType:INT32)
    ESP_FD2_HDCCTRL = 557863968, // ((VEHICLE_V71_START + 0X01F3) | VehiclePropertyType:INT32)
    ESP_FD2_AVHERRSTS = 557863969, // ((VEHICLE_V71_START + 0X01F4) | VehiclePropertyType:INT32)
    ESP_FD2_AVHSTS = 557863970, // ((VEHICLE_V71_START + 0X01F5) | VehiclePropertyType:INT32)
    ESP_FD2_CDPACTV = 557863971, // ((VEHICLE_V71_START + 0X01F6) | VehiclePropertyType:INT32)
    ESP_FD2_DRIVINGMODDIS = 557863972, // ((VEHICLE_V71_START + 0X01F7) | VehiclePropertyType:INT32)
    ESP_FD2_ROLLINGCOUNTER_ESP2 = 557863973, // ((VEHICLE_V71_START + 0X01F8) | VehiclePropertyType:INT32)
    ESP_FD2_CHECKSUM_ESP1 = 557863974, // ((VEHICLE_V71_START + 0X01F9) | VehiclePropertyType:INT32)
    ESP_FD2_ESPFUNCOFFSTS = 557863975, // ((VEHICLE_V71_START + 0X01FA) | VehiclePropertyType:INT32)
    ESP_FD2_IMC_ACTV = 557863976, // ((VEHICLE_V71_START + 0X01FB) | VehiclePropertyType:INT32)
    ESP_FD2_ROLLINGCOUNTER_ESP1 = 557863977, // ((VEHICLE_V71_START + 0X01FC) | VehiclePropertyType:INT32)
    ESP_FD2_CHECKSUM_ABS3 = 557863978, // ((VEHICLE_V71_START + 0X01FD) | VehiclePropertyType:INT32)
    ESP_FD2_ABSFAILSTS = 557863979, // ((VEHICLE_V71_START + 0X01FE) | VehiclePropertyType:INT32)
    ESP_FD2_EBDFAILSTS = 557863980, // ((VEHICLE_V71_START + 0X01FF) | VehiclePropertyType:INT32)
    ESP_FD2_ROLLINGCOUNTER_ABS3 = 557863981, // ((VEHICLE_V71_START + 0X0200) | VehiclePropertyType:INT32)
    ESP_FD2_FRESHNESSVALUE_ESP_FD2 = 557863982, // ((VEHICLE_V71_START + 0X0201) | VehiclePropertyType:INT32)
    ESP_FD2_MAC_CHECK_ESP_FD2 = 558912559, // ((VEHICLE_V71_START + 0X0202) | VehiclePropertyType:INT64)
    EPB1_CHECKSUM_EPB1 = 557863984, // ((VEHICLE_V71_START + 0X0203) | VehiclePropertyType:INT32)
    EPB1_PRESSBRAKEPEDALPOINTOUT = 557863985, // ((VEHICLE_V71_START + 0X0204) | VehiclePropertyType:INT32)
    EPB1_EPBERRSTS = 557863986, // ((VEHICLE_V71_START + 0X0205) | VehiclePropertyType:INT32)
    EPB1_EPBSTS = 557863987, // ((VEHICLE_V71_START + 0X0206) | VehiclePropertyType:INT32)
    EPB1_EPBTEXTDISP = 557863988, // ((VEHICLE_V71_START + 0X0207) | VehiclePropertyType:INT32)
    EPB1_ROLLINGCOUNTER_EPB1 = 557863989, // ((VEHICLE_V71_START + 0X0208) | VehiclePropertyType:INT32)
    FCM1_CAMERAFAILSTS = 557863990, // ((VEHICLE_V71_START + 0X0209) | VehiclePropertyType:INT32)
    FCM1_WIDELIGHTOUTPUTREQ = 557863991, // ((VEHICLE_V71_START + 0X020A) | VehiclePropertyType:INT32)
    FCM1_DLPFUNCSTS = 557863992, // ((VEHICLE_V71_START + 0X020B) | VehiclePropertyType:INT32)
    HCU_HC3_HCU_BATTLOWTEMWARN = 557863993, // ((VEHICLE_V71_START + 0X020C) | VehiclePropertyType:INT32)
    HCU_HC3_HCU_BATTLOWHEATSTS = 557863994, // ((VEHICLE_V71_START + 0X020D) | VehiclePropertyType:INT32)
    HCU_HC3_HCU_BATTLOWTEMPWARN = 557863995, // ((VEHICLE_V71_START + 0X020E) | VehiclePropertyType:INT32)
    HCU_HC3_HCU_BATTLOWTEMPHEATSTS = 557863996, // ((VEHICLE_V71_START + 0X020F) | VehiclePropertyType:INT32)
    HCU_HC6_HCU_ACLINEDISCONNECT = 557863997, // ((VEHICLE_V71_START + 0X0210) | VehiclePropertyType:INT32)
    HCU_HC6_HCU_WORKSHOPMODE = 557863998, // ((VEHICLE_V71_START + 0X0211) | VehiclePropertyType:INT32)
    HCU_HC6_HCU_INTELBATTTEMPMAGSTS = 557863999, // ((VEHICLE_V71_START + 0X0212) | VehiclePropertyType:INT32)
    HCU_HC8_HCU_TOTALSCORE = 557864000, // ((VEHICLE_V71_START + 0X0213) | VehiclePropertyType:INT32)
    HCU_HC16_HCU_POWERTRAINSTS = 557864001, // ((VEHICLE_V71_START + 0X0214) | VehiclePropertyType:INT32)
    HCU_HC16_TGTRCPRTNTRQSTS = 557864002, // ((VEHICLE_V71_START + 0X0215) | VehiclePropertyType:INT32)
    BMS4_CHECKSUM_BMS4 = 557864003, // ((VEHICLE_V71_START + 0X0216) | VehiclePropertyType:INT32)
    BMS4_BMS_BATTPOWEERR = 557864004, // ((VEHICLE_V71_START + 0X0217) | VehiclePropertyType:INT32)
    BMS4_BMS_ERRCATEGORY = 557864005, // ((VEHICLE_V71_START + 0X0218) | VehiclePropertyType:INT32)
    BMS4_BMS_POWERDOWNCMPLT = 557864006, // ((VEHICLE_V71_START + 0X0219) | VehiclePropertyType:INT32)
    BMS4_ROLLINGCOUNTER_BMS4 = 557864007, // ((VEHICLE_V71_START + 0X021A) | VehiclePropertyType:INT32)
    BMS19_BMS_BATTKEEPTEMPSTS = 557864008, // ((VEHICLE_V71_START + 0X021B) | VehiclePropertyType:INT32)
    BMS30_BMS_SOCLOWWARN = 557864009, // ((VEHICLE_V71_START + 0X021C) | VehiclePropertyType:INT32)
    BMS66_BMS_SOCJUMPWARN_RM = 557864010, // ((VEHICLE_V71_START + 0X021D) | VehiclePropertyType:INT32)
    BMS66_BMS_PACKOVERCHRGWARN_RM = 557864011, // ((VEHICLE_V71_START + 0X021E) | VehiclePropertyType:INT32)
    BMS66_BMS_CHRGSTS_RM = 557864012, // ((VEHICLE_V71_START + 0X021F) | VehiclePropertyType:INT32)
    BMS66_BMS_ISOLATIONRVALUE_RM = 557864013, // ((VEHICLE_V71_START + 0X0220) | VehiclePropertyType:INT32)
    BMS66_BMS_PACKCURR_RM = 559961166, // ((VEHICLE_V71_START + 0X0221) | VehiclePropertyType:FLOAT)
    BMS66_BMS_SOCOVERWARN_RM = 557864015, // ((VEHICLE_V71_START + 0X0222) | VehiclePropertyType:INT32)
    BMS66_BMS_BATTOVERTEMPRANK_RM = 557864016, // ((VEHICLE_V71_START + 0X0223) | VehiclePropertyType:INT32)
    BMS66_BMS_BATTOVERVOLTRANK_RM = 557864017, // ((VEHICLE_V71_START + 0X0224) | VehiclePropertyType:INT32)
    BMS66_BMS_BATTSOCLOWRANK_RM = 557864018, // ((VEHICLE_V71_START + 0X0225) | VehiclePropertyType:INT32)
    BMS66_BMS_VCUISOLATEERR_RM = 557864019, // ((VEHICLE_V71_START + 0X0226) | VehiclePropertyType:INT32)
    BMS66_BMS_BATTUNDERVOLTRANK_RM = 557864020, // ((VEHICLE_V71_START + 0X0227) | VehiclePropertyType:INT32)
    BMS66_BMS_CELLDELTAVOLTRANK_RM = 557864021, // ((VEHICLE_V71_START + 0X0228) | VehiclePropertyType:INT32)
    BMS66_BMS_CELLOVERVOLTRANK_RM = 557864022, // ((VEHICLE_V71_START + 0X0229) | VehiclePropertyType:INT32)
    BMS66_BMS_CELLUNDERVOLTRANK_RM = 557864023, // ((VEHICLE_V71_START + 0X022A) | VehiclePropertyType:INT32)
    BMS66_BMS_DELTATEMPRANK_RM = 557864024, // ((VEHICLE_V71_START + 0X022B) | VehiclePropertyType:INT32)
    BMS66_BMS_PACKMISMATCHWARN_RM = 557864025, // ((VEHICLE_V71_START + 0X022C) | VehiclePropertyType:INT32)
    BMS66_BMS_PACKVOLT_RM = 559961178, // ((VEHICLE_V71_START + 0X022D) | VehiclePropertyType:FLOAT)
    HCU_FD1_TMCU_ACTROTSPD = 557864027, // ((VEHICLE_V71_START + 0X022E) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_ACCELPEDALPOSN_DIAGVALID = 557864028, // ((VEHICLE_V71_START + 0X022F) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_POWERTRAINSTS = 557864029, // ((VEHICLE_V71_START + 0X0230) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_RACELAUNCHSTS = 557864030, // ((VEHICLE_V71_START + 0X0231) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_RACELAUNCHAVAIL = 557864031, // ((VEHICLE_V71_START + 0X0232) | VehiclePropertyType:INT32)
    HCU_PT5_HCUIMMOAUTHRES = 557864032, // ((VEHICLE_V71_START + 0X0233) | VehiclePropertyType:INT32)
    HCU_PT5_HCU_ECODRVHABIT_INDCN = 559961185, // ((VEHICLE_V71_START + 0X0234) | VehiclePropertyType:FLOAT)
    HCU_PT5_HCU_TOTDRVREQPOWER = 557864034, // ((VEHICLE_V71_START + 0X0235) | VehiclePropertyType:INT32)
    HCU_PT5_HCU_AVAILPOWER_TM = 559961187, // ((VEHICLE_V71_START + 0X0236) | VehiclePropertyType:FLOAT)
    HCU_PT7_CHECKSUM_HCU_PT7 = 557864036, // ((VEHICLE_V71_START + 0X0237) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_1 = 557864037, // ((VEHICLE_V71_START + 0X0238) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_2 = 557864038, // ((VEHICLE_V71_START + 0X0239) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_3 = 557864039, // ((VEHICLE_V71_START + 0X023A) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_4 = 557864040, // ((VEHICLE_V71_START + 0X023B) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_5 = 557864041, // ((VEHICLE_V71_START + 0X023C) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_6 = 557864042, // ((VEHICLE_V71_START + 0X023D) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_HYBSYSERR_7 = 557864043, // ((VEHICLE_V71_START + 0X023E) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_12VSYSERR_1 = 557864044, // ((VEHICLE_V71_START + 0X023F) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_12VSYSERR_2 = 557864045, // ((VEHICLE_V71_START + 0X0240) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_12VSYSERR_3 = 557864046, // ((VEHICLE_V71_START + 0X0241) | VehiclePropertyType:INT32)
    HCU_PT7_PCU_ENRGFLOWIND = 557864047, // ((VEHICLE_V71_START + 0X0242) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_MODSWTFAIL = 557864048, // ((VEHICLE_V71_START + 0X0243) | VehiclePropertyType:INT32)
    HCU_PT7_HCU_OPDSTS = 557864049, // ((VEHICLE_V71_START + 0X0244) | VehiclePropertyType:INT32)
    HCU_PT7_ROLLINGCOUNTER_HCU_PT7 = 557864050, // ((VEHICLE_V71_START + 0X0245) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_HYDROGENREFILLINGSYSERR_1 = 557864051, // ((VEHICLE_V71_START + 0X0246) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_HYDROGENREFILLINGSYSERR_2 = 557864052, // ((VEHICLE_V71_START + 0X0247) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_HYDROGENREFILLINGSYSERR_3 = 557864053, // ((VEHICLE_V71_START + 0X0248) | VehiclePropertyType:INT32)
    HCU_PT8_HCU_HYDROGENREFILLINGSYSERR_4 = 557864054, // ((VEHICLE_V71_START + 0X0249) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_ENERSAVEPOWERMOD = 557864055, // ((VEHICLE_V71_START + 0X024A) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_SOCLOW_INDCN = 557864056, // ((VEHICLE_V71_START + 0X024B) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_REMOVECHRGLINE_INDCN = 557864057, // ((VEHICLE_V71_START + 0X024C) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_SHIFT_P_INDCN = 557864058, // ((VEHICLE_V71_START + 0X024D) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_CONNECTCHRGLINE_INDCN = 557864059, // ((VEHICLE_V71_START + 0X024E) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_EGYRECFB = 557864060, // ((VEHICLE_V71_START + 0X024F) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_ACSTSREQ = 557864061, // ((VEHICLE_V71_START + 0X0250) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_REFUREQ = 557864062, // ((VEHICLE_V71_START + 0X0251) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_OILTANKCOVERSTS = 557864063, // ((VEHICLE_V71_START + 0X0252) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_INHBCHRGINFO = 557864064, // ((VEHICLE_V71_START + 0X0253) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_GEARCHNGPORNTXT = 557864065, // ((VEHICLE_V71_START + 0X0254) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_OPDFAIL = 557864066, // ((VEHICLE_V71_START + 0X0255) | VehiclePropertyType:INT32)
    HCU_PT4_CARWASHMODE_STS = 557864067, // ((VEHICLE_V71_START + 0X0256) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_TOTALSCORE_P0 = 557864068, // ((VEHICLE_V71_START + 0X0257) | VehiclePropertyType:INT32)
    HCU_PT4_IMDTLYCHRGSTS = 557864069, // ((VEHICLE_V71_START + 0X0258) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_AGS_FAILURE = 557864070, // ((VEHICLE_V71_START + 0X0259) | VehiclePropertyType:INT32)
    HCU_HP5_HCU_PRSBRKWARN = 557864071, // ((VEHICLE_V71_START + 0X025A) | VehiclePropertyType:INT32)
    HCU_HP5_DRVMODDISP = 557864072, // ((VEHICLE_V71_START + 0X025B) | VehiclePropertyType:INT32)
    HCU_HP5_DRVMODDISPSTS = 557864073, // ((VEHICLE_V71_START + 0X025C) | VehiclePropertyType:INT32)
    HCU_HP5_DRVGEARDISPSTS = 557864074, // ((VEHICLE_V71_START + 0X025D) | VehiclePropertyType:INT32)
    HCU_HP5_HCU_SHFTNOTALWD_WARN = 557864075, // ((VEHICLE_V71_START + 0X025E) | VehiclePropertyType:INT32)
    HCU_HP5_DRVFAIMSG = 557864076, // ((VEHICLE_V71_START + 0X025F) | VehiclePropertyType:INT32)
    IBOOSTER_FD2_IB_WARN = 557864077, // ((VEHICLE_V71_START + 0X0260) | VehiclePropertyType:INT32)
    ESP11_ESP_HMI_WARNINGON = 557864078, // ((VEHICLE_V71_START + 0X0261) | VehiclePropertyType:INT32)
    ESP10_TGTRCPRTNTRQSTS = 557864079, // ((VEHICLE_V71_START + 0X0262) | VehiclePropertyType:INT32)
    DMD_L1_LEFTDMDDRVFAILSTS = 557864080, // ((VEHICLE_V71_START + 0X0263) | VehiclePropertyType:INT32)
    DMD_L1_LEFTAUXDRVFAILSTS = 557864081, // ((VEHICLE_V71_START + 0X0264) | VehiclePropertyType:INT32)
    DMD_R1_RIGHTDMDDRVFAILSTS = 557864082, // ((VEHICLE_V71_START + 0X0265) | VehiclePropertyType:INT32)
    DMD_R1_RIGHTAUXDRVFAILSTS = 557864083, // ((VEHICLE_V71_START + 0X0266) | VehiclePropertyType:INT32)
    PPMI1_PPMIERRSTS = 557864084, // ((VEHICLE_V71_START + 0X0267) | VehiclePropertyType:INT32)
    TMCU_FD3_TMCU_ACTSPD_RM = 557864085, // ((VEHICLE_V71_START + 0X0268) | VehiclePropertyType:INT32)
    BMS19_BMS_INTELENGIDLCHRGNSTS = 557864086, // ((VEHICLE_V71_START + 0X0269) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_SOCJUMPWARN_RM = 557864087, // ((VEHICLE_V71_START + 0X026A) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_PACKOVERCHRGWARN_RM = 557864088, // ((VEHICLE_V71_START + 0X026B) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_ISOLATIONRVALUE_RM = 557864089, // ((VEHICLE_V71_START + 0X026C) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_PACKCURR_RM = 559961242, // ((VEHICLE_V71_START + 0X026D) | VehiclePropertyType:FLOAT)
    BMS_FD12_BMS_SOCOVERWARN_RM = 557864091, // ((VEHICLE_V71_START + 0X026E) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_BATTOVERTEMPRANK_RM = 557864092, // ((VEHICLE_V71_START + 0X026F) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_BATTOVERVOLTRANK_RM = 557864093, // ((VEHICLE_V71_START + 0X0270) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_BATTSOCLOWRANK_RM = 557864094, // ((VEHICLE_V71_START + 0X0271) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_VCUISOLATEERR_RM = 557864095, // ((VEHICLE_V71_START + 0X0272) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_BATTUNDERVOLTRANK_RM = 557864096, // ((VEHICLE_V71_START + 0X0273) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_CELLDELTAVOLTRANK_RM = 557864097, // ((VEHICLE_V71_START + 0X0274) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_CELLOVERVOLTRANK_RM = 557864098, // ((VEHICLE_V71_START + 0X0275) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_CELLUNDERVOLTRANK_RM = 557864099, // ((VEHICLE_V71_START + 0X0276) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_DELTATEMPRANK_RM = 557864100, // ((VEHICLE_V71_START + 0X0277) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_PACKMISMATCHWARN_RM = 557864101, // ((VEHICLE_V71_START + 0X0278) | VehiclePropertyType:INT32)
    BMS_FD12_BMS_PACKVOLT_RM = 559961254, // ((VEHICLE_V71_START + 0X0279) | VehiclePropertyType:FLOAT)
    BMS_FD14_BMS_BATTSOCLIMRESP = 557864103, // ((VEHICLE_V71_START + 0X027A) | VehiclePropertyType:INT32)
    ACM1_ACM_PARKERRCATEGORY = 557864104, // ((VEHICLE_V71_START + 0X027B) | VehiclePropertyType:INT32)
    ACM1_ACM_ERRCAGY = 557864105, // ((VEHICLE_V71_START + 0X027C) | VehiclePropertyType:INT32)
    TMCU6_TMCU_ACTPOSROTSPD = 557864106, // ((VEHICLE_V71_START + 0X027D) | VehiclePropertyType:INT32)
    DHT_FD1_TCU_ERRCATEGORY = 557864107, // ((VEHICLE_V71_START + 0X027E) | VehiclePropertyType:INT32)
    DHT_FD1_TCU_CLUTCHHOTWARNING = 557864108, // ((VEHICLE_V71_START + 0X027F) | VehiclePropertyType:INT32)
    IDC_L3_FD3_CHECKSUM_IDC_L3_2 = 557864109, // ((VEHICLE_V71_START + 0X0280) | VehiclePropertyType:INT32)
    IDC_L3_FD3_HWP_OPERSTS = 557864110, // ((VEHICLE_V71_START + 0X0281) | VehiclePropertyType:INT32)
    IDC_L3_FD3_EGOVEHSPD = 559961263, // ((VEHICLE_V71_START + 0X0282) | VehiclePropertyType:FLOAT)
    IDC_L3_FD3_HWP_SPDMAXLIM = 559961264, // ((VEHICLE_V71_START + 0X0283) | VehiclePropertyType:FLOAT)
    IDC_L3_FD3_HWP_CRSSPDSETVAL = 557864113, // ((VEHICLE_V71_START + 0X0284) | VehiclePropertyType:INT32)
    IDC_L3_FD3_HWP_VEHSPDUNIT = 557864114, // ((VEHICLE_V71_START + 0X0285) | VehiclePropertyType:INT32)
    IDC_L3_FD3_HWP_OPERSTSDECISION = 557864115, // ((VEHICLE_V71_START + 0X0286) | VehiclePropertyType:INT32)
    IDC_L3_FD3_HWP_TEXTWARN = 557864116, // ((VEHICLE_V71_START + 0X0287) | VehiclePropertyType:INT32)
    IDC_L3_FD3_HWP_CHIMEWARN = 557864117, // ((VEHICLE_V71_START + 0X0288) | VehiclePropertyType:INT32)
    IDC_L3_FD3_ROLLINGCOUNTER_IDC_L3_2 = 557864118, // ((VEHICLE_V71_START + 0X0289) | VehiclePropertyType:INT32)
    IDC_L3_FD3_FRESHNESSVALUE_IDC_L3_FD3 = 557864119, // ((VEHICLE_V71_START + 0X028A) | VehiclePropertyType:INT32)
    IDC_L3_FD3_MAC_CHECK_IDC_L3_FD3 = 558912696, // ((VEHICLE_V71_START + 0X028B) | VehiclePropertyType:INT64)
    FCU_FD1_FCU_STS = 557864121, // ((VEHICLE_V71_START + 0X028C) | VehiclePropertyType:INT32)
    FCU_FD1_FCU_DEIONIZERMNTNFLG = 557864122, // ((VEHICLE_V71_START + 0X028D) | VehiclePropertyType:INT32)
    FCU_FD1_FCU_POWERLIMFLG = 557864123, // ((VEHICLE_V71_START + 0X028E) | VehiclePropertyType:INT32)
    FCU_FD1_FCU_HYDROGENLEAKAGELVL = 557864124, // ((VEHICLE_V71_START + 0X028F) | VehiclePropertyType:INT32)
    FCU_FD1_FCU_STADISP = 557864125, // ((VEHICLE_V71_START + 0X0290) | VehiclePropertyType:INT32)
    HMS_FD1_HMS_ERRLVL = 557864126, // ((VEHICLE_V71_START + 0X0291) | VehiclePropertyType:INT32)
    HMS_FD2_HMS_TANKTOTSTATEOFCHARGE = 559961279, // ((VEHICLE_V71_START + 0X0292) | VehiclePropertyType:FLOAT)
    HMS_FD3_HMS_LEAKAGEERRLVL = 557864128, // ((VEHICLE_V71_START + 0X0293) | VehiclePropertyType:INT32)
    HMS_FD3_HMS_TANKPRES = 557864129, // ((VEHICLE_V71_START + 0X0294) | VehiclePropertyType:INT32)
    HMS_FD3_HMS_TANKTEMPER = 557864130, // ((VEHICLE_V71_START + 0X0295) | VehiclePropertyType:INT32)
    GLO_NASS1_E_CALLSTS = 557864131, // ((VEHICLE_V71_START + 0X0296) | VehiclePropertyType:INT32)
    HUT_FD1_HUT_WADEMODESET = 557864132, // ((VEHICLE_V71_START + 0X0297) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ0 = 557864133, // ((VEHICLE_V71_START + 0X0298) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ1 = 557864134, // ((VEHICLE_V71_START + 0X0299) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ2 = 557864135, // ((VEHICLE_V71_START + 0X029A) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ3 = 557864136, // ((VEHICLE_V71_START + 0X029B) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ4 = 557864137, // ((VEHICLE_V71_START + 0X029C) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ5 = 557864138, // ((VEHICLE_V71_START + 0X029D) | VehiclePropertyType:INT32)
    HUT40_MUSICFRQ6 = 557864139, // ((VEHICLE_V71_START + 0X029E) | VehiclePropertyType:INT32)
    HUT40_ALCMPARTITIONSWT = 557864140, // ((VEHICLE_V71_START + 0X029F) | VehiclePropertyType:INT32)
    HUT40_ALCMMASTERSWT = 557864141, // ((VEHICLE_V71_START + 0X02A0) | VehiclePropertyType:INT32)
    HUT13_EMGYVIDEOUPLOADSWTSTS = 557864142, // ((VEHICLE_V71_START + 0X02A1) | VehiclePropertyType:INT32)
    HUT33_P2P_PRKGDIRECTSWTCMD = 557864143, // ((VEHICLE_V71_START + 0X02A2) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_PATH_NAMENUM = 557864144, // ((VEHICLE_V71_START + 0X02A3) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_RETURN_BTN = 557864145, // ((VEHICLE_V71_START + 0X02A4) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_SELFPARLOT_ENDING = 557864146, // ((VEHICLE_V71_START + 0X02A5) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_COMFIRM_BTN = 557864147, // ((VEHICLE_V71_START + 0X02A6) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_LEARNING_SET_PARKLOT = 557864148, // ((VEHICLE_V71_START + 0X02A7) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_SET_PATH_ENDING = 557864149, // ((VEHICLE_V71_START + 0X02A8) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_LEARNINGTYPESELE = 557864150, // ((VEHICLE_V71_START + 0X02A9) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_PATH_DELETE = 557864151, // ((VEHICLE_V71_START + 0X02AA) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_PATH_SELECT = 557864152, // ((VEHICLE_V71_START + 0X02AB) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_HAVP_PATH_SETTOP = 557864153, // ((VEHICLE_V71_START + 0X02AC) | VehiclePropertyType:INT32)
    HUT_FD4_SLOTNUM_VR = 557864154, // ((VEHICLE_V71_START + 0X02AD) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_PAVP_CONFIRM = 557864155, // ((VEHICLE_V71_START + 0X02AE) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_PAVP_DROPOFF_AREA_REQ = 557864156, // ((VEHICLE_V71_START + 0X02AF) | VehiclePropertyType:INT32)
    HUT_FD4_HUT_PAVP_PATHLIST_REQ = 557864157, // ((VEHICLE_V71_START + 0X02B0) | VehiclePropertyType:INT32)
    HUT_FD4_USERDEFINEDPARKCMD = 557864158, // ((VEHICLE_V71_START + 0X02B1) | VehiclePropertyType:INT32)
    HUT_FD4_USERDEFPRKMENUSELCMD = 557864159, // ((VEHICLE_V71_START + 0X02B2) | VehiclePropertyType:INT32)
    HUT_FD5_HUT_EXTERNALDEVICEID = 558912736, // ((VEHICLE_V71_START + 0X02B3) | VehiclePropertyType:INT64)
    HUT_FD5_HUT_FUNCSET = 558912737, // ((VEHICLE_V71_START + 0X02B4) | VehiclePropertyType:INT64)
    DOCK1_DOCK_EXTERNALDEVICEID_P = 558912738, // ((VEHICLE_V71_START + 0X02B5) | VehiclePropertyType:INT64)
    DOCK1_DOCK_FUNCSTS_P = 558912739, // ((VEHICLE_V71_START + 0X02B6) | VehiclePropertyType:INT64)
    DOCK2_DOCK_HUT_EXTERNALDEVICEID_E = 558912740, // ((VEHICLE_V71_START + 0X02B7) | VehiclePropertyType:INT64)
    DOCK2_DOCK_HUT_FUNCSTS_E = 558912741, // ((VEHICLE_V71_START + 0X02B8) | VehiclePropertyType:INT64)
    AR_HUD1_HUD_VIBRATIONCORRNRESP = 557864166, // ((VEHICLE_V71_START + 0X02B9) | VehiclePropertyType:INT32)
    HUD1_HUD_MANUFACTOR = 557864167, // ((VEHICLE_V71_START + 0X02BA) | VehiclePropertyType:INT32)
    HUD1_HUD_RESLTYPE = 557864168, // ((VEHICLE_V71_START + 0X02BB) | VehiclePropertyType:INT32)
    HAP_FD1_HAVP_STARTBTN_ENA = 557864169, // ((VEHICLE_V71_START + 0X02BC) | VehiclePropertyType:INT32)
    HAP_FD1_HAVP_STOPBTN_ENA = 557864170, // ((VEHICLE_V71_START + 0X02BD) | VehiclePropertyType:INT32)
    HAP_FD1_HAVP_SELFPARLOTENDING_ENA = 557864171, // ((VEHICLE_V71_START + 0X02BE) | VehiclePropertyType:INT32)
    HAP_FD1_HAVP_WORKSTS = 557864172, // ((VEHICLE_V71_START + 0X02BF) | VehiclePropertyType:INT32)
    HAP_FD1_HAVP_FUNCSTS = 557864173, // ((VEHICLE_V71_START + 0X02C0) | VehiclePropertyType:INT32)
    HAP_FD1_PAVP_FUNCSTS = 557864174, // ((VEHICLE_V71_START + 0X02C1) | VehiclePropertyType:INT32)
    HAP_FD1_PAVP_TEXTDIS = 557864175, // ((VEHICLE_V71_START + 0X02C2) | VehiclePropertyType:INT32)
    HAP_FD1_PAVP_STATUS = 557864176, // ((VEHICLE_V71_START + 0X02C3) | VehiclePropertyType:INT32)
    HAP_FD1_PAVP_TEXTDIS2 = 557864177, // ((VEHICLE_V71_START + 0X02C4) | VehiclePropertyType:INT32)
    HAP_FD1_PARALLELSLOTBTN_ENA = 557864178, // ((VEHICLE_V71_START + 0X02C5) | VehiclePropertyType:INT32)
    HAP_FD1_VERTICALSLOTBTN_ENA = 557864179, // ((VEHICLE_V71_START + 0X02C6) | VehiclePropertyType:INT32)
    HAP_FD1_CLOCKWISEBTN_ENA = 557864180, // ((VEHICLE_V71_START + 0X02C7) | VehiclePropertyType:INT32)
    HAP_FD1_ANTICLOCKWISEBTN_ENA = 557864181, // ((VEHICLE_V71_START + 0X02C8) | VehiclePropertyType:INT32)
    HAP_FD1_SLOTCONFIRMBTN_ENA = 557864182, // ((VEHICLE_V71_START + 0X02C9) | VehiclePropertyType:INT32)
    HAP_FD3_FRNTCAMDIRTYSTS = 557864183, // ((VEHICLE_V71_START + 0X02CA) | VehiclePropertyType:INT32)
    HAP_FD3_REARCAMDIRTYSTS = 557864184, // ((VEHICLE_V71_START + 0X02CB) | VehiclePropertyType:INT32)
    HAP_FD3_LECAMDIRTYSTS = 557864185, // ((VEHICLE_V71_START + 0X02CC) | VehiclePropertyType:INT32)
    HAP_FD3_RICAMDIRTYSTS = 557864186, // ((VEHICLE_V71_START + 0X02CD) | VehiclePropertyType:INT32)
    HAP_FD3_DVRCAMDIRTYSTS = 557864187, // ((VEHICLE_V71_START + 0X02CE) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM1 = 557864188, // ((VEHICLE_V71_START + 0X02CF) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE1 = 557864189, // ((VEHICLE_V71_START + 0X02D0) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE1 = 557864190, // ((VEHICLE_V71_START + 0X02D1) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS1 = 557864191, // ((VEHICLE_V71_START + 0X02D2) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE1 = 557864192, // ((VEHICLE_V71_START + 0X02D3) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST1 = 557864193, // ((VEHICLE_V71_START + 0X02D4) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM2 = 557864194, // ((VEHICLE_V71_START + 0X02D5) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE2 = 557864195, // ((VEHICLE_V71_START + 0X02D6) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE2 = 557864196, // ((VEHICLE_V71_START + 0X02D7) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS2 = 557864197, // ((VEHICLE_V71_START + 0X02D8) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE2 = 557864198, // ((VEHICLE_V71_START + 0X02D9) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST2 = 557864199, // ((VEHICLE_V71_START + 0X02DA) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM3 = 557864200, // ((VEHICLE_V71_START + 0X02DB) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE3 = 557864201, // ((VEHICLE_V71_START + 0X02DC) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE3 = 557864202, // ((VEHICLE_V71_START + 0X02DD) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS3 = 557864203, // ((VEHICLE_V71_START + 0X02DE) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE3 = 557864204, // ((VEHICLE_V71_START + 0X02DF) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST3 = 557864205, // ((VEHICLE_V71_START + 0X02E0) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM4 = 557864206, // ((VEHICLE_V71_START + 0X02E1) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE4 = 557864207, // ((VEHICLE_V71_START + 0X02E2) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE4 = 557864208, // ((VEHICLE_V71_START + 0X02E3) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS4 = 557864209, // ((VEHICLE_V71_START + 0X02E4) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE4 = 557864210, // ((VEHICLE_V71_START + 0X02E5) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST4 = 557864211, // ((VEHICLE_V71_START + 0X02E6) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM5 = 557864212, // ((VEHICLE_V71_START + 0X02E7) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE5 = 557864213, // ((VEHICLE_V71_START + 0X02E8) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE5 = 557864214, // ((VEHICLE_V71_START + 0X02E9) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS5 = 557864215, // ((VEHICLE_V71_START + 0X02EA) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE5 = 557864216, // ((VEHICLE_V71_START + 0X02EB) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST5 = 557864217, // ((VEHICLE_V71_START + 0X02EC) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM6 = 557864218, // ((VEHICLE_V71_START + 0X02ED) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE6 = 557864219, // ((VEHICLE_V71_START + 0X02EE) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE6 = 557864220, // ((VEHICLE_V71_START + 0X02EF) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS6 = 557864221, // ((VEHICLE_V71_START + 0X02F0) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE6 = 557864222, // ((VEHICLE_V71_START + 0X02F1) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST6 = 557864223, // ((VEHICLE_V71_START + 0X02F2) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM7 = 557864224, // ((VEHICLE_V71_START + 0X02F3) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE7 = 557864225, // ((VEHICLE_V71_START + 0X02F4) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE7 = 557864226, // ((VEHICLE_V71_START + 0X02F5) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS7 = 557864227, // ((VEHICLE_V71_START + 0X02F6) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE7 = 557864228, // ((VEHICLE_V71_START + 0X02F7) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST7 = 557864229, // ((VEHICLE_V71_START + 0X02F8) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM8 = 557864230, // ((VEHICLE_V71_START + 0X02F9) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE8 = 557864231, // ((VEHICLE_V71_START + 0X02FA) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE8 = 557864232, // ((VEHICLE_V71_START + 0X02FB) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS8 = 557864233, // ((VEHICLE_V71_START + 0X02FC) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE8 = 557864234, // ((VEHICLE_V71_START + 0X02FD) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST8 = 557864235, // ((VEHICLE_V71_START + 0X02FE) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM9 = 557864236, // ((VEHICLE_V71_START + 0X02FF) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE9 = 557864237, // ((VEHICLE_V71_START + 0X0300) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE9 = 557864238, // ((VEHICLE_V71_START + 0X0301) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS9 = 557864239, // ((VEHICLE_V71_START + 0X0302) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE9 = 557864240, // ((VEHICLE_V71_START + 0X0303) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST9 = 557864241, // ((VEHICLE_V71_START + 0X0304) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHNUM10 = 557864242, // ((VEHICLE_V71_START + 0X0305) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHTYPE10 = 557864243, // ((VEHICLE_V71_START + 0X0306) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_LOTTYPE10 = 557864244, // ((VEHICLE_V71_START + 0X0307) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_STATUS10 = 557864245, // ((VEHICLE_V71_START + 0X0308) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PATH_LEARNRATE10 = 557864246, // ((VEHICLE_V71_START + 0X0309) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_PAHDST10 = 557864247, // ((VEHICLE_V71_START + 0X030A) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LEARNING_PATHSUM = 557864248, // ((VEHICLE_V71_START + 0X030B) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_LRNGDST = 557864249, // ((VEHICLE_V71_START + 0X030C) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_HUT_TEXTDISP = 557864250, // ((VEHICLE_V71_START + 0X030D) | VehiclePropertyType:INT32)
    HAP_FD7_HAVP_SCFAIL_INFO = 557864251, // ((VEHICLE_V71_START + 0X030E) | VehiclePropertyType:INT32)
    HAP_FD7_PAVP_SCFAIL_INFO = 557864252, // ((VEHICLE_V71_START + 0X030F) | VehiclePropertyType:INT32)
    HCU_HC13_HCU_H2CONTNSDISTANCE = 557864253, // ((VEHICLE_V71_START + 0X0310) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_BRKPEDALSTS = 557864254, // ((VEHICLE_V71_START + 0X0311) | VehiclePropertyType:INT32)
    HCU_FD1_HCU_BRKPEDALSTSVALID = 557864255, // ((VEHICLE_V71_START + 0X0312) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_BATTPOWERREDWARN = 557864256, // ((VEHICLE_V71_START + 0X0313) | VehiclePropertyType:INT32)
    HCU_PT4_HCU_WADEMODESETSTS = 557864257, // ((VEHICLE_V71_START + 0X0314) | VehiclePropertyType:INT32)
    HCU_HP5_HCU_ENRGFLOWIND_DHT = 557864258, // ((VEHICLE_V71_START + 0X0315) | VehiclePropertyType:INT32)
    BMS7_BMS_BATTSOCLIMRESP = 557864259, // ((VEHICLE_V71_START + 0X0316) | VehiclePropertyType:INT32)
    VEHICLE_V71_END = 553673260, // (VEHICLE_V71_START + 0X0FFF)
    VEHICLE_B01_START = 553673261, // (VEHICLE_V71_END + 0X0001)
    HUT_FD1_BRKPEDALFEELREQ = 557867594, // ((VEHICLE_B01_START + 0X001D) | VehiclePropertyType:INT32)
    HUT_FD1_CCOSWTREQ_HUT = 557867595, // ((VEHICLE_B01_START + 0X001E) | VehiclePropertyType:INT32)
    HUT2_FRNTWINHEATSWT = 557867596, // ((VEHICLE_B01_START + 0X001F) | VehiclePropertyType:INT32)
    HUT32_VOLLAMPDISPSTS = 557867597, // ((VEHICLE_B01_START + 0X0020) | VehiclePropertyType:INT32)
    HUT33_MBRMONRENREQ_TC = 557867598, // ((VEHICLE_B01_START + 0X0021) | VehiclePropertyType:INT32)
    T_BOX_FD4_T_BOX_REMTVIDEOMONRENAREQ_APP = 557867599, // ((VEHICLE_B01_START + 0X0022) | VehiclePropertyType:INT32)
    T_BOX_FD6_T_BOX_REMTLIMITSTART = 557867600, // ((VEHICLE_B01_START + 0X0023) | VehiclePropertyType:INT32)
    DVR_FD1_DVRREMTCNSTS = 557867601, // ((VEHICLE_B01_START + 0X0024) | VehiclePropertyType:INT32)
    DVR_FD1_DVR_360VIDEOCAPTUREREQ = 557867602, // ((VEHICLE_B01_START + 0X0025) | VehiclePropertyType:INT32)
    DVR_FD1_DVRSN = 558916179, // ((VEHICLE_B01_START + 0X0026) | VehiclePropertyType:INT64)
    EEM1_LIMITSTARTSTS = 557867604, // ((VEHICLE_B01_START + 0X0027) | VehiclePropertyType:INT32)
    IFC_FD2_ELKACTSTS = 557867605, // ((VEHICLE_B01_START + 0X0028) | VehiclePropertyType:INT32)
    IFC_FD2_INTELLIGENTEVAACTSTS = 557867606, // ((VEHICLE_B01_START + 0X0029) | VehiclePropertyType:INT32)
    IFC_FD2_LCKSTSINDCR = 557867607, // ((VEHICLE_B01_START + 0X002A) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_VEHHLDREQ_DVR = 557867608, // ((VEHICLE_B01_START + 0X002B) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_DRVOFF_REQ_DVR = 557867609, // ((VEHICLE_B01_START + 0X002C) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_MODSTS_DVR = 557867610, // ((VEHICLE_B01_START + 0X002D) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_DECEL_REQ_DVR = 557867611, // ((VEHICLE_B01_START + 0X002E) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_ENGTRQREQ_DVR = 559964764, // ((VEHICLE_B01_START + 0X002F) | VehiclePropertyType:FLOAT)
    ACC_FD2_LONGCTRL_ENGTRQREQACTIVE_DVR = 557867613, // ((VEHICLE_B01_START + 0X0030) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_DECELTOSTOPREQ_DVR = 557867614, // ((VEHICLE_B01_START + 0X0031) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_PARKSHIFTREQ_DVR = 557867615, // ((VEHICLE_B01_START + 0X0032) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_DECEL_REQVALUE_DVR = 559964768, // ((VEHICLE_B01_START + 0X0033) | VehiclePropertyType:FLOAT)
    ACC_FD2_LONGCTRL_HCUTRQREQACTIVE_DVR = 557867617, // ((VEHICLE_B01_START + 0X0034) | VehiclePropertyType:INT32)
    ACC_FD2_LONGCTRL_HCUTRQREQ_DVR = 559964770, // ((VEHICLE_B01_START + 0X0035) | VehiclePropertyType:FLOAT)
    ACC_FD2_ADAS_DRIVERINLOOP_MONITOR = 557867619, // ((VEHICLE_B01_START + 0X0036) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJEXIST = 559964772, // ((VEHICLE_B01_START + 0X0037) | VehiclePropertyType:FLOAT)
    ACC_FD4_AEBOBJVIDEOCFMD = 557867621, // ((VEHICLE_B01_START + 0X0038) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJVX = 559964774, // ((VEHICLE_B01_START + 0X0039) | VehiclePropertyType:FLOAT)
    ACC_FD4_AEBOBJCNTALIVE = 557867623, // ((VEHICLE_B01_START + 0X003A) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJDY = 559964776, // ((VEHICLE_B01_START + 0X003B) | VehiclePropertyType:FLOAT)
    ACC_FD4_AEBOBJSTATE = 557867625, // ((VEHICLE_B01_START + 0X003C) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJOBSTCL = 559964778, // ((VEHICLE_B01_START + 0X003D) | VehiclePropertyType:FLOAT)
    ACC_FD4_AEBOBJVY = 559964779, // ((VEHICLE_B01_START + 0X003E) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJEXIST = 559964780, // ((VEHICLE_B01_START + 0X003F) | VehiclePropertyType:FLOAT)
    ACC_FD4_AEBOBJAX = 559964781, // ((VEHICLE_B01_START + 0X0040) | VehiclePropertyType:FLOAT)
    ACC_FD4_AEBOBJAY = 559964782, // ((VEHICLE_B01_START + 0X0041) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJOBSTCL = 559964783, // ((VEHICLE_B01_START + 0X0042) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJDY = 559964784, // ((VEHICLE_B01_START + 0X0043) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJVX = 559964785, // ((VEHICLE_B01_START + 0X0044) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJCNTALIVE = 557867634, // ((VEHICLE_B01_START + 0X0045) | VehiclePropertyType:INT32)
    ACC_FD4_CYCLISTOBJVY = 559964787, // ((VEHICLE_B01_START + 0X0046) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJSTATE = 557867636, // ((VEHICLE_B01_START + 0X0047) | VehiclePropertyType:INT32)
    ACC_FD4_CYCLISTOBJVIDEOCFMD = 557867637, // ((VEHICLE_B01_START + 0X0048) | VehiclePropertyType:INT32)
    ACC_FD4_CYCLISTOBJAX = 559964790, // ((VEHICLE_B01_START + 0X0049) | VehiclePropertyType:FLOAT)
    ACC_FD4_CYCLISTOBJAY = 559964791, // ((VEHICLE_B01_START + 0X004A) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJEXIST = 559964792, // ((VEHICLE_B01_START + 0X004B) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJOBSTCL = 559964793, // ((VEHICLE_B01_START + 0X004C) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJDY = 559964794, // ((VEHICLE_B01_START + 0X004D) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJVX = 559964795, // ((VEHICLE_B01_START + 0X004E) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJVY = 559964796, // ((VEHICLE_B01_START + 0X004F) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJVIDEOCFMD = 557867645, // ((VEHICLE_B01_START + 0X0050) | VehiclePropertyType:INT32)
    ACC_FD4_ACCTGTOBJDX = 559964798, // ((VEHICLE_B01_START + 0X0051) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJAY = 559964799, // ((VEHICLE_B01_START + 0X0052) | VehiclePropertyType:FLOAT)
    ACC_FD4_PEDOBJCNTALIVE = 557867648, // ((VEHICLE_B01_START + 0X0053) | VehiclePropertyType:INT32)
    ACC_FD4_PEDOBJAX = 559964801, // ((VEHICLE_B01_START + 0X0054) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJID = 557867650, // ((VEHICLE_B01_START + 0X0055) | VehiclePropertyType:INT32)
    ACC_FD4_ACCTGTOBJOBSTCL = 559964803, // ((VEHICLE_B01_START + 0X0056) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJDY = 559964804, // ((VEHICLE_B01_START + 0X0057) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJEXIST = 559964805, // ((VEHICLE_B01_START + 0X0058) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJVY = 559964806, // ((VEHICLE_B01_START + 0X0059) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJVX = 559964807, // ((VEHICLE_B01_START + 0X005A) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJAX = 559964808, // ((VEHICLE_B01_START + 0X005B) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJAY = 559964809, // ((VEHICLE_B01_START + 0X005C) | VehiclePropertyType:FLOAT)
    ACC_FD4_ACCTGTOBJSTATE = 557867658, // ((VEHICLE_B01_START + 0X005D) | VehiclePropertyType:INT32)
    ACC_FD4_ACCTGTOBJVIDEOCFMD = 557867659, // ((VEHICLE_B01_START + 0X005E) | VehiclePropertyType:INT32)
    ACC_FD4_ACCTGTOBJCNTALIVE = 557867660, // ((VEHICLE_B01_START + 0X005F) | VehiclePropertyType:INT32)
    ACC_FD4_RADARHORMISALIGN = 559964813, // ((VEHICLE_B01_START + 0X0060) | VehiclePropertyType:FLOAT)
    ACC_FD4_HANDSOFFDETD = 557867662, // ((VEHICLE_B01_START + 0X0061) | VehiclePropertyType:INT32)
    ACC_FD4_DRVGONHIWAY = 557867663, // ((VEHICLE_B01_START + 0X0062) | VehiclePropertyType:INT32)
    ACC_FD4_ROADCRVT = 559964816, // ((VEHICLE_B01_START + 0X0063) | VehiclePropertyType:FLOAT)
    ACC_FD4_SPDLIMITDST = 557867665, // ((VEHICLE_B01_START + 0X0064) | VehiclePropertyType:INT32)
    ACC_FD4_ROADCRVTFAR = 559964818, // ((VEHICLE_B01_START + 0X0065) | VehiclePropertyType:FLOAT)
    ACC_FD4_RAMPDST = 557867667, // ((VEHICLE_B01_START + 0X0066) | VehiclePropertyType:INT32)
    ACC_FD4_SPDLIMIT = 557867668, // ((VEHICLE_B01_START + 0X0067) | VehiclePropertyType:INT32)
    ACC_FD4_CURVEDST = 557867669, // ((VEHICLE_B01_START + 0X0068) | VehiclePropertyType:INT32)
    ACC_FD4_NAVSPDLIMUNIT = 557867670, // ((VEHICLE_B01_START + 0X0069) | VehiclePropertyType:INT32)
    ACC_FD4_TUNNELDST = 557867671, // ((VEHICLE_B01_START + 0X006A) | VehiclePropertyType:INT32)
    ACC_FD4_DRVGONRILANE = 557867672, // ((VEHICLE_B01_START + 0X006B) | VehiclePropertyType:INT32)
    ACC_FD4_BRDGDST = 557867673, // ((VEHICLE_B01_START + 0X006C) | VehiclePropertyType:INT32)
    ACC_FD4_RADARDRVRATTENTIVE = 557867674, // ((VEHICLE_B01_START + 0X006D) | VehiclePropertyType:INT32)
    ACC_FD4_ACC_DATARECREQ = 557867675, // ((VEHICLE_B01_START + 0X006E) | VehiclePropertyType:INT32)
    ACC_FD4_ACC_SOSREQ = 557867676, // ((VEHICLE_B01_START + 0X006F) | VehiclePropertyType:INT32)
    ACC_FD4_PEDOBJSTATE = 557867677, // ((VEHICLE_B01_START + 0X0070) | VehiclePropertyType:INT32)
    ACC_FD4_MAPDATAVALID = 557867678, // ((VEHICLE_B01_START + 0X0071) | VehiclePropertyType:INT32)
    ACC_FD4_DRVRSTS = 557867679, // ((VEHICLE_B01_START + 0X0072) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJOVERLAP = 557867680, // ((VEHICLE_B01_START + 0X0073) | VehiclePropertyType:INT32)
    ACC_FD4_AEBOBJDRVBY = 557867681, // ((VEHICLE_B01_START + 0X0074) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_ABA_LEVEL = 557867682, // ((VEHICLE_B01_START + 0X0075) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_ABA_REQ = 557867683, // ((VEHICLE_B01_START + 0X0076) | VehiclePropertyType:INT32)
    AEB_FD1_EBA_TGTDECEL_REQ = 557867684, // ((VEHICLE_B01_START + 0X0077) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_VEHHLDREQ = 557867685, // ((VEHICLE_B01_START + 0X0078) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_TGTDECEL_REQ = 557867686, // ((VEHICLE_B01_START + 0X0079) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_TGTDECEL_REQVALUE = 559964839, // ((VEHICLE_B01_START + 0X007A) | VehiclePropertyType:FLOAT)
    AEB_FD1_AEB_AWB_REQ = 557867688, // ((VEHICLE_B01_START + 0X007B) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_AWB_LEVEL = 557867689, // ((VEHICLE_B01_START + 0X007C) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_ABP_REQ = 557867690, // ((VEHICLE_B01_START + 0X007D) | VehiclePropertyType:INT32)
    AEB_FD1_AEB_BRKREQFUNC = 557867691, // ((VEHICLE_B01_START + 0X007E) | VehiclePropertyType:INT32)
    ECM1_BRKPEDALSTS = 557867692, // ((VEHICLE_B01_START + 0X007F) | VehiclePropertyType:INT32)
    ECM9_ACCPEDALOVERRIDE = 557867693, // ((VEHICLE_B01_START + 0X0080) | VehiclePropertyType:INT32)
    ECM9_ECMRESPACC = 557867694, // ((VEHICLE_B01_START + 0X0081) | VehiclePropertyType:INT32)
    ECM9_ECMACCINTERFACE = 557867695, // ((VEHICLE_B01_START + 0X0082) | VehiclePropertyType:INT32)
    ECM10_TGTGEAR_UP = 557867696, // ((VEHICLE_B01_START + 0X0083) | VehiclePropertyType:INT32)
    ECM10_TGTGEAR_DOWN = 557867697, // ((VEHICLE_B01_START + 0X0084) | VehiclePropertyType:INT32)
    ECM2_ACCELPEDALPOSN = 559964850, // ((VEHICLE_B01_START + 0X0085) | VehiclePropertyType:FLOAT)
    ECM4_DRVENGTRQREQ = 559964851, // ((VEHICLE_B01_START + 0X0086) | VehiclePropertyType:FLOAT)
    ECM4_DRVENGTRQREQVLD = 557867700, // ((VEHICLE_B01_START + 0X0087) | VehiclePropertyType:INT32)
    ECM7_ENGTRQESTIMDACTGEAR = 559964853, // ((VEHICLE_B01_START + 0X0088) | VehiclePropertyType:FLOAT)
    ECM7_ENGTRQESTIMDACTGEAR_QF = 557867702, // ((VEHICLE_B01_START + 0X0089) | VehiclePropertyType:INT32)
    ECM8_TRQLOSS = 559964855, // ((VEHICLE_B01_START + 0X008A) | VehiclePropertyType:FLOAT)
    DCT3_TRSMTQIDLE = 559964856, // ((VEHICLE_B01_START + 0X008B) | VehiclePropertyType:FLOAT)
    DCT5_CURRENTGEARVALID = 557867705, // ((VEHICLE_B01_START + 0X008C) | VehiclePropertyType:INT32)
    DCT5_CURRENT_GEAR = 557867706, // ((VEHICLE_B01_START + 0X008D) | VehiclePropertyType:INT32)
    DCT5_SHIFT_IN_PROGRESS = 557867707, // ((VEHICLE_B01_START + 0X008E) | VehiclePropertyType:INT32)
    DCT5_SHIFTINPRGVALID = 557867708, // ((VEHICLE_B01_START + 0X008F) | VehiclePropertyType:INT32)
    DCT5_TARGETGEARVALID = 557867709, // ((VEHICLE_B01_START + 0X0090) | VehiclePropertyType:INT32)
    DCT5_TARGET_GEAR = 557867710, // ((VEHICLE_B01_START + 0X0091) | VehiclePropertyType:INT32)
    DCT7_FRESHNESSVALUE_DCT7 = 557867723, // ((VEHICLE_B01_START + 0X009E) | VehiclePropertyType:INT32)
    DCT7_MAC_CHECK_DCT7 = 558916300, // ((VEHICLE_B01_START + 0X009F) | VehiclePropertyType:INT64)
    ABS1_FLWHEELDRIVEDIRECTION = 557867725, // ((VEHICLE_B01_START + 0X00A0) | VehiclePropertyType:INT32)
    ABS1_FRWHEELDRIVEDIRECTION = 557867726, // ((VEHICLE_B01_START + 0X00A1) | VehiclePropertyType:INT32)
    ABS2_RLWHEELDRIVEDIRECTION = 557867727, // ((VEHICLE_B01_START + 0X00A2) | VehiclePropertyType:INT32)
    ABS2_RRWHEELDRIVEDIRECTION = 557867728, // ((VEHICLE_B01_START + 0X00A3) | VehiclePropertyType:INT32)
    ABS3_CHECKSUM_ABS3 = 557867729, // ((VEHICLE_B01_START + 0X00A4) | VehiclePropertyType:INT32)
    ABS3_ABSFAILSTS = 557867730, // ((VEHICLE_B01_START + 0X00A5) | VehiclePropertyType:INT32)
    ABS3_EBDFAILSTS = 557867731, // ((VEHICLE_B01_START + 0X00A6) | VehiclePropertyType:INT32)
    ABS3_VEHSPDVLD = 557867732, // ((VEHICLE_B01_START + 0X00A7) | VehiclePropertyType:INT32)
    ABS3_VEHSPD = 559964885, // ((VEHICLE_B01_START + 0X00A8) | VehiclePropertyType:FLOAT)
    ABS3_VEHODOINFOSTS = 557867734, // ((VEHICLE_B01_START + 0X00A9) | VehiclePropertyType:INT32)
    ABS3_VEHODOINFO = 557867735, // ((VEHICLE_B01_START + 0X00AA) | VehiclePropertyType:INT32)
    ABS3_ABSACTV = 557867736, // ((VEHICLE_B01_START + 0X00AB) | VehiclePropertyType:INT32)
    ABS3_EBDACTV = 557867737, // ((VEHICLE_B01_START + 0X00AC) | VehiclePropertyType:INT32)
    ABS3_ROLLINGCOUNTER_ABS3 = 557867738, // ((VEHICLE_B01_START + 0X00AD) | VehiclePropertyType:INT32)
    ESP1_CHECKSUM_ESP1 = 557867739, // ((VEHICLE_B01_START + 0X00AE) | VehiclePropertyType:INT32)
    ESP1_ESPFUNCOFFSTS = 557867740, // ((VEHICLE_B01_START + 0X00AF) | VehiclePropertyType:INT32)
    ESP1_ESPACTVINFOLMP = 557867741, // ((VEHICLE_B01_START + 0X00B0) | VehiclePropertyType:INT32)
    ESP1_ESPBRKLMPCTRL = 557867742, // ((VEHICLE_B01_START + 0X00B1) | VehiclePropertyType:INT32)
    ESP1_ESPFAILSTS = 557867743, // ((VEHICLE_B01_START + 0X00B2) | VehiclePropertyType:INT32)
    ESP1_ESP_MASTERCYLBRKPRESSVLD = 557867744, // ((VEHICLE_B01_START + 0X00B3) | VehiclePropertyType:INT32)
    ESP1_MSRACTV = 557867745, // ((VEHICLE_B01_START + 0X00B4) | VehiclePropertyType:INT32)
    ESP1_VDCACTV = 557867746, // ((VEHICLE_B01_START + 0X00B5) | VehiclePropertyType:INT32)
    ESP1_PTCACTV = 557867747, // ((VEHICLE_B01_START + 0X00B6) | VehiclePropertyType:INT32)
    ESP1_BTCACTV = 557867748, // ((VEHICLE_B01_START + 0X00B7) | VehiclePropertyType:INT32)
    ESP1_ESP_MASTERCYLBRKPRESS = 559964901, // ((VEHICLE_B01_START + 0X00B8) | VehiclePropertyType:FLOAT)
    ESP1_ROLLINGCOUNTER_ESP1 = 557867750, // ((VEHICLE_B01_START + 0X00B9) | VehiclePropertyType:INT32)
    ESP1_FRESHNESSVALUE_ESP1 = 557867751, // ((VEHICLE_B01_START + 0X00BA) | VehiclePropertyType:INT32)
    ESP1_MAC_CHECK_ESP1 = 558916328, // ((VEHICLE_B01_START + 0X00BB) | VehiclePropertyType:INT64)
    ESP2_CHECKSUM_ESP2 = 557867753, // ((VEHICLE_B01_START + 0X00BC) | VehiclePropertyType:INT32)
    ESP2_BRKDSKOVRHEATD = 557867754, // ((VEHICLE_B01_START + 0X00BD) | VehiclePropertyType:INT32)
    ESP2_NOBRKFORCE = 557867755, // ((VEHICLE_B01_START + 0X00BE) | VehiclePropertyType:INT32)
    ESP2_VEHSTANDSTILL = 557867756, // ((VEHICLE_B01_START + 0X00BF) | VehiclePropertyType:INT32)
    ESP2_AEBIBACTV = 557867757, // ((VEHICLE_B01_START + 0X00C0) | VehiclePropertyType:INT32)
    ESP2_AEBBAACTV = 557867758, // ((VEHICLE_B01_START + 0X00C1) | VehiclePropertyType:INT32)
    ESP2_AEBAVAILABLE = 557867759, // ((VEHICLE_B01_START + 0X00C2) | VehiclePropertyType:INT32)
    ESP2_ABPAVAILABLE = 557867760, // ((VEHICLE_B01_START + 0X00C3) | VehiclePropertyType:INT32)
    ESP2_ABPACTV = 557867761, // ((VEHICLE_B01_START + 0X00C4) | VehiclePropertyType:INT32)
    ESP2_ABAAVAILABLE = 557867762, // ((VEHICLE_B01_START + 0X00C5) | VehiclePropertyType:INT32)
    ESP2_ABAACTV = 557867763, // ((VEHICLE_B01_START + 0X00C6) | VehiclePropertyType:INT32)
    ESP2_CDDACTV = 557867764, // ((VEHICLE_B01_START + 0X00C7) | VehiclePropertyType:INT32)
    ESP2_CDDAVAILABLE = 557867765, // ((VEHICLE_B01_START + 0X00C8) | VehiclePropertyType:INT32)
    ESP2_AWBAVAILABLE = 557867766, // ((VEHICLE_B01_START + 0X00C9) | VehiclePropertyType:INT32)
    ESP2_AWBACTV = 557867767, // ((VEHICLE_B01_START + 0X00CA) | VehiclePropertyType:INT32)
    ESP2_HDCFAULT = 557867768, // ((VEHICLE_B01_START + 0X00CB) | VehiclePropertyType:INT32)
    ESP2_HDCCTRL = 557867769, // ((VEHICLE_B01_START + 0X00CC) | VehiclePropertyType:INT32)
    ESP2_AVHERRSTS = 557867770, // ((VEHICLE_B01_START + 0X00CD) | VehiclePropertyType:INT32)
    ESP2_AVHSTS = 557867771, // ((VEHICLE_B01_START + 0X00CE) | VehiclePropertyType:INT32)
    ESP2_CDPACTV = 557867772, // ((VEHICLE_B01_START + 0X00CF) | VehiclePropertyType:INT32)
    ESP2_DRIVINGMODREQ_ESP = 557867774, // ((VEHICLE_B01_START + 0X00D1) | VehiclePropertyType:INT32)
    ESP2_TAB_AVAILABLE = 557867775, // ((VEHICLE_B01_START + 0X00D2) | VehiclePropertyType:INT32)
    ESP2_TAB_STS = 557867776, // ((VEHICLE_B01_START + 0X00D3) | VehiclePropertyType:INT32)
    ESP2_ROLLINGCOUNTER_ESP2 = 557867777, // ((VEHICLE_B01_START + 0X00D4) | VehiclePropertyType:INT32)
    ESP2_FRESHNESSVALUE_ESP2 = 557867778, // ((VEHICLE_B01_START + 0X00D5) | VehiclePropertyType:INT32)
    ESP2_MAC_CHECK_ESP2 = 558916355, // ((VEHICLE_B01_START + 0X00D6) | VehiclePropertyType:INT64)
    ESP_FD2_BRKDSKOVRHEATD = 557867780, // ((VEHICLE_B01_START + 0X00D7) | VehiclePropertyType:INT32)
    ESP_FD2_NOBRKFORCE = 557867781, // ((VEHICLE_B01_START + 0X00D8) | VehiclePropertyType:INT32)
    ESP_FD2_VEHSTANDSTILL = 557867782, // ((VEHICLE_B01_START + 0X00D9) | VehiclePropertyType:INT32)
    ESP_FD2_AEBIBACTV = 557867783, // ((VEHICLE_B01_START + 0X00DA) | VehiclePropertyType:INT32)
    ESP_FD2_AEBBAACTV = 557867784, // ((VEHICLE_B01_START + 0X00DB) | VehiclePropertyType:INT32)
    ESP_FD2_AEBAVAILABLE = 557867785, // ((VEHICLE_B01_START + 0X00DC) | VehiclePropertyType:INT32)
    ESP_FD2_ABPAVAILABLE = 557867786, // ((VEHICLE_B01_START + 0X00DD) | VehiclePropertyType:INT32)
    ESP_FD2_ABPACTV = 557867787, // ((VEHICLE_B01_START + 0X00DE) | VehiclePropertyType:INT32)
    ESP_FD2_ABAAVAILABLE = 557867788, // ((VEHICLE_B01_START + 0X00DF) | VehiclePropertyType:INT32)
    ESP_FD2_ABAACTV = 557867789, // ((VEHICLE_B01_START + 0X00E0) | VehiclePropertyType:INT32)
    ESP_FD2_CDDACTV = 557867790, // ((VEHICLE_B01_START + 0X00E1) | VehiclePropertyType:INT32)
    ESP_FD2_CDDAVAILABLE = 557867791, // ((VEHICLE_B01_START + 0X00E2) | VehiclePropertyType:INT32)
    ESP_FD2_AWBAVAILABLE = 557867792, // ((VEHICLE_B01_START + 0X00E3) | VehiclePropertyType:INT32)
    ESP_FD2_AWBACTV = 557867793, // ((VEHICLE_B01_START + 0X00E4) | VehiclePropertyType:INT32)
    ESP_FD2_ESPBRKLMPCTRL = 557867794, // ((VEHICLE_B01_START + 0X00E5) | VehiclePropertyType:INT32)
    ESP_FD2_ESP_MASTERCYLBRKPRESSVLD = 557867795, // ((VEHICLE_B01_START + 0X00E6) | VehiclePropertyType:INT32)
    ESP_FD2_MSRACTV = 557867796, // ((VEHICLE_B01_START + 0X00E7) | VehiclePropertyType:INT32)
    ESP_FD2_VDCACTV = 557867797, // ((VEHICLE_B01_START + 0X00E8) | VehiclePropertyType:INT32)
    ESP_FD2_PTCACTV = 557867798, // ((VEHICLE_B01_START + 0X00E9) | VehiclePropertyType:INT32)
    ESP_FD2_BTCACTV = 557867799, // ((VEHICLE_B01_START + 0X00EA) | VehiclePropertyType:INT32)
    ESP_FD2_ESP_MASTERCYLBRKPRESS = 559964952, // ((VEHICLE_B01_START + 0X00EB) | VehiclePropertyType:FLOAT)
    ESP_FD2_EBDACTV = 557867801, // ((VEHICLE_B01_START + 0X00EC) | VehiclePropertyType:INT32)
    ESP_FD2_ABSACTV = 557867802, // ((VEHICLE_B01_START + 0X00ED) | VehiclePropertyType:INT32)
    ESP8_CCO_ACTIVE = 557867803, // ((VEHICLE_B01_START + 0X00EE) | VehiclePropertyType:INT32)
    ESP8_CCO_TGTVEL = 559964956, // ((VEHICLE_B01_START + 0X00EF) | VehiclePropertyType:FLOAT)
    ESP8_CCO_WARN = 557867805, // ((VEHICLE_B01_START + 0X00F0) | VehiclePropertyType:INT32)
    ESP8_ESPFUNCOFFSTS_MODE = 557867806, // ((VEHICLE_B01_START + 0X00F1) | VehiclePropertyType:INT32)
    ELD1_REARELDLCKSTS = 557867807, // ((VEHICLE_B01_START + 0X00F2) | VehiclePropertyType:INT32)
    BCM3_ANTITHEFTSTS = 557867808, // ((VEHICLE_B01_START + 0X00F3) | VehiclePropertyType:INT32)
    BCM3_HAZARDLMPSWTSTS = 557867809, // ((VEHICLE_B01_START + 0X00F4) | VehiclePropertyType:INT32)
    AC2_AC_FRNTWINHEATENASTS = 557867810, // ((VEHICLE_B01_START + 0X00F5) | VehiclePropertyType:INT32)
    AC2_AC_FRNTWINHEATDISLSTS = 557867811, // ((VEHICLE_B01_START + 0X00F6) | VehiclePropertyType:INT32)
    EPS1_EPS_FAILSTS = 557867812, // ((VEHICLE_B01_START + 0X00F7) | VehiclePropertyType:INT32)
    EPS1_EPS_TRQSNSRSTS = 557867813, // ((VEHICLE_B01_START + 0X00F8) | VehiclePropertyType:INT32)
    EPS1_CHECKSUM_EPS1 = 557867814, // ((VEHICLE_B01_START + 0X00F9) | VehiclePropertyType:INT32)
    EPS1_EPS_STEERMOD = 557867815, // ((VEHICLE_B01_START + 0X00FA) | VehiclePropertyType:INT32)
    EPS1_ROLLINGCOUNTER_EPS1 = 557867816, // ((VEHICLE_B01_START + 0X00FB) | VehiclePropertyType:INT32)
    EPS2_EPS_LKATORQOVRLDLVD = 559964969, // ((VEHICLE_B01_START + 0X00FC) | VehiclePropertyType:FLOAT)
    EPS2_EPS_LKATORQOVRLDLVDVLD = 557867818, // ((VEHICLE_B01_START + 0X00FD) | VehiclePropertyType:INT32)
    EPS2_EPS_LKATRQOVLDLVDSTS = 557867819, // ((VEHICLE_B01_START + 0X00FE) | VehiclePropertyType:INT32)
    EPS_FD1_EPS_TRQSNSRSTS = 557867820, // ((VEHICLE_B01_START + 0X00FF) | VehiclePropertyType:INT32)
    EPS_FD1_EPS_LKATORQOVRLDLVD = 559964973, // ((VEHICLE_B01_START + 0X0100) | VehiclePropertyType:FLOAT)
    EPS_FD1_EPS_LKATORQOVRLDLVDVLD = 557867822, // ((VEHICLE_B01_START + 0X0101) | VehiclePropertyType:INT32)
    EPS_FD1_EPS_LKATRQOVLDLVDSTS = 557867823, // ((VEHICLE_B01_START + 0X0102) | VehiclePropertyType:INT32)
    CSA1_LTURNLMPSWTSTS = 557867824, // ((VEHICLE_B01_START + 0X0103) | VehiclePropertyType:INT32)
    CSA1_RTURNLMPSWTSTS = 557867825, // ((VEHICLE_B01_START + 0X0104) | VehiclePropertyType:INT32)
    CSA2_STEERWHEELSPD = 559964978, // ((VEHICLE_B01_START + 0X0105) | VehiclePropertyType:FLOAT)
    CSA2_STEERWHEELSPDSIGN = 557867827, // ((VEHICLE_B01_START + 0X0106) | VehiclePropertyType:INT32)
    CSA2_SAS_STS = 557867828, // ((VEHICLE_B01_START + 0X0107) | VehiclePropertyType:INT32)
    CSA2_HWA_OK_OFFSWT = 557867829, // ((VEHICLE_B01_START + 0X0108) | VehiclePropertyType:INT32)
    CSA3_PHONESWTSTS = 557867830, // ((VEHICLE_B01_START + 0X0109) | VehiclePropertyType:INT32)
    CSA3_SOUNDSOURCEMODSWTSTS = 557867831, // ((VEHICLE_B01_START + 0X010A) | VehiclePropertyType:INT32)
    HALDEX1_ESTIMATEDTORQUE = 559964984, // ((VEHICLE_B01_START + 0X010B) | VehiclePropertyType:FLOAT)
    R_PBOX1_RTURNLMPFAILSTS = 557867833, // ((VEHICLE_B01_START + 0X010C) | VehiclePropertyType:INT32)
    R_PBOX1_LTURNLMPFAILSTS = 557867834, // ((VEHICLE_B01_START + 0X010D) | VehiclePropertyType:INT32)
    ABM1_CRASHOUTPUTSTS = 557867835, // ((VEHICLE_B01_START + 0X010E) | VehiclePropertyType:INT32)
    ABM2_VEHLGTACCEL = 559964988, // ((VEHICLE_B01_START + 0X010F) | VehiclePropertyType:FLOAT)
    ABM2_VEHLATACCEL = 559964989, // ((VEHICLE_B01_START + 0X0110) | VehiclePropertyType:FLOAT)
    ABM2_VEHYAWRATE = 559964990, // ((VEHICLE_B01_START + 0X0111) | VehiclePropertyType:FLOAT)
    ABM2_VEHLGTACCELVLD = 557867839, // ((VEHICLE_B01_START + 0X0112) | VehiclePropertyType:INT32)
    ABM2_VEHLATACCELVLD = 557867840, // ((VEHICLE_B01_START + 0X0113) | VehiclePropertyType:INT32)
    ABM2_VEHDYNYAWRATEVLD = 557867841, // ((VEHICLE_B01_START + 0X0114) | VehiclePropertyType:INT32)
    HAP_FD2_DVRWORKSTS = 557867842, // ((VEHICLE_B01_START + 0X0115) | VehiclePropertyType:INT32)
    DDCM1_DRVWINANTIPINCHSTS = 557867843, // ((VEHICLE_B01_START + 0X0116) | VehiclePropertyType:INT32)
    DDCM1_DRVSIDEREARANTIPINCHSTS = 557867844, // ((VEHICLE_B01_START + 0X0117) | VehiclePropertyType:INT32)
    PDCM1_PASSWINANTIPINCHSTS = 557867845, // ((VEHICLE_B01_START + 0X0118) | VehiclePropertyType:INT32)
    PDCM1_PASSSIDEREARWINANTIPINCHSTS = 557867846, // ((VEHICLE_B01_START + 0X0119) | VehiclePropertyType:INT32)
    ESCL2_ROTSTEERWHEEL = 557867847, // ((VEHICLE_B01_START + 0X011A) | VehiclePropertyType:INT32)
    STBS1_STBS_SOUNDREQ = 557867848, // ((VEHICLE_B01_START + 0X011B) | VehiclePropertyType:INT32)
    STBS1_VOLUPREQ_SC = 557867849, // ((VEHICLE_B01_START + 0X011C) | VehiclePropertyType:INT32)
    STBS1_VOLDWNREQ_SC = 557867850, // ((VEHICLE_B01_START + 0X011D) | VehiclePropertyType:INT32)
    STBS1_DISPTODREQ_SC = 557867851, // ((VEHICLE_B01_START + 0X011E) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFLCORNRAR1 = 557867852, // ((VEHICLE_B01_START + 0X011F) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFLCORNRAR2 = 557867853, // ((VEHICLE_B01_START + 0X0120) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFLMIDLAR1 = 557867854, // ((VEHICLE_B01_START + 0X0121) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFLMIDLAR2 = 557867855, // ((VEHICLE_B01_START + 0X0122) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFRMIDLAR1 = 557867856, // ((VEHICLE_B01_START + 0X0123) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFRMIDLAR2 = 557867857, // ((VEHICLE_B01_START + 0X0124) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFRCORNRAR1 = 557867858, // ((VEHICLE_B01_START + 0X0125) | VehiclePropertyType:INT32)
    FPAS1_FPAS_OBJFRCORNRAR2 = 557867859, // ((VEHICLE_B01_START + 0X0126) | VehiclePropertyType:INT32)
    FPAS1_FPAS_WORKSTS = 557867860, // ((VEHICLE_B01_START + 0X0127) | VehiclePropertyType:INT32)
    FPAS1_FPAS_AUTOMODSTS = 557867861, // ((VEHICLE_B01_START + 0X0128) | VehiclePropertyType:INT32)
    FPAS1_FPAS_SOUNDINDCN = 557867862, // ((VEHICLE_B01_START + 0X0129) | VehiclePropertyType:INT32)
    FPAS1_FPAS_NEARBRRDSTN = 557867863, // ((VEHICLE_B01_START + 0X012A) | VehiclePropertyType:INT32)
    IFC1_IFCTRQOVLCMDPROTNVALUE = 557867864, // ((VEHICLE_B01_START + 0X012B) | VehiclePropertyType:INT32)
    IFC1_IFCTRQOVLCMDREQVALUE = 559965017, // ((VEHICLE_B01_START + 0X012C) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJEXIST = 559965018, // ((VEHICLE_B01_START + 0X012D) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEBJAOBJID = 557867867, // ((VEHICLE_B01_START + 0X012E) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJEXIST = 559965020, // ((VEHICLE_B01_START + 0X012F) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJID = 557867869, // ((VEHICLE_B01_START + 0X0130) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJVIDEOCFMD = 557867870, // ((VEHICLE_B01_START + 0X0131) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJVX = 559965023, // ((VEHICLE_B01_START + 0X0132) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEBJAOBJDX = 559965024, // ((VEHICLE_B01_START + 0X0133) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJVIDEOCFMD = 557867873, // ((VEHICLE_B01_START + 0X0134) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJSTATE = 557867874, // ((VEHICLE_B01_START + 0X0135) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJCNTALIVE = 557867875, // ((VEHICLE_B01_START + 0X0136) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJDY = 559965028, // ((VEHICLE_B01_START + 0X0137) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJSTATE = 557867877, // ((VEHICLE_B01_START + 0X0138) | VehiclePropertyType:INT32)
    ACC_FD3_AEBJAOBJOBSTCL = 559965030, // ((VEHICLE_B01_START + 0X0139) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEBJAOBJVY = 559965031, // ((VEHICLE_B01_START + 0X013A) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEBJAOBJAX = 559965032, // ((VEHICLE_B01_START + 0X013B) | VehiclePropertyType:FLOAT)
    ACC_FD3_AEBJAOBJAY = 559965033, // ((VEHICLE_B01_START + 0X013C) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJOBSTCL = 559965034, // ((VEHICLE_B01_START + 0X013D) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJDY = 559965035, // ((VEHICLE_B01_START + 0X013E) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJDX = 559965036, // ((VEHICLE_B01_START + 0X013F) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJVX = 559965037, // ((VEHICLE_B01_START + 0X0140) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJVY = 559965038, // ((VEHICLE_B01_START + 0X0141) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJAY = 559965039, // ((VEHICLE_B01_START + 0X0142) | VehiclePropertyType:FLOAT)
    ACC_FD3_JAPEDOBJCNTALIVE = 557867888, // ((VEHICLE_B01_START + 0X0143) | VehiclePropertyType:INT32)
    ACC_FD3_JAPEDOBJAX = 559965041, // ((VEHICLE_B01_START + 0X0144) | VehiclePropertyType:FLOAT)
    DSM1_DRVSEATMASSGMODESTS = 557867890, // ((VEHICLE_B01_START + 0X0145) | VehiclePropertyType:INT32)
    DSM1_DRVSEATMASSGLVLSETSTS = 557867891, // ((VEHICLE_B01_START + 0X0146) | VehiclePropertyType:INT32)
    DSM1_DRVSEATLUMSMADJSTS = 557867892, // ((VEHICLE_B01_START + 0X0147) | VehiclePropertyType:INT32)
    HUT39_DRVSEATMASSGLVLSET = 557867893, // ((VEHICLE_B01_START + 0X0148) | VehiclePropertyType:INT32)
    HUT39_DRVSEATMASSGMODSET = 557867894, // ((VEHICLE_B01_START + 0X0149) | VehiclePropertyType:INT32)
    HUT26_DRVSEATSUPPORTPOSNSWT = 557867895, // ((VEHICLE_B01_START + 0X014A) | VehiclePropertyType:INT32)
    VMDR1_MBRMONRENASTS = 557867896, // ((VEHICLE_B01_START + 0X014B) | VehiclePropertyType:INT32)
    AMP1_CLARI_FIRESP = 557867897, // ((VEHICLE_B01_START + 0X014C) | VehiclePropertyType:INT32)
    CSA1_HORNSWTSTS = 557867898, // ((VEHICLE_B01_START + 0X014D) | VehiclePropertyType:INT32)
    HAP_FD1_MEB_AVHREQ = 557867899, // ((VEHICLE_B01_START + 0X014E) | VehiclePropertyType:INT32)
    VEHICLE_B01_END = 553677356, // (VEHICLE_B01_START + 0X0FFF)
    VEHICLE_V51_START = 553677357, // (VEHICLE_B01_END + 0X0001)
    AC1_ACCBNCLNENASTS = 557871662, // ((VEHICLE_V51_START + 0X0001) | VehiclePropertyType:INT32)
    AC1_ACCBNCLNOFFCODE = 557871663, // ((VEHICLE_V51_START + 0X0002) | VehiclePropertyType:INT32)
    AC5_L_EAHENASTS = 557871664, // ((VEHICLE_V51_START + 0X0003) | VehiclePropertyType:INT32)
    ACC_FD3_FRESHNESSVALUE_ACC_FD3 = 557871665, // ((VEHICLE_V51_START + 0X0004) | VehiclePropertyType:INT32)
    ACC_FD3_MAC_CHECK_ACC_FD3 = 558920242, // ((VEHICLE_V51_START + 0X0005) | VehiclePropertyType:INT64)
    BMS_FD1_BMS_STS = 557871667, // ((VEHICLE_V51_START + 0X0006) | VehiclePropertyType:INT32)
    BMS_FD1_BMS_BATTCURR = 559968820, // ((VEHICLE_V51_START + 0X0007) | VehiclePropertyType:FLOAT)
    BMS_FD1_BMS_BATTVOLT = 559968821, // ((VEHICLE_V51_START + 0X0008) | VehiclePropertyType:FLOAT)
    BMS_FD10_BMS_CHRGTIME = 557871670, // ((VEHICLE_V51_START + 0X0009) | VehiclePropertyType:INT32)
    BMS_FD10_BMS_DCCHRGCONNECT = 557871671, // ((VEHICLE_V51_START + 0X000A) | VehiclePropertyType:INT32)
    BMS_FD10_BMS_SAFETYCNSTS = 557871672, // ((VEHICLE_V51_START + 0X000B) | VehiclePropertyType:INT32)
    BMS_FD10_BMS_SYSERR = 557871673, // ((VEHICLE_V51_START + 0X000C) | VehiclePropertyType:INT32)
    BMS_FD10_BMS_BATTSOCLOWRANK = 557871674, // ((VEHICLE_V51_START + 0X000D) | VehiclePropertyType:INT32)
    DCDC_FD1_DCDC_FAILSTS = 557871675, // ((VEHICLE_V51_START + 0X000E) | VehiclePropertyType:INT32)
    E_PARK1_E_PARK_FAULTLMPSTS = 557871676, // ((VEHICLE_V51_START + 0X000F) | VehiclePropertyType:INT32)
    EPS_FD1_EPS_ASSIRAT = 557871677, // ((VEHICLE_V51_START + 0X0010) | VehiclePropertyType:INT32)
    ESCL2_UNLOCKRTRYPSHINDON = 557871678, // ((VEHICLE_V51_START + 0X0011) | VehiclePropertyType:INT32)
    HUT_FD1_HUT_VSGSOUNDREQ = 557871679, // ((VEHICLE_V51_START + 0X0012) | VehiclePropertyType:INT32)
    HUT10_HUT_ECOSPDLIM = 557871680, // ((VEHICLE_V51_START + 0X0013) | VehiclePropertyType:INT32)
    HUT25_ENGSPDDISPSWT_VR = 557871683, // ((VEHICLE_V51_START + 0X0016) | VehiclePropertyType:INT32)
    HUT30_ENGSPDDISPSWT = 557871684, // ((VEHICLE_V51_START + 0X0017) | VehiclePropertyType:INT32)
    IFC_FD2_FRESHNESSVALUE_FC_FD2 = 557871685, // ((VEHICLE_V51_START + 0X0018) | VehiclePropertyType:INT32)
    IFC_FD2_MAC_CHECK_FC_FD2 = 558920262, // ((VEHICLE_V51_START + 0X0019) | VehiclePropertyType:INT64)
    MCU_FD1_MCU_ACTROTSPD = 557871687, // ((VEHICLE_V51_START + 0X001A) | VehiclePropertyType:INT32)
    MCU_FD1_MCU_SYSERRSTS = 557871688, // ((VEHICLE_V51_START + 0X001B) | VehiclePropertyType:INT32)
    OBC_FD2_OBC_CCLINECONNECTSTS = 557871693, // ((VEHICLE_V51_START + 0X0020) | VehiclePropertyType:INT32)
    VCU_FD1_VCU_CRSSPD = 557871694, // ((VEHICLE_V51_START + 0X0021) | VehiclePropertyType:INT32)
    VCU_FD1_VCU_CRSCTRLSTS = 557871695, // ((VEHICLE_V51_START + 0X0022) | VehiclePropertyType:INT32)
    VCU_FD1_VCU_RACELAUNCHSTS = 557871696, // ((VEHICLE_V51_START + 0X0023) | VehiclePropertyType:INT32)
    VCU_FD1_VCU_RACELAUNCHAVLSTS = 557871697, // ((VEHICLE_V51_START + 0X0024) | VehiclePropertyType:INT32)
    VCU_FD3_VCU_WORDDISPINFO = 557871698, // ((VEHICLE_V51_START + 0X0025) | VehiclePropertyType:INT32)
    VCU_FD3_VCU_DRVMOD = 557871699, // ((VEHICLE_V51_START + 0X0026) | VehiclePropertyType:INT32)
    VCU_FD3_VCU_GEARSTSIP = 557871700, // ((VEHICLE_V51_START + 0X0027) | VehiclePropertyType:INT32)
    VCU_FD3_VCU_GEARSTS = 557871701, // ((VEHICLE_V51_START + 0X0028) | VehiclePropertyType:INT32)
    VCU_FD3_EGYRGNLVL = 557871702, // ((VEHICLE_V51_START + 0X0029) | VehiclePropertyType:INT32)
    VCU_FD3_VCU_ECOACLIMNSTS = 557871703, // ((VEHICLE_V51_START + 0X002A) | VehiclePropertyType:INT32)
    VCU_FD4_VCU_CHRGNSTS = 557871704, // ((VEHICLE_V51_START + 0X002B) | VehiclePropertyType:INT32)
    VCU_FD4_VCU_PWRLO = 557871705, // ((VEHICLE_V51_START + 0X002C) | VehiclePropertyType:INT32)
    VCU_FD4_VCU_COOLGCIRCFLT = 557871706, // ((VEHICLE_V51_START + 0X002D) | VehiclePropertyType:INT32)
    VCU_FD4_VCU_RDYLEDSTS = 557871707, // ((VEHICLE_V51_START + 0X002E) | VehiclePropertyType:INT32)
    VCU_FD4_VCU_BATTU = 559968860, // ((VEHICLE_V51_START + 0X002F) | VehiclePropertyType:FLOAT)
    VCU_FD4_VCU_HUTBATTKEEPTEMPSET = 557871709, // ((VEHICLE_V51_START + 0X0030) | VehiclePropertyType:INT32)
    VCU_FD4_VCU_SYSFLTLMP = 557871710, // ((VEHICLE_V51_START + 0X0031) | VehiclePropertyType:INT32)
    VCU_FD5_VCU_ECODRVHABIT_INDCN = 557871711, // ((VEHICLE_V51_START + 0X0032) | VehiclePropertyType:INT32)
    VCU_FD5_VCU_DRVSCODST = 557871712, // ((VEHICLE_V51_START + 0X0033) | VehiclePropertyType:INT32)
    VCU_FD5_VCU_AGSFAILRFLG = 557871713, // ((VEHICLE_V51_START + 0X0034) | VehiclePropertyType:INT32)
    VCU5_VCU_AUTHPASSED = 557871714, // ((VEHICLE_V51_START + 0X0035) | VehiclePropertyType:INT32)
    VCU5_VCU_PARKFLTLMP = 557871715, // ((VEHICLE_V51_START + 0X0036) | VehiclePropertyType:INT32)
    BMS_FD10_BMS_CHRGSTS = 557871716, // ((VEHICLE_V51_START + 0X0037) | VehiclePropertyType:INT32)
    BMS_FD10_BMS_SOE = 559968869, // ((VEHICLE_V51_START + 0X0038) | VehiclePropertyType:FLOAT)
    BMS_FD11_BMS_SOH = 559968870, // ((VEHICLE_V51_START + 0X0039) | VehiclePropertyType:FLOAT)
    MCU_FD1_MCU_OPERMOD = 557871719, // ((VEHICLE_V51_START + 0X003A) | VehiclePropertyType:INT32)
    OBC11_OBC_MODSTS_EV = 557871720, // ((VEHICLE_V51_START + 0X003B) | VehiclePropertyType:INT32)
    OBC11_OBC_CONNECTSTS_EV = 557871721, // ((VEHICLE_V51_START + 0X003C) | VehiclePropertyType:INT32)
    OBC12_OBC_V2VSTS = 557871722, // ((VEHICLE_V51_START + 0X003D) | VehiclePropertyType:INT32)
    OBC12_OBC_V2VINFO = 557871723, // ((VEHICLE_V51_START + 0X003E) | VehiclePropertyType:INT32)
    OBC12_OBC_DCHRGCONNECTSTS = 557871724, // ((VEHICLE_V51_START + 0X003F) | VehiclePropertyType:INT32)
    VCU_FD1_VCU_BRKPEDLSTS = 557871725, // ((VEHICLE_V51_START + 0X0040) | VehiclePropertyType:INT32)
    VCU_FD1_VCU_ACTACCRPEDLRAT = 559968878, // ((VEHICLE_V51_START + 0X0041) | VehiclePropertyType:FLOAT)
    VEHICLE_V51_END = 553681452, // (VEHICLE_V51_START + 0X0FFF)
    CSA3_BLISPOTVISSW,    
	DMS_FD1_DISTRCTNLVL_ACC,
	DMS_FD1_DROWSNSLVL_ACC,
	DMS_FD1_DMSSTS,
	DMS_FD1_HEADDETN,
	DMS_FD1_HEADYAWANG,
	DMS_FD1_HEADPITCHANG,
	DMS_FD1_HEADROLLANG,
	DMS_FD1_HEADPOSNX,
	DMS_FD1_HEADPOSNY,
	DMS_FD1_HEADPOSNZ,
	DMS_FD1_LEYEOPENSTS,
	DMS_FD1_LEYEOPENSTSQLTY,
	DMS_FD1_REYEOPENSTS,
	DMS_FD1_REYEOPENSTSQLTY,
	DMS_FD1_ROBUSTGAZEYAWANG,
	DMS_FD1_ROBUSTGAZEPITCHANG,
	DMS_FD1_DRVBEHVSTS,
	HUT_FD4_HUT_HAVP_APA_PERMIT,
	HUT_FD4_HUT_HAVP_SET_STARTING,
	BCM12_OMSWORKCMD,
	DMS_FD1_OMSALARMREQ,
	APS4_APS_APS_WORKSTS,
	HAP2_APA_APA_MENUDISPCTRLCMD,
	HAP1_FD1_HAP_SWTDISPCTRLCMD,
	AP2_APA_APA_MENUDISPCTRLCMD,
	HAP_FD1_APS_ESP_BRKDISTANCE,
};

enum class DCT_FD4_DrvModDisp : int32_t
{
    DCT7_DrvModDisp_0 = 0,
    DCT7_DrvModDisp_1 = 1,
    DCT7_DrvModDisp_2 = 2,
    DCT7_DrvModDisp_3 = 3,
    DCT7_DrvModDisp_4 = 4,
    DCT7_DrvModDisp_5 = 5,
};

enum class BCM1_DrvDoorSts : int32_t
{
    BCM1_DrvDoorSts_0 = 0,
    BCM1_DrvDoorSts_1 = 1,
};

enum class HUT15_ParkLmpSet : int32_t
{
    HUT15_ParkLmpSet_0 = 0,
    HUT15_ParkLmpSet_1 = 1,
};

enum class HUT15_SportModLightSet : int32_t
{
    HUT15_SportModLightSet_0 = 0,
    HUT15_SportModLightSet_1 = 1,
    HUT15_SportModLightSet_2 = 2,
    HUT15_SportModLightSet_3 = 3,
};

template<typename ENUM, typename U = typename std::underlying_type<ENUM>::type>
inline constexpr U toInt(ENUM const value)
{
    return static_cast<U>(value);
}

inline constexpr VehiclePropertyType getPropType(int32_t prop)
{
    return static_cast<VehiclePropertyType>(
               prop & toInt(VehiclePropertyType::MASK));
}

//#define toInt(x) static_cast<uint32_t>(x)
//template <typename T, std::size_t N>
//constexpr std::size_t countof(T const (&)[N]) noexcept
//{
//    return N;
//}

} // namespace vehicle
} // namespace platform
} // namespace bosch

#endif  //_TYPE_H_
