# stitcher_module

Introduction: 

Stitcher module implemets OpenCV 3.0.0 stitching with catching of some precomputed values into yuri2.

Use-case: 

Stitcher takes in images in jpeg format and outputs one composed by stitching inputs together. Module is aplicable only to cameras which dont change relative position to eachother other-wise image becames distorted, this is because module only calculates positioning of images at the start of the module. Best results of stitching can be achieved on lanscapes with cameras focused to distance.

Run flags: 

GPU - Module can speed up the calculations by using gpu acceleration. this flag toggels GPU use ON/OFF.

Warpers - OpenCV stitching has 3 Warper settings (0 - CylindricalWarper / 1 - PlaneWarper / 2 - SphericalWarper).

cam_count - Number of cameras used.

Blender - Use blender to smoothen the edges(1 - true/0 - false).

Exposure_compensate - Use exposure compensator to set images exposure(1 - true/0 - false).

Current Framerates after procesing times: 

5x1600x900 -> 4 fps

Usage 

yuri2 5cams_stitch_to_web.xml (xml found in Configs)
