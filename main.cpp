#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <algorithm>

struct input_entry {
    public:
    input_entry(char c): c(c), count(1), p(0) {}
    char c;
    int count;
    float p = 0;
    input_entry * left {nullptr};
    input_entry * right {nullptr};

    bool operator < (input_entry r) {
        return p > r.p;
    }
};

class huffman_list {
    std::vector<input_entry>  v;
    void calculate_sign_count() {
        sign_count = 0;
        for (int i = 0; i < v.size(); i++, sign_count += v[i].count);
    }
    void calculate_p () {
        assert (sign_count != 0);
        for (int i = 0; i < v.size(); i++) {
            v[i].p = (float)v[i].count / sign_count;
        }
    }
    public:
    int sign_count;

    huffman_list(std::string filename): sign_count{0} {
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
        std::sort (v.begin(), v.end());
    }

    void print_list () {
        for (int i = 0; i < v.size(); i++) {
            std::cout << v[i].c << "  ::  " << v[i].count << "von "<<  v[i].p << std::endl;
        }
    }
    input_entry * operator [] (int i){
        return &v[i];
    }


};

class huffman_tree {
    struct node {
        input_entry * data;
        node * next {nullptr};
    };
    node * head {nullptr};

public:
    huffman_tree (huffman_list l) {
        for (int i = 0; i < l.sign_count; i++) {
            node * new_node = new node();
            new_node->data = l[i];
            if (!head)
                head = new_node;
            else {
                new_node->next = head;
                head = new_node;
            }
        }


        while (head->next && head->next->next) {
            input_entry * d1 = head->data;
            input_entry * d2 = head->next->data;
            node *new_node = new node();
            new_node->data = new input_entry('-');
            new_node->data->count = d1->count + d2->count;
            if (d1->count < d2->count) {
                new_node->data->left = d1;
                new_node->data->right = d2;
            }
            else {
                new_node->data->left = d2;
                new_node->data->right = d1;
            }

            node * rem1 {head}; node * rem2 {head->next};
            delete rem1; delete rem2;
            head = head -> next -> next;

            node * n = head;
            while (n->next && n->next->data->count < new_node->data->count)
                n = n->next;

            new_node->next = n->next;
            n->next = new_node;


        }

            if (head->next) {
                input_entry * d1 = head->data;
                node * tmp {head};
                head = head ->next;
                delete tmp;

                input_entry * new_entry;
                new_entry->c = '-';
                new_entry->count = d1->count + head->data->count;
            }




    }

};



int main() {
    huffman_list h("input.txt");
    h.print_list();
    std::cout << h.get_entry().c << std::endl;
    return 0;
}