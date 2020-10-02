#include <iostream>
#include <cmath>
#include "cmf.h"
int main(int argc, char** argv)
{
    __only2d
    (
        if (CMF_DIM != cmf::GetDim()) std::cout << "BAD DIM" << std::endl;
        cmf::Initialize();
        cmf::ReadInput("input.ptl");
        cmf::RefinementBlock domain("Domain");

        std::string filename = "output/main.tex";
        cmf::TikzObject picture;
        picture.Open(filename);
        domain.Render(&picture);
        picture.Close();
        cmf::Finalize();
    )
    return 0;
}
