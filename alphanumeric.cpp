#include <iostream>
#include <pthread.h>
#include <vector>
#include <string>
#include <cstring>
#include <cctype>

using namespace std;

string input_string;
vector<string> words;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int current_word = 0;
bool signal = false;
void* arf;
pthread_t alpha_thread, numeric_thread;

void* alpha(void* arg) {
    string thread_name = (char*) arg;
    while (true) {
        pthread_mutex_lock(&mutex);
        if (current_word == words.size()) {
            // at the end of the string
            pthread_mutex_unlock(&mutex);
            break;
        }
        string word = words[current_word];
        pthread_mutex_unlock(&mutex);
        if (isalpha(word[0])) {
            cout << "alpha: " << word << endl;
            current_word++;
        } else if (current_word == words.size()) {
           pthread_exit(NULL);
        } else {
            // Dont exit thread please
            arf = arg;
            break;
        }
    }
    pthread_exit(NULL);
}

void* numeric(void* arg) {
    string thread_name = (char*) arg;
    while (true) {
        pthread_mutex_lock(&mutex);
        if (current_word == words.size()) {
            // at the end of the string
            pthread_mutex_unlock(&mutex);
            break;
        }
        string word = words[current_word];
        pthread_mutex_unlock(&mutex);
        if (isdigit(word[0])) {
            cout << "numeric: " << word << endl;
            current_word++;
        } else if (current_word == words.size()) {
            pthread_exit(NULL);
        } else {
            // dont exit thread please
            arf = arg;
            break;
        }
    }
   pthread_exit(NULL);
}

void helper() {
    while (current_word != words.size()) {
        pthread_create(&alpha_thread, NULL, alpha, (void*) "alpha");
        pthread_create(&numeric_thread, NULL, numeric, (void*) "numeric");
        pthread_join(alpha_thread, NULL);
        pthread_join(numeric_thread, NULL);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Enter string: ";
        getline(cin, input_string);
    } else {
        input_string = argv[1];
    }
    char* str = new char[input_string.length() + 1];
    strcpy(str, input_string.c_str());
    char* token = strtok(str, " ");
    while (token != NULL) {
        words.push_back(token);
        token = strtok(NULL, " ");
    }
    delete[] str;
    helper();
    return 0;
}
