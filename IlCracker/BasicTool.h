#include <string>

constexpr int COLOR_RED = 12;
constexpr int COLOR_YELLOW = 14;
constexpr int COLOR_WHITE = 7;
constexpr int COLOR_GRAY = 8;

std::string getClassAccessFlagsMapping(short flag);
short charsToShort(char* chars);
char* shortToChars(short s);
void SEND(const char* prefix, const char* msg, int color);

class Range{
public:
    int32_t min, max;
    inline bool inRange(int32_t value){
        return value >= min && value <= max;
    }
};

inline bool inrange(uint32_t min, int32_t value, int32_t max){
    return value >= min && value <= max;
}
