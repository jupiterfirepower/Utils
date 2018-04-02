// g++ mainrec.cpp -Wall -Wextra -std=c++1z -o recdir -lstdc++fs

#include <iostream>
#include <sstream>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace std;

std::string get_fileext(std::string pth)
{
    std::cout <<"pth:"<< pth<<std::endl;
    std::string filename(pth);
    std::string::size_type idx;

    idx = filename.rfind('.');

    if(idx != std::string::npos)
    {
	std::string extension = filename.substr(idx+1);
	return extension;
    }
    else
    {
	// No extension found
	return "";
    }
}

int main()
{
    std::stringstream ss;

    for(auto i = fs::recursive_directory_iterator(".");
             i != fs::recursive_directory_iterator();
           ++i )
    {
        auto cur = std::string(i->path());
        std::cout << std::string(i.depth(), ' ') << *i <<std::endl;
        std::cout <<"ext: "  << ::get_fileext(cur)<<std::endl;
        if(fs::is_symlink(i->symlink_status()))
            std::cout << " -> " << fs::read_symlink(*i);
        std::cout << '\n';

	if (fs::is_directory(i->status()))
	{
	    ss << cur <<std::endl;
	}
	else if (fs::is_regular_file(i->status()) && get_fileext(cur)=="cpp")
	{
	    std::ifstream ifs(cur.c_str());
	    std::string content( (std::istreambuf_iterator<char>(ifs)),
    	                     (std::istreambuf_iterator<char>()));
	    ss << content << std::endl;
        }
    }

    std::ofstream out("res.cpp");
    out << ss.str();
    out.close();

    return 0;
}
