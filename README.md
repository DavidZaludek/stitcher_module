# stitcher_module

**Introduction:** 

Stitcher module implemets OpenCV 3.0.0 stitching with catching of some precomputed values into yuri2 (https://projects.iim.cz/yuri) .


**Use-case:** 

Stitcher takes in images in jpeg format and outputs one composed by stitching inputs together. Module is aplicable only to cameras which dont change relative position to eachother other-wise image becames distorted, this is because module only calculates positioning of images at the start of the module. Best results of stitching can be achieved on lanscapes with cameras focused to distance.

**Install:** 

Copy stitcher_module form Modules/ to yuri modules add stitcher_module to CMakeLists.txt in yuri modules.

**Run flags:**  

- **GPU**  - Module can speed up the calculations by using gpu acceleration. this flag toggels GPU use ON/OFF.
- **Warpers**  - OpenCV stitching has 3 Warper settings (0 - CylindricalWarper / 1 - PlaneWarper / 2 - SphericalWarper).
- **cam_count**  - Number of cameras used.
- **Blender**  - Use blender to smoothen the edges(1 - true/0 - false).
- **Exposure_compensate**  - Use exposure compensator to set images exposure(1 - true/0 - false).

**Current Framerates after procesing times:** 

- **5x1600x900**  -> 5.5 fps (without GPU,with blending,with exposure compensate)
- **5x1600x900**  -> 1.8 fps (with GPU slower probably because OpenCV is not optimized for such usage.)

>It is possible to achieve better performance by disabling blending and exposure. Exposure compensate can be disabled when u set it manaualy in camera options, for that we used set_cam.sh which is included in configs.


**Usage:** 
- yuri2 5cams_stitch_to_web.xml 

>xml found in Configs. This outputs current frame to localIp:port/stream.jpg. To view it u can use Javavisualizer which is simple java application which downloads the image and outputs it to JFrame.
