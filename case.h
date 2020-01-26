#pragma once

class Case {
    int x{};
    int y{};
public:
    Case();
    Case(const int i, const int j);
    Case(const char a, const int k);

    void set(int i, int j){x=i;y=j;}
    void set(char a, int k){x=int(a)-65;y=k-1;}

    int get(const int a) const;
    int distance(const Case cprime) const;
    Case operator+(const Case c1) const ;
    bool operator==(const Case c1) const;
};
typedef Case Move; // it's the same type, but it makes the code easier to understand
Move d_move(const Case c_start, const Case c_end);
