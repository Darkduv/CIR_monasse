#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-avoid-const-params-in-decls"
#pragma once

class Case {
    int x{};
    int y{};
public:
    Case();
    Case(int i, int j);
    Case(char a, int k);

    void set(int i, int j){x=i;y=j;}
    void set(char a, int k){x=int(a)-65;y=k-1;}

    int get(int a) const;
    int distance(Case c_prime) const;
    Case operator+(Case c1) const ;
    bool operator==(Case c1) const;
};
typedef Case Move; // it's the same type, but it makes the code easier to understand
Move d_move(Case c_start, Case c_end);
