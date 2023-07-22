#ifndef VIEW
#define VIEW

#include <string>

using std::string;

class View{
    public:
        View();
        ~View();

        virtual void draw() const = 0;
        virtual void print_constraints() const = 0;
        virtual void message(string msg) const = 0;
        virtual void warning(string wrn) const = 0;
        virtual void error(string err) const = 0;
};

#endif // VIEW