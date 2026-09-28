// ============================================================
// PHASE 1 - BASICS REFRESH
// Goal: comfortable with pointers before real project.
//
//   Part 1: struct vs class
//   Part 2: pointer basics
//   Part 3: new / delete
//   Part 4: link two nodes manually  (node1->next = node2)
//   Part 5: linked list of 3 nodes, traverse & print
//
// Compile : g++ -std=c++17 -Wall -Wextra phase1_basics.cpp -o phase1
// Run     : ./phase1       (Windows: phase1.exe)
// ============================================================
#include <iostream>
#include <string>
using namespace std;

// ---------- Part 1: struct vs class ----------
// ONLY difference: default access.
//   struct -> members public  by default
//   class  -> members private by default
struct PageS {              // struct: url is public, can touch directly
    string url;
};

class PageC {               // class: url is private, need getter
    string url;
public:
    PageC(const string& u) : url(u) {}
    string getUrl() const { return url; }
};

void part1_structVsClass() {
    cout << "--- Part 1: struct vs class ---\n";

    PageS a;
    a.url = "google.com";                       // direct access OK
    cout << "struct Page url : " << a.url << "\n";

    PageC b("youtube.com");
    // b.url = "x";                             // ERROR: url is private
    cout << "class  Page url : " << b.getUrl() << "\n\n";
}

// ---------- Part 2: pointer basics ----------
void part2_pointerBasics() {
    cout << "--- Part 2: pointer basics ---\n";

    int x = 10;
    int* p = &x;             // p stores ADDRESS of x
    cout << "x  = " << x  << "\n";
    cout << "&x = " << &x << "   (address of x)\n";
    cout << "p  = " << p  << "   (same address)\n";
    cout << "*p = " << *p << "   (value at that address)\n";

    *p = 20;                 // change x THROUGH pointer
    cout << "after *p = 20, x = " << x << "\n\n";
}

// ---------- Part 3: new / delete ----------
// Node used in Part 3, 4, 5. "next" = pointer to another Page.
struct Page {
    string url;
    Page*  next;
};

void part3_newDelete() {
    cout << "--- Part 3: new / delete ---\n";

    Page* pg = new Page;          // allocate on HEAP (lives until we delete)
    pg->url  = "github.com";      // p->member  is same as (*p).member
    pg->next = nullptr;           // always set next, never leave garbage
    cout << "Page url : " << pg->url << "\n";

    delete pg;                    // free memory (every new needs one delete)
    pg = nullptr;                 // no dangling pointer
    cout << "freed.\n\n";
}

// ---------- Part 4: link two nodes manually ----------
void part4_linkTwoNodes() {
    cout << "--- Part 4: link two nodes ---\n";

    Page* node1 = new Page{"google.com",  nullptr};
    Page* node2 = new Page{"youtube.com", nullptr};

    node1->next = node2;          // THE key line: node1 now points to node2

    cout << node1->url << " -> " << node1->next->url << "\n\n";

    delete node1;
    delete node2;
}

// ---------- Part 5: linked list of 3 nodes ----------
void part5_list3Nodes() {
    cout << "--- Part 5: linked list of 3 nodes ---\n";

    // build:  head -> google.com -> youtube.com -> github.com -> NULL
    Page* head = new Page{"google.com", nullptr};
    head->next = new Page{"youtube.com", nullptr};
    head->next->next = new Page{"github.com", nullptr};

    // traverse & print
    for (Page* cur = head; cur != nullptr; cur = cur->next) {
        cout << cur->url << " -> ";
    }
    cout << "NULL\n";

    // free every node: save next BEFORE delete, else pointer lost
    while (head != nullptr) {
        Page* temp = head;
        head = head->next;
        delete temp;
    }
    cout << "all 3 nodes freed.\n\n";

    // NOTE: this exact pattern (new node + link pointer + walk & free)
    // is what Stack class in Phase 2 does.
}

int main() {
    part1_structVsClass();
    part2_pointerBasics();
    part3_newDelete();
    part4_linkTwoNodes();
    part5_list3Nodes();
    return 0;
}
