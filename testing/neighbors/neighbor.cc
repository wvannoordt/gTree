#include <iostream>
#include <cmath>
#include "gTree.h"

int main(int argc, char** argv)
{
    gTree::Initialize();
    gTree::ReadInput("input.ptl");
    gTree::RefinementBlock domain("Domain");
    double coords[2];
    coords[0] = 0.55;
    coords[1] = 0.55;
    domain.RefineAt(coords, 3);
    coords[0] = 0.55;
    coords[1] = 0.56;
    domain.RefineAt(coords, 3);
    coords[0] = 0.55;
    coords[1] = 0.54;
    domain.RefineAt(coords, 3);
    std::string filename = "output/main.tex";
    gTree::TikzObject picture;
    picture.Open(filename);
	picture.SetClip(0,0,1,1);
    domain.Render(&picture);
    picture.Close();
    gTree::Finalize();
    return 0;
}