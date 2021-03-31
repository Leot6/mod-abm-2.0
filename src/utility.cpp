//
// Created by Leot on 2021/3/30.
//
#include "utility.hpp"
#include <iomanip>

int32_t ConvertTimeDateToSeconds(std::string time_date) {
    tm tm_time1;
    time_t time_sec1;
    tm_time1.tm_isdst = -1;
    const char *time_date1 = time_date.c_str();
    strptime(time_date1, "%Y-%m-%d %H:%M:%S", &tm_time1);
    time_sec1 = mktime(&tm_time1);
    int32_t converted_time_sec = time_sec1;
    return converted_time_sec;
}

int32_t ComputeTheAccumulatedSecondsFrom0Clock(std::string time_date) {
    std::string time_0_clock = time_date.substr(0,10) + "00:00:00";
    int32_t accumulated_sec = ConvertTimeDateToSeconds(time_date) - ConvertTimeDateToSeconds(time_0_clock);
    return accumulated_sec;
}

std::string ConvertTimeSecondToDate(int32_t time_sec){
    int32_t time_test = time_sec;
    time_t time_sec2;
    tm tm_time2;
    tm_time2.tm_isdst = -1;
    char time_date2[128]= {0};
    time_sec2 = time_test;
    tm_time2 = *localtime(&time_sec2);
    strftime(time_date2, 64, "%Y-%m-%d %H:%M:%S", &tm_time2);
    std::string converted_time_date = time_date2;
    return converted_time_date;
}

std::time_t GetTimeStamp(){
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp =
            std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    std::time_t timestamp = tp.time_since_epoch().count();
    return timestamp;
}

int CheckFileExistence(std::string& path_to_file) {
    struct stat buffer;
    if (stat(path_to_file.c_str(), &buffer) != 0){
        fmt::print("[ERROR] File \"{}\" does not exist! \n", path_to_file);
        exit(0);
    }
}

std::vector<Request> LoadRequestsFromCsvFile(std::string path_to_csv){
    CheckFileExistence(path_to_csv);
    auto s_time = GetTimeStamp();
    std::vector<Request> all_requests = {};
    std::ifstream taxi_data_csv(path_to_csv); //load the taxi data file
    std::string line;
    getline(taxi_data_csv,line);  // ignore the first line
    while (getline(taxi_data_csv,line)){  // read every line
        std::istringstream readstr(line); // string every line
        std::vector<std::string> data_line;
        std::string info;
        while (getline(readstr, info, ',')) {
            data_line.push_back(info);
        }
        Request request;
        request.request_time_ms = ComputeTheAccumulatedSecondsFrom0Clock(data_line[0]) * 1000;
        request.request_time_date = data_line[0];
        request.origin.node_id = std::stoi(data_line[2]);
        request.origin.lon = std::stod(data_line[3]);
        request.origin.lat = std::stod(data_line[4]);
        request.destination.node_id = std::stoi(data_line[5]);
        request.destination.lon = std::stod(data_line[6]);
        request.destination.lat = std::stod(data_line[7]);

        all_requests.emplace_back(request);
    }
    fmt::print("[DEBUG] ({}s) Load request data from {}, with {} requests.\n",
               double (GetTimeStamp() - s_time)/1000, path_to_csv, all_requests.size());
    return std::move(all_requests);
}

std::vector<Pos> LoadStationsFromCsvFile(std::string path_to_csv){
    CheckFileExistence(path_to_csv);
    auto s_time = GetTimeStamp();
    std::vector<Pos> all_stations = {};
    std::ifstream data_csv(path_to_csv); //load the station data file
    std::string line;
    getline(data_csv,line);  // ignore the first line
    while (getline(data_csv,line)){  // read every line
        std::istringstream readstr(line); // string every line
        std::vector<std::string> data_line;
        std::string info;
        while (getline(readstr, info, ',')) {
            data_line.push_back(info);
        }
        Pos station;
        station.node_id = std::stoi(data_line[0]);
        station.lon = std::stod(data_line[1]);
        station.lat = std::stod(data_line[2]);
        all_stations.emplace_back(station);
    }
    fmt::print("[DEBUG] ({}s) Load station data from {}, with {} stations.\n",
               double (GetTimeStamp() - s_time)/1000, path_to_csv, all_stations.size());
    return std::move(all_stations);
}

std::vector<Pos> LoadNodesFromCsvFile(std::string path_to_csv){
    CheckFileExistence(path_to_csv);
    auto s_time = GetTimeStamp();
    std::vector<Pos> all_nodes = {};
    std::ifstream data_csv(path_to_csv); //load the station data file
    std::string line;
    getline(data_csv,line);  // ignore the first line
    while (getline(data_csv,line)){  // read every line
        std::istringstream readstr(line); // string every line
        std::vector<std::string> data_line;
        std::string info;
        while (getline(readstr, info, ',')) {
            data_line.push_back(info);
        }
        Pos node;
        node.node_id = std::stoi(data_line[0]);
        node.lon = std::stod(data_line[1]);
        node.lat = std::stod(data_line[2]);
        all_nodes.emplace_back(node);
    }
    fmt::print("[DEBUG] ({}s) Load network node data from {}, with {} stations.\n",
               double (GetTimeStamp() - s_time)/1000, path_to_csv, all_nodes.size());
    return std::move(all_nodes);
}

//std::vector<Pos> LoadTravelDistanceTableFromCsvFile(std::string path_to_csv){
//    CheckFileExistence(path_to_csv);
//    auto s_time = GetTimeStamp();
//    std::vector<Pos> all_nodes = {};
//    std::ifstream data_csv(path_to_csv); //load the station data file
//    std::string line;
//    getline(data_csv,line);  // ignore the first line
//    while (getline(data_csv,line)){  // read every line
//        std::istringstream readstr(line); // string every line
//        std::vector<std::string> data_line;
//        std::string info;
//        while (getline(readstr, info, ',')) {
//            data_line.push_back(info);
//        }
//        Pos node;
//        node.node_id = std::stoi(data_line[0]);
//        node.lon = std::stod(data_line[1]);
//        node.lat = std::stod(data_line[2]);
//        all_nodes.emplace_back(node);
//    }
//    fmt::print("[DEBUG] ({}s) Load network node data from {}, with {} stations.\n",
//               double (GetTimeStamp() - s_time)/1000, path_to_csv, all_nodes.size());
//    return std::move(all_nodes);
//}