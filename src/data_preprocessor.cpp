//
// Created by Leot on 2021/3/30.
//

#include "types.hpp"
#include "utility.hpp"
//#include "demand_generator.hpp"

bool compare_request(Request req1, Request req2){
    return (req1.request_time_ms == req2.request_time_ms) && (req1.origin.lat == req2.origin.lat);
}

int PreprocessRequestDate(std::string path_to_data_file){
    std::string path_to_csv = path_to_data_file + ".csv";
    std::string path_to_bin = path_to_data_file + ".bin";
    auto request_data = LoadRequestsFromCsvFile(path_to_csv);

    WriteObjectVectorToBinary<Request>(request_data, path_to_bin);
    fmt::print("[INFO] Data has been saved to a binary file: {}\n", path_to_bin);

    auto s_time = GetTimeStamp();
    auto request_date_from_bin = ReadObjectVectorFromBinary<Request>(path_to_bin);
    auto duration = static_cast<double>(GetTimeStamp() - s_time) / 1000.0;
    if(std::equal(request_data.begin(), request_data.end(), request_date_from_bin.begin(), compare_request)){
        fmt::print("[DEBUG] ({}s) Sucessfully read request data from the binary file!\n", duration);
    }
}

bool compare_station(Pos sta1, Pos sta2){
    return (sta1.node_id == sta2.node_id) && (sta1.lat == sta2.lat);
}

int PreprocessStationDate(std::string path_to_data_file){
    std::string path_to_csv = path_to_data_file + ".csv";
    std::string path_to_bin = path_to_data_file + ".bin";
    auto station_data = LoadStationsFromCsvFile(path_to_csv);

    WriteObjectVectorToBinary<Pos>(station_data, path_to_bin);
    fmt::print("[INFO] Data has been saved to a binary file: {}\n", path_to_bin);

    auto s_time = GetTimeStamp();
    auto station_date_from_bin = ReadObjectVectorFromBinary<Pos>(path_to_bin);
    auto duration = static_cast<double>(GetTimeStamp() - s_time) / 1000.0;
    if(std::equal(station_data.begin(), station_data.end(), station_date_from_bin.begin(), compare_station)){
        fmt::print("[DEBUG] ({}s) Sucessfully read request data from the binary file!\n", duration);
    }
}

int main(int argc, const char *argv[]) {
//    PreprocessRequestDate("./datalog/taxi-data/manhattan-taxi-20160525");
//    PreprocessStationDate("./datalog/map-data/stations-101");

    std::string a = "test";
    fmt::print("a is {}\n", a);
    std::string b = std::move(a);
    fmt::print("a is {}\n", a);

}