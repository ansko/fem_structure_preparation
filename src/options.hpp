#ifndef OPTIONS_HPP
#define OPTIONS_HPP


#include <map>
#include <string>
#include <fstream>


class Options
{
public:
    std::string shape;
    size_t vertices;
    size_t N;
    double ar;
    double Lr;
    double th;
    double tau;
    size_t fails_max;
};


Options read_options_file(std::string fname)
{
    Options o;
    std::ifstream fin(fname);
    std::string parameter;
    std::string value_str;
    double value_double;
    size_t value_size_t;

    while (fin >> parameter)
      {
        if(parameter == "shape")
          {
            fin >> value_str;
            o.shape = value_str;
          }
        else if(parameter == "vertices")
          {
            fin >> value_size_t;
            o.vertices = value_size_t;
          }
        else if(parameter == "N" || parameter == "disks_number")
          {
            fin >> value_size_t;
            o.N = value_size_t;
          }
        else if(parameter == "ar" || parameter == "aspect_ratio")
          {
            fin >> value_double;
            o.ar = value_double;
          }
        else if(parameter == "Lr")
          {
            fin >> value_double;
            o.Lr = value_double;
          }
        else if(parameter == "th" || parameter == "thickess"
                || parameter == "disk_thickness")
          {
            fin >> value_double;
            o.th = value_double;
          }
        else if(parameter == "tau")
          {
            fin >> value_double;
            o.tau = value_double;
          }
        else if(parameter == "fails_max")
          {
            fin >> value_size_t;
            o.fails_max = value_size_t;
          }
      }
    return o;
}


#endif  // OPTIONS_HPP
