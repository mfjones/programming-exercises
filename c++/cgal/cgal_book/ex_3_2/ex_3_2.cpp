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

// Rotates a point p about the origin by a given angle.
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

// Shoots a vertical ray in an arrangement rotated by the given angle.
// Outputs the given object hit after applying the inverse rotation.
void shootRay(const Walk_pl& pl, const Point& q, double angle) {
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

  // Read in:
  // 1. The direction in which to shoot ray queries,
  // 2. The error bound on the angle approximation.
  // 3. THe collection of segments in the arrangement.
  // 4. The collection of query points.
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

  // Approximate the angle.
  Number_type sin_alpha, cos_alpha, denom;
  CGAL::rational_rotation_approximation<Number_type>(dir.x(), dir.y(), sin_alpha, cos_alpha, denom, error, 1);
  double apx_angle = M_PI_2 - asin(CGAL::to_double(sin_alpha / denom));

  std::cout << "Ray shooting with angle of direction " << apx_angle << std::endl << std::endl;

  // Rotate the arrangement and perform the queries.
  std::list<Segment> rotated_segments = rotateSegments(segments, apx_angle);

  Traits traits;
  Arrangement arr(&traits);
  insert(arr, rotated_segments.begin(), rotated_segments.end());

  std::cout << "Rotated arrangement:" << std::endl;
  std::cout << std::setfill('*') << std::setw(60) << "" << std::endl;
  print_arrangement(arr);
  std::cout << std::setfill('*') << std::setw(60) << "" << std::endl << std::endl;

  Walk_pl walk_pl(arr);
  for (const Point& query : query_points) {
    shootRay(walk_pl, query, apx_angle);
  }
}

