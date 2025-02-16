#include <iostream>
#include <vector>
#include <fstream>

class input_entry {
    public:
    input_entry(char c): c(c), count(1), p(0) {}
    char c;
    int count;
    float p;
};

class huffman_list {
    std::vector<input_entry>  v;
    int sign_count;
    void calculate_sign_count() {
        sign_count = 0;
        for (int i = 0; i < v.size(); i++, sign_count += v[i].count);
    }
    int calculate_p () {
        for (int i = 0; i < v.size(); i++)
            v[i].p = v[i].count / sign_count;
    }
    public:
    huffman_list(std::string filename) {
        char c;
        int found_pos;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Fehler: Datei konnte nicht geöffnet werden." << std::endl;
            return;
        }
        while (file.get(c)){
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
                input_entry entry(c);
                v.push_back(entry);
            }
        }
        calculate_sign_count();
        calculate_p();
    }

    void print_list () {
        std::cout << "ich wurde ausgeührt" << v.size() << std::endl;
        for (int i = 0; i < v.size(); i++) {
            std::cout << v[i].c << "  ::  " << v[i].count <<  v[i].p << std::endl;
        }
    }
};





int main() {
    huffman_list h("input.txt");
    h.print_list();
    return 0;
}