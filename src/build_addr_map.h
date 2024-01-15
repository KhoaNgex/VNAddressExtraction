#ifndef BUILD_ADDR_MAP_H
#define BUILD_ADDR_MAP_H
#include <string>
#include <unordered_map>

struct CapXa
{
    std::string name;
    std::string type;
    std::string slug;
    std::string code;
    std::string parent_code;
};

struct CapHuyen
{
    std::string name;
    std::string type;
    std::string slug;
    std::string code;
    std::string parent_code;
    std::unordered_map<std::string, CapXa> xa_phuong;
};

struct CapTinh
{
    std::string name;
    std::string slug;
    std::string type;
    std::string code;
    std::unordered_map<std::string, CapHuyen> quan_huyen;
};
extern std::unordered_map<std::string, CapTinh> dict_data_1_10;
extern std::unordered_map<std::string, CapTinh> dict_data_11_20;
extern std::unordered_map<std::string, CapTinh> dict_data_21_30;
extern std::unordered_map<std::string, CapTinh> dict_data_31_40;
extern std::unordered_map<std::string, CapTinh> dict_data_41_50;
extern std::unordered_map<std::string, CapTinh> dict_data_51_60;
extern std::unordered_map<std::string, CapTinh> dict_data_61_63;

#endif