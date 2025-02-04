#include <vector>
#include <string>

class Project {
private:
	std::vector<std::string> paths;
	std::vector<std::string> name;
public:
	Project(std::string jarPath);
	std::string searchClassFile(std::string name);
};