#ifndef TIGERAPI_PROPERTIES_H
#define TIGERAPI_PROPERTIES_H

#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <ctime>
#include "utils.h"

namespace TIGER_API {
    class Properties {
    public:
        void load(std::ifstream& input) {
            // skip BOM
            char bom[3];
            input.read(bom, 3);
            if (!(bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF)) {
                // if not BOM, reset file pointer to start
                input.seekg(0);
            }

            std::string line;
            while (std::getline(input, line)) {
                // skip empty line and comment line
                if (line.empty() || line[0] == '#' || line[0] == '!') {
                    continue;
                }

                // find the first non-escaped equal or colon
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
                    
                    // handle escape character
                    value = unescape(value);
                    
                    // store as utility::string_t
                    utility::string_t key_t = utility::conversions::to_string_t(key);
                    utility::string_t value_t = utility::conversions::to_string_t(value);
                    properties[key_t] = value_t;
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
            // write file header comment
            output << "# Properties" << std::endl;
            output << "# " << get_current_datetime() << std::endl;
            output << std::endl;

            // write all properties
            for (const auto& pair : properties) {
                std::string key = utility::conversions::to_utf8string(pair.first);
                std::string value = utility::conversions::to_utf8string(pair.second);
                
                // escape special characters
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