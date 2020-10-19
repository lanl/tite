#ifndef TITE_UTLITY_REQUIRES_HPP
#define TITE_UTLITY_REQUIRES_HPP

#define TITE_REQUIRES(...) typename std::enable_if<__VA_ARGS__>::type* = nullptr

#endif
