#ifndef RGB_H
#define RGB_H

#include <string>
#include <sstream>

struct RGB {
  RGB() :r(0), g(0), b(0) {}
  RGB(unsigned char r, unsigned char g, unsigned char b) :r(r), g(g), b(b) {}
  unsigned char r, g, b;

  std::string str() const {
    std::ostringstream ss;
    ss << "RGB(" << int(r) << ',' << int(g) << ',' << int(b) << ')';
    return ss.str();
  }
};

#endif
