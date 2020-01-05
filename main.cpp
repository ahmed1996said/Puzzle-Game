// ECE 309 - Spring 2019
// Programming Project
// Read image to create and solve puzzle.

#include <iostream>
using std::cout;
using std::endl;

#include "CImg.h"
using namespace cimg_library;

#include "Puzzle.h"

void scale_image(CImg<unsigned char>&);
void print_help();
void ui(CImg<unsigned char>&);

int main(int argc, char** argv) {
    CImg<unsigned char> img(argv[1]);  // open file and create image
    // crop right or bottom to make square
    scale_image(img);
    print_help();
    ui(img);
    return 0;
}
// scale_image:
// (1) crop image to make it square
// (2) scale to between 256x256 and 640x640
// (3) make sure dimension is a multiple of 16
void scale_image(CImg<unsigned char>& img) {
    if (img.width() > img.height()) {
        img = img.crop(0,0,img.height()-1, img.height()-1);
    }
    else if (img.width() < img.height()) {
        img = img.crop(0,0, img.width()-1, img.width()-1);
    }
    // max size = 640x640
    if (img.width() > 640) img.resize(640,640);
    // min size = 256x256
    else if (img.width() < 256) img.resize(256,256);
    // scale to nearest multiple of 16
    else if (img.width() % 16) {
        int w = img.width() + (16 - img.width()%16);
        img.resize(w,w);
    }
}

// command list for user
void print_help() {
    cout << "Commands: " << endl;
    cout << "r = rotate clockwise" << endl;
    cout << "arrows = move to neighboring square" << endl;
    cout << "u = move up a level" << endl;
    cout << "d = move down a level" << endl;
    cout << "o = show original image" << endl;
    cout << "q = quit (or just close the display)" << endl;
}

// user interface
// -- create display windows for original and puzzle images
// -- display puzzle image and wait for user input
// -- capture user keyboard commands and perform operations
// -- after each action that changes the puzzle, redisplay
void ui(CImg<unsigned char>& img) {
    Puzzle p(img, 4);  // create puzzle with four levels of subblock
    CImgDisplay disp(p.getImage(), "PUZZLE");
    CImgDisplay orig(img, "ORIGINAL", 3, false, true);  // create display for original, but don't display

    while (!disp.is_closed()) {
        if (disp.is_key()) {
            bool changed = false;
            bool close = false;
            switch (disp.key()) {
                case cimg::keyD:
                    changed = p.downLevel(); break;
                case cimg::keyU:
                    changed = p.upLevel(); break;
                case cimg::keyARROWUP:
                    changed = p.selectUp(); break;
                case cimg::keyARROWLEFT:
                    changed = p.selectLeft(); break;
                case cimg::keyARROWDOWN:
                    changed = p.selectDown(); break;
                case cimg::keyARROWRIGHT:
                    changed = p.selectRight(); break;
                case cimg::keyR:
                    changed = p.rotateCurrent(); break;
                case cimg::keyO:
                    if (orig.is_closed()) {
                        orig.show();
                    }
                    break;
                case cimg::keyQ:
                    close = true; break;
            }
            if (changed) {
                disp.display(p.getImage());
                if (p.solved()) {
                    cout << endl;
                    cout << "PUZZLE SOLVED!!!" << endl;
                    cout << endl;
                }
            }
            if (close) {
                disp.close();
            }
        }
        disp.wait();
    }
}
