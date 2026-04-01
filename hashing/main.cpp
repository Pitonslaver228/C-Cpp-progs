#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cctype>

using namespace std;

// функция для получения хэша слова
string getHash(const string& word) {
    unordered_map<char, int> freq;

    // считаем частоты символов
    for (char c : word) {
        freq[c]++;
    }

    int maxCount = 0;
    char maxChar = 0;

    // ищем первый символ с максимальной частотой
    for (char c : word) {
        if (freq[c] > maxCount) {
            maxCount = freq[c];
            maxChar = c;
        }
    }

    return string(1, maxChar) + to_string(maxCount);
}

int main() {
    string text;
    cout << "Enter text: ";
    getline(cin, text);

    // разбиваем текаст на слова
    istringstream stream(text);
    string word;
    unordered_map<string, vector<string>> hashTable;

    while (stream >> word) {
        string hash = getHash(word);
        hashTable[hash].push_back(word);
    }

    string searching_word;
    cout << "Enter word you want to search: ";
    cin >> searching_word;

    string searchingHash = getHash(searching_word);
    cout << "Hash: " << searchingHash << endl;

    // проверяем на наличие ключа-хэша
    if (hashTable.count(searchingHash)) {
        const auto& candidates = hashTable[searchingHash];
        bool found = false;

        // перебираем вектор слов если ключ присутствует и сравниваем с искомым 
        for (const string& candidate : candidates) {
            if (candidate == searching_word) {
                cout << "The word has searched!" << endl;
                found = true;
                break;
            }
        }

        // если не найдено искомое слово то коллизия
        if (!found) {
            cout << "Collision." << endl;
            cout << "Words with similar hash: ";
            for (const string& c : candidates) {
                cout << c << " ";
            }
            cout << endl;
        }
    } else {
        cout << "The word hasn't searched." << endl;
    }

    return 0;
}
