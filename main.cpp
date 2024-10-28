#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"Compilation_Principle.h"
#include"Lexcial_Analysis.cpp"
#include"Syntax_Analysis.cpp"


int main(int argc, char *argv[])
{
    if (argc <= 3)
    {
        std::cerr << "Usage: " << "<input path> <lexcial output path> <synatx output path>" << std::endl;
        exit(EXIT_FAILURE);
    }

    const char *input_path = argv[1];
    const char *lexcial_output_path = argv[2];
    const char *synatx_output_path = argv[3];

    

    init_lexcial_analysis();
    init_syntax_analysis();

    

    std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial;
    lexcial = lexcial_analysis(input_path, lexcial_output_path);



    return 0;
}