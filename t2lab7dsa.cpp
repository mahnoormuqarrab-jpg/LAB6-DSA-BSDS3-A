#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
struct Student {
    int id;
    string name;
    float gpa;
    Student *left, *right;
    Student(int i, string n, float g) {
        id = i;
        name = n;
        gpa = g;
        left = right = nullptr;
    }
};
void show(Student* s) {
    cout << "  ID: " << setw(5) << s->id
         << " | Name: " << setw(10) << left << s->name << right
         << " | GPA: " << fixed << setprecision(2) << s->gpa << endl;
}
Student* insert(Student* root, int id, string name, float gpa) {
    if (root == nullptr)
        return new Student(id, name, gpa);

    if (id < root->id)
        root->left = insert(root->left, id, name, gpa);
    else if (id > root->id)
        root->right = insert(root->right, id, name, gpa);
    else
        cout << "  ID " << id << " already exists.\n";

    return root;
}
Student* search(Student* root, int id) {
    if (root == nullptr || root->id == id)
        return root;
    if (id < root->id)
        return search(root->left, id);
    return search(root->right, id);
}
Student* findMin(Student* root) {
    while (root->left != nullptr)
        root = root->left;
    return root;
}
Student* remove(Student* root, int id, bool& found) {
    if (root == nullptr)
        return nullptr;
    if (id < root->id) {
        root->left = remove(root->left, id, found);
    }
    else if (id > root->id) {
        root->right = remove(root->right, id, found);
    }
    else {
        found = true;

        if (root->left == nullptr) {
            Student* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == nullptr) {
            Student* temp = root->left;
            delete root;
            return temp;
        }
        // two children, replace with inorder successor
        Student* succ = findMin(root->right);
        root->id = succ->id;
        root->name = succ->name;
        root->gpa = succ->gpa;
        bool tmp;
        root->right = remove(root->right, succ->id, tmp);
    }
    return root;
}
void inorder(Student* root) {
    if (root == nullptr) return;
    inorder(root->left);
    show(root);
    inorder(root->right);
}
int count(Student* root) {
    if (root == nullptr) return 0;
    return 1 + count(root->left) + count(root->right);
}
// tree is sorted by ID so we have to check every node for GPA
void findHiLo(Student* root, Student*& hi, Student*& lo) {
    if (root == nullptr) return;
    if (hi == nullptr || root->gpa > hi->gpa) hi = root;
    if (lo == nullptr || root->gpa < lo->gpa) lo = root;
    findHiLo(root->left, hi, lo);
    findHiLo(root->right, hi, lo);
}
void above(Student* root, float t, int& n) {
    if (root == nullptr) return;
    above(root->left, t, n);
    if (root->gpa > t) {
        show(root);
        n++;
    }
    above(root->right, t, n);
}
void freeTree(Student* root) {
    if (root == nullptr) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
void menu() {
    cout << "\n===== Student Records (BST) =====\n";
    cout << "1. Insert student\n";
    cout << "2. Search by ID\n";
    cout << "3. Delete by ID\n";
    cout << "4. List all\n";
    cout << "5. Count students\n";
    cout << "6. Highest & lowest GPA\n";
    cout << "7. Students above GPA\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
}
int main() {
    Student* root = nullptr;

    root = insert(root, 50, "Ali", 3.40);
    root = insert(root, 30, "Sara", 3.85);
    root = insert(root, 70, "Hamza", 2.60);
    root = insert(root, 20, "Ayesha", 3.10);
    root = insert(root, 40, "Bilal", 2.90);
    root = insert(root, 60, "Zainab", 3.95);
    root = insert(root, 80, "Usman", 2.20);
    int choice;
    do {
        menu();
        if (!(cin >> choice)) break;
        if (choice == 1) {
            int id;
            string name;
            float gpa;
            cout << "ID: ";
            cin >> id;
            cout << "Name: ";
            cin >> name;
            cout << "GPA: ";
            cin >> gpa;
            root = insert(root, id, name, gpa);
        }
        else if (choice == 2) {
            int id;
            cout << "ID to search: ";
            cin >> id;
            Student* s = search(root, id);
            if (s) show(s);
            else cout << "  Not found.\n";
        }
        else if (choice == 3) {
            int id;
            bool found = false;
            cout << "ID to delete: ";
            cin >> id;
            root = remove(root, id, found);
            if (found) cout << "  Deleted.\n";
            else cout << "  ID not found.\n";
        }
        else if (choice == 4) {
            if (root == nullptr) cout << "  (empty)\n";
            else inorder(root);
        }
        else if (choice == 5) {
            cout << "  Total students: " << count(root) << endl;
        }
        else if (choice == 6) {
            Student* hi = nullptr;
            Student* lo = nullptr;
            findHiLo(root, hi, lo);
            if (hi == nullptr) {
                cout << "  (empty)\n";
            } else {
                cout << "  Highest GPA:\n";
                show(hi);
                cout << "  Lowest GPA:\n";
                show(lo);
            }
        }
        else if (choice == 7) {
            float t;
            int n = 0;
            cout << "Threshold: ";
            cin >> t;
            above(root, t, n);
            if (n == 0) cout << "  No student above " << t << endl;
        }
    } while (choice != 0);
    freeTree(root);
    return 0;
}