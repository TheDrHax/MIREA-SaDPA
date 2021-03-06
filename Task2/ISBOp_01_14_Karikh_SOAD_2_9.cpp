/**
 * ISBOp_01_14_Karikh_SOAD_2_9.cpp
 *
 * Практическое задание 2 (https://vk.com/doc-76267187_443134832)
 * Вариант: двоичное дерево с принудительной балансировкой
 *
 * Карих Дмитрий, ИСБОп-01-14
 * https://github.com/TheDrHax/MIREA-SaDPA
 */

#include <stdio.h>
#include <math.h>
#include <iostream>

#define CHILD_ID(tmp) tmp->parent->key > tmp->key ? 0 : 1
#define CHILD_PTR(tmp) tmp->parent->children[CHILD_ID(tmp)]

class Tree {
    private:
        Tree* parent = NULL;
        Tree* children[2] = {NULL,NULL};
        int key;

        /**
         * Возвращает элемент по ключу, если он существует.
         * Иначе возвращает элемент, который подходит для вставки
         * нового элемента с указанным ключом.
         */
        Tree* find(int key) {
            Tree* tmp = this;

            while (true) {
                if (key < tmp->key && tmp->children[0] != NULL) {
                    tmp = tmp->children[0];
                } else if (key > tmp->key && tmp->children[1] != NULL) {
                    tmp = tmp->children[1];
                } else {
                    return tmp;
                }
            }
        }

        /**
         * Возвращает указатель на максимальный элемент.
         */
        Tree* max() {
            Tree* tmp = this;
            while (tmp->children[1] != NULL) {
                tmp = tmp->children[1];
            }
            return tmp;
        }

        /**
         * Возвращает указатель на минимальный элемент.
         */
        Tree* min() {
            Tree* tmp = this;
            while (tmp->children[0] != NULL) {
                tmp = tmp->children[0];
            }
            return tmp;
        }

        /**
         * Заменяет текущий элемент другим
         */
        void replace(Tree* ptr) {
            key = ptr->key;
            for (int i = 0; i <= 1; i++) {
                children[i] = ptr->children[i];
                ptr->children[i] = NULL;
            }
            delete ptr;
        }

    public:
        Tree(int key, Tree* parent) {
            this->key = key;
            this->parent = parent;
        }

        Tree(int key) : Tree(key, NULL) {}

        ~Tree() {
            delete children[0];
            delete children[1];
        }

        /**
         * Выводит дерево в формате graphviz.
         */
        void print() {
            if (parent == NULL)
                std::cout << "digraph G {" << std::endl;

            if (children[0] != NULL) {
                children[0]->print();
            }
            if (children[1] != NULL) {
                children[1]->print();
            }

            if (parent != NULL)
                printf("  \"%d\" -> \"%d\"\n", parent->key, key);

            if (parent == NULL)
                std::cout << "}" << std::endl;
        }

        int size() {
            int size = 1;

            if (children[0] != NULL) size += children[0]->size();
            if (children[1] != NULL) size += children[1]->size();

            return size;
        }

        /**
         * Добавляет новый элемент в дерево, если он ещё не существует.
         */
        void add(int key) {
            Tree* tmp = find(key);
            if (tmp->key != key) {
                Tree* new_tree = new Tree(key, tmp);
                CHILD_PTR(new_tree) = new_tree;
            }
            lazy_balance();
        }

        /**
         * Удаляет ключ из дерева.
         */
        void remove(int key) {
            Tree* tmp = find(key);

            // Ключ не найден, выходим
            if (tmp->key != key) {
                return;
            }

            // У элемента нет потомков
            else if (tmp->children[0] == NULL && tmp->children[1] == NULL) {
                CHILD_PTR(tmp) = NULL;
                delete tmp;
            }

            // У элемента есть левый потомок
            else if (tmp->children[0] != NULL && tmp->children[1] == NULL) {
                tmp->replace(tmp->children[0]);
            }

            // У элемента есть правый потомок
            else if (tmp->children[1] != NULL && tmp->children[0] == NULL) {
                tmp->replace(tmp->children[1]);
            }

            // У элемента два потомка
            else if (tmp->children[0] != NULL && tmp->children[1] != NULL) {
                int tmp_key = tmp->children[1]->min()->key;
                tmp->remove(tmp_key);
                tmp->key = tmp_key;
            }

            lazy_balance();
            return;
        }

        /**
         * Вращает элементы отнсительно выбранного ключа.
         * Направление указывается значением boolean:
         *  против часовой стрелки: true 
         *  по часовой стрелке: false
         */
        void rotate(int key, bool left) {
            Tree* tmp = find(key);
            if (tmp->key != key) {
                return;
            }

            int a,b;
            if (left) {
                a = 0; b = 1;
            } else {
                a = 1; b = 0;
            }

            Tree* p = tmp->children[a];
            if (p == NULL) {
                return;
            }
            tmp->children[a] = tmp->children[a]->children[a];
            if (tmp->children[a] != NULL) {
                tmp->children[a]->parent = tmp;
            }

            p->children[a] = p->children[b];
            p->children[b] = tmp->children[b];
            if (p->children[b] != NULL) {
                p->children[b]->parent = p;
            }
            tmp->children[b] = p;
            p->parent = tmp;

            int key_tmp = p->key;
            p->key = tmp->key;
            tmp->key = key_tmp;
        }

        /**
         * Преобразует дерево в лозу с заменой корневого элемента
         */
        void vine() {
            Tree* tmp = this;

            while (tmp != NULL) {
                while (tmp->children[1] != NULL) {
                    rotate(tmp->key, false);
                }
                tmp = tmp->children[0];
            }
        }

        /**
         * Пытается сбалансировать дерево при любом количестве
         * элементов (но при неправильном количестве обычно делает
         * только хуже)
         */
        void balance() {
            float size_log = log2(size() + 1);

            vine(); // необходимо для балансировки

            Tree* tmp = NULL;
            for (int i = 1; i < size_log; i++) {
                tmp = this;
                for (int j = 0; j < pow(2, size_log-i) - 1; j++) {
                    rotate(tmp->key, true);
                    tmp = tmp->children[0];
                }
            }
        }

        /**
         * Балансирует дерево, если у него есть 2^n-1 элементов
         */
        void lazy_balance() {
            float size_log = log2(size() + 1);
            if (floor(size_log) == size_log) {
                balance();
            }
        }
};

int main(int argc, char **argv) {
    Tree* tree = new Tree(0);
    tree->print();

    char cmd_raw[6];
    std::string cmd;
    int arg;

    srand(time(NULL));
    while (tree->size() < 7) {
        tree->add(rand() % 100);
    }

    std::cout << "add <key>, remove <key>, size, rotate_left/right, vine, balance, exit" << std::endl;
    while (true) {
        tree->print();
        scanf("%s", cmd_raw);
        cmd = cmd_raw;

        if (!cmd.compare("add")) {
            scanf("%d", &arg);
            tree->add(arg);
        } else if (!cmd.compare("remove")) {
            scanf("%d", &arg);
            tree->remove(arg);
        } else if (!cmd.compare("size")) {
            printf("%d\n", tree->size());
        } else if (!cmd.compare("rotate")) {
            scanf("%s", cmd_raw);
            cmd = cmd_raw;
            scanf("%d", &arg);
            tree->rotate(arg, !cmd.compare("left"));
        } else if (!cmd.compare("vine")) {
            tree->vine();
        } else if (!cmd.compare("balance")) {
            tree->balance();
        } else if (!cmd.compare("exit")) {
            break;
        }
    }

    delete tree;

    return 0;
}

