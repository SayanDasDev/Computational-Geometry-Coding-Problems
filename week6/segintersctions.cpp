#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>

#define PROJECT_NAME "segintersctions"

// Use a small epsilon for floating-point comparisons to handle precision issues.
const double EPS = 1e-9;

// ## 1. Core Data Structures

// Represents a point in 2D space.
struct Point {
    double x, y;

    // Overload operators for comparison.
    bool operator<(const Point& other) const {
        if (std::abs(y - other.y) > EPS) {
            return y > other.y; // Higher y-coordinate comes first
        }
        return x < other.x - EPS; // Then smaller x-coordinate
    }

    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
    }
};

// Represents a line segment with two endpoints and a unique ID.
struct Segment {
    Point p1, p2;
    int id;

    // Constructor ensures p1 is the upper endpoint.
    Segment(Point start, Point end, int seg_id) : id(seg_id) {
        if (start < end) {
            p1 = start;
            p2 = end;
        } else {
            p1 = end;
            p2 = start;
        }
    }
};


// ## 2. Custom Comparators

// Functor for comparing points in the event queue.
// Follows the same logic as Point::operator<
struct PointCmp {
    bool operator()(const Point& a, const Point& b) const {
        if (std::abs(a.y - b.y) > EPS) {
            return a.y > b.y;
        }
        if (std::abs(a.x - b.x) > EPS) {
            return a.x < b.x;
        }
        return false;
    }
};

// Functor for comparing segments in the status structure.
// The order depends on the x-coordinate intersection with the sweep line.
struct SegmentCmp {
    // A static member to hold the current y-coordinate of the sweep line.
    static double sweep_line_y;

    // Calculates the x-coordinate of a segment at the sweep line's y-level.
    double getXatY(const Segment* s) const {
        if (std::abs(s->p1.y - s->p2.y) < EPS) { // Horizontal segment
            return s->p1.x;
        }
        return s->p1.x + (sweep_line_y - s->p1.y) * (s->p2.x - s->p1.x) / (s->p2.y - s->p1.y);
    }

    bool operator()(const Segment* a, const Segment* b) const {
        double x1 = getXatY(a);
        double x2 = getXatY(b);
        if (std::abs(x1 - x2) > EPS) {
            return x1 < x2;
        }
        double slope_a = (a->p2.y - a->p1.y) / (a->p2.x - a->p1.x);
        double slope_b = (b->p2.y - b->p1.y) / (b->p2.x - b->p1.x);
        return slope_a < slope_b;
    }
};

// Initialize the static member.
double SegmentCmp::sweep_line_y = 0;

// Define container types for clarity.
using EventQueue = std::set<Point, PointCmp>;
using Status = std::set<Segment*, SegmentCmp>;


// ## 3. The Bentley-Ottmann Algorithm Class

class BentleyOttmann {
public:
    struct Intersection {
        Point p;
        std::vector<int> segment_ids;
    };

    std::vector<Intersection> find(std::vector<Segment>& segments) {
        initialize(segments);
        while (!event_queue.empty()) {
            Point p = *event_queue.begin();
            event_queue.erase(event_queue.begin());
            handleEventPoint(p);
        }
        return intersections;
    }

private:
    EventQueue event_queue;
    Status status;
    std::vector<Intersection> intersections;
    std::map<Point, std::vector<Segment*>, PointCmp> upper_endpoints;

    void initialize(std::vector<Segment>& segments) {
        for (Segment& s : segments) {
            event_queue.insert(s.p1);
            event_queue.insert(s.p2);
            upper_endpoints[s.p1].push_back(&s);
        }
    }

