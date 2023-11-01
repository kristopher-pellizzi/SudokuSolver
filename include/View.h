#ifndef VIEW
#define VIEW

#include <iostream>
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
        virtual void print(string s) const = 0;
        virtual std::istream& get_input_stream() = 0;
};

#endif // VIEW