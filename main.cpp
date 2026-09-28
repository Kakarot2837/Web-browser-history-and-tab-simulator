// ============================================================
//  WEB BROWSER HISTORY & TAB SIMULATOR
//  Status  : PHASE 2 done (single tab, back/forward with 2 stacks)
//
//  Compile : g++ -std=c++17 -Wall -Wextra main.cpp -o browser
//  Run     : ./browser          (Windows: browser.exe)
//
//  Roadmap : P3 tree history   | P4 multiple tabs | P5 closed-tab stack
//            P6 hash search    | P7 heap          | P8 graph grouping
//            P9 menu           | P10 polish + viva
// ============================================================
#include <iostream>
#include <string>
using namespace std;


// ============================================================
// PHASE 2 - STEP 1: Stack<T>  (linked-list based)
//
// LIFO = last pushed, first popped. push/pop/peek all O(1).
// Template, so SAME class works for string now and for Tab in
// Phase 5 (Stack<Tab> "recently closed").
// Node pattern = same as Page in Phase 1 (new node + next pointer).
// ============================================================
template <typename T>
class Stack {
private:
    struct Node {                    // one box in the chain
        T     data;
        Node* next;                  // points to node BELOW this one
        Node(const T& d, Node* n) : data(d), next(n) {}
    };

    Node* topNode;                   // newest element (nullptr = empty)
    int   count;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }            // free all nodes when Stack dies

    // Copy would duplicate the POINTER, not the nodes -> two stacks
    // delete same node (crash). So copying is blocked on purpose.
    Stack(const Stack&)            = delete;
    Stack& operator=(const Stack&) = delete;

    bool isEmpty() const { return topNode == nullptr; }
    int  size()    const { return count; }

    // push: new node points to old top, then becomes the top
    void push(const T& value) {
        topNode = new Node(value, topNode);
        count++;
    }

    // pop: remove top, give its value back in 'out'. false if empty.
    bool pop(T& out) {
        if (isEmpty()) return false;
        Node* old = topNode;
        out       = old->data;       // copy value out BEFORE delete
        topNode   = old->next;       // move top down one
        delete old;
        count--;
        return true;
    }

    // peek: read top without removing. false if empty.
    bool peek(T& out) const {
        if (isEmpty()) return false;
        out = topNode->data;
        return true;
    }

    // clear: delete every node (used by destructor and by visit())
    void clear() {
        while (topNode != nullptr) {
            Node* old = topNode;
            topNode   = old->next;   // save next BEFORE delete
            delete old;
        }
        count = 0;
    }

    // print: TOP first.  Example: [ c, b, a ]  (c = top)
    void print() const {
        if (isEmpty()) { cout << "[ ]"; return; }
        cout << "[ ";
        for (Node* p = topNode; p != nullptr; p = p->next) {
            cout << p->data;
            if (p->next != nullptr) cout << ", ";
        }
        cout << " ]";
    }
};


// ============================================================
// PHASE 2 - STEPS 2, 3, 4: single tab history (TWO STACK method)
//
//   backStack    : pages behind us   (top = page we just left)
//   currentUrl   : page open now
//   forwardStack : pages ahead of us (top = page we just came back from)
//
//   visit(url) : current -> back,  url becomes current,  forward CLEARED
//   goBack()   : current -> forward, top of back becomes current
//   goForward(): current -> back,    top of forward becomes current
//
// PHASE 3 will replace this class with a tree (branches kept).
// ============================================================
class SingleTabHistory {
private:
    string        currentUrl;        // "" = nothing opened yet
    Stack<string> backStack;
    Stack<string> forwardStack;

public:
    // Step 2: open a new page
    void visit(const string& url) {
        if (url.empty()) {
            cout << "  (!) empty url ignored\n";
            return;
        }
        if (!currentUrl.empty()) {
            backStack.push(currentUrl);      // remember where we were
        }
        currentUrl = url;
        forwardStack.clear();                // new path -> old forward pages lost
    }

    // Step 3: go one page back. false if nothing behind.
    bool goBack() {
        string prev;
        if (!backStack.pop(prev)) {
            cout << "  (!) cannot go back: no earlier page\n";
            return false;
        }
        forwardStack.push(currentUrl);       // page we leave -> forward
        currentUrl = prev;
        return true;
    }

    // Step 4: go one page forward. false if nothing ahead.
    bool goForward() {
        string next;
        if (!forwardStack.pop(next)) {
            cout << "  (!) cannot go forward: no later page\n";
            return false;
        }
        backStack.push(currentUrl);          // page we leave -> back
        currentUrl = next;
        return true;
    }

    // getters (used by tests, and later by menu)
    string current()      const { return currentUrl; }
    int    backCount()    const { return backStack.size(); }
    int    forwardCount() const { return forwardStack.size(); }

