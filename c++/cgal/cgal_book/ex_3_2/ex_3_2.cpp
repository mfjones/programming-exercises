//
// Created by Mitchell Jones on 6/5/20.
// Exercise 3.2 in CGAL Arrangements and Their Applications, Fogel et. al.
//

#include <CGAL/Arr_walk_along_line_point_location.h>
#include <cmath>
#include <iostream>

#include "arr_exact_construction_segments.h"
#include "arr_print.h"
#include "read_objects.h"

typedef CGAL::Arr_walk_along_line_point_location<Arrangement> Walk_pl;

Point rotatePoint(const Point& p, double angle) {
  double x = CGAL::to_double(p.x());
  double y = CGAL::to_double(p.y());
  double r_x = x * cos(angle) - y * sin(angle);
  double r_y = x * sin(angle) + y * cos(angle);
  return Point(r_x, r_y);
}

std::list<Segment> rotateSegments(const std::list<Segment>& segments, double angle) {
  std::list<Segment> rotated_segments;
  for (const auto& segment : segments) {
    const Point& p1 = segment.source();
    const Point& p2 = segment.target();
    rotated_segments.emplace_back(Segment(rotatePoint(p1, angle), rotatePoint(p2, angle)));
  }

  return rotated_segments;
}

void shoot_vertical_ray(const Walk_pl& pl, const Point& q, double angle) {
  typename Arrangement::Vertex_const_handle    v;
  typename Arrangement::Halfedge_const_handle  e;
  typename Arrangement::Face_const_handle      f;

  double inverse_angle = -angle;

  // Perform the vertical ray-shooting query.
  CGAL::Object obj = pl.ray_shoot_up(q);

  // Print the result.
  std::cout << "Shooting from (" << q << ") : ";
  // Hit an edge.
  if (CGAL::assign(e, obj)) {
    Point s = rotatePoint(e->curve().source(), inverse_angle);
    Point t = rotatePoint(e->curve().target(), inverse_angle);
    std::cout << "hit an edge: "
              << "(" << s << ") "
              << "(" << t << ")"
              << std::endl;
  }
  // Hit a vertex.
  else if (CGAL::assign(v, obj)) {
    Point p = rotatePoint(v->point(), inverse_angle);
    std::cout << "hit " << ((v->is_isolated()) ? "an isolated" : "a")
              << " vertex: (" << p << ")" << std::endl;
  }
  // Didn't hit anything.
  else if (CGAL::assign(f, obj)) {
    CGAL_assertion(f->is_unbounded());
    std::cout << "hit nothing." << std::endl;
  }
  // This should never happen.
  else CGAL_error_msg("Invalid object!");
}

int main() {
  const char* filename = "segments.dat";

  std::ifstream is;
  if (!open_stream(is, filename)) exit(1);

  Point dir;
  is >> dir;
  double error;
  is >> error;

  std::cout << "dir: (" << dir.x() << ", " << dir.y()
            << ") with error: " << error << std::endl;

  std::list<Segment> segments;
  unsigned int n;
  is >> n;
  for (unsigned int i = 0; i < n; ++i) {
    Segment s;
    is >> s;
    segments.push_back(s);
  }

  std::list<Point> query_points;
  unsigned int q;
  is >> q;
  for (unsigned int i = 0; i < q; ++i) {
    Point p;
    is >> p;
    query_points.push_back(p);
  }
  close_stream(is);

  double alpha = std::atan(CGAL::to_double(dir.y()/dir.x()));
  double beta = M_PI_2 - alpha;

  Number_type sin_beta, cos_beta, denom;
  CGAL::rational_rotation_approximation<Number_type>(beta, sin_beta, cos_beta, denom, error, 1);
  double beta_apx = asin(CGAL::to_double(sin_beta/denom));

  std::cout << "Ray shooting with angle of direction " << beta
            << " (using approx. value " << beta_apx << ")" << std::endl << std::endl;

  std::list<Segment> rotated_segments = rotateSegments(segments, beta_apx);

  Traits traits;
  Arrangement arr(&traits);
  insert(arr, rotated_segments.begin(), rotated_segments.end());

  std::cout << "Rotated arrangement:" << std::endl;
  std::cout << std::setfill('*') << std::setw(60) << "" << std::endl;
  print_arrangement(arr);
  std::cout << std::setfill('*') << std::setw(60) << "" << std::endl << std::endl;

  Walk_pl walk_pl(arr);
  for (const Point& query : query_points) {
    shoot_vertical_ray(walk_pl, query, beta_apx);
  }
}

