//
// Created by Ahmed Al Mahrooqi on 2019-04-04.

#include "CImg.h"
using namespace cimg_library;
#include"Puzzle.h"
#include <stdlib.h>
#include <ctime>


Puzzle::Puzzle(const CImg<unsigned char> &image, int levels){

    //seed random function with current time in seconds
    time_t now = time(0);
    tm *ltm = localtime(&now);
    srand(ltm->tm_sec);

    tree = new QuadTree(image,levels);



}

Puzzle::~Puzzle() {delete tree;}

const CImg<unsigned char> &Puzzle::getImage() const {


     tree->draw_puzzle();
     return tree->getPuzzle();
}



bool Puzzle::rotateCurrent() {


    return tree->rotate_selection();

    }



bool Puzzle::downLevel() {
    return tree->down_level();
}

bool Puzzle::upLevel() {
    return tree->up_level();
}

bool Puzzle::selectRight() {
    return tree->right();

}

bool Puzzle::selectLeft() {
    return tree->left();
}

bool Puzzle::selectUp() {
    return tree->up();
}

bool Puzzle::selectDown() {
    return tree->down();
}






