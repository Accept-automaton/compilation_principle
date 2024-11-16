#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"Compilation_Principle.h"
#include"Lexcial_Analysis.cpp"
#include"Syntax_Analysis.cpp"


int main(int argc, char *argv[])
{
    // if (argc <= 3)
    // {
    //     std::cout << "Usage: " << "<input path> <lexcial output path> <synatx output path>" << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    // const char *input_path = argv[1];
    // const char *lexcial_output_path = argv[2];
    // const char *syntax_output_path = argv[3];

    const char *input_path = "program.txt";
    const char *lexcial_output_path = "lexcial_result.txt";
    const char *syntax_output_path = "synatx_result.txt";

    
    init_syntax_analysis();


    std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial;
    lexcial = lexcial_analysis(input_path, lexcial_output_path);
    syntax_analysis(lexcial, syntax_output_path);


    return 0;
}