//
// Created by Ahmed Al Mahrooqi on 2019-04-06.
//

#ifndef PUZZLE_QUADTREE_H
#define PUZZLE_QUADTREE_H


#include "CImg.h"
//#include "Puzzle.h"

using namespace cimg_library;

class xy{
public:
    int x;
    int y;
    xy(int h=0,int v=0):x(h),y(v){}
    bool operator==(xy& other)
    { return (x ==other.x)&&(y==other.y);}
    bool operator!=(xy &other){ return (x!=other.x)||(y!=other.y); }

};


class QuadTree{
    const CImg<unsigned char> original;
    CImg<unsigned char> puzzled;
    int max_levels;

    class QTNode{
    private:
            const xy org_loc;       //coordinates of original position in image
            xy puz_loc;             //coordinates of puzzle location
            int level;
            int width;
            QTNode* parent;
            QTNode* child[4];

    public:
            QTNode(int size,xy location,int lvl,QTNode* p=nullptr);

            QTNode* getParent(){return parent;}
            QTNode* getChild(int i){return child[i];}
            int getLevel(){return level;}

            bool decide();              //returns a random decision
            void subDivide();           //sub divides, creates new children
            void rotate();              //rotates the ordering of the children
            void update_children();     //updates children locations after parent rotates
            bool coordinates_match();   //returns whether original location matches puzzle location

            void draw(const CImg<unsigned char>& o,CImg<unsigned char>& p);     //draws  node in the puzzle
            void highlight(CImg<unsigned char>& p);                             //highlights a selected node with white border
        };
    QTNode* selection;      //pointer to a selected node
    QTNode* root;           //pointer to root(level 0 node)


    public:
    QuadTree(const CImg<unsigned char> &image,int l=4);
    CImg<unsigned char>& getPuzzle(){return puzzled;}

    void draw_puzzle();             //draws every node in the tree
    bool rotate_selection();        //calls rotate() on selected node

    bool down_level();             //updates puzzle selection to lower level
    bool up_level();               //updates puzzle selection to upper level
    bool left();
    bool right();
    bool up();
    bool down();

    bool check_solved();        //recursively compares orginal vs. puzzle location of each node in tree

};




#endif //PUZZLE_QUADTREE_H
