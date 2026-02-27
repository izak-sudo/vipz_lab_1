#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

struct Book {
    std::string mAuthor;
    std::string mNameBook;
    int mYear;
    int mPages;
    int mPrice;
};

struct Node {
    Book mpInfo;
    Node* mpNext;
};

// Передаємо вказівник за посиланням (Node*&), щоб мати змогу змінювати голову списку
void sortYear(Node*& head) {
    if (head == nullptr || head->mpNext == nullptr) {
        return;
    }

    bool swapped;
    do {
        swapped = false;
        Node* curr = head;
        Node* prev = nullptr;

        while (curr->mpNext != nullptr) {
            Node* nextNode = curr->mpNext;

            if (curr->mpInfo.mYear > nextNode->mpInfo.mYear) {
                // Переприв'язуємо вказівники
                curr->mpNext = nextNode->mpNext;
                nextNode->mpNext = curr;

                if (prev == nullptr) {
                    head = nextNode; // Оновлюємо голову, якщо змінили перший елемент
                } else {
                    prev->mpNext = nextNode;
                }

                prev = nextNode; // curr залишається тим самим об'єктом, змістився лише prev
                swapped = true;
            } else {
                prev = curr;
                curr = curr->mpNext;
            }
        }
    } while (swapped);
}

void deleteByAuthor(Node*& head, char targetLetter) {
    Node* curr = head;
    Node* prev = nullptr;

    while (curr != nullptr) {
        // Перевіряємо, чи рядок не порожній і чи починається на задану літеру
        if (!curr->mpInfo.mAuthor.empty() && curr->mpInfo.mAuthor[0] == targetLetter) {
            Node* nodeToDelete = curr;

            if (prev == nullptr) {
                head = curr->mpNext;
                curr = head;
            } else {
                prev->mpNext = curr->mpNext;
                curr = curr->mpNext;
            }

            delete nodeToDelete;
        } else {
            prev = curr;
            curr = curr->mpNext;
        }
    }
}

// Нова реалізована функція додавання, яка сама створює вузол
void insertNewBook(Node*& head, const Book& newBookData) {
    Node* newNode = new Node;
    newNode->mpInfo = newBookData;
    newNode->mpNext = nullptr;

    // Якщо список порожній або новий елемент має бути першим
    if (head == nullptr || newBookData.mYear < head->mpInfo.mYear) {
        newNode->mpNext = head;
        head = newNode;
        return;
    }

    // Шукаємо позицію для вставки
    Node* temp = head;
    while (temp->mpNext != nullptr && temp->mpNext->mpInfo.mYear <= newBookData.mYear) {
        temp = temp->mpNext;
    }

    newNode->mpNext = temp->mpNext;
    temp->mpNext = newNode;
}

void freeList(Node*& head) {
    Node* temp = head;
    while (temp != nullptr) {
        Node* next = temp->mpNext;
        delete temp;
        temp = next;
    }
    head = nullptr;
}

void printList(Node* pHead) {
    std::cout << "\n";
    std::cout << std::left << std::setw(15) << "Author"
              << std::setw(15) << "Name of book"
              << std::setw(10) << "Year"
              << std::setw(10) << "Pages"
              << std::setw(10) << "Price" << "\n";
    std::cout << "------------------------------------------------------------\n";
    
    while (pHead != nullptr) {
        std::cout << std::left << std::setw(15) << pHead->mpInfo.mAuthor
                  << std::setw(15) << pHead->mpInfo.mNameBook
                  << std::setw(10) << pHead->mpInfo.mYear
                  << std::setw(10) << pHead->mpInfo.mPages
                  << std::setw(10) << pHead->mpInfo.mPrice << "\n";
        pHead = pHead->mpNext;
    }
}

int main() {
    std::ifstream file("file.txt");
    if (!file.is_open()) {
        std::cerr << "File not opened." << std::endl;
        return 1;
    }

    Node* head = nullptr;
    Node* tail = nullptr;
    Book tempBook;

    while (file >> tempBook.mAuthor >> tempBook.mNameBook >> tempBook.mYear >> tempBook.mPages >> tempBook.mPrice) {
        Node* newNode = new Node;
        newNode->mpInfo = tempBook;
        newNode->mpNext = nullptr;

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->mpNext = newNode;
            tail = newNode;
        }
    }
    file.close();

    std::cout << "--- Initial Data (Unsorted) ---";
    printList(head);

    std::cout << "\n--- After Sorting by Year ---";
    sortYear(head);
    printList(head);

    std::cout << "\n--- After Deleting authors starting with 'K' ---";
    deleteByAuthor(head, 'K');
    printList(head);

    // Використання нової функції для додавання
    Book newBook = {"Kostenko", "Marysya", 1961, 180, 220};
    std::cout << "\n--- After Adding new book ---";
    insertNewBook(head, newBook);
    printList(head);

    freeList(head);

    return 0;
}
