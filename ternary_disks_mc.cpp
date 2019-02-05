#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "src/point.hpp"
#include "src/utils.hpp"
#include "src/convex_polygon.hpp"
#include "src/polygonal_cylinder.hpp"
#include "src/figures_collection.hpp"
#include "src/options.hpp"


int main()
{
    Options o = read_options_file("options");

    if(o.shape != "disks")
      {
        std::cout << "Shape is not disks: " << o.shape << std::endl;
        return 0;
      }
    size_t vertices = o.vertices;
    size_t N = o.N;
    double th = o.th;
    double R = o.ar * th/2;
    double L = o.Lr * R;
    size_t fails_max = o.fails_max;
    double tau = o.tau;

    if(true)
        std::cout << "vertices: " << vertices << "\n"
                  << "N: " << N << "\n"
                  << "th: " << th << "\n"
                  << "R: " << R << "\n"
                  << "L: " << L << "\n"
                  << "fails_max: " << fails_max << "\n"
                  << "tau: " << tau << "\n";

    double central_angle = 2*M_PI / vertices;
    FiguresCollection fc(L);
    size_t N_done = 0;
    size_t fails_done = 0;

    srand(time(NULL));

    while(N_done < N && fails_done < fails_max)
      {
        double x = L/2;
        double y = L/2;
        double z = L/2;
        std::vector<Point> pts({});
        for(size_t idx = 0; idx < vertices; ++idx)
          {
            pts.push_back(Point(x + R * sin(idx*central_angle),
                                y + R * cos(idx*central_angle),
                                z));
          }
        double dx = static_cast<double>(rand()) / RAND_MAX * L;
        double dy = static_cast<double>(rand()) / RAND_MAX * L;
        double dz = static_cast<double>(rand()) / RAND_MAX * L;
        double ox = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;
        double oy = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;
        double oz = static_cast<double>(rand()) / RAND_MAX * 2 * M_PI;
        ConvexPolygon top(pts);
        ConvexPolygon bottom(pts);
        top.translate(0, 0, th/2);
        bottom.translate(0, 0, -th/2);
        PolygonalCylinder pc(top, bottom);
        pc.rotate(ox, oy, oz);
        pc.translate(dx, dy, dz);
        if(fc.try_add(0, pc))
          {
            ++N_done;
            std::cout << "Done: " << N_done
                      << "; Fails: " << fails_done << std::endl;
            std::vector<Point> int_pts({});
            for(size_t idx = 0; idx < vertices; ++idx)
              {
                int_pts.push_back(Point(x + (R + th*tau) * sin(idx*central_angle),
                                        y + (R + th*tau) * cos(idx*central_angle),
                                        z));
              }
            ConvexPolygon int_top(pts);
            ConvexPolygon int_bottom(pts);
            int_top.translate(0, 0, th*(1+tau)/2);
            int_bottom.translate(0, 0, -th*(1+tau)/2);
            PolygonalCylinder interface(int_top, int_bottom);
            interface.rotate(ox, oy, oz);
            interface.translate(dx, dy, dz);
            fc.add(1, interface);
          }
        else
          {
            ++fails_done;
          }
      }
    double fi = vertices * N_done * 0.5 * sin(central_angle) * R*R * th / L/L/L;
    std::cout << "Fillers: " << N_done
              << ", fi: " << fi << std::endl;

    fc.save_csg(std::string("py_mc_disks.geo"));

    return 0;
}
