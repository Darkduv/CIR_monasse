#pragma once
#include "case.h"
#include <string>

class Plateau; //le .h a juste besoin de savoir que le plateau existe mais pas besoin des méthodes

class Piece {
protected:
    int couleur; // 0 = noir, 1 = blanc
    Case c;
    static std::string const name;
public:
    virtual bool permission_bouge(Case c) const {return false;} // il faut la définir
    virtual std::string get_name();
    virtual void bouge(Case c);
    int get_color(){return couleur;}
    Case get(){return c;}
    Piece(Case c, int couleur);
    Piece();
    virtual ~Piece(){}
};

class Roi : public Piece {
    static std::string const name;
    bool rock = true;

public:
    Roi(Case c, int couleur);
    virtual void bouge(Case c);
    virtual std::string get_name();
    virtual bool permission_bouge(Case c) const;
    bool rocker(Case c){}
};

class Dame : public Piece {
    static std::string const name;
public:
    Dame(Case c, int couleur);
    virtual std::string get_name();
    virtual bool permission_bouge(Case c) const;
};


class Fou : public Piece {
    static std::string const name;
public:
    Fou(Case c, int couleur);
    virtual std::string get_name();
    virtual bool permission_bouge(Case c) const;
};

class Cavalier : public Piece {
    static std::string const name;
public:
    Cavalier(Case c, int couleur);
    virtual std::string get_name();
    virtual bool permission_bouge(Case c) const;
};

class Tour : public Piece {
    static std::string const name;
    bool rock = true;
public:
    Tour(Case c, int couleur);
    virtual void bouge(Case c);
    virtual std::string get_name();
    virtual bool permission_bouge(Case c) const;
};

class Pion : public Piece {
    static std::string const name;
public:
    Pion(Case c, int couleur);
    virtual void bouge(Case c);
    virtual std::string get_name();
    virtual bool permission_bouge(Case c) const;
};