    void handleEventPoint(const Point& p) {
        SegmentCmp::sweep_line_y = p.y;
        std::vector<Segment*> U = upper_endpoints.count(p) ? upper_endpoints[p] : std::vector<Segment*>();
        std::vector<Segment*> L, C;
        
        for (Segment* s : status) {
            if (s->p2 == p) L.push_back(s);
            // Check for collinearity AND if point is within the segment's bounding box
            else if (std::abs((p.y - s->p1.y) * (s->p2.x - s->p1.x) - (p.x - s->p1.x) * (s->p2.y - s->p1.y)) < EPS) {
                if (p.x >= std::min(s->p1.x, s->p2.x) - EPS && p.x <= std::max(s->p1.x, s->p2.x) + EPS &&
                    p.y >= std::min(s->p1.y, s->p2.y) - EPS && p.y <= std::max(s->p1.y, s->p2.y) + EPS) {
                    C.push_back(s);
                }
            }
        }
        
        if (U.size() + L.size() + C.size() > 1) {
            Intersection new_intersection;
            new_intersection.p = p;
            for(auto seg : U) new_intersection.segment_ids.push_back(seg->id);
            for(auto seg : L) new_intersection.segment_ids.push_back(seg->id);
            for(auto seg : C) new_intersection.segment_ids.push_back(seg->id);
            intersections.push_back(new_intersection);
        }
        
        for (Segment* s : L) status.erase(s);
        for (Segment* s : C) status.erase(s);
        SegmentCmp::sweep_line_y = p.y - EPS;
        for (Segment* s : U) status.insert(s);
        for (Segment* s : C) status.insert(s);
        
        if (U.empty() && C.empty()) {
            // This case handles when p is only a lower endpoint. The segments in L have been
            // removed from the status. We now find the new neighbors that surround the
            // position of the removed segments.
            Segment *sl = nullptr, *sr = nullptr;

            // Find the first segment in the status that is to the right of p.
            auto it = status.begin();
            for (; it != status.end(); ++it) {
                if (SegmentCmp().getXatY(*it) > p.x + EPS) {
                    sr = *it;
                    break;
                }
            }

            // The segment to the left is the one just before sr.
            if (it != status.begin()) {
                sl = *std::prev(it);
            }
            
            findNewEvent(sl, sr, p);
        } else {
            std::vector<Segment*> union_set = U;
            union_set.insert(union_set.end(), C.begin(), C.end());
            
            auto min_max = std::minmax_element(union_set.begin(), union_set.end(), [this](Segment* a, Segment* b){
                return SegmentCmp()(a, b);
            });
            Segment* leftmost = *min_max.first;
            Segment* rightmost = *min_max.second;

            auto it_left = status.find(leftmost);
            if (it_left != status.begin()) findNewEvent(*std::prev(it_left), leftmost, p);
            
            auto it_right = status.find(rightmost);
            if (std::next(it_right) != status.end()) findNewEvent(rightmost, *std::next(it_right), p);
        }
    }

    void findNewEvent(Segment* s1, Segment* s2, const Point& p) {
        if (!s1 || !s2) return;
        bool found;
        Point intersection_pt = getIntersection(*s1, *s2, found);
        if (found && (intersection_pt.y < p.y - EPS || (std::abs(intersection_pt.y - p.y) < EPS && intersection_pt.x > p.x + EPS))) {
            event_queue.insert(intersection_pt);
        }
    }

    Point getIntersection(const Segment& s1, const Segment& s2, bool& found) {
        found = false;
        Point p1 = s1.p1, p2 = s1.p2, p3 = s2.p1, p4 = s2.p2;
        double det = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
        if (std::abs(det) < EPS) return {};
        double t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / det;
        double u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / det;
        if (t >= -EPS && t <= 1 + EPS && u >= -EPS && u <= 1 + EPS) {
            found = true;
            return {p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y)};
        }
        return {};
    }
};

// ## 4. Input Processing Function

/**
 * @brief Reads line segments from a file.
 * @param filename The path to the input file.
 * @return A vector of Segment objects.
 *
 * The file should contain four space-separated numbers per line (x1 y1 x2 y2),
 * each representing a line segment.
 */
std::vector<Segment> readSegmentsFromFile(const std::string& filename) {
    std::vector<Segment> segments;
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return segments; // Return empty vector on error
    }

    std::string line;
    int segment_id_counter = 1;
    double x1, y1, x2, y2;

    // Read the file line by line
    while (std::getline(infile, line)) {
        // Use a stringstream to parse the numbers from the line
        std::istringstream iss(line);
        if (iss >> x1 >> y1 >> x2 >> y2) {
            Point p1 = {x1, y1};
            Point p2 = {x2, y2};
            segments.emplace_back(p1, p2, segment_id_counter++);
        } else {
            // Warn about lines that don't have four numbers
            if(!line.empty()) {
                std::cerr << "Warning: Skipping malformed line: " << line << std::endl;
            }
        }
    }

    infile.close();
    return segments;
}


// ## 5. Main Function - Entry Point

int main(int argc, char* argv[]) {
    // Check for the correct number of command-line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file.in>" << std::endl;
        return 1; // Return an error code
    }

    std::string filename = argv[1];
    std::vector<Segment> segments = readSegmentsFromFile(filename);

    // Exit if file was not read or contained no valid segments
    if (segments.empty()) {
        std::cerr << "No valid segments were read from the file. Exiting." << std::endl;
        return 1;
    }
    
    std::cout << "Read " << segments.size() << " segments from file '" << filename << "'.\n";
    
    BentleyOttmann solver;
    std::vector<BentleyOttmann::Intersection> intersections = solver.find(segments);
    
    std::cout << "\nFound " << intersections.size() << " intersection points:\n";
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& i : intersections) {
        std::cout << "  - Point (" << i.p.x << ", " << i.p.y << ") involves segments: ";
        // Sort IDs for consistent output
        std::vector<int> sorted_ids = i.segment_ids;
        std::sort(sorted_ids.begin(), sorted_ids.end());
        for (int id : sorted_ids) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }

    return 0;
}

