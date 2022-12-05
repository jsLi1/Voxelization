#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/console/parse.h>
#include <pcl/visualization/cloud_viewer.h>
#include<string>
#include <thread>

using namespace std;

int main(int argc, char** argv)
{

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	// 装载点云数据
	pcl::io::loadPCDFile("C:\\Users\\JS-L\\Desktop\\rabbit.pcd", *cloud);

	// 创建滤波器对象
	pcl::VoxelGrid<pcl::PointXYZ> sor;//滤波器处理对象
	sor.setInputCloud(cloud);//设置输入点云
	sor.setLeafSize(3, 3, 3);//设置滤波器处理时采用的体素大小的参数，体素大小是长宽高均为0.01
	sor.filter(*cloud_filtered);//执行下采样，下采样之后的点云数据保存到 cloud_filtered 中
								//std::cerr << "PointCloud after filtering: " << cloud_filtered->width * cloud_filtered->height
								//	<< " data points (" << pcl::getFieldsList(*cloud_filtered) << ").";
								////保存转换的输入点云
								//pcl::io::savePCDFileASCII("C:\\Users\\JS-L\\Desktop\\测试\\benthi_control_A_D30_centre_filter1.pcd", *cloud_filtered);
	int voxelSize = 3;
	//rendering by z axis
	pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("Voxelization"));
	viewer->setBackgroundColor(255, 255, 255);
	pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ> v_color(cloud_filtered, "z");
	viewer->addPointCloud<pcl::PointXYZ>(cloud_filtered, v_color, "vertices color");

	for (int i = 0; i<cloud_filtered->size(); i++) {

		double x = cloud_filtered->points[i].x;
		double y = cloud_filtered->points[i].y;
		double z = cloud_filtered->points[i].z;

		Eigen::Vector3f center(floor(x / voxelSize)*voxelSize + voxelSize / 2, floor(y / voxelSize)*voxelSize + voxelSize / 2, floor(z / voxelSize)*voxelSize + voxelSize / 2);

		Eigen::Quaternionf rotation(1, 0, 0, 0);
		string cube = "cube" + to_string(i);
		viewer->addCube(center, rotation, voxelSize, voxelSize, voxelSize, cube);

		//shape rendering
		viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, cube);
	}


	//pcl::visualization::CloudViewer viewer("Cloud Viewer");

	//viewer->showCloud();//showCloud函数是同步的，会在此处等待直到渲染显示为止

	while (!viewer->wasStopped())
	{
		//在此处添加其他处理
		viewer->spinOnce(100);
		this_thread::sleep_for(100ms);

	}
	system("pause");
	return (0);
}
