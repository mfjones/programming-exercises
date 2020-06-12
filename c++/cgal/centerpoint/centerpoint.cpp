//
// Created by Mitchell Jones on 6/12/20.
//

#include "radon.h"

#include <fstream>

// Open an input stream.
inline bool open_stream(std::ifstream& is, const char* filename)
{
  is.open(filename);
  if (is.is_open()) return true;
  std::string error_message("Failed to open ");
  error_message.append(filename).append("!");
  CGAL_error_msg(error_message.c_str());
  return false;
}

int main() {
  // File contains 10000 points sampled UAR from the square [0,20]^2.
  // Hence, we expect the centerpoint to be roughly (10, 10).
  const char* filename = "points.dat";

  // Read in:
  // 1. The dimension d.
  // 2. Number of points n.
  // 3. The collection of n d-dimensional points, of the
  //    form (d, x_1, ..., x_d).
  std::ifstream is;
  if (!open_stream(is, filename)) exit(1);
  int d, n;
  is >> d;
  is >> n;
  Point_set points;
  for (unsigned int i = 0; i < n; ++i) {
    Point_d p;
    is >> p;
    points.push_back(p);
  }
  is.close();

  std::cout << "Computing the centerpoint of " << n
            << " points in " << d << "-dimensions.." << std::endl;

  Point_d cpnt = apxCenterpoint(d, points);

  std::cout << "Approximate centerpoint: ";
  printPoint(cpnt);
  std::cout << std::endl;
}