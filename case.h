#pragma once

class Case {
    int x;
    int y;
public:
    Case();
    Case(int i, int j);
    Case(char a, int k);

    void set(int i, int j){x=i;y=j;}
    void set(char a, int k){x=int(a)-65;y=k-1;}

    int get(int a) const;
    int distance(const Case cprime) const;
    Case operator+(const Case c1) const ;
    bool operator==(const Case c1) const;
};
typedef Case Deplacement;
Deplacement d_deplacement(const Case c_start, const Case c_end);
