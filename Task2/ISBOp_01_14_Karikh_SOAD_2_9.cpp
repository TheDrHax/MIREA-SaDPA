#include <stdio.h>
#include <iostream>

class Tree {
    private:
        Tree* parent = NULL;
        Tree* left;
        Tree* right;
        int key;

        /**
         * Возвращает элемент по ключу, если он существует.
         * Иначе возвращает элемент, который подходит для вставки
         * нового элемента с указанным ключом.
         */
        Tree* find(int key) {
            Tree* tmp = this;

            while (true) {
                if (key < tmp->key && tmp->left != NULL) {
                    tmp = tmp->left;
                } else if (key > tmp->key && tmp->right != NULL) {
                    tmp = tmp->right;
                } else {
                    return tmp;
                }
            }
        }

    public:
        Tree(int key) {
            this->key = key;
        }

        Tree(int key, Tree* parent) {
            this->key = key;
            this->parent = parent;
        }

        /**
         * Выводит дерево в формате graphviz.
         */
        void print() {
            if (parent == NULL)
                std::cout << "digraph G {" << std::endl;

            if (left != NULL) {
                left->print();
            }
            if (right != NULL) {
                right->print();
            }

            if (parent != NULL)
                printf("  \"%d\" -> \"%d\"\n", parent->key, key);

            if (parent == NULL)
                std::cout << "}" << std::endl;
        }

        /**
         * Добавляет новый элемент в дерево, если он ещё не существует.
         */
        void add(int key) {
            Tree* tmp = find(key);

            if (key < tmp->key) {
                tmp->left = new Tree(key, tmp);
            } else if (key > tmp->key) {
                tmp->right = new Tree(key, tmp);
            } else {
                tmp->key = key;
            }
        }

        /**
         * Удаляет ключ из дерева.
         */
        void remove(int key) {
            Tree* tmp = find(key);
            
            // Удалять корневой элемент нельзя, так как дерево
            // потеряется в памяти. Также мы не можем удалить
            // элемент, если его нет.
            if (tmp->parent == NULL || tmp->key != key) {
                return;
            }

            // Ищем указатель на текущий элемент у родителя для
            // быстрого доступа к нему без лишних условий.
            Tree** parent_ptr = NULL;
            if (tmp->parent->left == tmp) {
                parent_ptr = &tmp->parent->left;
            } else if (tmp->parent->right == tmp) {
                parent_ptr = &tmp->parent->right;
            } else {
                printf("WTF: %s\n",
                       "parent doesn't know about this element");
                throw "WTF";
            }

            // Если у элемента нет потомков, то просто удаляем его.
            // Также не забываем очистить указатель у родителя.
            if (tmp->left == NULL && tmp->right == NULL) {
                (*parent_ptr) = NULL;
                delete tmp;
                return;
            }

            // Если есть один дочерний элемент, то заменяем
            // удаляемый дочерним. Не забываем заменить родителя у
            // дочернего элемента.
            if (tmp->left == NULL && tmp->right != NULL) { // правый
                (*parent_ptr) = tmp->right;
                tmp->right->parent = tmp->parent;
                delete tmp;
                return;
            } else if (tmp->left != NULL && tmp->right == NULL) { // левый
                (*parent_ptr) = tmp->left;
                tmp->left->parent = tmp->parent;
                delete tmp;
                return;
            }

            // Проблема: у удаляемого элемента есть два дочерних.
            if (tmp->left != NULL && tmp->right != NULL) {
                if (tmp->right->left == NULL) {
                    // Не затрагивая левую ветвь заменяем текущий
                    // элемент на правый дочерний. У правого дочернего
                    // нет левой ветви, так что мы ничего не теряем.
                    Tree* tmp_right = tmp->right;
                    
                    tmp->key = tmp->right->key;
                    tmp->right = tmp->right->right;
                    if (tmp->right != NULL) {
                        tmp->right->parent = tmp;
                    }
                    
                    delete tmp_right;
                } else {
                    // Ищем элемент, не имеющий левых потомков, в
                    // правом поддереве и заменяем удаляеммый элемент
                    // на него.
                    Tree* tmp_left = tmp->right->left;

                    while (tmp_left->left != NULL) {
                        tmp_left = tmp_left->left;
                    }

                    tmp->key = tmp_left->key;
                    tmp->right->remove(tmp_left->key);
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
    
    std::cout << "add <key>, remove <key>, exit" << std::endl;
    while (true) {
        scanf("%s", cmd_raw);
        cmd = cmd_raw;
        
        if (!cmd.compare("add")) {
            scanf("%d", &arg);
            tree->add(arg);
            tree->print();
        } else if (!cmd.compare("remove")) {
            scanf("%d", &arg);
            tree->remove(arg);
            tree->print();
        } else if (!cmd.compare("exit")) {
            break;
        }
    }

    delete tree;

    return 0;
}

