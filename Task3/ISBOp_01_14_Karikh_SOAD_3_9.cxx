#include <iostream>
#include <vector>
#include <math.h>

#define WORD_COUNT 20
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
 * Выводит вектор в виде кричащего предложения с учётом переноса строк
 */
void print_vector(std::vector<int> words, long int state) {
    int current_line_size = 0;
    
    // Проходим по всем словам
    for (int word = 0; word < WORD_COUNT; word++) {
        PRINT("A", words[word]);
        current_line_size += words[word];
        
        if ((state >> word) & 1) { // Требуется перенос?
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

    std::vector<int> words = generator(WORD_COUNT);
    print_vector(words, 0);
    
    long int breaks = brute_force(words);
    printf("%ld (%d)\n", breaks, check_state(words, breaks));
    print_vector(words, breaks);
    
    
	return 0;
}

