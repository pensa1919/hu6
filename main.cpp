#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <string>

struct input_entry {
    public:
    input_entry(char c): c(c), count(1), p(0) {}
    char c;
    int count;
    float p = 0;
    std::string code {""};
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
            std::cout << v[i].c << "  ::  " << v[i].count << "von "<<  v[i].p << v[i].code<< std::endl;
        }
    }
    void set_code (char c, std::string s) {

        int i = 0;
        for (;i < v.size() && v[i].c != c;i++);
        if (i < v.size() && v[i].c == c) {
            std::cout << "set" << s;
            v[i].code = s;
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
    huffman_list * list;


public:
    huffman_tree (huffman_list &l) {
    list = &l;
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

        std::cout << "vor while";

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

            if (head && head->next) {
                node * rem1 = head;
                node * rem2 = head->next;
                head = head->next->next;
                delete rem1;
                delete rem2;
            }




            node * n = head;
            while (n->next && n->next->data->count < new_node->data->count)
                n = n->next;

            new_node->next = n->next;
            n->next = new_node;


        }
        std::cout << "ende while";


            if (head->next) {
                input_entry * d1 = head->data;
                node * tmp {head};
                head = head ->next;
                delete tmp;

                input_entry * new_entry = new input_entry('-');
                new_entry->count = d1->count + head->data->count;
            }
        std::cout << "called";
        assert(head);
        create_code(head->data,"");



    }
    void create_code (input_entry * n, std::string s) {
            if (!n) return;
            if (!n->left && !n->right) {
                list->set_code(n->c, s);
                return;
            }
            create_code(n->left,s+"0");
            create_code(n->right,s+"1");
    }
};



int main() {
    huffman_list h("input.txt");
    huffman_tree t(h);
    h.print_list();
    return 0;
}