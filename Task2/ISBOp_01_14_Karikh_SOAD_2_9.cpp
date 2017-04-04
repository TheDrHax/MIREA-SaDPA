#include <stdio.h>
#include <math.h>
#include <iostream>

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

    public:
        Tree(int key, Tree* parent) {
            this->key = key;
            this->parent = parent;
        }
        
        Tree(int key) : Tree(key, NULL) {}

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

            if (key < tmp->key) {
                tmp->children[0] = new Tree(key, tmp);
            } else if (key > tmp->key) {
                tmp->children[1] = new Tree(key, tmp);
            }
            
            balance();
        }

        /**
         * Удаляет ключ из дерева.
         */
        void remove(int key) {
            Tree* tmp = find(key);
            
            // Мы не можем удалить элемент, если его нет.
            if (tmp->key != key) {
                return;
            }
            
            // Удаление корневого элемента
            if (tmp->parent == NULL) {
                if (tmp->children[0] != NULL && tmp->children[1] != NULL) {
                    Tree* min = tmp->children[1]->min();
                    min->children[0] = tmp->children[0];
                    min->children[0]->parent = min;
                } else if (tmp->children[1] == NULL) {
                    tmp->children[1] = tmp->children[0]->children[1];
                    tmp->key = tmp->children[0]->key;
                    tmp->children[0] = tmp->children[0]->children[0];
                    balance();
                    return;
                }
                
                tmp->children[0] = tmp->children[1]->children[0];
                tmp->key = tmp->children[1]->key;
                tmp->children[1] = tmp->children[1]->children[1];
                balance();
                return;
            }

            // Ищем указатель на текущий элемент у родителя для
            // быстрого доступа к нему без лишних условий.
            Tree** parent_ptr = NULL;
            if (tmp->parent->children[0] == tmp) {
                parent_ptr = &tmp->parent->children[0];
            } else if (tmp->parent->children[1] == tmp) {
                parent_ptr = &tmp->parent->children[1];
            } else {
                printf("WTF: %s\n",
                       "parent doesn't know about this element");
                throw "WTF";
            }

            // Если у элемента нет потомков, то просто удаляем его.
            // Также не забываем очистить указатель у родителя.
            if (tmp->children[0] == NULL && tmp->children[1] == NULL) {
                (*parent_ptr) = NULL;
                delete tmp;
                balance();
                return;
            }

            // Если есть один дочерний элемент, то заменяем
            // удаляемый дочерним. Не забываем заменить родителя у
            // дочернего элемента.
            if (tmp->children[0] == NULL && tmp->children[1] != NULL) { // правый
                (*parent_ptr) = tmp->children[1];
                tmp->children[1]->parent = tmp->parent;
                delete tmp;
                balance();
                return;
            } else if (tmp->children[0] != NULL && tmp->children[1] == NULL) { // левый
                (*parent_ptr) = tmp->children[0];
                tmp->children[0]->parent = tmp->parent;
                delete tmp;
                balance();
                return;
            }

            // Проблема: у удаляемого элемента есть два дочерних.
            if (tmp->children[0] != NULL && tmp->children[1] != NULL) {
                if (tmp->children[1]->children[0] == NULL) {
                    // Не затрагивая левую ветвь заменяем текущий
                    // элемент на правый дочерний. У правого дочернего
                    // нет левой ветви, так что мы ничего не теряем.

                    Tree* tmp_right = tmp->children[1];
                    
                    tmp->key = tmp->children[1]->key;
                    tmp->children[1] = tmp->children[1]->children[1];
                    if (tmp->children[1] != NULL) {
                        tmp->children[1]->parent = tmp;
                    }
                    
                    delete tmp_right;
                } else {
                    // Ищем элемент, не имеющий левых потомков, в
                    // правом поддереве и заменяем удаляеммый элемент
                    // на него.
                    Tree* tmp_left = tmp->children[1]->children[0]->min();
                    tmp->key = tmp_left->key;
                    tmp->children[1]->remove(tmp_left->key);
                }
            }
            
            balance();
        }

        /**
         * Преобразует дерево в лозу с заменой корневого элемента
         */
        void vine() {
            Tree* tmp = NULL;
            
            // Заменяем корневой элемент максимальным
            while (this->children[1] != NULL) {
                rotate_right(this->key);
            }

            // Причёсываем дерево (превращаем его в лозу)
            tmp = this->children[0];
            while (tmp != NULL) {
                if (tmp->children[1] != NULL) {
                    Tree* p = tmp;
                    
                    tmp->parent->children[0] = tmp->children[1];
                    tmp = tmp->children[1];
                    tmp->parent = p->parent;
                    
                    p->children[1] = tmp->children[0];
                    if (p->children[1] != NULL) {
                        p->children[1]->parent = p;
                    }

                    tmp->children[0] = p;
                    if (tmp->children[0] != NULL) {
                        tmp->children[0]->parent = tmp;
                    }
                }
                if (tmp->children[1] == NULL) {
                    tmp = tmp->children[0];
                }
            }
        }

        void rotate_left(int key) {
            Tree* tmp = find(key);
            if (tmp->key != key) {
                return;
            }
            Tree* p = tmp->children[0];
            
            if (tmp->children[0] == NULL) {
                return;
            }
            tmp->children[0] = tmp->children[0]->children[0];
            if (tmp->children[0] != NULL) {
                tmp->children[0]->parent = tmp;
            }
            
            p->children[0] = p->children[1];
            p->children[1] = tmp->children[1];
            if (p->children[1] != NULL) {
                p->children[1]->parent = p;
            }
            tmp->children[1] = p;
            p->parent = tmp;
            
            int key_tmp = p->key;
            p->key = tmp->key;
            tmp->key = key_tmp;
        }
        
        void rotate_right(int key) {
            Tree* tmp = find(key);
            if (tmp->key != key) {
                return;
            }
            Tree* p = tmp->children[1];
            
            if (tmp->children[1] == NULL) {
                return;
            }
            tmp->children[1] = tmp->children[1]->children[1];
            if (tmp->children[1] != NULL) {
                tmp->children[1]->parent = tmp;
            }
            
            p->children[1] = p->children[0];
            p->children[0] = tmp->children[0];
            if (p->children[0] != NULL) {
                p->children[0]->parent = p;
            }
            tmp->children[0] = p;
            p->parent = tmp;
            
            int key_tmp = p->key;
            p->key = tmp->key;
            tmp->key = key_tmp;
        }
        
        void balance() {
            float log = log2(size() + 1);
            if (floor(log) != log) {
                return;
            }
            
            vine();
            Tree* tmp = this;

            int n = 1;
            while (tmp->children[0] != NULL) {
                tmp = tmp->children[0];
                n++;
            }
            n = log2(n + 1);

            for (int i = 1; i < n; i++) {
                tmp = this;
                for (int j = 0; j < pow(2, n-i) - 1; j++) {
                    if (tmp->children[0] != NULL) {
                        rotate_left(tmp->key);
                    }
                    tmp = tmp->children[0];
                }
            }
        }
};

int main(int argc, char **argv) {
    Tree* tree = new Tree(0);
    tree->print();
    
    char cmd_raw[6];
    std::string cmd;
    int arg;
    
    for (int i = 1; i < 7; i++) {
        tree->add(i);
    }
    
    printf("add <key>, remove <key>, size, rotate_left <key>,\n");
    printf("rotate_right <key>, vine, balance, exit\n");
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
        } else if (!cmd.compare("rotate_left")) {
            scanf("%d", &arg);
            tree->rotate_left(arg);
        } else if (!cmd.compare("rotate_right")) {
            scanf("%d", &arg);
            tree->rotate_right(arg);
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

