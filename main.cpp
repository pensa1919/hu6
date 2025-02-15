#include <iostream>
#include <vector>
#include <fstream>

struct input_entry {
    char c;
    int count;
};

bool get_char(std::string filename, char & c) {
    std::ifstream file(filename);
    if (!file)
        return false;
    file.get(c);
    return true;
}

void create_list (std::string filename, std::vector<input_entry> & v) {
    char c;
    int found_pos;
    while (get_char(filename, c)){
        found_pos = -1;
        for (int i = 0; i < v.size(); i++) {
            if (v[i].c == c) {
                found_pos = i;
                break;
            }
        }
        if (found_pos != -1) {
            v[found_pos].count++;
            found_pos = -1;
        }
        else {
            input_entry entry;
            entry.c = c;
            entry.count = 1;
            v.push_back(entry);
        }

    }
}

void print_list (std::vector<input_entry> & v) {
    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i].c << "  ::  " << v[i].count << std::endl;
    }
}


int main() {
    std::vector<input_entry> v;
    create_list ("input.txt", v);
    print_list(v);


    return 0;
}