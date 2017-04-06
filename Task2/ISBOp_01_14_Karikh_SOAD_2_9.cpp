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

            // Удалять корневой элемент нельзя, так как дерево
            // потеряется в памяти. Также мы не можем удалить
            // элемент, если его нет.
            if (tmp->parent == NULL || tmp->key != key) {
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
            
            // Откладываем первый элемент на будущее
            Tree* p = tmp->children[a];
            
            // Если первого элемента нет, то поворот невозможен
            if (p == NULL) {
                return;
            }
            
            // Заменяем первый элемент на первого его потомка
            // (например, на левый потомок левого элемента)
            tmp->children[a] = tmp->children[a]->children[a];
            
            // Если первый потомок существует, то обновим его
            // указатель на родителя.
            if (tmp->children[a] != NULL) {
                tmp->children[a]->parent = tmp;
            }
            
            // Заменяем первого потомка первого элемента на второй
            // его потомок, так как первый потомок уже перенесён
            // в другое место.
            p->children[a] = p->children[b];
            
            // Вторым потомком первого элемента будет второй потомок
            // его нового родителя, так как на его место мы поставим
            // сам элемент.
            p->children[b] = tmp->children[b];
            
            // Если второй потомок существует, обновим его указатель
            // на родителя.
            if (p->children[b] != NULL) {
                p->children[b]->parent = p;
            }
            
            // Заменим второго потомка элемента на сохранённый ранее
            // элемент, так как бывший потомок был перенесён.
            tmp->children[b] = p;
            p->parent = tmp;
            
            // Меняем местами ключи у второго потомка и основного
            // элемента.
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

        void balance() {
            // Если количество элементов не равно 2^n-1, то выходим
            float size_log = log2(size() + 1);
            if (floor(size_log) != size_log) {
                return;
            }
            
            vine(); // необходимо для балансировки

            Tree* tmp = NULL;
            for (int i = 1; i < size_log; i++) {
                tmp = this;
                for (int j = 0; j < pow(2, size_log-i) - 1; j++) {
                    if (tmp->children[0] != NULL) {
                        rotate(tmp->key, true);
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
