#ifndef UTILS_HPP
#define UTILS_HPP


#include <cmath>
#include <vector>

#include "point.hpp"


double distance(Point pt1, Point pt2)
{
    double dx = pt2.x() - pt1.x();
    double dy = pt2.y() - pt1.y();
    double dz = pt2.z() - pt1.z();
    return sqrt(dx*dx + dy*dy + dz*dz);
}


Point center(std::vector<Point> pts)
{
    double x = 0, y = 0, z = 0;
    size_t n = pts.size();
    for (auto & pt : pts)
      {
        x += pt.x();
        y += pt.y();
        z += pt.z();
      }
    return Point(x/n, y/n, z/n);
}


double angle(Point &pt1, Point &pt2, Point &pt3)
{
    double d12 = distance(pt1, pt2);
    double d13 = distance(pt1, pt3);
    double d23 = distance(pt2, pt3);
    return (d12*d12 + d23*d23 - d13*d13) / 2 / d12 / d23;
}


bool pt_inside_triangle(Point pt, std::vector<Point> tr_pts)
{
    Point tr_pt_1 = tr_pts[0];
    Point tr_pt_2 = tr_pts[1];
    Point tr_pt_3 = tr_pts[2];
    double p = (distance(tr_pt_1, tr_pt_2) + distance(tr_pt_2, tr_pt_3)
               + distance(tr_pt_3, tr_pt_1)) / 2;
    double p12 = (distance(pt, tr_pt_1) + distance(pt, tr_pt_2)
               + distance(tr_pt_1, tr_pt_2)) / 2;
    double p23 = (distance(pt, tr_pt_2) + distance(pt, tr_pt_3)
               + distance(tr_pt_2, tr_pt_3)) / 2;
    double p31 = (distance(pt, tr_pt_3) + distance(pt, tr_pt_1)
               + distance(tr_pt_3, tr_pt_1)) / 2;
    double s = sqrt(p * (p-distance(tr_pt_1, tr_pt_2))
                      * (p-distance(tr_pt_2, tr_pt_3))
                      * (p-distance(tr_pt_3, tr_pt_1)));
    double s12 = sqrt(p12 * (p12-distance(pt, tr_pt_1))
                          * (p12-distance(pt, tr_pt_2))
                          * (p12-distance(tr_pt_1, tr_pt_2)));
    double s23 = sqrt(p23 * (p23-distance(pt, tr_pt_2))
                          * (p23-distance(pt, tr_pt_3))
                          * (p23-distance(tr_pt_2, tr_pt_3)));
    double s31 = sqrt(p31 * (p31-distance(pt, tr_pt_3))
                          * (p31-distance(pt, tr_pt_1))
                          * (p31-distance(tr_pt_3, tr_pt_1)));
    return 0.999*s < s12+s23+s31 && s12+s23+s31 < 1.001*s;
}


bool segment_crosses_triangle(std::vector<Point> seg, std::vector<Point> tr)
{
    Point pt_seg_1 = seg[0];
    Point pt_seg_2 = seg[1];
    Point tr_pt_1 = tr[0];
    Point tr_pt_2 = tr[1];
    Point tr_pt_3 = tr[2];
    double dx11 = pt_seg_1.x() - tr_pt_1.x();
    double dy11 = pt_seg_1.y() - tr_pt_1.y();
    double dz11 = pt_seg_1.z() - tr_pt_1.z();
    double dx12 = pt_seg_1.x() - tr_pt_2.x();
    double dy12 = pt_seg_1.y() - tr_pt_2.y();
    double dz12 = pt_seg_1.z() - tr_pt_2.z();
    double dx13 = pt_seg_1.x() - tr_pt_3.x();
    double dy13 = pt_seg_1.y() - tr_pt_3.y();
    double dz13 = pt_seg_1.z() - tr_pt_3.z();
    double dx21 = pt_seg_2.x() - tr_pt_1.x();
    double dy21 = pt_seg_2.y() - tr_pt_1.y();
    double dz21 = pt_seg_2.z() - tr_pt_1.z();
    double dx22 = pt_seg_2.x() - tr_pt_2.x();
    double dy22 = pt_seg_2.y() - tr_pt_2.y();
    double dz22 = pt_seg_2.z() - tr_pt_2.z();
    double dx23 = pt_seg_2.x() - tr_pt_3.x();
    double dy23 = pt_seg_2.y() - tr_pt_3.y();
    double dz23 = pt_seg_2.z() - tr_pt_3.z();
    double vol1 = (dx11*(dy12*dz13 - dy13*dz12)
                  - dy11*(dx12*dz13 - dx13*dz12)
                  + dz11*(dx12*dy13 - dx13*dy12)) / 6;
    double vol2 = (dx21*(dy22*dz23 - dy23*dz22)
                  - dy21*(dx22*dz23 - dx23*dz22)
                  + dz21*(dx22*dy23 - dx23*dy22)) / 6;
    if (vol1 == 0 || vol2 == 0)
      {
        return true;
      }
    else if (vol1*vol2 > 0)
      {
        return false;
      }
    double coeff = fabs(vol1) / (fabs(vol1) + fabs(vol2));
    Point cross(pt_seg_1.x() + (pt_seg_2.x()-pt_seg_1.x()) * coeff,
                pt_seg_1.y() + (pt_seg_2.y()-pt_seg_1.y()) * coeff,
                pt_seg_1.z() + (pt_seg_2.z()-pt_seg_1.z()) * coeff);
    return pt_inside_triangle(cross, tr);
}

bool segment_crosses_triangle(Point pt_1, Point pt_2, Point pt_3,
    Point pt_4, Point pt_5)
{
    std::vector<Point> seg({pt_1, pt_2});
    std::vector<Point> tr({pt_3, pt_4, pt_5});
    return segment_crosses_triangle(seg, tr);
}

bool segment_crosses_triangle(Point pt_1, Point pt_2, std::vector<Point> tr)
{
    std::vector<Point> seg({pt_1, pt_2});
    return segment_crosses_triangle(seg, tr);
}

bool are_segments_parallel(std::vector<Point> seg1, std::vector<Point> seg2)
{
    double dx1 = seg1[1].x() - seg1[0].x();
    double dy1 = seg1[1].y() - seg1[0].y();
    double dz1 = seg1[1].z() - seg1[0].z();
    double dx2 = seg2[1].x() - seg2[0].x();
    double dy2 = seg2[1].y() - seg2[0].y();
    double dz2 = seg2[1].z() - seg2[0].z();
    double dr1 = sqrt(dx1*dx1 + dy1*dy1 + dz1*dz1);
    double dr2 = sqrt(dx2*dx2 + dy2*dy2 + dz2*dz2);
    double cos_angle = (dx1*dx2 + dy1*dy2 + dz1*dz2) / dr1 / dr2;
    return fabs(1 - abs(cos_angle)) < 0.001;
}


#endif  // UTILS_HPP