    // print both stacks + current page
    void display() const {
        cout << "  Back    : "; backStack.print();    cout << "\n";
        cout << "  Current : " << (currentUrl.empty() ? "(blank)" : currentUrl) << "\n";
        cout << "  Forward : "; forwardStack.print(); cout << "\n";
    }
};


// ============================================================
// PHASE 2 - STEP 5: tests
// check() prints PASS/FAIL so you do not need to eyeball all output.
// ============================================================
int passed = 0;
int failed = 0;

void check(bool ok, const string& what) {
    if (ok) { passed++; cout << "  [PASS] " << what << "\n"; }
    else    { failed++; cout << "  [FAIL] " << what << "\n"; }
}

// true if current page + both stack sizes match expected
bool isState(const SingleTabHistory& t, const string& cur, int back, int fwd) {
    return t.current() == cur && t.backCount() == back && t.forwardCount() == fwd;
}

int main() {
    cout << "=== Browser Simulator | Phase 2: single tab, two-stack history ===\n";
    cout << "(stacks print TOP first)\n\n";

    // ---------- TEST 0: Stack alone ----------
    cout << "TEST 0: Stack<int> alone\n";
    Stack<int> s;
    int v = 0;
    check(s.isEmpty(), "new stack is empty");
    check(!s.pop(v),   "pop on empty stack returns false");
    s.push(10); s.push(20); s.push(30);
    check(s.size() == 3,          "size = 3 after 3 push");
    check(s.peek(v) && v == 30,   "peek = 30 (last in)");
    bool lifo = s.pop(v) && v == 30 && s.pop(v) && v == 20 && s.pop(v) && v == 10;
    check(lifo,        "pop order = 30, 20, 10 (LIFO)");
    check(s.isEmpty(), "empty again");

    // ---------- TEST 1: visit 5 URLs ----------
    cout << "\nTEST 1: visit 5 URLs\n";
    SingleTabHistory tab;
    const string urls[5] = {
        "google.com", "youtube.com", "github.com",
        "wikipedia.org", "stackoverflow.com"
    };
    for (int i = 0; i < 5; i++) {
        cout << "> visit " << urls[i] << "\n";
        tab.visit(urls[i]);
        tab.display();
        check(isState(tab, urls[i], i, 0),
              "on " + urls[i] + ", back=" + to_string(i) + ", forward=0");
    }

    // ---------- TEST 2: back, back, forward ----------
    cout << "\nTEST 2: back, back, forward\n";

    cout << "> back\n";
    tab.goBack();
    tab.display();
    check(isState(tab, "wikipedia.org", 3, 1), "on wikipedia.org, back=3, forward=1");

    cout << "> back\n";
    tab.goBack();
    tab.display();
    check(isState(tab, "github.com", 2, 2), "on github.com, back=2, forward=2");

    cout << "> forward\n";
    tab.goForward();
    tab.display();
    check(isState(tab, "wikipedia.org", 3, 1), "on wikipedia.org, back=3, forward=1");

    // ---------- TEST 3: new link from middle of history ----------
    cout << "\nTEST 3: visit new link from middle of history\n";
    cout << "> visit news.com\n";
    tab.visit("news.com");
    tab.display();
    check(isState(tab, "news.com", 4, 0), "on news.com, forward cleared");
    cout << "> forward\n";
    check(!tab.goForward(), "forward refused (stackoverflow.com is LOST)");
    // ^ This lost branch is exactly why Phase 3 upgrades to a tree.

    // ---------- TEST 4: walk to both ends ----------
    cout << "\nTEST 4: walk to both ends\n";
    cout << "> back x4\n";
    for (int i = 0; i < 4; i++) tab.goBack();
    tab.display();
    check(isState(tab, "google.com", 0, 4), "on google.com (oldest), back=0, forward=4");
    cout << "> back once more\n";
    check(!tab.goBack(), "back refused (nothing behind)");

    cout << "> forward x4\n";
    for (int i = 0; i < 4; i++) tab.goForward();
    tab.display();
    check(isState(tab, "news.com", 4, 0), "on news.com (newest), back=4, forward=0");
    cout << "> forward once more\n";
    check(!tab.goForward(), "forward refused (nothing ahead)");

    // ---------- TEST 5: brand new tab ----------
    cout << "\nTEST 5: brand new tab, nothing visited\n";
    SingleTabHistory fresh;
    fresh.display();
    check(!fresh.goBack(),          "back refused on empty tab");
    check(!fresh.goForward(),       "forward refused on empty tab");
    check(isState(fresh, "", 0, 0), "empty tab still blank");

    // ---------- summary ----------
    cout << "\n=== " << passed << " passed, " << failed << " failed ===\n";
    return failed == 0 ? 0 : 1;
}
