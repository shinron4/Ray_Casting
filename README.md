# Ray_Casting

This Program is a C based Implementation of ray Casting algorithm for rendering 3D objects on the 2D screen.
In the progmram 3D object is supposed to be made of made cubicle voxels.

Compilation:

cc colorConversion.c intersect.c raycast.c -o raycast

Running:

After Compiling the program type in the terminal:

./raycast voxelCenterFile "RGBCOLOR"
 
1. Replace voxelCenterFile with the voxel center file.
2. Replace RGBCOLOR with for e.g. #ff0000

Format of the voxelCenter file is as follows:

1. first line gives n, where n is the number of the voxel center's in the file then n line follows.

2. Each of the subsequent n lines contains 3 space seprated floats x y z, where (x,y,z) is the center of each  voxel. 

2 Sample .cen files are given in the repository.

Input: 

1. Integer d, where d is the maximum depth of the octree.
2. float a, where a is the distance of viewer fro the screen.

second line is the
Output:

Ouput is a .ppm file named as output.ppm.
