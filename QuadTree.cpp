//
// Created by Ahmed Al Mahrooqi on 2019-04-11.
//

#include "QuadTree.h"



QuadTree::QTNode::QTNode(int size, xy location, int lvl, QuadTree::QTNode *p) :org_loc(location),puz_loc(location),level(lvl),width(size),parent(p){

    for(int i=0;i<4;i++)
        child[i]  = nullptr;

    if(level<4) {
        if (decide() || level == 0) {   //make a decision, or if in level 0 override decision and subdivide

            subDivide();

            int n = rand()%3;
            for(int i=0;i<=n;i++)           //rotate subdivided block random number of times
            rotate();
        }
    }


}

bool QuadTree::QTNode::decide() {

    float prob = 0.60;
    int x = rand();
    bool decision = false;
    if (x < (int)(prob * RAND_MAX)) decision = true;

    return decision;
}

void QuadTree::QTNode::subDivide() {

    int new_level = level+1;
    int new_size= width/2;

    //child 0
    xy child0_location = org_loc;
    child[0] = new QTNode(new_size,child0_location,new_level,this);

    //child 1
    xy child1_location(org_loc.x + new_size,org_loc.y);
    child[1] = new QTNode(new_size,child1_location,new_level,this);

    //child 2
    xy child2_location(org_loc.x + new_size , org_loc.y + new_size);
    child[2] = new QTNode(new_size,child2_location,new_level,this);

    //child 3
    xy child3_location(org_loc.x , org_loc.y + new_size);
    child[3] = new QTNode(new_size,child3_location,new_level,this);

}

void QuadTree::QTNode::draw(const CImg<unsigned char> &o,CImg<unsigned char> &p) {


    // For a non-leaf node, each of
    //the child nodes must be drawn.
    if(child[0]) {

        for(int i=0;i<4;i++)
        child[i]->draw(o,p);


    }
    else        //For a leaf node, this means copying the pixel
    {           //data from the original image into the correct location in the puzzle image.
        CImg<unsigned char> snippet= o.get_crop(org_loc.x,org_loc.y,org_loc.x+width-1, org_loc.y+width-1);
        unsigned char blk[3] = {0x00,0x00,0x00};


        p.draw_image(puz_loc.x,puz_loc.y,snippet);

        //draw border:
        p.draw_line(puz_loc.x,puz_loc.y,puz_loc.x+width,puz_loc.y,blk);
        p.draw_line(puz_loc.x+width,puz_loc.y,puz_loc.x+width,puz_loc.y+width,blk);
        p.draw_line(puz_loc.x+width,puz_loc.y+width,puz_loc.x,puz_loc.y+width,blk);
        p.draw_line(puz_loc.x,puz_loc.y+width,puz_loc.x,puz_loc.y,blk);



    }

}

void QuadTree::QTNode::highlight(CImg<unsigned char>& p) {


    unsigned char wht[3] = {0xFF,0xFF,0xFF};
    p.draw_line(puz_loc.x,puz_loc.y,puz_loc.x+width,puz_loc.y,wht);
    p.draw_line(puz_loc.x+width,puz_loc.y,puz_loc.x+width,puz_loc.y+width,wht);
    p.draw_line(puz_loc.x+width,puz_loc.y+width,puz_loc.x,puz_loc.y+width,wht);
    p.draw_line(puz_loc.x,puz_loc.y+width,puz_loc.x,puz_loc.y,wht);

    p.draw_line(puz_loc.x+1,puz_loc.y+1,puz_loc.x+width+1,puz_loc.y+1,wht);
    p.draw_line(puz_loc.x+width-1,puz_loc.y-1,puz_loc.x+width-1,puz_loc.y+width-1,wht);
    p.draw_line(puz_loc.x+width-1,puz_loc.y+width-1,puz_loc.x-1,puz_loc.y+width-1,wht);
    p.draw_line(puz_loc.x+1,puz_loc.y+width+1,puz_loc.x+1,puz_loc.y+1,wht);


}


