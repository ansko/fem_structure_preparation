#ifndef CONVEX_POLYGON_HPP
#define CONVEX_POLYGON_HPP


#include <vector>

#include "point.hpp"
#include "utils.hpp"


class ConvexPolygon
{
public:
    ConvexPolygon(std::vector<Point> pts)
    : _pts(pts)
    {}

    std::vector<Point> pts()
      {
        return this->_pts;
      }

    //def __str__(self):
    //    return '\n'.join(['Polygon'] + ['\t' + str(pt) for pt in self.pts])

    void translate(double dx=0, double dy=0, double dz=0)
      {
        for (auto & pt : this->_pts)
          {
            pt.translate(dx, dy, dz);
          }
        return;
      }

    void rotate(double ox=0, double oy=0, double oz=0)
      {
        for (auto & pt : this->_pts)
          {
            pt.rotate(ox, oy, oz);
          }
        return;
      }

    Point center()
      {
        return ::center(this->_pts);
      }

    bool crosses(ConvexPolygon &other)
      {
        Point c_self = this->center();
        Point c_other = other.center();
        size_t n_this = this->_pts.size();
        size_t n_other = other.pts().size();
        for (size_t idx_this = 0; idx_this < this->_pts.size(); ++idx_this)
          {
            std::vector<Point> tr_self;
            Point pt_minus_this, pt_this;
            if(idx_this == 0)
              {
                pt_minus_this = this->_pts[n_this-1];
                pt_this = this->_pts[0];
              }
            else
              {
                pt_minus_this = this->_pts[idx_this-1];
                pt_this = this->_pts[idx_this];
              }
            tr_self = {pt_minus_this, pt_this, c_self};
            for (size_t idx_other = 0; idx_other < other.pts().size();
                ++idx_other)
              {
                std::vector<Point> tr_other;
                Point pt_minus_other, pt_other;
                if(idx_other == 0)
                  {
                    pt_minus_other = other.pts()[n_other-1];
                    pt_other = other.pts()[0];
                  }
                else
                  {
                    pt_minus_other = other.pts()[idx_other-1];
                    pt_other = other.pts()[idx_other];
                  }
                tr_other = {pt_minus_other, pt_other,  c_other};
/*                if (segment_crosses_triangle(
                        this->_pts[idx_this], this->_pts[idx_this+1], tr_other) 
                    || segment_crosses_triangle(
                        this->_pts[idx_this+1], c_self, tr_other)
                    || segment_crosses_triangle(
                        c_self, this->_pts[idx_this], tr_other)
                    || segment_crosses_triangle(
                        other.pts()[idx_other], other.pts()[idx_other+1], tr_self)
                    || segment_crosses_triangle(
                        other.pts()[idx_other+1], c_other, tr_self)
                    || segment_crosses_triangle(
                        c_other, other.pts()[idx_other], tr_self))*/
                if (segment_crosses_triangle(
                        pt_minus_this, pt_this, tr_other) 
                    || segment_crosses_triangle(
                        pt_minus_this, c_self, tr_other)
                    || segment_crosses_triangle(
                        c_self, pt_this, tr_other)
                    || segment_crosses_triangle(
                        pt_minus_other, pt_other, tr_self)
                    || segment_crosses_triangle(
                        pt_minus_other, c_other, tr_self)
                    || segment_crosses_triangle(
                        c_other, pt_other, tr_self))
                  {
                        return true;
                  }
              }
          }
        return false;
      }


    bool is_regular()
      {
        double d = distance(this->_pts[-1], this->_pts[0]);
        for(size_t idx = 1; idx < this->_pts.size(); ++idx)
          {
            double new_d = distance(this->_pts[idx], this->_pts[idx-1]);
            if (!(0.999*d < new_d && new_d < 1.001*d))
              {
                return false;
              }
          }
        return true;
      }

private:
    std::vector <Point> _pts;
};


#endif  // CONVEX_POLYGON_HPP
