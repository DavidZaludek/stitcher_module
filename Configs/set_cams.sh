#!/bin/bash

EXP=$1

for CAM in {0..4}; do
	v4l2-ctl -d /dev/video$CAM -c exposure_auto=1
	v4l2-ctl -d /dev/video$CAM -c exposure_absolute=$EXP
	v4l2-ctl -d /dev/video$CAM -c exposure_auto_priority=1
done