void QuadTree::QTNode::rotate() {

    if(!getChild(0) ) return;   //check if no children, can't rotate

    //copy coordinates
    xy c0 = getChild(0)->puz_loc;
    xy c1 = getChild(1)->puz_loc;
    xy c2 = getChild(2)->puz_loc;
    xy c3 = getChild(3)->puz_loc;

    //update coordinates
    child[0]->puz_loc = c1;
    child[1]->puz_loc = c2;
    child[2]->puz_loc = c3;
    child[3]->puz_loc = c0;


    //change children ordering
    auto temp0=child[0];
    auto temp1=child[1];
    auto temp2=child[2];
    auto temp3=child[3];

    child[0]=temp3;
    child[1]=temp0;
    child[2]=temp1;
    child[3]=temp2;


     //update children's children
    for(int i=0;i<4;i++)
    child[i]->update_children();


}

void QuadTree::QTNode::update_children() {

    if(!getChild(0)) return;


    //child 0 will always have the same location as the parent:
    child[0]->puz_loc.x = puz_loc.x;
    child[0]->puz_loc.y = puz_loc.y ;

    int refx = child[0]->puz_loc.x;
    int refy = child[0]->puz_loc.y;
    int w = child[0]->width;

    //compute the children location using child 0 as a reference, and width w
    //child 1 :
    child[1]->puz_loc.x = refx + w ;
    child[1]->puz_loc.y = refy;

    //child 2 :
    child[2]->puz_loc.x = refx+w;
    child[2]->puz_loc.y = refy+w;

    //child 3 :
    child[3]->puz_loc.x = refx;
    child[3]->puz_loc.y = refy+w;


    //update  children recursively
    for(int i=0;i<4;i++)
    child[i]->update_children();

}

bool QuadTree::QTNode::coordinates_match() {

    xy org = org_loc;
    if(org != puz_loc) return false;

    if(getChild(0)) {
        for(int i=0;i<4;i++)
        if(!child[i]->coordinates_match()) return false;    //recursively check,if coordinates don't match, return false
    }

    if(org==puz_loc) return true;

    return false;
}


void QuadTree::draw_puzzle() {


  root->draw(original,puzzled);     //draw the puzzle on root
  selection->highlight(puzzled);    //highlight selection

}

QuadTree::QuadTree(const CImg<unsigned char> &image, int l) :original(image),max_levels(l){


  puzzled = original; //copy orginal image into the puzzled

  xy origin(0,0);

  if(max_levels>0)
      root = new QTNode(image.width(), origin,0 ); //create a root node (level0), with correct location and image width

  selection = root;                    //make first selection at root

}

bool QuadTree::down_level() {


  if(!selection->getChild(0))       //if selected node has no children, can't move down. exit
      return false;

  selection=selection->getChild(0);

  return true;


}


bool QuadTree::up_level() {


  if(!selection->getParent())   //if selected node has no parent, can't move up. exit
      return false;

  selection=selection->getParent();

  return true;


}

bool QuadTree::left() {


  int i;
  int l = selection->getLevel();
  auto old_selection = selection;                 //save old selection to recover old position when function returns false

  if(!selection->getParent())                      //if no parent, selecting at root, return false, exit function.
      return false;

  auto temp = selection->getParent();


      for(i=0;i<4;i++) {                              // find out the child number
          if (temp->getChild(i) == selection)
              break;
      }
  //case 1 : if its inner selection
      if( i==1 ||i==2){
          if(i==1)
              i = 0;
          else if(i==2)
              i=i+1;

          selection = temp->getChild(i);
          return true;
      }

      //case 2: if its on the edge
      else if (i==0||i==3){

          if(selection->getParent()&&selection->getParent()->getLevel()>0) {
              selection = selection->getParent();
              if(left()){
              while(selection->getLevel() < l && selection->getChild(0)) { //select neighbor on same level if possible
                  if (i == 0)
                      selection = selection->getChild(1);
                  else
                      selection = selection->getChild(2);
              }
              return true;}
          }

      }
  selection = old_selection;
      return false;

}


