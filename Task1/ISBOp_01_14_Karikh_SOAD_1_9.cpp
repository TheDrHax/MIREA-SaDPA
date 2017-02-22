/**
 * ISBOp_01_14_Karikh_SOAD_1_9.cpp
 *
 * Практическое задание 1 (https://vk.com/doc-76267187_442123883)
 * Вариант 9
 *
 * Карих Дмитрий, ИСБОп-01-14
 * https://github.com/TheDrHax/MIREA-SaDPA
 */

#include <iostream>

/**
 * Элемент для хранения данных
 */
struct Node {
    int data; // поле данных
    Node *next; // указатель на следующий элемент
};

/**
 * Стек (чистая реализация)
 */
class Stack {
    private:
        Node *top; // указатель на вершину стека

    public:
        Stack() {
            top = NULL;
        }
        ~Stack() {
            while (top != NULL) {
                pop(); // удаляет элемент, запрашивая его значение
            }
        }

        /**
         * Помещает новый элемент в вершину стека
         */
        void push(int a) {
            if (top == NULL) {
                top = new Node;
                top->next = NULL;
                top->data = a;
            } else {
                Node *top_old = top;
                top = new Node;
                top->data = a;
                top->next = top_old;
            }
        }

        /**
         * Забирает элемент из вершины стека
         */
        int pop() {
            if (top == NULL) {
                return -1;
            }

            Node *top_old = top;
            int data = top->data;
            top = top->next;
            delete top_old;
            return data;
        }

        /**
         * Считывает элемент без его удаления
         */
        int pick() {
            if (top == NULL) {
                return -1;
            }
            return top->data;
        }
};

/**
 * Очередь (реализация на двух стеках)
 */
class StackedQueue : public Stack {
    private:
        /**
         * Указатель на обратный стек, использующийся для реорганизации
         * основного стека во время добавления элементов.
         */
        Stack *backward;

    public:
        StackedQueue() {
            backward = new Stack;
        }
        ~StackedQueue() {
            delete backward;
        }

        /**
         * Помещает новый элемент в очередь
         */
        void push(int a) {
            // Переносим основной стек в обратный
            // (с обратным порядком элементов)
            while (pick() != -1) {
                backward->push(pop());
            }

            // Обращаемся к методу push() родительского
            // класса Stack, так как в этом классе он перегружен
            Stack::push(a); // добавляем новый элемент

            // Возвращаем все элементы из обратного стека в основной
            while (backward->pick() != -1) {
                Stack::push(backward->pop());
            }
        }
};

int main(int argc, char **argv) {
    StackedQueue *queue = new StackedQueue;
    bool running = true; // условие для остановки бесконечного цикла
    int answer = 0; // ответ пользователя

    while (running) {
        std::cout << "0. exit" << std::endl;
        std::cout << "1. push" << std::endl;
        std::cout << "2. pop" << std::endl;
        std::cout << "3. pick" << std::endl;
        std::cout << "Please choose the next action: ";

        std::cin >> answer;
        switch (answer) {
            case 1:
                std::cout << "Please enter a number: ";
                std::cin >> answer;
                queue->push(answer);
                std::cout << std::endl << "Pushed: "
                          << answer << std::endl;
                break;

            case 2:
                if (queue->pick() != -1) {
                    std::cout << std::endl << "Popped: "
                              << queue->pop() << std::endl;
                } else {
                    std::cout << std::endl
                              << "Queue is empty! (╯°□°）╯︵ ┻━┻"
                              << std::endl;
                }
                break;

            case 3:
                if (queue->pick() != -1) {
                    std::cout << std::endl << "Picked: "
                              << queue->pick() << std::endl;
                } else {
                    std::cout << std::endl
                              << "Queue is empty! (╯°□°）╯︵ ┻━┻"
                              << std::endl;
                }
                break;

            default:
                running = false;
                break;
        }
        std::cout << std::endl << "Press Enter to continue..."
                  << std::endl;

        /**
         * Ждём, пока пользователь нажмёт Enter
         * (system("PAUSE") всё-равно не работает без cin.ignore())
         */
        std::string tmp;
        std::cin.ignore(); // чистим буффер ввода
        getline(std::cin, tmp); // ждём
    }

    delete queue;

    return 0;
}

