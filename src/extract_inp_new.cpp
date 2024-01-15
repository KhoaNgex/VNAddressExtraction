#include <list>
#include <regex>
#include <sstream>
#include <cctype>
#include <iostream>
#include "extract_inp_new.h"
#include "build_addr_map.h"

std::vector<std::string> huyen_prefix = {
    "huyen",
    "hyen", "huyn", "huen", "quan", "qun", "h", "q"};

std::vector<std::string> huyen_prefix_v2 = {
    "huyen",
    "hyen", "huyn", "huen", "quan", "qun", "pho", "tp"};

std::vector<std::string> tinh_del = {
    "tinh",
    "tin",
    "thanh pho",
    "thanh fho",
    " t ",
    ",t ",
    "t.p"
    "tnh",
    "tp",
    "t."};

std::vector<std::string> quan_del = {
    "quan",
    " q ",
    "q.",
    "huyen",
    "hyen",
    "huyn",
    " h ",
    "h.",
    ",h ",
    ",q ",
    "thanh pho",
    "thanh fho",
    "t.pho",
    "t.p",
    "tp",
    "thi xa",
    "i xa",
    "t.x",
    "tx",
};

std::string
to_lowercase(const std::string &input)
{
    std::string output = input;
    transform(output.begin(), output.end(), output.begin(), ::tolower);
    return output;
}

std::string remove_vietnamese_tone(const std::string &text)
{
    std::string result(text);
    result = regex_replace(result, std::regex("à|á|ạ|ả|ã|â|ầ|ấ|ậ|ẩ|ẫ|ă|ằ|ắ|ặ|ẳ|ẵ|/g"), "a");
    result = regex_replace(result, std::regex("À|Á|Ạ|Ả|Ã|Â|Ầ|Ấ|Ậ|Ẩ|Ẫ|Ă|Ằ|Ắ|Ặ|Ẳ|Ẵ|/g"), "A");
    result = regex_replace(result, std::regex("è|é|ẹ|ẻ|ẽ|ê|ề|ế|ệ|ể|ễ|/g"), "e");
    result = regex_replace(result, std::regex("È|É|Ẹ|Ẻ|Ẽ|Ê|Ề|Ế|Ệ|Ể|Ễ|/g"), "E");
    result = regex_replace(result, std::regex("ì|í|ị|ỉ|ĩ|/g"), "i");
    result = regex_replace(result, std::regex("Ì|Í|Ị|Ỉ|Ĩ|/g"), "I");
    result = regex_replace(result, std::regex("ò|ó|ọ|ỏ|õ|ô|ồ|ố|ộ|ổ|ỗ|ơ|ờ|ớ|ợ|ở|ỡ|/g"), "o");
    result = regex_replace(result, std::regex("Ò|Ó|Ọ|Ỏ|Õ|Ô|Ồ|Ố|Ộ|Ổ|Ỗ|Ơ|Ờ|Ớ|Ợ|Ở|Ỡ|/g"), "O");
    result = regex_replace(result, std::regex("ù|ú|ụ|ủ|ũ|ư|ừ|ứ|ự|ử|ữ|/g"), "u");
    result = regex_replace(result, std::regex("Ù|Ú|Ụ|Ủ|Ũ|Ư|Ừ|Ứ|Ự|Ử|Ữ|/g"), "U");
    result = regex_replace(result, std::regex("ỳ|ý|ỵ|ỷ|ỹ|/g"), "y");
    result = regex_replace(result, std::regex("Ỳ|Ý|Ỵ|Ỷ|Ỹ|/g"), "Y");
    result = regex_replace(result, std::regex("đ"), "d");
    result = regex_replace(result, std::regex("Đ"), "D");
    return result;
}

int get_levenshtein_distance(const std::string &str1, const std::string &str2)
{
    int m = str1.length();
    int n = str2.length();

    // std::cout << m << n << std::endl;
    int dp[m + 1][n + 1];

    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0)
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + std::min({dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1]});
        }
    }

    return dp[m][n];
}

bool is_two_string_similar(const std::string &str1, const std::string &str2, int tolerance)
{
    int distance = get_levenshtein_distance(str1, str2);
    return distance <= tolerance;
}

bool isNumeric(const std::string &str)
{
    return !str.empty() && std::all_of(str.begin(), str.end(), [](char c)
                                       { return std::isdigit(c); });
}

std::pair<std::string, int> remove_prefix(const std::string &input, const std::string &pattern)
{
    std::string output = input;
    std::regex regex(pattern, std::regex_constants::icase);
    std::smatch match;
    int wordCount = 0;
    while (regex_search(output, match, regex))
    {
        output = match.prefix().str() + match.suffix().str();
    }

    std::istringstream iss(output);
    std::string word;
    while (iss >> word)
    {
        wordCount++;
    }
    return std::make_pair(output, wordCount);
}

