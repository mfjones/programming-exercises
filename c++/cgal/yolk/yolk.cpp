// Computes the extremal yolk of a point set. See this paper:
// https://arxiv.org/abs/1912.01639

#include <CGAL/Cartesian.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/number_utils.h>
#include <cstdlib>

#include <math.h>

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Cartesian<CGAL::Exact_rational> Kernel;
typedef CGAL::Arr_linear_traits_2<Kernel> Traits_2;
typedef Traits_2::Point_2 Point_2;
typedef Traits_2::Line_2 Line_2;
typedef std::vector<Point_2> Point_set;
typedef std::vector<Line_2> Line_set;

// program and solution types
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

Line_set compute_lines(
    Point_set & points, bool median_lines = false) {

  Line_set lines;

  int n = points.size();
  double half = ceil(n/2.0);

  // Compute all pairwise lines.
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      Line_2 line(points[i], points[j]);

      // All lines.
      if (!median_lines) {
        lines.push_back(line);
        continue;
      }

      // Test if point is a median line.
      int on = 0; int pos = 0; int neg = 0;
      for (int k = 0; k < n; k++) {
        Point_2 p = points[k];
        if (line.has_on_negative_side(p)) {
          neg++;
        } else if (line.has_on_positive_side(p)) {
          pos++;
        } else {
          on++;
        }
      }

      if (pos + on >= half && neg + on >= half) {
        lines.push_back(line);
      }
    }
  }

  return lines;
}

int main(int argc, char *argv[])
{
  // Get the name of the input file from the command line, or use the default
  // points.dat file if no command-line parameters are given.
  const char* filename = (argc > 1) ? argv[1] : "points2.dat";

  // Open the input file.
  std::ifstream in_file(filename);

  if (! in_file.is_open()) {
    std::cerr << "Failed to open " << filename << "!" << std::endl;
    return 1;
  }

  // Read the points from the file, and construct their dual lines.
  // The input file format should be (all coordinate values are integers):
  // <n>                                 // number of point.
  // <x_1> <y_1>                         // point #1.
  // <x_2> <y_2>                         // point #2.
  //   :      :       :      :
  // <x_n> <y_n>                         // point #n.
  Point_set points;

  size_t n;
  in_file >> n;
  points.resize(n);
  unsigned int k;
  for (k = 0; k < n; ++k) {
    double px, py;
    in_file >> px >> py;
    points[k] = Point_2(px, py);
  }
  in_file.close();

  // Compute lines for egg/yolk and solve LP.
  Line_set lines = compute_lines(points);

  Program lp(CGAL::LARGER, false, 0, false, 0);
  int r = 0;
  int x = 1;
  int y = 2;

  // Set obj function to minimize r, let r >= 0.
  lp.set_c(r, 1);
  lp.set_l(r, true, 0);

  int i = 0;
  for (Line_2 & line : lines) {
    double a = CGAL::to_double(line.a());
    double b = CGAL::to_double(line.b());
    double c = CGAL::to_double(line.c());

    double norm = CGAL::to_double(
        CGAL::sqrt(CGAL::square(a) + CGAL::square(b))
      );

    lp.set_a(r, i, 1);
    lp.set_a(x, i, -a/norm);
    lp.set_a(y, i, -b/norm);
    lp.set_b(i, c/norm);

    lp.set_a(r, i+1, 1);
    lp.set_a(x, i+1, a/norm);
    lp.set_a(y, i+1, b/norm);
    lp.set_b(i+1, -c/norm);

    i += 2;
  }

  Solution s = CGAL::solve_quadratic_program(lp, ET());
  Solution::Variable_value_iterator vals = s.variable_values_begin();

  // Output disk.
  double sq_radius = CGAL::to_double(
      CGAL::square(s.objective_value())
    );
  double center_x = CGAL::to_double(*(++vals));
  double center_y = CGAL::to_double(*(++vals));

  std::cout << "(" << center_x << ", " << center_y << ") with sq radius = " << sq_radius << std::endl;

  return (0);
}
