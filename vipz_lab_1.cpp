#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

struct Book {
    string mAuthor;
    string mNameBook;
    int mYear;
    int mPages;
    int mPrice;
};

struct Node {
    Book mpInfo;
    Node* mpNext;
};

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
                curr->mpNext = nextNode->mpNext;
                nextNode->mpNext = curr;

                if (prev == nullptr) {
                    head = nextNode;
                }
                else {
                    prev->mpNext = nextNode;
                }

                prev = nextNode;
                swapped = true;
            }
            else {
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
        if (!curr->mpInfo.mAuthor.empty() && curr->mpInfo.mAuthor[0] == targetLetter) {
            Node* nodeToDelete = curr;

            if (prev == nullptr) {
                head = curr->mpNext;
                curr = head;
            }
            else {
                prev->mpNext = curr->mpNext;
                curr = curr->mpNext;
            }

            delete nodeToDelete;
        }
        else {
            prev = curr;
            curr = curr->mpNext;
        }
    }
}

void insertNewBook(Node*& head, const Book& newBookData) {
    Node* newNode = new Node;
    newNode->mpInfo = newBookData;
    newNode->mpNext = nullptr;

    if (head == nullptr || newBookData.mYear < head->mpInfo.mYear) {
        newNode->mpNext = head;
        head = newNode;
        return;
    }

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
    cout << "\n";
    cout << left << setw(15) << "Author"
        << setw(15) << "Name of book"
        << setw(10) << "Year"
        << setw(10) << "Pages"
        << setw(10) << "Price" << "\n";
    cout << "------------------------------------------------------------\n";

    while (pHead != nullptr) {
        cout << left << setw(15) << pHead->mpInfo.mAuthor
            << setw(15) << pHead->mpInfo.mNameBook
            << setw(10) << pHead->mpInfo.mYear
            << setw(10) << pHead->mpInfo.mPages
            << setw(10) << pHead->mpInfo.mPrice << "\n";
        pHead = pHead->mpNext;
    }
}

int main() {
    ifstream file("file.txt");
    if (!file.is_open()) {
        cerr << "File not opened." << endl;
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
        }
        else {
            tail->mpNext = newNode;
            tail = newNode;
        }
    }
    file.close();

    cout << "--- Initial Data (Unsorted) ---";
    printList(head);

    cout << "\n--- After Sorting by Year ---";
    sortYear(head);
    printList(head);

    cout << "\n--- After Deleting authors starting with 'K' ---";
    deleteByAuthor(head, 'K');
    printList(head);

    Book newBook = { "Kostenko", "Marysya", 1961, 180, 220 };
    cout << "\n--- After Adding new book ---";
    insertNewBook(head, newBook);
    printList(head);

    freeList(head);

    return 0;
}
