#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"Compilation_Principle.h"


void init_syntax_analysis()
{
    return;
}

void syntax_analysis(std::vector< std::pair<std::string, std::pair<std::string, std::string> > > lexcial, const char *syntax_output_path)
{
    std::vector< std::pair<std::string, std::pair<std::string, std::string> > > syntax;
    
    //code

    std::ofstream syntax_output_file(syntax_output_path);
    if (!syntax_output_file)
    {
        std::cerr << "Error opening file: " << syntax_output_path << std::endl;
        exit(EXIT_FAILURE);
    }

    int id = 1;
    for (auto item: lexcial)
    {
        syntax_output_file << id << "\t" << item.first << "#" << item.second.first << "\t" << item .second.second << std::endl;
        id += 1;
    }
    syntax_output_file.close();
    return;
}

