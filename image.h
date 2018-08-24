#ifndef IMAGE_H
#define IMAGE_H

#include "matrix.h"
#include "rgb.h"

class Image : public Matrix<RGB> {
public:
  Image() {}

  Image(int width, int height) :
    Matrix<RGB>(width, height) {
  }

  void save_ppm(const std::string& filename) {
    const char *comment = "# Yaniv 2018 (C)";
    const int MAX_COLOR_COMPONENT_VALUE = 255;

    FILE* fp;

    fp = fopen(filename.c_str(), "wb");
    if (fp != NULL) {
      fprintf(fp, "P3\n %s\n %d\n %d\n %d\n", comment, width_, height_,
              MAX_COLOR_COMPONENT_VALUE);
      /* write image data bytes to the file */
      for (auto i = data_.begin(); i != data_.end(); i++) {
        fprintf(fp, "%d %d %d\n", i->r, i->g, i->b);
      }
      fclose(fp);
      printf("OK - file %s saved\n", filename.c_str());
    } else {
      printf("Error opening %s\n", filename.c_str());
    }
  }

  void show(std::string filename) {
    save_ppm(filename);
    std::string cmd = "open ";
    cmd += filename;
    system(cmd.c_str());
  }

  void show() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
std::string tmp_file = std::tmpnam(nullptr);
#pragma GCC diagnostic pop
    tmp_file += ".ppm";
    save_ppm(tmp_file);
    std::string cmd = "open ";
    cmd += tmp_file;
    system(cmd.c_str());
  }
};

#endif
