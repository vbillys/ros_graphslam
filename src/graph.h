#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Pose.h"
#include "nav_msgs/OccupancyGrid.h"
#include "tf/transform_listener.h"

using namespace std;
using namespace geometry_msgs;

struct ScanGrid {
	// The size, based on the range of the laserscanner
	int width, height;
	// The bounds in the real map
	int ymax, ymin, xmax, xmin;
	// The grid, stored in row-major order
	vector<double> grid;
};

// A node in the graph, contains its own little occupancygrid to later be combined with all nodes in the map
struct Node {
	unsigned int id;
	Pose robot_pose;
	sensor_msgs::LaserScan laser_scan;
	// The occupancygrid for the scan at this position
	ScanGrid scan_grid;
};

// An edge represents the connection between two nodes
struct Edge {
	Node * parent;
	Node * child;	
};

class Graph {

	public:
		vector<Node> node_list;
		vector<Edge> edge_list;
		//
		Graph(double resolution, double range_threshold);
		void addNode(Pose pose, sensor_msgs::LaserScan scan);
		void generateMap(nav_msgs::OccupancyGrid& cur_map);
		void solve(unsigned int iterations);
	private:
		unsigned int idCounter;
		Node * last_node;
		double resolution, range_threshold;
		ScanGrid scanToOccGrid(sensor_msgs::LaserScan& scan, Pose& pose);

};