import numpy as np
# import open3d as o3d
import math
import time
import sys
import os
import numba
from numba import jit
#######################李修改过，不用open3d

@jit()
def distance_point3d(p0, p1):
    d = (p0[0] - p1[0])**2 + (p0[1] - p1[1])**2 + (p0[2] - p0[2])**2
    return math.sqrt(d)

@jit()
def furthest_point_sample(points, sample_count):
    points_index = np.arange(points.shape[0], dtype=np.int)
    A = np.array([np.random.choice(points_index)])
    B = np.setdiff1d(points_index, A) #返回两个数组中在第一个数组，不在第二个数组部分
    #print(A)
    #print(B)
    min_dis_B2A = []
    for i in range(len(B)):
        Pa_index = A[0]
        Pb_index = B[i]
        Pa = points[Pa_index]
        Pb = points[Pb_index]
        dis = distance_point3d(Pb, Pa)
        min_dis_B2A.append(dis)
    min_dis_B2A = np.array(min_dis_B2A)
    #print('iter ', len(A), ': ', A)
    while len(A) < sample_count:
        longest_points_in_B_index = np.argmax(min_dis_B2A)
        longest_points_index = B[longest_points_in_B_index]

        # update A and B
        A = np.append(A, longest_points_index)
        B = np.delete(B, longest_points_in_B_index)
        min_dis_B2A = np.delete(min_dis_B2A, longest_points_in_B_index)

        # update min_dis_B2A
        for i in range(len(B)):
            Pa_index = A[-1]
            Pb_index = B[i]
            Pa = points[Pa_index]
            Pb = points[Pb_index]
            dis = distance_point3d(Pb, Pa)
            min_dis_B2A[i] = min(dis, min_dis_B2A[i])
        
        #print('iter ', len(A), ': ', A)
    return A

# class RenderClass(object):
#     def __init__(self, pcd, points, sampled_index):
#         self.pcd = pcd
#         self.points = points
#         self.sampled_index = sampled_index
#         self.current_render_index = 0
    
#     def vis_callback(self, vis):
#         if self.current_render_index < len(self.sampled_index):
#             mesh_sphere = o3d.geometry.TriangleMesh.create_sphere(radius=0.005)

#             mesh_sphere.paint_uniform_color([0.8, 0.1, 0.2])
#             mesh_sphere.translate(self.points[self.sampled_index[self.current_render_index]])
#             self.current_render_index = self.current_render_index + 1
#             time.sleep(0.1)
#             vis.add_geometry(mesh_sphere, False)
#         else:
#             vis.clear_geometries()
#             vis.add_geometry(self.pcd, False)
#             self.current_render_index = 0

def get_filelist(path):
        Filelist = []
        for home, dirs, files in os.walk(path):
            for filename in files:
                Filelist.append(os.path.join(home, filename))
        return Filelist

if __name__=='__main__':
    path = r'C:\Users\JS-L\Desktop\重新制作的十倍数据集-sgl采样\测试集' #注意，此处路径不要有中文
    Filelist = get_filelist(path)
    for file in Filelist:
        # ply = o3d.io.read_point_cloud(file)
        # points = np.loadtxt(file,dtype=float,delimiter=',')
        points=np.loadtxt(file)
        #points=data_f[10:]
        pcd_array = np.asarray(points)
        print(file,pcd_array.shape)
        sample_count = 4096#固定FPS降采样后的点数
        ratio=0.1
        center_points_index = furthest_point_sample(pcd_array[0:4096], sample_count-int(sample_count*ratio))
        edg_points_index = furthest_point_sample(pcd_array[4096:8192], int(sample_count*ratio))
        sampled_points_index=np.concatenate((center_points_index,edg_points_index),axis=0)
        sample_point=np.zeros((4096,5))
        for i in range(4096):
            sample_point[i]=pcd_array[sampled_points_index[i]]
        # np.savetxt("id//"+file[0:len(file)- 3]+"txt",sampled_points_index)
        # np.savetxt(file[0:len(file)- 3]+"txt",sampled_points_index)
        np.savetxt('C:\\Users\\JS-L\\Desktop\\重新制作的十倍数据集-sgl采样\\测试集十倍\\'+file[43:len(file)- 4]+"J."+"txt",sample_point,fmt="%f %f %f %d %d")#J仅仅是使保存的文件不重名而已