#ifndef ENUMCOLOROS_H
#define ENUMCOLOROS_H

    enum 
        {
        SUCCESSUFUL = 0, 
        FAILURE     = 1,
        POISON      = -8111991
        };

#define RED(print)     "\033[1;31m" print "\033[0m"
#define GREEN(print)   "\033[1;32m" print "\033[0m"
#define YELLOW(print)  "\033[1;33m" print "\033[0m"
#define BLUE(print)    "\033[1;34m" print "\033[0m"
#define VIOLET(print)  "\033[1;35m" print "\033[0m"

#endif