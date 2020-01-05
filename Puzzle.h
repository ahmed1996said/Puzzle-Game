#ifndef ECE309_PUZZLE_H
#define ECE309_PUZZLE_H


#include "QuadTree.h"
using namespace cimg_library;

class Puzzle {

public:
    const CImg<unsigned char> img;

    QuadTree* tree;

    Puzzle(const CImg<unsigned char>& image, int levels=4);
    ~Puzzle();
    const CImg<unsigned char>& getImage() const;


    bool downLevel();
    bool upLevel();
    bool selectRight();
    bool selectLeft();
    bool selectUp();
    bool selectDown();
    bool rotateCurrent();
    bool solved(){ return tree->check_solved();}

    //update_selection();
};


#endif //ECE309_PUZZLE_H
