#pragma once
/*
#include <iostream>
class Mystreambuf: public std::streambuf
{
};


enum verboselevel {CRITICAL, ERROR, WARNING, INFO, DEBUG};


class Mystream: public std::ostream
{
    private:
        int verboselevel;
        Mystreambuf nostreambuf;
        std::ostream nocout;
    public:
        Mystream() :nocout(&nostreambuf),verboselevel(CRITICAL){}
        void set_verboselevel(int i); 
        std::ostream& out(int i);
        std::ostream& out();
};


extern Mystream c;
*/