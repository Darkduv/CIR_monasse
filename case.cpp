#include "case.h"
#include <algorithm>
#include <cstdlib>

Case::Case(){};

Case::Case(int i, int j){
    x=i;
    y=j;
};

Case::Case(char a, int k){
    x=int(a)-65; // le A correspond à un 0
    y=k-1;
};

int Case::distance(const Case c_prime) const{
    return std::max(abs(c_prime.x - x), abs(c_prime.y -y));
}

int Case::get(int a) const{
    switch (a) {
    case 0:
        return x;
    case 1:
        return y;
    default:
        return 0;
    }
}

Case Case::operator+(const Case c1) const{
    return Case(x+c1.x,y+c1.y);
}

bool Case::operator==(const Case c1) const{
    return (x==c1.x && y==c1.y);
}

Deplacement d_deplacement(const Case c_start, const Case c_end){
    int dx = c_end.get(0) - c_start.get(0);
    int dy = c_end.get(1) - c_start.get(1);
    if (dx != 0)
        dx /= std::abs(dx);
    if (dy != 0)
        dy /= std::abs(dy);
    return Deplacement(dx, dy);
};