bool QuadTree::right() {

  int i;
  int l = selection->getLevel();
  auto old_selection = selection;             //save old selection to recover old position when function returns false

  if(!selection->getParent())                 //if no parent, selecting at root, do nothing, exit function.
      return false;

  auto temp = selection->getParent();
  for(i=0;i<4;i++) {                          // find out the child number
      if (temp->getChild(i) == selection)
          break;
  }
  //case 1 : if its inner selection
  if( i==0 ||i==3){
      if(i==0)
          i = i + 1;
      else if(i==3)
          i=i-1;

      selection = temp->getChild(i);
      return true;
  }

  //case 2: if its on the edge
  else if(i==1||i==2){
      if(selection->getParent()&&selection->getParent()->getLevel()>0) {
          selection = selection->getParent();
          if(right()){
          while(selection->getLevel() < l && selection->getChild(0)) {    //select neighbor on same level if possible

              if (i == 1)
                  selection = selection->getChild(0);
              else
                  selection = selection->getChild(3);
          }

          return true;
      }
      }
  }


  selection = old_selection;
  return false;


}


bool QuadTree::up() {

  int i;
  int l = selection->getLevel();
  auto old_selection = selection;             //save old selection to recover old position when function returns false


  if(!selection->getParent())                 //if no parent, selecting at root, do nothing, exit function.
      return false;

  auto temp = selection->getParent();

  for(i=0;i<4;i++) {                          //identify the child number of current selection
      if (temp->getChild(i) == selection)
          break;
  }

  //case 1 : if its inner selection
  if( i==2 ||i==3){
      if(i==2)
          i = i - 1;
      else if(i==3)
          i=0;

      selection = temp->getChild(i);
      return true;
  }

  //case 2: if its on the edge
  else if(i==0||i==1){

      if(selection->getParent()&&selection->getParent()->getLevel()>0) {
          selection = selection->getParent();
          if(up()){
          while(selection->getLevel() < l && selection->getChild(0)) {      //select neighbor on same level if possible
              if (i == 0)
                  selection = selection->getChild(3);
              else
                  selection = selection->getChild(2);
          }
          return true;
      }
      }

  }
  selection = old_selection;
  return false;





}

bool QuadTree::down() {

  int i;
  int l = selection->getLevel();
  auto old_selection = selection; //save old selection to recover old position when function returns false

  if(!selection->getParent())    //if no parent, selecting at root, do nothing, exit function.
      return false;

  auto temp = selection->getParent();

  for(i=0;i<4;i++) {                      //identify the child number of current selection
      if (temp->getChild(i) == selection)
          break;
  }

  //case 1 : if its inner selection
  if( i==0 ||i==1){
      if(i==0)
          i = i + 3;
      else if(i==1)
          i=i+1;

      selection = temp->getChild(i);
      return true;


  //case 2: if its on the edge
  }  else if(i==2||i==3){

      if(selection->getParent()&&selection->getParent()->getLevel()>0) {
          selection = selection->getParent();

          if (down()) {                                                           //check if edges have neighbors
              while (selection->getLevel() < l && selection->getChild(0)) {       //select neighbor on same level if possible
                  if (i == 2)
                      selection = selection->getChild(1);
                  else
                      selection = selection->getChild(0);
              }
              return true;
          }
      }
  }
  selection = old_selection;
  return false;

}

bool QuadTree::rotate_selection() {


   auto p = selection->getParent();
   if(!p) return false;

   p->rotate();     //call rotate on selected node
   return true;

}

bool QuadTree::check_solved() {
   return  root->coordinates_match();   //start checking at root, returns bool after recursively checking each node in the tree

}