bool is_substring_at_end(const std::string &sub, const std::string &str)
{
    int subLength = sub.length();
    int strLength = str.length();

    if (strLength < subLength)
        return false;

    for (int i = 1; i <= subLength; i++)
    {
        if (sub[subLength - i] != str[strLength - i])
        {
            return false;
        }
    }
    return true;
}

void strim_str_at_last(std::string &str)
{
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (!isalnum(str[i]))
        {
            str.erase(i, 1);
        }
        else
        {
            break;
        }
    }
}

bool strim_str_at_last_comma(std::string &str)
{
    bool res = true;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (!isalpha(str[i]))
        {
            if (str[i] == ',')
            {
                res = false;
            }
            str.erase(i, 1);
        }
        else
        {
            break;
        }
    }
    return res;
}

std::string get_word_fragile(const std::string &str, std::string &af_inp)
{
    std::string tmp_str = str;
    std::string word;
    char &last_char = tmp_str.back();
    bool is_num = false;
    while (!std::isspace(last_char) && last_char != ',' && last_char != '-' && last_char != '.')
    {
        if (is_num && (last_char == 'q' || last_char == 'p' || last_char == 'Q' || last_char == 'P'))
            break;
        if (isdigit(last_char))
            is_num = true;
        word = last_char + word;
        tmp_str.pop_back();
        if (std::isupper(last_char) || tmp_str.empty())
            break;
        last_char = tmp_str.back();
    }
    strim_str_at_last(tmp_str);

    word[0] = std::tolower(word[0]);
    af_inp = tmp_str;
    return word;
}

// std::string get_non_last_word(const std::string &str)
// {
//     std::string word;
//     int i = str.length() - 1;
//     while (i >= 0 && !std::isspace(str[i]) && str[i] != ',' && str[i] != '-')
//     {
//         word = str[i] + word;
//         if (std::isupper(str[i]))
//             break;
//         i--;
//     }
//     word[0] = std::tolower(word[0]);
//     return word;
// }

bool should_ignore_word(std::string word, std::string after_word)
{
    if (after_word != "")
    {
        after_word[0] = std::tolower(after_word[0]);
    }
    if (word.size() == 1 and after_word != "giang")
        return true;
    char c_0 = word[0];
    char c_1 = word[1];

    std::string word_lc = to_lowercase(word);
    if (word.size() == 2 && (word_lc != "yn" && word_lc != "nm") && !((c_1 == 'a' || c_1 == 'o' || c_1 == 'e' || c_1 == 'i' || c_1 == 'u' || c_1 == 'A' || c_1 == 'O' || c_1 == 'E' || c_1 == 'I' || c_1 == 'U')) &&
        !((c_0 == 'a' || c_0 == 'o' || c_0 == 'e' || c_0 == 'i' || c_0 == 'u' || c_0 == 'A' || c_0 == 'O' || c_0 == 'E' || c_0 == 'I' || c_0 == 'U')))
    {
        return true;
    }
    return false;
}

int find_last_substring(const std::string &input, const std::string &substring)
{
    size_t pos = input.rfind(substring);
    if (pos != std::string::npos)
    {
        return static_cast<int>(pos);
    }
    else
    {
        return -1;
    }
}

