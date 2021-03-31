//
// Created by Leot on 2021/3/26.
//

#pragma once
#include "types.hpp"
#include <sys/stat.h>
#include <fmt/format.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>

/// \brief A function converting "%Y-%m-%d %H:%M:%S" to seconds.
int32_t ConvertTimeDateToSeconds(std::string time_date);

/// \brief A function converting seconds to "%Y-%m-%d %H:%M:%S".
std::string ConvertTimeSecondToDate(int32_t time_sec);

/// \brief A function computing the accumulated seconds of the day from "%Y-%m-%d %H:%M:%S",
/// e.g. "2015-05-25 01:00:00" -> 3600
int32_t ComputeTheAccumulatedSecondsFrom0Clock(std::string time_date);

/// \brief The current time of the system, used to calculate the computational time
std::time_t GetTimeStamp();

/// \brief A function to check whether the data file is existing.
int CheckFileExistence(std::string& path_to_file);

/// \brief A function loading the taxi trip data from a csv file.
std::vector<Request> LoadRequestsFromCsvFile(std::string path_to_csv);

/// \brief A function loading the vehicle station data from a csv file.
std::vector<Pos> LoadStationsFromCsvFile(std::string path_to_csv);

/// \brief A function loading the road network node data from a csv file.
std::vector<Pos> LoadNodesFromCsvFile(std::string path_to_csv);

///// \brief A function loading the precomputed travel distance of each node pair from a csv file.
//std::vector<Pos> LoadTravelDistanceTableFromCsvFile(std::string path_to_csv);
//
///// \brief A function loading the precomputed mean travel time of each node pair from a csv file.
//std::vector<Pos> LoadMeanTravelTimeTableFromCsvFile(std::string path_to_csv);
//
///// \brief A function loading the precomputed minimum mean travel time path of each node pair from a csv file.
//std::vector<Pos> LoadShortestPathTableFromCsvFile(std::string path_to_csv);

template <typename T>
std::vector<T> ReadObjectVectorFromBinary(const std::string & file_path) {
    struct stat buffer;
    if (stat(file_path.c_str(), &buffer) != 0){
        fmt::print("[ERROR] File {} does not exist! \n", file_path);
        exit(0);
    }

    std::ifstream FILE(file_path, std::ios::in | std::ios::binary);
    std::streampos begin, end;
    begin = FILE.tellg();
    FILE.seekg(0,std::ios::end);
    end = FILE.tellg();
//    std::cout << "size is: " << (end-begin) << " bytes.\n";
    unsigned long size = end - begin;
    FILE.close();

    std::ifstream INFILE(file_path, std::ios::in | std::ios::binary);
    std::vector<T> object_vector;
    object_vector.reserve(size);
    INFILE.read(reinterpret_cast<char *>(&object_vector[0]), size*sizeof(T));
    INFILE.close();
    return std::move(object_vector);
}

template <typename T>
int WriteObjectVectorToBinary(const std::vector<T> & object_vector, const std::string & file_path) {
    std::ofstream FILE;
    FILE.open(file_path, std::ios::out | std::ios::binary);
    if (!object_vector.empty()) {
        FILE.write((char *) object_vector.data(), object_vector.size() * sizeof(T));
        return 0;
    }
    return -1;
}