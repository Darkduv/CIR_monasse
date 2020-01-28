#pragma once
#include "case.h"
#include <string>

class Board; // This .h  file need to know Board class exists but do not need its methods

class Piece {
protected:
    int color; // 0 = black, 1 = white
    Case c;
    static std::string const name;
public:
    virtual bool permission_move(Case aCase) const{return false;} // we need to define it (?)
    virtual std::string get_name() const;
    virtual void move(Case end_of_move_case);
    int get_color() const {return color;}
    Case get(){return c;}
    Piece(Case c, int col);
    // Piece();
    virtual ~Piece()= default;
};

class King : public Piece {
    static std::string const name;

public:
    King(Case c, int col);
    virtual void move(Case c);
    virtual std::string get_name() const;
    virtual bool permission_move(Case case_arrive) const;
};

class Queen : public Piece {
    static std::string const name;
public:
    Queen(Case c, int col);
    virtual std::string get_name() const;
    virtual bool permission_move(Case c) const;
};


class Bishop : public Piece {
    static std::string const name;
public:
    Bishop(Case c, int col);
    virtual std::string get_name() const;
    virtual bool permission_move(Case c) const;
};

class Knight : public Piece {
    static std::string const name;
public:
    Knight(Case c, int col);
    virtual std::string get_name() const;
    virtual bool permission_move(Case c) const;
};

class Tower : public Piece {
    static std::string const name;
public:
    Tower(Case c, int col);
    virtual void move(Case c);
    virtual std::string get_name() const;
    virtual bool permission_move(Case c) const;
};

class Pawn : public Piece {
    static std::string const name;
public:
    Pawn(Case c, int col);
    virtual void move(Case c);
    virtual std::string get_name() const;
    virtual bool permission_move(Case arrival_case) const;
};
