#ifndef POINT_IN_FACE_H
#define POINT_IN_FACE_H

#include <CGAL/Arr_accessor.h>
#include <CGAL/number_utils.h>
#include <CGAL/Triangle_2.h>

typedef CGAL::Triangle_2<Kernel> Triangle;

// Orthogonal distance from a point c to line through points a, b.
template <typename Kernel>
Number_type orthog_dist(
    typename Kernel::Point_2 a,
    typename Kernel::Point_2 b,
    typename Kernel::Point_2 c) {
  return CGAL::abs(
      (b.x() - a.x())*(c.y() - a.y()) -
      (b.y() - a.y())*(c.x() - a.x())
    ) / CGAL::sqrt(
        CGAL::square(a.x() - b.x()) + CGAL::square(a.y() - b.y())
    );
}

// Arbitrary point inside simple polygon.
template <typename Arrangement, typename Kernel>
typename Kernel::Point_2 point_in_face(
    typename Arrangement::Face_const_handle f,
    const Kernel & ker
  ) {
  typedef typename Kernel::Point_2 K_Point_2;
  typedef typename Arrangement::Ccb_halfedge_const_circulator
      Halfedge_const_circulator;
  typedef typename Arrangement::Halfedge_const_handle Halfedge_const_handle;
  typedef typename Arrangement::Vertex_const_handle Vertex_const_handle;

  Halfedge_const_circulator circ = f->outer_ccb();
  Halfedge_const_circulator curr = circ;

  // Find a convex vertex v, such a vertex must exist.
  Vertex_const_handle v, a, b;
  do {
    Halfedge_const_handle he = curr;
    K_Point_2 p1 = he->source()->point();
    K_Point_2 p2 = he->target()->point();
    K_Point_2 p3 = he->next()->target()->point();

    if (CGAL::orientation(p1, p2, p3) == CGAL::LEFT_TURN) {
      a = he->source();
      v = he->target();
      b = he->next()->target();
      break;
    }
  } while (++curr != circ);

  K_Point_2 pa = a->point();
  K_Point_2 pb = b->point();
  K_Point_2 pv = v->point();
  Triangle t(pa, pv, pb);

  // For every other vertex s, not equal to a, b, v, check if s is
  // inside the triangle abv. If so, find the vertex q s.t.
  // orthogonal distance from ab to q is maximum.
  bool inside_pt = false;
  Number_type max_dist = 0;

  Vertex_const_handle q;
  curr = circ;
  do {
    Vertex_const_handle s = curr->source();
    if (s == v || s == a || s == b
          || t.bounded_side(s->point()) == CGAL::ON_UNBOUNDED_SIDE) {
      continue;
    }

    // So q is inside the triangle, compute orthogonal
    // distance from ab.
    K_Point_2 ps = s->point();
    double dist = orthog_dist<Kernel>(pa, pb, ps);
    if (dist >= max_dist) {
      max_dist = dist;
      q = s;
      inside_pt = true;
    }
  } while (++curr != circ);

  typename Kernel::Construct_midpoint_2 midpoint
      = ker.construct_midpoint_2_object();

  // Finally, return either the midpoint of ab if inside is false,
  // otherwise return the midpoint of vq.
  if (inside_pt) {
    return midpoint(pv, q->point());
  }
  return midpoint(pv, midpoint(pa, pb));
}

#endif
