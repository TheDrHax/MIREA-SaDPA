#include <iostream>
#include <vector>
#include <math.h>

#define WORD_COUNT 10
#define LINE_MAX 20

#define REPEAT(action,count) for (int _i_ = 0; _i_ < count; _i_++) action;

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

    for (unsigned int word = 0; word < words.size(); word++) {
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

    sum += pow((LINE_MAX - current_line_size), 3);
    
    return sum;
}

/**
 * Выводит вектор в виде кричащего предложения с учётом переноса строк
 */
void print_vector(std::vector<int> words, long int breaks) {
    int current_line_size = 0;
    
    printf("%ld (%d)\n", breaks, check_state(words, breaks));
    
    // Проходим по всем словам
    for (unsigned int word = 0; word < words.size(); word++) {
        REPEAT(printf("A"), words[word]);
        current_line_size += words[word];
        
        if ((breaks >> word) & 1 || word == words.size() - 1) {
            // Добиваем текущую строку и ставим ограничитель
            REPEAT(printf(" "), LINE_MAX - current_line_size);
            printf("| %d\n", LINE_MAX - current_line_size);
            current_line_size = 0;
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
long int greedy(std::vector<int> words) {
    long int solution = 0;
    int current_line_size = 0;

    for (unsigned int word = 0; word < words.size(); word++) {
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
    for (long int state = 0; state < pow(2, words.size()); state++) {
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

/**
 * Восходящий динамический алгоритм
 */
long int dynamic(std::vector<int> words) {
    long int result = 0;

    // Таблица возможных переносов и остатков
    int S[words.size()][words.size()] = {};
    for (unsigned int i = 0; i < words.size(); i++) {
        S[i][i] = LINE_MAX - words[i];
        for (unsigned int j = i+1; j < words.size(); j++) {
            S[i][j] = S[i][j-1] - words[j] - 1;
        }
    }

    // Идём "по диагонали", подбирая наиболее удачные переносы строк
    int R[words.size() + 1] = {};
    int B[words.size()] = {};
    for (unsigned int j = 0; j < words.size(); j++) {
        for (int i = j; i >= 0; i--) {
            // Игнорируем слишком длинные строки
            if (S[i][j] < 0) continue;

            int cost = R[i] + pow(S[i][j], 3);
            if (R[j+1] > cost || R[j+1] == 0) {
                R[j+1] = cost;
                B[j] = i;
            }
        }
    }

    // Восстанавливаем код состояния, начиная с последней строки
    int j = words.size();
    while (j > 0) {
        int i = B[j-1];
        if (result != 0) {
            result |= 1;
            result = result << (j-i);
        } else { // Игнорируем перенос после последней строки
            result = 1;
        }
        j = i;
    }

    return result >> 1;
}

int main(int argc, char **argv) {
	srand(time(NULL));

    std::vector<int> words = generator(WORD_COUNT);

    print_vector(words, greedy(words));
    print_vector(words, brute_force(words));
    print_vector(words, dynamic(words));
    
	return 0;
}