AddressInfo search_parts(std::string input, bool has_tinh)
{
    AddressInfo result;
    result.tinh = "";
    std::pair<int, std::string> tinh_pos_main = std::make_pair(-1, "");

    std::vector<std::unordered_map<std::string, CapTinh>> list_all_dict = {
        dict_data_1_10,
        dict_data_11_20,
        dict_data_21_30,
        dict_data_31_40,
        dict_data_41_50,
        dict_data_51_60,
        dict_data_61_63};

    //// lay tinh
    if (has_tinh)
    {
        if (is_substring_at_end("HCM", input))
        {
            result.tinh = "Hồ Chí Minh";
            tinh_pos_main = std::make_pair(4, "79");
            for (int i = 1; i <= 3; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("H.C.M", input))
        {
            result.tinh = "Hồ Chí Minh";
            tinh_pos_main = std::make_pair(4, "79");
            for (int i = 1; i <= 5; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("HN", input))
        {
            result.tinh = "Hà Nội";
            tinh_pos_main = std::make_pair(0, "01");
            for (int i = 1; i <= 2; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("H.N", input))
        {
            result.tinh = "Hà Nội";
            tinh_pos_main = std::make_pair(0, "01");
            for (int i = 1; i <= 3; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("TTH", input))
        {
            result.tinh = "Thừa Thiên Huế";
            tinh_pos_main = std::make_pair(3, "46");
            for (int i = 1; i <= 3; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("T.T.H", input))
        {
            result.tinh = "Thừa Thiên Huế";
            tinh_pos_main = std::make_pair(3, "46");
            for (int i = 1; i <= 5; i++)
            {
                input.pop_back();
            }
        }
        else
        {
            std::string af_input = "";
            std::string old_input = input;
            std::string _1stLastWord = get_word_fragile(input, af_input);
            input = af_input;
            std::string _2ndLastWord = get_word_fragile(input, af_input);
            input = af_input;
            std::string tmp_str_1 = "";
            std::string tmp_str_2 = "";
            std::string _3rdLastWord = "";
            std::string _4thLastWord = "";
            if (af_input != "")
            {
                _3rdLastWord = get_word_fragile(af_input, tmp_str_1);
                if (tmp_str_1 != "")
                {
                    _4thLastWord = get_word_fragile(tmp_str_1, tmp_str_2);
                }
            }

            bool is_cont = true;
            std::string _3rdLastWord_lc = to_lowercase(_3rdLastWord);
            std::string _4thLastWord_lc = to_lowercase(_4thLastWord);
            for (auto prefix : huyen_prefix)
            {
                if (_3rdLastWord_lc == prefix)
                {
                    if (_3rdLastWord_lc != "quan" || (_3rdLastWord == "quan" && _4thLastWord != "nho" & _4thLastWord != "dinh"))
                    {
                        is_cont = false;
                        break;
                    }
                }
            }

            if (_3rdLastWord_lc == "h" && _1stLastWord == "minh" && _2ndLastWord[0] == 'c')
                is_cont = true;

            if (is_cont)
            {
                std::vector<std::string> w_utility = {_1stLastWord, _2ndLastWord, _3rdLastWord, _4thLastWord};
                // lay danh sach tinh ung tuyen: 1 mang chac chan, 1 mang ko chac chan
                std::list<std::pair<int, std::string>> matchingIds_conf;
                std::list<std::pair<int, std::string>> matchingIds_not_conf;

                // Tim kiem o tat ca cac map hien co
                // std::cout << lastWord << std::endl;

                std::vector<std::string> w_used;
                for (auto util : w_utility)
                {
                    std::string after_word = "";
                    if (w_used.size() != 0)
                    {
                        after_word = w_used.back();
                    }
                    if (!should_ignore_word(util, after_word))
                    {
                        std::string prefix = "tinh";
                        if (util.compare(0, prefix.length(), prefix) == 0 and util.length() > 4)
                        {
                            util.erase(0, prefix.length());
                        }
                        w_used.push_back(util);
                    }
                }

                int lst_idx = 0;
                for (const auto &myMap : list_all_dict)
                {
                    for (const auto &pair : myMap)
                    {
                        const std::string &id = pair.first;
                        const CapTinh &item = pair.second;
                        size_t lastDashPos = item.slug.find_last_of('-');
                        std::string last_word_slug = item.slug.substr(lastDashPos + 1);
                        // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                        if (last_word_slug == w_used[0])
                        {
                            matchingIds_conf.push_back(std::make_pair(lst_idx, id));
                        }
                        else if (is_two_string_similar(w_used[0], last_word_slug, 2))
                        {
                            matchingIds_not_conf.push_back(std::make_pair(lst_idx, id));
                        }
                    }
                    lst_idx++;
                }
                // for (auto item : matchingIds_conf)
                // {
                //     std::cout << list_all_dict[item.first][item.second].slug << "||";
                // }
                // std::cout << std::endl;
                // std::cout
                //     << "---------------------------" << std::endl;
                // for (auto item : matchingIds_not_conf)
                // {
                //     std::cout << list_all_dict[item.first][item.second].slug << "||";
                // }

                std::pair<int, std::string> tinh_pos = std::make_pair(-1, "");
                int tinh_conf_sz = matchingIds_conf.size();
                int tinh_not_conf_sz = matchingIds_not_conf.size();

                if (tinh_conf_sz >= 1)
                {
                    if (tinh_conf_sz == 1)
                    {
                        tinh_pos = matchingIds_conf.front();
                    }
                    else if (w_used.size() >= 2)
                    {
                        int min_dist = 9999;
                        for (const auto &_pair : matchingIds_conf)
                        {
                            const int lst_idx = _pair.first;
                            const std::string id = _pair.second;
                            const std::string slug = list_all_dict[lst_idx][id].slug;
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == w_used[1])
                            {
                                tinh_pos = std::make_pair(lst_idx, id);
                                break;
                            }
                            else
                            {
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, w_used[1]);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    tinh_pos = std::make_pair(lst_idx, id);
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                    }
                    else
                    {
                        tinh_pos = matchingIds_conf.front();
                    }
                }
                else if (tinh_conf_sz == 0)
                {
                    if (tinh_not_conf_sz >= 1)
                    {
                        if (w_used.size() >= 2)
                        {
                            int min_dist = 9999;
                            for (const auto &_pair : matchingIds_not_conf)
                            {
                                const int lst_idx = _pair.first;
                                const std::string id = _pair.second;
                                const std::string slug = list_all_dict[lst_idx][id].slug;
                                std::string _2nd_last_slug;

                                std::size_t lastDash = slug.find_last_of('-');
                                if (lastDash != std::string::npos)
                                {
                                    std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                    if (secondLastDash != std::string::npos)
                                    {
                                        _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                    }
                                    else
                                    {
                                        _2nd_last_slug = slug.substr(0, lastDash);
                                    }
                                }
                                // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                                if (_2nd_last_slug == w_used[1])
                                {
                                    tinh_pos = std::make_pair(lst_idx, id);
                                    break;
                                }
                                else
                                {
                                    int tmp_dist = get_levenshtein_distance(_2nd_last_slug, w_used[1]);
                                    if (tmp_dist < min_dist && tmp_dist <= 2)
                                    {
                                        tinh_pos = std::make_pair(lst_idx, id);
                                        min_dist = tmp_dist;
                                    }
                                }
                            }
                        }
                        else
                        {
                            tinh_pos = matchingIds_not_conf.front();
                        }
                    }
                    else
                    {
                        tinh_pos = std::make_pair(-1, "");
                    }
                }
                if (tinh_pos.first != -1)
                {
                    result.tinh = list_all_dict[tinh_pos.first][tinh_pos.second].name;
                    tinh_pos_main = tinh_pos;
                    // std::cout << list_all_dict[tinh_pos.first][tinh_pos.second].slug << std::endl;
                }
                else
                {
                    input = old_input;
                }
            }
            else
            {
                input = old_input;
            }
        }
    }
    // lay huyen
    // xoa ky tu ko lien quan
    // b1: xoa tu thua
    std::tuple<int, std::string, std::string> huyen_pos_main = std::make_tuple(-1, "", "");

    if (result.tinh != "")
    {
        std::string input_lc = to_lowercase(input) + " ";
        bool is_del_success = false;
        bool should_cont_del = true;
        for (auto str : tinh_del)
        {
            int del_position = find_last_substring(input_lc, str);
            bool tmp_flag = false;
            if (del_position != -1)
            {
                // std::cout << str << std::endl;
                int tmp_count = 0;
                std::string tmp_str = "";
                for (int i = del_position + str.length(); i < input_lc.length(); ++i)
                {
                    if (input_lc[i] != ' ')
                    {
                        ++tmp_count;
                        tmp_str += input_lc[i];
                        if ((str == "tinh" && (tmp_str == "gia" || tmp_str == "bien")) || tmp_str == "sonla" || (tmp_str == "ng" && str == "thanh pho"))
                        {
                            tmp_flag = true;
                            break;
                        }
                    }
                }
                if (str == "tin")
                {
                    int tmp_i = del_position - 2;
                    std::string tmp_word = "";
                    while (tmp_i >= 0 && input_lc[tmp_i] != ' ')
                    {
                        tmp_word = input_lc[tmp_i] + tmp_word;
                        tmp_i--;
                    }
                    if (tmp_word == "thuong")
                        tmp_flag = true;
                }

                if (tmp_flag == true)
                    continue;

                if (tmp_count <= 5)
                {
                    input.erase(del_position);
                    is_del_success = true;
                    break;
                }
            }
        }
        if (is_del_success == false && should_cont_del == true)
        {
            std::size_t found = input_lc.find_last_of(",");
            if (found != std::string::npos)
            {
                std::string substr = input_lc.substr(found + 1);
                std::stringstream ss(substr);
                std::string word;
                int wordCount = 0;
                bool tmp_flag = false;

                while (ss >> word)
                {

                    if (word == "ria")
                        tmp_flag = true;
                    wordCount++;
                }
                if (wordCount == 1 || tmp_flag)
                {
                    input.erase(found);
                }
            }
        }
    }
    // b2: nhan dien dau ,
    int num_comma = 0;
    input.push_back(' ');
    while (!input.empty())
    {
        char tmp_ch = input.back();
        if (tmp_ch == ',')
            num_comma++;
        if (isalnum(tmp_ch))
            break;
        if (!input.empty())
            input.pop_back();
    }

    if (num_comma < 2)
    {
        if (is_substring_at_end("BC", input))
        {
            result.quan_huyen = "Bình Chánh";
            huyen_pos_main != std::make_tuple(4, "79", "785");
            for (int i = 1; i <= 2; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("GV", input))
        {
            result.quan_huyen = "Gò Vấp";
            huyen_pos_main != std::make_tuple(4, "79", "764");
            for (int i = 1; i <= 2; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("BT", input))
        {
            result.quan_huyen = "Bình Thạnh";
            huyen_pos_main != std::make_tuple(4, "79", "765");
            for (int i = 1; i <= 2; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("TB", input))
        {
            result.quan_huyen = "Tân Bình";
            huyen_pos_main != std::make_tuple(4, "79", "766");
            for (int i = 1; i <= 2; i++)
            {
                input.pop_back();
            }
        }
        else if (is_substring_at_end("PN", input))
        {
            result.quan_huyen = "Phú Nhuận";
            huyen_pos_main != std::make_tuple(4, "79", "768");
            for (int i = 1; i <= 2; i++)
            {
                input.pop_back();
            }
        }
        else
        {
            // lay huyen
            std::string af_input = "";
            std::string old_input = input;
            std::string _1stLastWord = get_word_fragile(input, af_input);
            input = af_input;
            std::string _2ndLastWord = "";
            if (input != "")
            {
                _2ndLastWord = get_word_fragile(input, af_input);
                input = af_input;
                for (const auto &prefix : huyen_prefix_v2)
                {
                    if (_2ndLastWord.length() > prefix.length() && _2ndLastWord.compare(0, prefix.length(), prefix) == 0)
                    {
                        if (prefix == "pho" && (_2ndLastWord[3] == 'n' || _2ndLastWord[3] == 'i'))
                            continue;
                        else
                        {
                            _2ndLastWord.erase(0, prefix.length());
                            break;
                        }
                    }
                }
            }
            else
            {
                _2ndLastWord = "";
            }

            // std::cout << input << "||" << _1stLastWord << "||" << _2ndLastWord << std::endl;

            if (tinh_pos_main.first != -1)
            {
                std::unordered_map<std::string, CapHuyen> huyen_map = list_all_dict[tinh_pos_main.first][tinh_pos_main.second].quan_huyen;
                std::list<std::string> matchingIds_conf;
                std::list<std::string> matchingIds_not_conf;
                for (const auto &pair : huyen_map)
                {
                    const std::string &id = pair.first;
                    const CapHuyen &item = pair.second;
                    size_t lastDashPos = item.slug.find_last_of('-');
                    std::string last_word_slug = item.slug.substr(lastDashPos + 1);
                    // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                    if (last_word_slug == _1stLastWord)
                    {
                        matchingIds_conf.push_back(id);
                    }
                    else if (is_two_string_similar(_1stLastWord, last_word_slug, 2))
                    {
                        matchingIds_not_conf.push_back(id);
                    }
                }

                std::string huyen_pos = "";
                int huyen_conf_sz = matchingIds_conf.size();
                int huyen_not_conf_sz = matchingIds_not_conf.size();

                if (huyen_conf_sz >= 1)
                {
                    if (huyen_conf_sz == 1)
                    {
                        huyen_pos = matchingIds_conf.front();
                    }
                    else
                    {
                        int min_dist = 9999;
                        for (const auto &id : matchingIds_conf)
                        {
                            const std::string slug = huyen_map[id].slug;
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                huyen_pos = id;
                                break;
                            }
                            else
                            {
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    huyen_pos = id;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                    }
                }
                else if (huyen_conf_sz == 0)
                {
                    if (huyen_not_conf_sz >= 1)
                    {
                        int min_dist = 9999;
                        for (const auto &id : matchingIds_not_conf)
                        {
                            const std::string slug = huyen_map[id].slug;
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                huyen_pos = id;
                                break;
                            }
                            else
                            {
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    huyen_pos = id;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                    }
                    else
                    {
                        huyen_pos = "";
                    }
                }
                if (huyen_pos != "")
                {
                    huyen_pos_main = std::make_tuple(tinh_pos_main.first, tinh_pos_main.second, huyen_map[huyen_pos].code);
                    result.quan_huyen = huyen_map[huyen_pos].name;
                }
                else
                {
                    input = old_input;
                }
            }
            else
            {
                std::tuple<int, std::string, std::string> huyen_pos = std::make_tuple(-1, "", "");
                std::list<std::tuple<int, std::string, std::string>> matchingIds_conf;
                std::list<std::tuple<int, std::string, std::string>> matchingIds_not_conf;
                int lst_idx = 0;
                for (const auto &myMap : list_all_dict)
                {
                    for (const auto &_tinh_pair : myMap)
                    {
                        std::unordered_map<std::string, CapHuyen> huyen_map = _tinh_pair.second.quan_huyen;
                        for (const auto &pair : huyen_map)
                        {
                            const std::string &id = pair.first;
                            const CapHuyen &item = pair.second;
                            size_t lastDashPos = item.slug.find_last_of('-');
                            std::string last_word_slug = item.slug.substr(lastDashPos + 1);
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (last_word_slug == _1stLastWord)
                            {
                                matchingIds_conf.push_back(std::make_tuple(lst_idx, _tinh_pair.first, id));
                            }
                            else if (is_two_string_similar(_1stLastWord, last_word_slug, 2))
                            {
                                matchingIds_not_conf.push_back(std::make_tuple(lst_idx, _tinh_pair.first, id));
                            }
                        }
                    }
                    lst_idx++;
                }

                int huyen_conf_sz = matchingIds_conf.size();
                int huyen_not_conf_sz = matchingIds_not_conf.size();

                if (huyen_conf_sz >= 1)
                {
                    if (huyen_conf_sz == 1)
                    {
                        huyen_pos = matchingIds_conf.front();
                    }
                    else
                    {
                        int min_dist = 9999;
                        for (const auto &_triple : matchingIds_conf)
                        {
                            int lst_idx = std::get<0>(_triple);
                            std::string tinh_id = std::get<1>(_triple);
                            std::string huyen_id = std::get<2>(_triple);

                            const std::string slug = list_all_dict[lst_idx][tinh_id].quan_huyen[huyen_id].slug;
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                huyen_pos = _triple;
                                break;
                            }
                            else
                            {
                                // std::cout << _2nd_last_slug << "|" << _2ndLastWord << std::endl;
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    huyen_pos = _triple;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                    }
                }
                else if (huyen_conf_sz == 0)
                {
                    if (huyen_not_conf_sz >= 1)
                    {
                        int min_dist = 9999;
                        for (const auto &_triple : matchingIds_not_conf)
                        {
                            int lst_idx = std::get<0>(_triple);
                            std::string tinh_id = std::get<1>(_triple);
                            std::string huyen_id = std::get<2>(_triple);

                            const std::string slug = list_all_dict[lst_idx][tinh_id].quan_huyen[huyen_id].slug;
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                huyen_pos = _triple;
                                break;
                            }
                            else
                            {
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    huyen_pos = _triple;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                    }
                    else
                    {
                        huyen_pos = std::make_tuple(-1, "", "");
                    }
                }
                if (std::get<0>(huyen_pos) != -1)
                {
                    huyen_pos_main = huyen_pos;
                    result.quan_huyen = list_all_dict[std::get<0>(huyen_pos)][std::get<1>(huyen_pos)].quan_huyen[std::get<2>(huyen_pos)].name;
                }
                else
                {
                    input = old_input;
                }
            }
        }
        // std::cout << _1stLastWord << "|" << _2ndLastWord << std::endl;
    }

    // lay xa
    // loai tu thua
    // std::cout << result.quan_huyen << std::endl;
    result.xa_phuong = "";
    // std::cout << "after process: " << input << std::endl;

    if (result.quan_huyen != "")
    {
        std::string input_lc = to_lowercase(input) + " ";
        bool is_del_success = false;

        for (auto str : quan_del)
        {
            int del_position = find_last_substring(input_lc, str);

            if (del_position != -1)
            {
                int tmp_count = 0;
                std::string tmp_str = "";
                for (int i = del_position + str.length(); i < input_lc.length(); ++i)
                {
                    if (input_lc[i] != ' ')
                    {
                        ++tmp_count;
                        tmp_str += input_lc[i];
                    }
                }
                // std::cout << del_position << std::endl;
                if (tmp_count <= 6)
                {
                    if ((str == "quan" && ((tmp_count >= 1 && tmp_str[0] != 'g') || tmp_count == 0)) || (str != "quan"))
                    {
                        input.erase(del_position);
                        is_del_success = true;
                        break;
                    }
                }
            }
        }
        if (is_del_success == false)
        {
            std::size_t found = input_lc.find_last_of(",");
            if (found != std::string::npos)
            {
                std::string substr = input_lc.substr(found + 1);
                std::stringstream ss(substr);
                std::string word;
                int wordCount = 0;

                while (ss >> word)
                {
                    wordCount++;
                }

                if (wordCount == 1 && word[0] != 'p')
                {
                    input.erase(found);
                }
            }
            int tmp_pos = find_last_substring(input_lc, "bac");
            int tmp_pos_2 = find_last_substring(input_lc, "duc");

            if (tmp_pos != -1 and tmp_pos_2 == -1 and input.length() - tmp_pos <= 3)
            {
                input.erase(tmp_pos);
            }
        }
    }

    while (!input.empty())
    {
        char tmp_ch = input.back();
        if (isalnum(tmp_ch))
            break;
        if (!input.empty())
            input.pop_back();
    }

    // std::cout << "after process: " << input << std::endl;

    std::string _1stLastWord = "";
    std::string _2ndLastWord = "";

    if (input != "")
    {
        std::string af_input = "";
        _1stLastWord = get_word_fragile(input, af_input);
        input = af_input;
        if (input != "")
        {
            _2ndLastWord = get_word_fragile(input, af_input);
            input = af_input;
        }
    }

    if (_1stLastWord != "")
    {
        if (std::get<2>(huyen_pos_main) != "")
        {
            std::unordered_map<std::string, CapXa> xa_map = list_all_dict[std::get<0>(huyen_pos_main)][std::get<1>(huyen_pos_main)].quan_huyen[std::get<2>(huyen_pos_main)].xa_phuong;
            std::list<std::string> matchingIds_conf;
            std::list<std::string> matchingIds_not_conf;
            for (const auto &pair : xa_map)
            {
                const std::string &id = pair.first;
                const CapXa &item = pair.second;
                size_t lastDashPos = item.slug.find_last_of('-');
                std::string last_word_slug = item.slug.substr(lastDashPos + 1);

                // std::cout << last_word_slug << std::endl;

                bool tmp_flag_l = isNumeric(last_word_slug);
                bool tmp_flag_2 = isNumeric(_1stLastWord);
                if (tmp_flag_l || tmp_flag_2)
                {
                    // std::cout << std::stoi(last_word_slug) << std::endl;
                    if (tmp_flag_2 && tmp_flag_l && std::stoi(last_word_slug) == std::stoi(_1stLastWord))
                        matchingIds_conf.push_back(id);
                }
                // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                else
                {
                    if (last_word_slug == _1stLastWord)
                    {
                        matchingIds_conf.push_back(id);
                    }
                    else if (is_two_string_similar(_1stLastWord, last_word_slug, 2))
                    {
                        matchingIds_not_conf.push_back(id);
                    }
                }
            }

            std::string xa_pos = "";
            int xa_conf_sz = matchingIds_conf.size();
            int xa_not_conf_sz = matchingIds_not_conf.size();

            if (xa_conf_sz >= 1)
            {
                if (xa_conf_sz == 1)
                {
                    xa_pos = matchingIds_conf.front();
                }
                else
                {
                    int min_dist = 9999;
                    for (const auto &id : matchingIds_conf)
                    {
                        const std::string slug = xa_map[id].slug;

                        if (!isNumeric(slug))
                        {
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                xa_pos = id;
                                break;
                            }
                            else
                            {
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    xa_pos = id;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                        else
                        {
                            xa_pos = matchingIds_conf.front();
                        }
                    }
                }
            }
            else if (xa_conf_sz == 0)
            {
                if (xa_not_conf_sz >= 1)
                {
                    int min_dist = 9999;
                    for (const auto &id : matchingIds_not_conf)
                    {
                        const std::string slug = xa_map[id].slug;

                        if (!isNumeric(slug))
                        {
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                xa_pos = id;
                                break;
                            }
                            else
                            {
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    xa_pos = id;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                        else
                        {
                            xa_pos = matchingIds_conf.front();
                        }
                    }
                }
                else
                {
                    xa_pos = "";
                }
            }
            if (xa_pos != "")
            {
                result.xa_phuong = xa_map[xa_pos].name;
                // std::cout << xa_map[xa_pos].slug << std::endl;
            }
        }
        else if (tinh_pos_main.first != -1)
        {
            std::unordered_map<std::string, CapHuyen> huyen_map = list_all_dict[tinh_pos_main.first][tinh_pos_main.second].quan_huyen;
            std::list<std::pair<std::string, std::string>> matchingIds_conf;
            std::list<std::pair<std::string, std::string>> matchingIds_not_conf;

            for (const auto &huyen : huyen_map)
            {
                std::unordered_map<std::string, CapXa> xa_map = huyen.second.xa_phuong;
                for (const auto &_xa_pair : xa_map)
                {
                    const std::string &id = _xa_pair.first;
                    const CapXa &item = _xa_pair.second;
                    size_t lastDashPos = item.slug.find_last_of('-');
                    std::string last_word_slug = item.slug.substr(lastDashPos + 1);
                    // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                    bool tmp_flag_l = isNumeric(last_word_slug);
                    bool tmp_flag_2 = isNumeric(_1stLastWord);
                    if (tmp_flag_l || tmp_flag_2)
                    {
                        if (tmp_flag_2 && tmp_flag_l && std::stoi(last_word_slug) == std::stoi(_1stLastWord))
                            matchingIds_conf.push_back(std::make_pair(huyen.first, id));
                    }
                    else
                    {
                        if (last_word_slug == _1stLastWord)
                        {
                            matchingIds_conf.push_back(std::make_pair(huyen.first, id));
                        }
                        else if (is_two_string_similar(_1stLastWord, last_word_slug, 2))
                        {
                            matchingIds_not_conf.push_back(std::make_pair(huyen.first, id));
                        }
                    }
                }
            }
            std::pair<std::string, std::string> xa_pos = std::make_pair("", "");
            int xa_conf_sz = matchingIds_conf.size();
            int xa_not_conf_sz = matchingIds_not_conf.size();

            if (xa_conf_sz >= 1)
            {
                if (xa_conf_sz == 1)
                {
                    xa_pos = matchingIds_conf.front();
                }
                else
                {
                    int min_dist = 9999;
                    for (const auto &_pair : matchingIds_conf)
                    {
                        std::string huyen_id = _pair.first;
                        std::string xa_id = _pair.second;

                        const std::string slug = huyen_map[huyen_id].xa_phuong[xa_id].slug;

                        if (!isNumeric(slug))
                        {
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                xa_pos = _pair;
                                break;
                            }
                            else
                            {
                                // std::cout << _2nd_last_slug << "|" << _2ndLastWord << std::endl;
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    xa_pos = _pair;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                        else
                        {
                            xa_pos = matchingIds_conf.front();
                        }
                    }
                }
            }
            else if (xa_conf_sz == 0)
            {
                if (xa_not_conf_sz >= 1)
                {
                    int min_dist = 9999;
                    for (const auto &_pair : matchingIds_not_conf)
                    {
                        std::string huyen_id = _pair.first;
                        std::string xa_id = _pair.second;

                        const std::string slug = huyen_map[huyen_id].xa_phuong[xa_id].slug;
                        if (!isNumeric(slug))
                        {
                            std::string _2nd_last_slug;

                            std::size_t lastDash = slug.find_last_of('-');
                            if (lastDash != std::string::npos)
                            {
                                std::size_t secondLastDash = slug.find_last_of('-', lastDash - 1);
                                if (secondLastDash != std::string::npos)
                                {
                                    _2nd_last_slug = slug.substr(secondLastDash + 1, lastDash - secondLastDash - 1);
                                }
                                else
                                {
                                    _2nd_last_slug = slug.substr(0, lastDash);
                                }
                            }
                            // Kiem tra neu tu cuoi cung nam o cuoi chuoi slug
                            if (_2nd_last_slug == _2ndLastWord)
                            {
                                xa_pos = _pair;
                                break;
                            }
                            else
                            {
                                // std::cout << _2nd_last_slug << "|" << _2ndLastWord << std::endl;
                                int tmp_dist = get_levenshtein_distance(_2nd_last_slug, _2ndLastWord);
                                if (tmp_dist < min_dist && tmp_dist <= 2)
                                {
                                    xa_pos = _pair;
                                    min_dist = tmp_dist;
                                }
                            }
                        }
                        else
                        {
                            xa_pos = matchingIds_conf.front();
                        }
                    }
                }
                else
                {
                    xa_pos = std::make_pair("", "");
                }
            }
            if (xa_pos.first != "")
            {
                result.xa_phuong = huyen_map[xa_pos.first].xa_phuong[xa_pos.second].name;
                // std::cout << huyen_map[xa_pos.first].xa_phuong[xa_pos.second].slug << std::endl;
            }
        }
    }
    return result;
}

AddressInfo extract_addr_info_parts_from_raw(const std::string &input)
{
    // tien xu ly chuoi
    std::string preprocess_inp = remove_vietnamese_tone(input);
    // loai bo ky tu ko phai chu o cuoi chuoi
    bool has_tinh = strim_str_at_last_comma(preprocess_inp);
    // std::cout << preprocess_inp << std::endl;
    return search_parts(preprocess_inp, has_tinh);
}
