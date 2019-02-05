#ifndef FIGURES_COLLECTION_HPP
#define FIGURES_COLLECTION_HPP


#include <vector>
#include <string>
#include <fstream>

#include "point.hpp"
#include "convex_polygon.hpp"


class FiguresCollection
{
public:
    FiguresCollection(double L)
    : _L(L), _fillers({}), _interfaces({})
    {}

    std::vector<PolygonalCylinder> fillers()
      {
        return this->_fillers;
      }

    std::vector<PolygonalCylinder> interfaces()
      {
        return this->_interfaces;
      }

    void save_csg(std::string csg_fname)
      {
        std::ofstream fout(csg_fname);
        fout << "algebraic3d\n\n";
        fout << "solid box = orthobrick(0, 0, 0; "
            << this->_L << ", " << this->_L << ", " << this->_L << ");\n\n";
        if (!this->_fillers.empty())
          {
            for (size_t idx = 0; idx < this->_fillers.size(); ++idx)
              {
                fout << "solid filler_" << idx << " =\n";
                fout << this->_fillers[idx].csg_str() << std::endl;
                fout << "\t\tand\n\t\tbox;\n";
              }
            fout << "\n";
            if (!this->_interfaces.empty())
              {
                for (size_t idx = 0; idx < this->_interfaces.size(); ++idx)
                  {
                    fout << "solid interface_" << idx << "=\n";
                    fout << this->_interfaces[idx].csg_str() << std::endl;
                    fout << "\t\tand\n\t\tbox;\n";
                  }
                fout << "\n";
              }
          }
        else
          {
            fout << ";\n";
          }
        fout << "solid matrix = box";
        if (!this->_fillers.empty())
          {
            for(size_t idx = 0; idx < this->_fillers.size(); ++idx)
              {
                fout << " and not filler_" << idx;
              }
            if(!this->_interfaces.empty())
              {
                for(size_t idx = 0; idx  < this->_interfaces.size(); ++idx)
                  {
                    fout << " and not interface_" << idx;
                  }
              }
          }
        fout << ";\n\n";
        if (!this->_fillers.empty())
          {
            fout << "solid filler =\n";
            for(size_t idx = 0; idx < this->_fillers.size(); ++idx)
              {
                if(idx == 0)
                  {
                    fout << "\t\tfiller_0";
                  }
                else
                  {
                    fout << "\n\t\tor\n\t\tfiller_" << idx;
                  }
              }
            fout << ";\n";
            fout << "tlo filler;\n\n";
            if(!this->_interfaces.empty())
              {
                fout << "solid interface =\n";
                for(size_t idx = 0; idx < this->_interfaces.size(); ++idx)
                  {
                    if(idx == 0)
                      {
                        fout << "\t\tinterface_0 and not filler_0";
                      }
                    else
                      {
                        fout << "\t\tor\n\t\tinterface_" << idx << " "
                             << " and not filler_" << idx;
                      }
                  }
                fout << ";\n";
                fout << "tlo interface -transparent;\n\n";
              }
          }
        fout << "tlo matrix -transparent;";
      }


    bool try_add(size_t phase, PolygonalCylinder &figure)
      {
        size_t n = figure.top().pts().size();
        for(size_t idx = 0; idx < n; ++idx)
          {
            if (0 > figure.top().pts()[idx].x()
                || this->_L < figure.top().pts()[idx].x()
                || 0 > figure.top().pts()[idx].y()
                || this->_L < figure.top().pts()[idx].y()
                || 0 > figure.top().pts()[idx].z()
                || this->_L < figure.top().pts()[idx].z()
                || 0 > figure.bottom().pts()[idx].x()
                || this->_L < figure.bottom().pts()[idx].x()
                || 0 > figure.bottom().pts()[idx].y()
                || this->_L < figure.bottom().pts()[idx].y()
                || 0 > figure.bottom().pts()[idx].z()
                || this->_L < figure.bottom().pts()[idx].z())
              {
                return false;
              }
        }
        for(PolygonalCylinder & filler : this->_fillers)
          {
            if(filler.crosses(figure))
              {
                return false;
              }
          }
        this->add(phase, figure);
        return true;
      }

    void add(size_t phase, PolygonalCylinder &figure)
      {
        if(phase == 0)
          {
            this->_fillers.push_back(figure);
          }
        else if(phase == 1)
          {
            this->_interfaces.push_back(figure);
          }
        return;
      }

private:
    double _L;
    std::vector<PolygonalCylinder> _fillers;
    std::vector<PolygonalCylinder> _interfaces;
};


#endif  // FIGURES_COLLECTION_HPP
