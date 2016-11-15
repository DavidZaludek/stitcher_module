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



        <parameter name="fps_stats">@stats</parameter>
    </node>
    <node class="v4l2source" name="cam4">
        <parameter name="path">@dev4</parameter>
        <parameter name="resolution">@res</parameter>
        <parameter name="fps">@fps</parameter>
        <parameter name="format">@format</parameter>
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <!-- Decode to raw -->
    <node class="jpeg_decoder" name="dec0">
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <node class="jpeg_decoder" name="dec1">
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <node class="jpeg_decoder" name="dec2">
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <node class="jpeg_decoder" name="dec3">
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <node class="jpeg_decoder" name="dec4">
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <!-- Crop to correct size -->
    <node class="crop" name="crop0">
        <parameter name="fps_stats">@stats</parameter>
        <parameter name="geometry">@crop</parameter>
    </node>
    <node class="crop" name="crop1">
        <parameter name="fps_stats">@stats</parameter>
        <parameter name="geometry">@crop</parameter>
    </node>
    <node class="crop" name="crop2">
        <parameter name="fps_stats">@stats</parameter>
        <parameter name="geometry">@crop</parameter>
    </node>
    <node class="crop" name="crop3">
        <parameter name="fps_stats">@stats</parameter>
        <parameter name="geometry">@crop</parameter>
    </node>
    <node class="crop" name="crop4">
        <parameter name="fps_stats">@stats</parameter>
        <parameter name="geometry">@crop</parameter>
    </node>
    <!-- Merge images together -->
    <node class="stitcher_module" name="comb0">
        <parameter name="fps_stats">@stats</parameter>
        <parameter name="GPU">0</parameter>
        <parameter name="cam_count">4</parameter>
        <parameter name="Blender">1</parameter>
        <parameter name="Exposure_compensate">0</parameter>
    </node>
    <!-- Prepare for web -->
    <node class="jpeg_encoder" name="enc0">
        <parameter name="quality">@quality</parameter>
        <parameter name="fps_stats">@stats</parameter>
    </node>
    <!-- Web server modules -->
    <node class="web_image" name="img0">
        <parameter name="path">/stream.jpg</parameter>
    </node>
    <node class="webserver" name="webserver">
        <parameter name="address">@local_ip</parameter>
        <parameter name="port">@port</parameter>
    </node>
    <node class="web_control" name="control">
        <parameter name="path">/control</parameter>
        <parameter name="redirect">/</parameter>
    </node>
    <node class="web_directory" name="html">
        <parameter name="dir">@dir</parameter>
        <parameter name="path">/</parameter>
    </node>
    <!-- Link specifications -->
    <link name="cam_dec0" class="single" source="cam0:0" target="dec0:0" />
    <link name="cam_dec1" class="single" source="cam1:0" target="dec1:0" />
    <link name="cam_dec2" class="single" source="cam2:0" target="dec2:0" />
    <link name="cam_dec3" class="single" source="cam3:0" target="dec3:0" />
    <link name="cam_dec4" class="single" source="cam4:0" target="dec4:0" />
    <link name="dec_crop0" class="single" source="dec0:0" target="crop0:0" />
    <link name="dec_crop1" class="single" source="dec1:0" target="crop1:0" />
    <link name="dec_crop2" class="single" source="dec2:0" target="crop2:0" />
    <link name="dec_crop3" class="single" source="dec3:0" target="crop3:0" />
    <link name="dec_crop4" class="single" source="dec4:0" target="crop4:0" />
    <link name="crop_stitch0" class="single" source="crop0:0" target="comb0:0" />
    <link name="crop_stitch1" class="single" source="crop1:0" target="comb0:1" />
    <link name="crop_stitch2" class="single" source="crop2:0" target="comb0:2" />
    <link name="crop_stitch3" class="single" source="crop3:0" target="comb0:3" />
    <link name="crop_stitch4" class="single" source="crop4:0" target="comb0:4" />
    <link name="stitch_enc0" class="single" source="comb0:0" target="enc0:0" />
    <link name="enc_img0" class="single" source="enc0:0" target="img0:0" />
</app>
