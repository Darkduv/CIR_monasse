#pragma once
#include "case.h"
#include <string>

class Board; //le .h a juste besoin de savoir que le plateau existe mais pas besoin des méthodes

class Piece {
protected:
    int couleur; // 0 = noir, 1 = blanc
    Case c;
    static std::string const name;
public:
    virtual bool permission_bouge(Case c) const{return false;} // il faut la définir
    virtual std::string get_name() const;
    virtual void move(Case c);
    int get_color() const {return couleur;}
    Case get(){return c;}
    Piece(Case c, int couleur);
    Piece();
    virtual ~Piece(){}
};

class Roi : public Piece {
    static std::string const name;

public:
    Roi(Case c, int couleur);
    virtual void move(Case c);
    virtual std::string get_name() const;
    virtual bool permission_bouge(Case c) const;
};

class Dame : public Piece {
    static std::string const name;
public:
    Dame(Case c, int couleur);
    virtual std::string get_name() const;
    virtual bool permission_bouge(Case c) const;
};


class Fou : public Piece {
    static std::string const name;
public:
    Fou(Case c, int couleur);
    virtual std::string get_name() const;
    virtual bool permission_bouge(Case c) const;
};

class Cavalier : public Piece {
    static std::string const name;
public:
    Cavalier(Case c, int couleur);
    virtual std::string get_name() const;
    virtual bool permission_bouge(Case c) const;
};

class Tour : public Piece {
    static std::string const name;
public:
    Tour(Case c, int couleur);
    virtual void move(Case c);
    virtual std::string get_name() const;
    virtual bool permission_bouge(Case c) const;
};

class Pion : public Piece {
    static std::string const name;
public:
    Pion(Case c, int couleur);
    virtual void move(Case c);
    virtual std::string get_name() const;
    virtual bool permission_bouge(Case c) const;
};
