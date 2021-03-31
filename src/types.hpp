/// \author Jian Wen
/// \date 2021/01/29

#pragma once

#include <yaml-cpp/yaml.h>

#include <osrm/json_container.hpp>
#include <string>

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Geo Types
//////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief Position encoded in longitude/latitude.
/// \details lon in [-180, 180), lat in [-90, 90]. Undefined behaviors if out of bound.
struct Pos {
    size_t node_id = 0;
    float lon = 0.0;
    float lat = 0.0;
};

/// \brief Step of route consisting of distance, duration and a vector of continuous positions.
struct Step {
    int32_t distance_mm = 0;
    int32_t duration_ms = 0;
    std::vector<Pos> poses;
};

/// \brief Leg of route consisting of total distance, total duration as well as a vector of steps.
struct Leg {
    int32_t distance_mm = 0;
    int32_t duration_ms = 0;
    std::vector<Step> steps;
};

/// \brief Route consisting of total distance, total duration as well as a vector of legs.
struct Route {
    int32_t distance_mm = 0;
    int32_t duration_ms = 0;
    std::vector<Leg> legs;
};

/// \brief The type of the routing call.
enum class RoutingType {
    UNDEFINED, // uninitialized value
    TIME_ONLY, // only return the total travel time and distance
    FULL_ROUTE // return the full route with detailed maneuvers
};

/// \brief The status of the routing response.
enum class RoutingStatus {
    UNDEFINED, // uninitialized value
    OK,        // good route
    EMPTY,     // empty route
    ERROR      // error
};

/// \brief The response from the routing engine.
struct RoutingResponse {
    RoutingStatus status = RoutingStatus::UNDEFINED; // the status
    std::string message = ""; // message from the router, in case it does not return a good route
    Route route;              // the route
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Order Types
//////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief The order request generated by the demand generator.
struct Request {
    Pos origin;
    Pos destination;
    uint64_t request_time_ms = 0;
    std::string request_time_date = "";
};

/// \brief The status of the order.
enum class OrderStatus {
    UNDEFINED,   // uninitialized value
    REQUESTED,   // the order has been generated by the demand generator
    DISPATCHED,  // the vehicle has been dispatched to serve the order
    PICKED_UP,   // the order has been picked up
    DROPPED_OFF, // the order has been dropped off, order completed
    WALKAWAY     // the order isn't served due to constraints in dispatch time/pickup time etc
};

inline std::string to_string(const OrderStatus &s) {
    if (s == OrderStatus::UNDEFINED) {
        return "UNDEFINED";
    } else if (s == OrderStatus::REQUESTED) {
        return "REQUESTED";
    } else if (s == OrderStatus::DISPATCHED) {
        return "DISPATCHED";
    } else if (s == OrderStatus::PICKED_UP) {
        return "PICKED_UP";
    } else if (s == OrderStatus::DROPPED_OFF) {
        return "DROPPED_OFF";
    } else if (s == OrderStatus::WALKAWAY) {
        return "WALKAWAY";
    }

    assert(false && "Bad OrderStatus type!");
}

/// \brief The order that the simulation managed, containing all relavant data.
struct Order {
    size_t id;
    Pos origin;
    Pos destination;
    OrderStatus status = OrderStatus::UNDEFINED;
    int32_t request_time_ms = 0;
    std::string request_time_date = "";
    int32_t max_pickup_time_ms = 0;
    int32_t pickup_time_ms = 0;
    int32_t dropoff_time_ms = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Vehicle Types
//////////////////////////////////////////////////////////////////////////////////////////////////

/// \brief The operation associated with a waypoint.
enum class WaypointOp {
    UNDEFINED, // uninitialized value
    PICKUP,    // we pick up a order at this waypoint
    DROPOFF,   // we drop off a order at this waypoint
};

/// \brief The waypoint represents a stop along the way when the vehicle serves orders.
struct Waypoint {
    Pos pos;
    WaypointOp op;
    size_t order_id;
    Route route;
};

/// \brief The vehicle type that holds dispatched orders and a schedule to serve them.
struct Vehicle {
    size_t id;
    Pos pos;
    size_t capacity = 1;
    size_t load = 0;
    std::vector<Waypoint> schedule = {};
    int32_t dist_traveled_mm = 0; // accumulated distance traveled in meters
    int32_t loaded_dist_traveled_mm = 0; // accumulated distance traveled, weighted by the load, in meters
};
