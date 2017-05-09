#include <iostream>
#include <vector>
#include <math.h>

#define WORD_COUNT 10
#define LINE_MAX 20

#define PRINT(text,count) for (int i = 0; i < count; i++) printf(text);

/**
 * Возвращает вектор из N случайных чисел
 */
std::vector<int> generator(int N) {
    std::vector<int> result;
    for (int i = 0; i < N; i++) {
        result.push_back(rand() % 5 + 3);
    }
    return result;
}

/**
 * Возвращает сумму кубов остатков для выбранного состояния.
 * Для неудачных состояний (превышение размера строки) возвращает -1.
 */
int check_state(std::vector<int> words, long int state) {
    int current_line_size = 0;
    int sum = 0;

    for (int word = 0; word < WORD_COUNT; word++) {
        current_line_size += words[word];
        
        // Превышение размера строки
        if (current_line_size > LINE_MAX) {
            return -1;
        }
        
        // Подсчёт остатка при переносе строки
        if ((state >> word) & 1) {
            sum += pow((LINE_MAX - current_line_size), 3);
            current_line_size = 0;
            continue;
        }
        
        current_line_size++; // пробел
    }
    
    return sum;
}

/**
 * Выводит вектор в виде кричащего предложения с учётом переноса строк
 */
void print_vector(std::vector<int> words, long int breaks) {
    int current_line_size = 0;
    
    printf("%ld (%d)\n", breaks, check_state(words, breaks));
    
    // Проходим по всем словам
    for (int word = 0; word < WORD_COUNT; word++) {
        PRINT("A", words[word]);
        current_line_size += words[word];
        
        if ((breaks >> word) & 1) { // Требуется перенос?
            // Добиваем текущую строку и ставим ограничитель
            PRINT(" ", LINE_MAX - current_line_size);
            current_line_size = 0;
            printf("|\n");
        } else {
            printf(" ");
            current_line_size++;
        }
    }
    printf("\n");
}

/**
 * Максимально заполняет строки по порядку (не всегда эффективно)
 */
long int trivial(std::vector<int> words) {
    long int solution = 0;
    int current_line_size = 0;

    for (int word = 0; word < WORD_COUNT; word++) {
        if (LINE_MAX - current_line_size < words[word]) {
            solution |= 1 << (word - 1);
            current_line_size = 0;
        }

        // Пробел не играет роли в конце строки
        current_line_size += words[word] + 1;
    }
    
    return solution;
}

/**
 * Перебирает все состояния в поисках наиболее удачной
 * последовательности переносов
 */
long int brute_force(std::vector<int> words) {
    long int best_state = 0;
    int best_quality = -1;

    // Перебор всех состояний
    for (long int state = 0; state < pow(2, WORD_COUNT); state++) {
        int quality = check_state(words, state);

        if (quality == -1) {
            continue;
        }

        if (quality < best_quality || best_quality == -1) {
            best_state = state;
            best_quality = quality;
        }
    }
    
    return best_state;
}

int main(int argc, char **argv) {
	srand(time(NULL));

    std::vector<int> words;
    long int breaks_trivial, breaks_brute;
    while (true) {
        words = generator(WORD_COUNT);
        breaks_trivial = trivial(words);
        breaks_brute = brute_force(words);

        if (check_state(words, breaks_trivial) != check_state(words, breaks_brute)) {
            print_vector(words, 0);
            print_vector(words, breaks_trivial);
            print_vector(words, breaks_brute);
            break;
        }
    }
    
	return 0;
}

