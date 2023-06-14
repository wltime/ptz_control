ptz_control，目前包含两个话题，分别为/ptz_control与/ptz_absoluteControl

## 1.话题/ptz_control

- 作用：控制云台进行连续性的俯仰与旋转操作，主要用于手柄控制

- 消息类型为sensor/Joy，Joy中axes[0]控制云台俯仰，在Joy中范围为[-1,1]，负值代表**俯**，正值代表**仰**，axes[1]控制云台旋转，在Joy中范围为[-1,1]。其中，在俯仰绝对角度到达-90与270后，云台不会继续运动，此时需要减小仰角或者增大俯角。

## 2.话题/ptz_absoluteControl

- 作用：控制云台运动到绝对角度。

- 消息类型为std_msgs/Int32MultiArray，msg.data[0]控制俯仰，运动角度范围在[-90,270]，msg.data[1]控制旋转，运动角度范围为[0,360]。

  

