
#ifndef INTERSECTIONCOMPUTER_HPP
#define INTERSECTIONCOMPUTER_HPP
#include "Sketches.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

class IntersectionComputer : public Sketches {
public:
    static constexpr double epsilon = 1e-9;

    std::vector<Point2D> line_line_intersectionPoints(int index1, int index2) {
        const Line* line1 = getLine(index1);
        const Line* line2 = getLine(index2);
        if (!line1 || !line2) return {};

        const Point2D* p1_start = getPoint(line1->startPointIndex);
        const Point2D* p1_end = getPoint(line1->endPointIndex);
        const Point2D* p2_start = getPoint(line2->startPointIndex);
        const Point2D* p2_end = getPoint(line2->endPointIndex);
        if (!p1_start || !p1_end || !p2_start || !p2_end) return {};

        return computeLineLineIntersection(*p1_start, *p1_end, *p2_start, *p2_end);
    }

    std::vector<Point2D> line_circularArc_intersectionPoints(int lineIndex, int arcIndex) {
        const Line* line = getLine(lineIndex);
        const CircularArc* arc = getClosedConicSectionCurve(arcIndex);
        if (!line || !arc) return {};

        const Point2D* lineStart = getPoint(line->startPointIndex);
        const Point2D* lineEnd = getPoint(line->endPointIndex);
        const Point2D* center = getPoint(arc->center);
        const Point2D* arcStart = getPoint(arc->startPoint);
        const Point2D* arcEnd = getPoint(arc->endPoint);
        if (!lineStart || !lineEnd || !center || !arcStart || !arcEnd) return {};

        std::vector<Point2D> intersections = computeLineCircleIntersection(*lineStart, *lineEnd, *center, arc->radius);
        std::vector<Point2D> validIntersections;
        for (const auto& p : intersections) {
            if (isPointOnLineSegment(p, *lineStart, *lineEnd) && isPointOnArc(p, *center, *arcStart, *arcEnd, arc->angle))
                validIntersections.push_back(p);
        }
        return validIntersections;
    }

