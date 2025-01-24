## Kimbap making using robot arm based AI Vision

This work is the final of the first step that is dealing with controlling the robot arm moving based on the AI vision.

The AI Vision is used the pre-built model of YOLOV3 to train a custom dataset (kimbap images dataset). 

The task is detecting the objects within the depth information of different layer from total 5 layers.

When camera is detected with it depth information, the robot will operate on that specific tasks location of the object.

| **:memo: NOTE** |
|-----------------|

The changing or shifting between layer is manually push and pull by people (for this first stage). Later on, we are going to attached with automatically push and pull by robot itself since now the gap designed between robot and tray location are too closed which is not be able to let robot handle on this process. 
