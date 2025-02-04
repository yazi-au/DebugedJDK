#include "Project.h"

Project::Project(std::string path) {

}
std::string Project::searchClassFile(std::string name) {
	for (size_t i = 0; i < name.size(); i++){
		if (this->name[i].compare(name)) {
			return paths[i];
		}
	}
	return nullptr;
}