#include <span>
#include <ranges>
#include <charconv>
#include <algorithm>
#include <string_view>
#include "process.hpp"


DB process_input(std::span<const char> data)
{
    DB db;

    auto iter=data.begin();

    while (iter!=data.end()){
        //Scan for the end of the station name
        auto semi_col = std::ranges::find(iter,std::unreachable_sentinel,';');
        auto station=std::string_view(iter, semi_col);
        iter=semi_col+1;

        //Scan for the end of measured value
        auto new_line = std::ranges::find(iter, std::unreachable_sentinel,'\n');

        //Parse as float
        float fp_value;
        std::from_chars(iter.base(),new_line.base(),fp_value);
        iter=new_line+1;

        //lookup the station in our database

        auto it=db.find((std::string)station);
        if (it == db.end()) {
            // if it's not there, insert
            db.emplace(station, Record{1,fp_value,fp_value,fp_value});
            continue;
        }
        // Otherwise update the information
        it->second.min =std::min(it->second.min, fp_value);
        it->second.max =std::max(it->second.max, fp_value);
        it->second.sum +=fp_value;
        ++it->second.cnt;

        return db;

    }
}