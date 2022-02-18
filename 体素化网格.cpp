#include <thread>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>  //文件输入输出

using namespace std::chrono_literals;
using namespace std;
int main(int argc, char** argv) {
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("HelloMyFirstVisualPCL"));
	viewer->setBackgroundColor(1, 1, 1);

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("benthi_control_A_D30_centre_filter1.pcd", *cloud) == -1)
	{
		PCL_ERROR("Cloudn't read file!");
		return -1;
	}
	cout << "there are " << cloud->points.size() << " points before filtering." << endl;

	FILE*fp = NULL; fp = fopen("el.txt", "r");	//2DpointDatas.txt
	if (!fp)
	{
		printf("打开文件失败！！\n");
		int m;
		cin >> m;
		exit(0);
	}
	float x = 0, y = 0, z = 0;
	int i = 0;

	pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ> fildColor(cloud, "z"); // 按照z字段进行渲染
																								 //viewer->addPointCloud(cloud);
	viewer->addPointCloud<pcl::PointXYZ>(cloud, fildColor, "sample cloud");
	while (!feof(fp))
	{
		float voxel = 1.85;
		i++;
		fscanf(fp, "%f %f %f", &x, &y, &z);
		string cube = "cube" + to_string(i);
		float x_min = floor(x / voxel)*voxel;
		float x_max = floor(x / voxel)*voxel + voxel;
		float y_min = floor(y / voxel)*voxel;
		float y_max = floor(y / voxel)*voxel + voxel;
		float z_min = floor(z / voxel)*voxel;
		float z_max = floor(z / voxel)*voxel + voxel;
		double r = 0.5, g = 0.5, b = 0.5;
		viewer->addCube(x_min, x_max, y_min, y_max, z_min, z_max, r, g, b, cube);
		viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, cube);
	}
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		std::this_thread::sleep_for(100ms);
	}
	return 0;
}

