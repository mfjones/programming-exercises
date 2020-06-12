//
// Created by Mitchell Jones on 6/12/20.
//

#ifndef DATA_STRUCTURES_CGAL_CENTERPOINT_RADON_H_
#define DATA_STRUCTURES_CGAL_CENTERPOINT_RADON_H_

#include <CGAL/Cartesian_d.h>
#include <CGAL/Linear_algebraCd.h>

#include <iostream>
#include <random>

typedef double Number_type;
typedef CGAL::Cartesian_d<Number_type> Kernel;
typedef Kernel::Point_d Point_d;

typedef std::vector<Point_d> Point_set;
typedef std::list<Number_type> Coords;

typedef CGAL::Linear_algebraCd<Number_type> LA;
typedef LA::Matrix Matrix;
typedef LA::Vector Vector;

void printPoint(const Point_d& p) {
  std::cout << "(";
  for (int i = 0; i < p.dimension(); i++) {
    std::cout << p.cartesian(i);
    if (i < p.dimension() - 1) std::cout << ", ";
  }
  std::cout << ")";
}

Point_d radonPoint(int d, Point_set& points) {
  if (points.size() < d + 2) {
    throw std::invalid_argument("Point set must have at least d+2 points.");
  }

  std::vector<Vector> entries;
  // Construct a system of equations with d+1 rows and
  // points.size() variables/columns.
  for (auto & point : points) {
    std::vector<Number_type> col;
    col.reserve(d+1);
    for (int i = 0; i < d; i++) {
      col.push_back(point.cartesian(i));
    }
    col.emplace_back(1);
    Vector v(col.begin(), col.end());
    entries.push_back(v);
  }

  // Solve the linear system.
  Matrix A(entries.begin(), entries.end());
  Matrix spanning_vecs;
  LA::homogeneous_linear_solver(A, spanning_vecs);

  // Get the first column of the spanning vectors.
  Vector soln(points.size());
  Number_type pos_sum = 0;
  for (int i = 0; i < points.size(); i++) {
    soln[i] = spanning_vecs(i,0);
    if (soln[i] >= 0) {
      pos_sum += soln[i];
    }
  }

  // Compute the radon point.
  Coords radon_coords;
  for (int i = 0; i < d; i++) {
    Number_type coord = 0;
    for (int j = 0; j < points.size(); j++) {
      if (soln[j] >= 0) {
        coord += (soln[j]/pos_sum) * points.at(j).cartesian(i);
      }
    }
    radon_coords.push_back(coord);
  }

  return Point_d(d, radon_coords.begin(), radon_coords.end());
}

Point_d apxCenterpoint(int d, Point_set& points,
    bool show_progress = true, bool debug = false) {
  Point_set temp = points;
  unsigned int n = temp.size();
  unsigned int num_iters = 2 * d * n * (unsigned int)log(n);

  // Obtain a random number from hardware & seed the generator.
  std::random_device rd;
  std::mt19937 gen(rd());
  // Range to sample random numbers from (inclusive).
  std::uniform_int_distribution<int> dist(0, n - 1);

  unsigned int step = 2;
  unsigned int display_next = 0;

  // For num_iters iterations, do:
  // 1. Mark a random point in temp to delete.
  // 2. Sample d+2 points from temp, compute their radon point.
  // 3. Add the radon point to temp, and remove the marked point from step 1.
  for (unsigned int i = 0; i < num_iters; i++) {
    if (debug) {
      std::cout << "[  ";
      for (auto & p : temp) {
        std::cout << p << "  ";
      }
      std::cout << "]" << std::endl;
    }

    if (show_progress) {
      // Show a progress bar (fancy!)
      unsigned int percent = (100 * (i + 1)) / num_iters;
      if (percent >= display_next) {
        std::cout << "\r" << "[" << std::string(percent / 2, '=')
                  << std::string(100 / 2 - percent / 2, ' ') << "] ";
        std::cout << percent << "%" << " [Iteration " << i + 1 << " of "
                  << num_iters << "]";
        std::cout << " ... Approximate centerpoint so far: ";
        printPoint(temp.at(0));
        std::cout.flush();
        display_next += step;
      }
    }

    // Choose a point to delete and a random sample (with replacement)
    // of d + 2 points.
    int replace_index = dist(gen);
    Point_set sample;
    for (int j = 0; j < d + 2; j++) {
      sample.push_back(temp.at(dist(gen)));
    }

    // Replace marked point with radon point.
    Point_d radon_point = radonPoint(d, sample);
    temp[replace_index] = radon_point;
  }

  if (show_progress) std::cout << std::endl;

  // Finally, return an arbitrary element in the temp set.
  return temp.at(0);
}

#endif //DATA_STRUCTURES_CGAL_CENTERPOINT_RADON_H_
