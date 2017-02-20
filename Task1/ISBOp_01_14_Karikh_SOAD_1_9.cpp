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

using namespace std;

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
        int length; // текущая высота стека

    public:
        Stack() {
            length = 0;
        }
        ~Stack() {
            while (length > 0) {
                pop(); // удаляет элемент, запрашивая его значение
            }
        }

        /**
         * Возвращает текущую высоту стека
         */
        int size() {
            return length;
        }

        /**
         * Помещает новый элемент в вершину стека
         */
        void push(int a) {
            if (length == 0) {
                top = new Node;
                top->data = a;
            } else {
                Node *top_old = top;
                top = new Node;
                top->data = a;
                top->next = top_old;
            }
            length++;
        }

        /**
         * Забирает элемент из вершины стека
         */
        int pop() {
            if (length == 0) {
                return -1;
            }
            length--;

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
            if (length == 0) {
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
            while (size() > 0) {
                backward->push(pop());
            }

            // Обращаемся к методу push() родительского
            // класса Stack, так как в этом классе он перегружен
            Stack::push(a); // добавляем новый элемент

            // Возвращаем все элементы из обратного стека в основной
            while (backward->size() > 0) {
                Stack::push(backward->pop());
            }
        }
};

int main(int argc, char **argv) {
    StackedQueue *queue = new StackedQueue;
    bool running = true; // условие для остановки бесконечного цикла
    int answer = 0; // ответ пользователя

    while (running) {
        cout << "0. exit" << endl;
        cout << "1. push" << endl;
        cout << "2. pop" << endl;
        cout << "3. pick" << endl;
        cout << "Please choose the next action: ";

        cin >> answer;
        switch (answer) {
            case 1:
                cout << "Please enter a number: ";
                cin >> answer;
                queue->push(answer);
                cout << endl << "Pushed: " << answer << endl;
                break;

            case 2:
                if (queue->size() > 0) {
                    cout << endl << "Popped: " << queue->pop() << endl;
                } else {
                    cout << endl << "Queue is empty! (╯°□°）╯︵ ┻━┻" << endl;
                }
                break;

            case 3:
                if (queue->size() > 0) {
                    cout << endl << "Picked: " << queue->pick() << endl;
                } else {
                    cout << endl << "Queue is empty! (╯°□°）╯︵ ┻━┻" << endl;
                }
                break;

            default:
                running = false;
                break;
        }
        cout << endl << "Press Enter to continue..." << endl;

        /**
         * Ждём, пока пользователь нажмёт Enter
         * (system("PAUSE") всё-равно не работает без cin.ignore())
         */
        string tmp;
        cin.ignore(); // чистим буффер ввода
        getline(cin, tmp); // ждём
    }

    delete queue;

    return 0;
}

