#ifndef EXTRACT_INP_H
#define EXTRACT_INP_H

#include <string>
#include <vector>

struct AddressInfo {
    std::string xa_phuong;
    std::string quan_huyen;
    std::string tinh;
};

std::string to_lowercase(const std::string &);

std::string remove_vietnamese_tone(const std::string &);

std::pair<std::string, int> remove_prefix(const std::string &, const std::string &);

int get_levenshtein_distance(const std::string &, const std::string &);

bool is_two_string_similar(const std::string &, const std::string &, int);

bool is_substring_at_end(const std::string &, const std::string &);

AddressInfo search_parts(std::string, bool has_tinh);

AddressInfo extract_addr_info_parts_from_raw(const std::string &);

#endif