    std::vector<Point2D> circularArc_circularArc_intersectionPoints(int index1, int index2) {
        const CircularArc* arc1 = getClosedConicSectionCurve(index1);
        const CircularArc* arc2 = getClosedConicSectionCurve(index2);
        if (!arc1 || !arc2) return {};

        const Point2D* center1 = getPoint(arc1->center);
        const Point2D* center2 = getPoint(arc2->center);
        const Point2D* start1 = getPoint(arc1->startPoint);
        const Point2D* end1 = getPoint(arc1->endPoint);
        const Point2D* start2 = getPoint(arc2->startPoint);
        const Point2D* end2 = getPoint(arc2->endPoint);
        if (!center1 || !center2 || !start1 || !end1 || !start2 || !end2) return {};

        std::vector<Point2D> intersections = computeCircleCircleIntersection(*center1, arc1->radius, *center2, arc2->radius);
        std::vector<Point2D> validIntersections;
        for (const auto& p : intersections) {
            if (isPointOnArc(p, *center1, *start1, *end1, arc1->angle) && isPointOnArc(p, *center2, *start2, *end2, arc2->angle))
                validIntersections.push_back(p);
        }
        return validIntersections;
    }

private:
    static std::vector<Point2D> computeLineLineIntersection(const Point2D& p1, const Point2D& p2, const Point2D& q1, const Point2D& q2) {
        double det = (p2.x - p1.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q2.x - q1.x);
        if (std::abs(det) < epsilon) return {}; // Parallel lines

        double t = ((q1.x - p1.x) * (q2.y - q1.y) - (q1.y - p1.y) * (q2.x - q1.x)) / det;
        double s = ((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x)) / det;

        if (t >= 0 - epsilon && t <= 1 + epsilon && s >= 0 - epsilon && s <= 1 + epsilon)
            return { { p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y) } };
        return {};
    }

    static std::vector<Point2D> computeLineCircleIntersection(const Point2D& p1, const Point2D& p2, const Point2D& center, double radius) {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        double A = dx * dx + dy * dy;
        double B = 2 * (dx * (p1.x - center.x) + dy * (p1.y - center.y));
        double C = (p1.x - center.x) * (p1.x - center.x) + (p1.y - center.y) * (p1.y - center.y) - radius * radius;

        double discriminant = B * B - 4 * A * C;
        if (discriminant < -epsilon) return {};
        discriminant = std::max(discriminant, 0.0);

        double t1 = (-B + std::sqrt(discriminant)) / (2 * A);
        double t2 = (-B - std::sqrt(discriminant)) / (2 * A);

        std::vector<Point2D> points;
        if (t1 >= 0 - epsilon && t1 <= 1 + epsilon)
            points.push_back({ p1.x + t1 * dx, p1.y + t1 * dy });
        if (discriminant > epsilon && t2 >= 0 - epsilon && t2 <= 1 + epsilon)
            points.push_back({ p1.x + t2 * dx, p1.y + t2 * dy });
        return points;
    }

    static std::vector<Point2D> computeCircleCircleIntersection(const Point2D& c1, double r1, const Point2D& c2, double r2) {
        double d = std::hypot(c2.x - c1.x, c2.y - c1.y);
        if (d > r1 + r2 + epsilon || d < std::abs(r1 - r2) - epsilon) return {};

        double a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
        double h = std::sqrt(r1 * r1 - a * a);
        double cx = c1.x + a * (c2.x - c1.x) / d;
        double cy = c1.y + a * (c2.y - c1.y) / d;

        Point2D p1 = { cx + h * (c2.y - c1.y) / d, cy - h * (c2.x - c1.x) / d };
        Point2D p2 = { cx - h * (c2.y - c1.y) / d, cy + h * (c2.x - c1.x) / d };

        if (std::abs(h) < epsilon) return { p1 };
        return { p1, p2 };
    }

    static bool isPointOnLineSegment(const Point2D& p, const Point2D& start, const Point2D& end) {
        double cross = (p.x - start.x) * (end.y - start.y) - (p.y - start.y) * (end.x - start.x);
        if (std::abs(cross) > epsilon) return false;

        double dot = (p.x - start.x) * (end.x - start.x) + (p.y - start.y) * (end.y - start.y);
        if (dot < 0 - epsilon) return false;

        double len2 = (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y);
        return dot <= len2 + epsilon;
    }

    static bool isPointOnArc(const Point2D& p, const Point2D& center, const Point2D& start, const Point2D& end, double angle) {
        double theta = std::atan2(p.y - center.y, p.x - center.x);
        double thetaStart = std::atan2(start.y - center.y, start.x - center.x);
        double thetaEnd = std::atan2(end.y - center.y, end.x - center.x);
        theta = theta < 0 ? theta + 2 * M_PI : theta;
        thetaStart = thetaStart < 0 ? thetaStart + 2 * M_PI : thetaStart;
        thetaEnd = thetaEnd < 0 ? thetaEnd + 2 * M_PI : thetaEnd;

        if (angle > 2 * M_PI - epsilon) return true; // Full circle
        return (theta >= thetaStart - epsilon && theta <= thetaStart + angle + epsilon) ||
               (theta + 2 * M_PI >= thetaStart - epsilon && theta + 2 * M_PI <= thetaStart + angle + epsilon);
    }
};
#endif


#include <gtest/gtest.h>
#include "IntersectionComputer.hpp"

class IntersectionComputerTest : public ::testing::Test {
protected:
    IntersectionComputer sketch;
    int p0, p1, p2, p3, p4, p5;

    void SetUp() override {
        p0 = sketch.addPoint({0, 0});
        p1 = sketch.addPoint({2, 2});
        p2 = sketch.addPoint({0, 2});
        p3 = sketch.addPoint({2, 0});
        p4 = sketch.addPoint({1, 1});
        p5 = sketch.addPoint({3, 3});
    }
};

TEST_F(IntersectionComputerTest, LineLineIntersection) {
    int line1 = sketch.addLine({p0, p1});
    int line2 = sketch.addLine({p2, p3});
    auto points = sketch.line_line_intersectionPoints(line1, line2);
    ASSERT_EQ(points.size(), 1);
    EXPECT_NEAR(points[0].x, 1.0, 1e-6);
    EXPECT_NEAR(points[0].y, 1.0, 1e-6);
}

TEST_F(IntersectionComputerTest, LineArcIntersection) {
    int line = sketch.addLine({p0, p1});
    int arc = sketch.addClosedConicSectionCurve(CircularArc(p4, p0, p1, std::sqrt(2), M_PI));
    auto points = sketch.line_circularArc_intersectionPoints(line, arc);
    ASSERT_EQ(points.size(), 2);
}

TEST_F(IntersectionComputerTest, ArcArcIntersection) {
    int arc1 = sketch.addClosedConicSectionCurve(CircularArc(p0, p1, p2, 2, M_PI/2));
    int arc2 = sketch.addClosedConicSectionCurve(CircularArc(p3, p1, p2, 2, M_PI/2));
    auto points = sketch.circularArc_circularArc_intersectionPoints(arc1, arc2);
    ASSERT_EQ(points.size(), 2);
}
