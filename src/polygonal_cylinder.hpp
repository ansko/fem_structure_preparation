#ifndef POLYGONAL_CYLINDER_HPP
#define POLYGONAL_CYLINDER_HPP


#include <string>
#include <sstream>
#include <vector>

#include "point.hpp"
#include "convex_polygon.hpp"
#include "utils.hpp"


class PolygonalCylinder
{
public:
    PolygonalCylinder(ConvexPolygon &top, ConvexPolygon &bottom)
    : _top(top), _bottom(bottom),
      _big_distance(distance(top.pts()[0], top.pts()[top.pts().size()-1])),
      _small_distance(distance(top.center(), bottom.center())/2)
      {
        for(size_t idx=1; idx < top.pts().size()-1; ++idx)
          {
            this->_big_distance = std::max(this->_big_distance,
                distance(top.pts()[0], top.pts()[idx]));
          }
        this->_small_distance = std::min(this->_big_distance,
            this->_small_distance);
      }

    double big_distance()
      {
        return this->_big_distance;
      }

    double small_distance()
      {
        return this->_small_distance;
      }

    ConvexPolygon top()
      {
        return this->_top;
      }

    ConvexPolygon bottom()
      {
        return this->_bottom;
      }

    //def __str__(self):
    //    return '\n'.join(['PolygonalCylinder', 'Top', str(self.top),
    //        'Bottom', str(self.bottom)])

    std::string csg_str(size_t tabs=0)
      {
        std::stringstream result;
        result << "\t\tplane("
            << this->_top.center().x() << ", " << this->_top.center().y()
            << ", " << this->_top.center().z() << "; "
            << this->_top.center().x() - this->center().x()
            << ", " << this->_top.center().y() - this->center().y()
            << ", " << this->_top.center().z() - this->center().z() << ")";
        result << "\n\t\tand\n\t\t";
        result << "plane("
            << this->_bottom.center().x() << ", " << this->_bottom.center().y()
            << ", " << this->_bottom.center().z() << "; "
            << this->_bottom.center().x() - this->center().x()
            << ", " << this->_bottom.center().y() - this->center().y()
            << ", " << this->_bottom.center().z() - this->center().z() << ")";
        for(auto & side : this->sides())
          {
            result << "\n\t\tand\n";
            result << "\t\tplane("
                << side.center().x() << ", "
                << side.center().y() << ", "
                << side.center().z() << "; "
                << side.center().x() - this->center().x() << ", "
                << side.center().y() - this->center().y() << ", "
                << side.center().z() - this->center().z() << ")";
          }
        return result.str();
      }

    Point center()
      {
        return ::center({this->_top.center(), this->_bottom.center()});
      }

    std::vector<ConvexPolygon> sides()
      {
        size_t n = this->_top.pts().size();
        std::vector<ConvexPolygon> result({});
        for(size_t idx = 0; idx < n; ++idx)
          {
            Point pt_top_min = this->_top.pts()[idx == 0 ? n-1 : idx-1];
            Point pt_bot_min = this->_bottom.pts()[idx == 0 ? n-1 : idx-1];
            result.emplace_back(ConvexPolygon({
                this->_top.pts()[idx], pt_top_min,
                pt_bot_min, this->_bottom.pts()[idx]}));
          }
        return result;
      }

    std::vector<ConvexPolygon> all_facets()
      {
        std::vector<ConvexPolygon> result = this->sides();
        result.push_back(this->_top);
        result.push_back(this->_bottom);
        return result;
      }

    void translate(double dx=0, double dy=0, double dz=0)
      {
        this->_top.translate(dx, dy, dz);
        this->_bottom.translate(dx, dy, dz);
        return;
      }

    void rotate(double ox=0, double oy=0, double oz=0)
      {
        this->_top.rotate(ox, oy, oz);
        this->_bottom.rotate(ox, oy, oz);
        return;
      }

    bool crosses(PolygonalCylinder & other)
      {
        /*double far = this->_big_distance + other.big_distance();
        double close = this->_small_distance + other.small_distance();
        if(distance(this->center(), other.center()) < close)
          {
            return true;
          }
        else if (distance(this->center(), other.center()) > far)
          {
            return false;
          }*/
        for (ConvexPolygon & self_side : this->all_facets())
          {
            for (ConvexPolygon & other_side : other.all_facets())
              {
                if(self_side.crosses(other_side))
                  {
                    return true;
                  }
              }
          }
        return false;
      }

private:
    ConvexPolygon _top;
    ConvexPolygon _bottom;
    double _big_distance;
    double _small_distance;
};


#endif  // POLYGONAL_CYLINDER_HPP
