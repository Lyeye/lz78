#include <iostream>
#include <fstream>
#include <string>
#include <map>

uint8_t bits(size_t val) {
    uint8_t result = 0;
    while (val != 0) {
        val >>= 1;
        result++;
    }
    return result;
}

void write_pair(std::ofstream& output, const char c, const size_t id, const uint8_t id_bits) {
    uint8_t id_bytes = (id_bits + 7) / 8;
    char out[9];
    out[0] = c;
    *reinterpret_cast<size_t*>(out + 1) = id;
    output.write(out, 1 + id_bytes);
}

bool read_pair(std::ifstream& input, char& c, size_t& id, const uint8_t id_bits) {
    uint8_t id_bytes = (id_bits + 7) / 8;
    char in[9] = { 0 };
    input.read(in, 1 + id_bytes);
    c = in[0];
    id = *reinterpret_cast<size_t*>(in + 1);
    return (bool)input;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Missing c or d parameter. Exiting." << std::endl;
        return -1;
    }

    bool compress = true;
    if (argv[1][0] == 'c' && argv[1][1] == 0) {
        compress = true;
    } else if (argv[1][0] == 'd' && argv[1][1] == 0) {
        compress = false;
    } else {
        std::cout << "Invalid method parameter. Expected 'c' or 'd'. Exiting." << std::endl;
        return -1;
    }

    if (argc < 3) {
        std::cout << "Missing input file parameter. Exiting." << std::endl;
        return -1;
    }

    if (argc < 4) {
        std::cout << "Missing output file parameter. Exiting." << std::endl;
        return -1;
    }

    std::ifstream input(argv[2], std::ios::binary);
    if (!input) {
        std::cout << "Can not open input file '" << argv[2] << "'. Exiting." << std::endl;
        return -1;
    }

    std::ofstream output(argv[3], std::ios::binary);
    if (!output) {
        std::cout << "Can not open output file '" << argv[3] << "' for writing. Exiting." << std::endl;
        return -1;
    }

    if (compress) {
        std::map<std::string, size_t> dict;
        size_t next = 1;
        size_t candidate = 0;
        std::string next_str;
        char next_c = 0;
        while (input.read(&next_c, 1)) {
            next_str.append(1, next_c);
            auto id = dict.find(next_str);
            if (id == dict.end()) {
                write_pair(output, next_str[next_str.size() - 1], candidate, bits(next));
                dict.insert({next_str, next++});
                next_str.clear();
                candidate = 0;
            } else {
                candidate = (*id).second;
            }
        }
        if (candidate != 0) {
            auto id = dict.find(next_str.substr(0, next_str.size() - 1));
            if (id == dict.end()) {
                for (char c : next_str) {
                    write_pair(output, c, 0, bits(next));
                }
            } else {
                write_pair(output, next_str[next_str.size() - 1], (*id).second, bits(next));
            }
        }
    } else {
        std::map<size_t, std::string> dict;
        size_t next = 1;
        size_t dict_id = 0;
        char next_c = 0;
        std::string next_str;
        while (read_pair(input, next_c, dict_id, bits(next))) {
            auto dict_str = dict.find(dict_id);
            if (dict_str != dict.end())
                next_str.assign((*dict_str).second);
            next_str.append(1, next_c);
            dict.insert({next++, next_str});
            output.write(next_str.c_str(), next_str.size());
            next_str.clear();
        }
    }

    return 0;
}