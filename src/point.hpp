#ifndef POINT_HPP
#define POINT_HPP


#include <cmath>


class Point{
public:
    Point(double x=0, double y=0, double z=0)
    : _x(x), _y(y), _z(z)
    {}

    //def __str__(self):
    //    return 'Point({0}, {1}, {2})'.format(self.x, self.y, self.z)

    double x()
      {
        return this->_x;
      }
    double y()
      {
        return this->_y;
      }
    double z()
      {
        return this->_z;
      }

    void translate(double dx=0, double dy=0, double dz=0)
      {
        this->_x += dx;
        this->_y += dy;
        this->_z += dz;
        return;
      }

    void rotate(double ox=0, double oy=0, double oz=0)
      {
        if (ox != 0)
          {
            double m11 = 1,  m12 = 0,               m13 = 0,
                   m21 = 0,  m22 = cos(ox),  m23 = -sin(ox),
                   m31 = 0,  m32 = sin(ox),  m33 =  cos(ox);
            double new_x = m11*this->_x + m21*this->_y + m31*this->_z;
            double new_y = m12*this->_x + m22*this->_y + m32*this->_z;
            double new_z = m13*this->_x + m23*this->_y + m33*this->_z;
            this->_x = new_x;
            this->_y = new_y;
            this->_z = new_z;
          }
        if (oy != 0)
          {
            double m11 =  cos(oy), m12 = 0,  m13 = sin(oy),
                   m21 =  0,       m22 = 1,  m23 = 0,
                   m31 = -sin(oy), m32 = 0,  m33 = cos(oy);
            double new_x = m11*this->_x + m21*this->_y + m31*this->_z;
            double new_y = m12*this->_x + m22*this->_y + m32*this->_z;
            double new_z = m13*this->_x + m23*this->_y + m33*this->_z;
            this->_x = new_x;
            this->_y = new_y;
            this->_z = new_z;
          }
        if (oz != 0)
          {
            double m11 = cos(oz),  m12 = -sin(oz),  m13 = 0,
                   m21 = sin(oz),  m22 =  cos(oz),  m23 = 0,
                   m31 = 0,        m32 =  0,        m33 = 1;
            double new_x = m11*this->_x + m21*this->_y + m31*this->_z;
            double new_y = m12*this->_x + m22*this->_y + m32*this->_z;
            double new_z = m13*this->_x + m23*this->_y + m33*this->_z;
            this->_x = new_x;
            this->_y = new_y;
            this->_z = new_z;
          }
        return;
      }
private:
    double _x, _y, _z;
};


#endif  // POINT_HPP
