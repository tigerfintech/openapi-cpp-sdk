#ifndef TIGERAPI_PROPERTIES_H
#define TIGERAPI_PROPERTIES_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include "utils.h"

namespace TIGER_API {
    class Properties {
    public:
        void load(std::ifstream& input) {
            std::string line;
            while (std::getline(input, line)) {
                // 跳过空行和注释行
                if (line.empty() || line[0] == '#' || line[0] == '!') {
                    continue;
                }

                // 查找第一个非转义的等号或冒号
                size_t pos = 0;
                while ((pos = line.find_first_of("=:", pos)) != std::string::npos) {
                    if (pos == 0 || line[pos - 1] != '\\') {
                        break;
                    }
                    pos++;
                }

                if (pos != std::string::npos) {
                    std::string key = trim(line.substr(0, pos));
                    std::string value = trim(line.substr(pos + 1));
                    
                    // 处理转义字符
                    value = unescape(value);
                    
                    // 存储为 utility::string_t
                    properties[utility::conversions::to_string_t(key)] = 
                        utility::conversions::to_string_t(value);
                }
            }
        }

        void set_property(const utility::string_t& key, const utility::string_t& value) {
            properties[key] = value;
        }

        utility::string_t get_property(const utility::string_t& key) const {
            auto it = properties.find(key);
            if (it != properties.end()) {
                return it->second;
            }
            return utility::string_t();
        }

        void store(std::ofstream& output) const {
            // 写入文件头注释
            output << "# Properties" << std::endl;
            output << "# " << get_current_datetime() << std::endl;
            output << std::endl;

            // 写入所有属性
            for (const auto& pair : properties) {
                std::string key = utility::conversions::to_utf8string(pair.first);
                std::string value = utility::conversions::to_utf8string(pair.second);
                
                // 转义特殊字符
                key = escape(key);
                value = escape(value);
                
                output << key << "=" << value << std::endl;
            }
        }

    private:
        std::map<utility::string_t, utility::string_t> properties;

        static std::string trim(const std::string& str) {
            const std::string whitespace = " \t\n\r\f\v";
            size_t start = str.find_first_not_of(whitespace);
            if (start == std::string::npos) {
                return "";
            }
            size_t end = str.find_last_not_of(whitespace);
            return str.substr(start, end - start + 1);
        }

        static std::string unescape(const std::string& str) {
            std::string result;
            bool escaped = false;
            
            for (size_t i = 0; i < str.length(); i++) {
                if (escaped) {
                    switch (str[i]) {
                        case 'n': result += '\n'; break;
                        case 't': result += '\t'; break;
                        case 'r': result += '\r'; break;
                        case '\\': result += '\\'; break;
                        default: result += str[i]; break;
                    }
                    escaped = false;
                } else if (str[i] == '\\') {
                    escaped = true;
                } else {
                    result += str[i];
                }
            }
            
            return result;
        }

        static std::string escape(const std::string& str) {
            std::string result;
            for (char c : str) {
                switch (c) {
                    case '\n': result += "\\n"; break;
                    case '\t': result += "\\t"; break;
                    case '\r': result += "\\r"; break;
                    case '\\': result += "\\\\"; break;
                    case '=': result += "\\="; break;
                    case ':': result += "\\:"; break;
                    case ' ': result += "\\ "; break;
                    default: result += c; break;
                }
            }
            return result;
        }

        static std::string get_current_datetime() {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::string timestamp(30, '\0');
            std::strftime(&timestamp[0], timestamp.size(), 
                         "%Y-%m-%d %H:%M:%S", 
                         std::localtime(&time));
            return timestamp;
        }
    };
}

#endif //TIGERAPI_PROPERTIES_H 