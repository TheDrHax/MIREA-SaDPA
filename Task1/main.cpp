/**
 * Практическое задание 1 (сложный вариант, очередь AAA)
 *
 * @author Dmitry Karikh <the.dr.hax@gmail.com>
 * @see <a href="https://vk.com/doc-76267187_442123883?hash=db6e91f00542c3b141&dl=8e2648ac6abfb288ce">ПР1.pdf</a>
 */

#include <iostream>

using namespace std;

/**
 * Внутренняя структура
 */
struct Node {
    /**
     * Поле данных
     */
    int data;

    /**
     * Указатели на соседние структуры
     */
    Node *previous, *next;
};

/**
 * Стек (чистая реализация)
 */
class Stack {
    private:
        /**
         * Указатель на вершину стека
         */
        Node *top;

        /**
         * Текущая высота стека
         */
        int length;

    public:
        Stack() {
            length = 0;
        }
        ~Stack() {
            // Удаляем все оставшиеся элементы
            while (length > 0) {
                pop();
            }
        }

        /**
         * Возвращает текущую высоту стека
         * @return высота стека
         */
        int size() {
            return length;
        }

        /**
         * Помещает новый элемент в вершину стека
         * @param a значение нового элемента
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
         * @return значение элемента
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
         * @return значение элемента
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
 * @see Stack
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
         * @param a значение нового элемента
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
    bool running = true;
    int answer = 0;

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
         */
        string tmp;
        cin.ignore(); // чистим буффер ввода
        getline(cin, tmp); // ждём
    }

    delete queue;

    return 0;
}

