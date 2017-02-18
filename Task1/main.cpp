/**
 * Практическое задание 1 (сложный вариант, очередь AAA)
 * 
 * @author Dmitry Karikh <the.dr.hax@gmail.com>
 * @see <a href="https://vk.com/doc-76267187_442123883?hash=db6e91f00542c3b141&dl=8e2648ac6abfb288ce">ПР1.pdf</a>
 */
	
#include <iostream>
	
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
		int length = 0;
	
	public:
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
		 * @param a Значение нового элемента.
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
 * Очередь (чистая реализация)
 */
class Queue {
	private:
		/**
		 * Указатели на первый и последний элементы
		 */
		Node *first, *last;
	
		/**
		 * Текущая длина очереди
		 */
		int length = 0;
	
	public:
		~Queue() {
			// Удаляем все оставшиеся элементы
			while (length > 0) {
				pop();
			}
		}
	
		/**
		 * Возвращает текущую длину очереди
		 * @return длина очереди
		 */
		int size() {
			return length;
		}
	
		/**
		 * Помещает новый элемент в конец очереди
		 * @param a Значение нового элемента
		 */
		void push(int a) {
			if (length == 0) {
				first = last = new Node;
				first->data = a;
			} else {
				Node *last_old = last;
				last = new Node;
				last->data = a;
				last->previous = last_old;
				last->previous->next = last;
			}
			length++;
		}
	
		/**
		 * Забирает первый элемент из очереди
		 * @return значение элемента
		 */
		int pop() {
			if (length == 0) {
				return -1;
			}
			length--;
	
			Node *first_old = first;
			int data = first_old->data;
			first = first->next;
			delete first_old;
			return data;
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
		 * @param a начение нового элемента
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
	Stack *stack = new Stack;
	stack->push(1);
	stack->push(2);
	stack->push(3);
	
	std::cout << "Stack test:" << std::endl;
	while (stack->size() > 0) {
		std::cout << stack->pop() << std::endl;
	}
	std::cout << std::endl;
	
	delete stack;
	
	Queue *queue = new Queue;
	queue->push(1);
	queue->push(2);
	queue->push(3);
	
	std::cout << "Queue test:" << std::endl;
	while (queue->size() > 0) {
		std::cout << queue->pop() << std::endl;
	}
	std::cout << std::endl;
	
	delete queue;
	
	StackedQueue *squeue = new StackedQueue;
	squeue->push(1);
	squeue->push(2);
	squeue->push(3);
	
	std::cout << "StackedQueue test:" << std::endl;
	while (squeue->size() > 0) {
		std::cout << squeue->pop() << std::endl;
	}
	std::cout << std::endl;
	
	delete squeue;
	
	return 0;
}
	
