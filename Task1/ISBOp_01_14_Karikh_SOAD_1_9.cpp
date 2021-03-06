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
            while (!isEmpty()) {
                drop(); // удаляет элемент
            }
        }

        /**
         * Определяет, остались ли в стеке элементы
         */
        bool isEmpty() {
            return (top == NULL);
        }

        /**
         * Помещает новый элемент в вершину стека
         */
        void push(int a) {
            Node *top_old = top;
            top = new Node;
            top->data = a;
            top->next = top_old;
        }

        /**
         * Удаляет текущий элемемент
         */
        void drop() {
            if (!isEmpty()) {
                Node *top_old = top;
                top = top->next;
                delete top_old;
            }
        }

        /**
         * Считывает элемент без его удаления
         */
        int pick() {
            if (isEmpty()) {
                throw "Stack is empty! (╯°□°）╯︵ ┻━┻";
            }
            return top->data;
        }

        /**
         * Забирает элемент из вершины стека
         */
        int pop() {
            int data = pick();
            drop();
            return data;
        }
};

/**
 * Очередь (реализация на двух стеках)
 */
class StackedQueue : public Stack {
    private:
        /**
         * Обратный стек, использующийся для реорганизации
         * основного стека во время добавления элементов.
         */
        Stack backward;

    public:
        /**
         * Помещает новый элемент в очередь
         */
        void push(int a) {
            // Переносим основной стек в обратный
            // (с обратным порядком элементов)
            while (!isEmpty()) {
                backward.push(pop());
            }

            // Обращаемся к методу push() родительского
            // класса Stack, так как в этом классе он перегружен
            Stack::push(a); // добавляем новый элемент

            // Возвращаем все элементы из обратного стека в основной
            while (!backward.isEmpty()) {
                Stack::push(backward.pop());
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
                try {
                    std::cout << std::endl << "Popped: "
                              << queue->pop() << std::endl;
                } catch (const char *ex) {
                    std::cout << std::endl << ex << std::endl;
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
        std::cin.ignore(); // чистим буфер ввода
        getline(std::cin, tmp); // ждём
    }

    delete queue;

    return 0;
}

