#include "resources.hpp"

std::string get_asset_path(std::string resource_name)
{
    if (DirectoryExists("assets/"))
        return "assets/" + resource_name;
    else if (DirectoryExists("../../../assets/")) // vs2022
        return "../../../assets/" + resource_name;
    else if (DirectoryExists("../assets/")) // vscode
        return "../assets/" + resource_name;

    return "!! FAILED TO FIND" + resource_name;
}
