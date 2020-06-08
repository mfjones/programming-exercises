//
// Created by Mitchell Jones on 20/4/20.
// Exercise 2.1 in CGAL Arrangements and Their Applications, Fogel et. al.
//

#include "arr_inexact_construction_segments.h"
#include "arr_print.h"
#include "point_in_face.h"

int main() {
  // Construct the arrangement.
  Arrangement arr;
  Kernel ker;

  // Point p1(1, 3), p2(3, 5), p3(5, 3), p4(3, 1);
  // Segment s1(p1, p2), s2(p2, p3), s3(p3, p4), s4(p4, p1), s5(p1, p3);

  // Halfedge_handle e1 = arr.insert_in_face_interior(s1, arr.unbounded_face());
  // Vertex_handle   v1 = e1->source();
  // Vertex_handle   v2 = e1->target();
  // Halfedge_handle e2 = arr.insert_from_left_vertex(s2, v2);
  // Vertex_handle   v3 = e2->target();
  // Halfedge_handle e3 = arr.insert_from_right_vertex(s3, v3);
  // Vertex_handle   v4 = e3->target();
  // Halfedge_handle e4 = arr.insert_at_vertices(s4, v4, v1);
  // Halfedge_handle e5 = arr.insert_at_vertices(s5, v1, v3);

  Point p1(2,2), p2(4,2), p3(3,4), p4(3,3);
  Segment s1(p1, p4), s2(p2, p4), s3(p1, p3), s4(p2, p3);
  Halfedge_handle e1 = arr.insert_in_face_interior(
      s1, arr.unbounded_face());
  Vertex_handle v1 = e1->source();
  Vertex_handle v4 = e1->target();
  Halfedge_handle e2 = arr.insert_from_left_vertex(s2, v4);
  Vertex_handle v2 = e2->target();
  Halfedge_handle e4 = arr.insert_from_right_vertex(s4, v2);
  Vertex_handle v3 = e4->target();
  Halfedge_handle e3 = arr.insert_at_vertices(s3, v1, v3);

  print_arrangement(arr);

  Arrangement::Face_const_iterator fit;
  for (fit = arr.faces_begin(); fit != arr.faces_end(); ++fit) {
    if (fit->is_unbounded())
      continue;

    Kernel::Point_2 p = point_in_face<Arrangement, Kernel>(fit, ker);
    std::cout << p << std::endl;
  }

  return 0;
}
