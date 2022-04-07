# Copyright 2016 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from std_msgs.msg import String
from cv_bridge import CvBridge
import sensor_msgs
# import torch
import torch.utils.data
# import librealsense2
import cv2
import numpy as np
from skimage.filters import gaussian
from models.common import post_process_output
from utils.dataset_processing import evaluation, grasp
import math
class GGCNNReady:

    def __init__(self):
        self.AlignOK = False
        self.ColorOK = False
        self.GgcnnOK = False

    def set_align_ok(self, setflag):
        self.AlignOK = setflag
        if self.ColorOK:
            self.GgcnnOK = True

    def set_color_ok(self, setflag):
        self.ColorOK = setflag
        if self.AlignOK:
            self.GgcnnOK = True

    def get_ggcnn_ok(self):
        return self.GgcnnOK

    def refresh_ok(self):
        self.AlignOK = False
        self.ColorOK = False
        self.GgcnnOK = False


picture_ready = GGCNNReady()


class GraspGenerater(Node):

    def __init__(self):
        super().__init__('GGCNNOutputNode')

        # 网络初始化
        self.net = torch.load("/home/wzf/main_folder/MyProject/Graduation_project/src/mycnn/mycnn/epoch_45_iou_0.76")
        self.device = torch.device("cuda:0")
        # self.color_image = np.zeros([480, 640, 3], np.uint8)
        # self.depth_image = np.zeros([480, 640], np.uint16)
        # self.color_after_crop = np.zeros([300, 300, 3], np.uint8)
        # self.ggcnn_input = np.zeros([300, 300], np.float32)
        self.color_image = np.zeros([480, 640, 3], np.uint8)
        self.depth_image = np.zeros([480, 640])
        self.color_after_crop = np.zeros([300, 300, 3], np.uint8)
        self.ggcnn_input = np.zeros([300, 300])


        self.publisher_ = self.create_publisher(String, 'GGCNNOutput', 10)
        self.subscription_color = self.create_subscription(
            sensor_msgs.msg.Image,
            "/camera/color/image_raw",
            self.color_callback,
            5)
        self.subscription_align = self.create_subscription(
            sensor_msgs.msg.Image,
            "/camera/aligned_depth_to_color/image_raw",
            self.align_callback,
            5)
    # output_size = 300,shape = [640, 480]
    def get_crop_attr(self, center, output_size, shape):

        # 自主检测目标大致位置模式
        if center == [-1, -1]:
            pass
        # 人为设定裁剪位置
        else:
            pass

        left = max(0, min(center[1] - output_size // 2, shape[0] - output_size))
        top = max(0, min(center[0] - output_size // 2, shape[1] - output_size))
        return center, left, top

    def delete_zero(self, img):
        dst = cv2.inpaint(img, (img==0).astype(np.uint8), 3, cv2.INPAINT_NS)
        return dst


    def picture_crop(self, img, left, top, type):
        output_size = 300
        img_output = np.zeros((300, 300))
        if type == "rgb":
            img_output = img[top:min(480, top + output_size), left:min(640, left + output_size)]
            img_output.reshape((output_size, output_size, 3))
        elif type == "depth":
            img_output = img[top:min(480, top + output_size), left:min(640, left + output_size)]
            img_output.reshape((output_size, output_size))
        elif type == "depth_norm":
            img = cv2.medianBlur(img, 5)
            # img = cv2.medianBlur(img, 5)
            # img = cv2.morphologyEx(
            #     img, cv2.MORPH_OPEN, cv2.getStructuringElement(cv2.MORPH_RECT, (7, 7)))

            img_output = img[top:min(480, top + output_size), left:min(640, left + output_size)].astype(np.float32)/1000
            img_output = self.delete_zero(img_output)
            img_output = np.clip((img_output - img_output.mean()), -1, 1)
            img_output.reshape((output_size, output_size))

        return img_output

    def get_ggcnn_input(self, center):
        _, left, top = self.get_crop_attr(center, 300, [640, 480])
        self.color_after_crop = self.picture_crop(self.color_image, left, top, "rgb")
        self.ggcnn_input = self.picture_crop(self.depth_image, left, top, "depth_norm")

    def numpy_to_torch(self, s):
        if len(s.shape) == 2:
            return torch.from_numpy(np.expand_dims(s, 0).astype(np.float32))
        else:
            return torch.from_numpy(s.astype(np.float32))

    def get_ggcnn_output(self, vis=True):
        with torch.no_grad():
            x = self.numpy_to_torch(self.ggcnn_input).to(self.device)
            pos_output, cos_output, sin_output, width_output = self.net(x)
            q_img, ang_img, width_img = post_process_output(pos_output, cos_output,
                                                            sin_output, width_output)
            # if vis:
            x, y, angle, width = evaluation.plot_output(self.color_after_crop,
                                       self.ggcnn_input, q_img,
                                       ang_img, 1, grasp_width_img=width_img)
            cv2.circle(self.color_after_crop, (x, y), 5, [255, 0, 0], 4)
            # m = math.sqrt(5) * width
            # self.color_after_crop = cv2.rectangle(self.color_after_crop, (math.ceil(x+0.5*m*math.cos(angle)), math.ceil(y-0.5*m*math.sin(angle))),
            #                                     (math.ceil(x-0.5*m*math.cos(angle)), math.ceil(y+0.5*m*math.sin(angle))), (0, 255, 0), 2)
            alpha = math.pi/2 - angle
            p = []
            p.append((math.ceil(x+width*math.cos(angle)-0.5*width*math.cos(alpha)), math.ceil(y-width*math.sin(angle)-0.5*width*math.sin(alpha))))
            p.append((math.ceil(x+width*math.cos(angle)+0.5*width*math.cos(alpha)), math.ceil(y-width*math.sin(angle)+0.5*width*math.sin(alpha))))
            p.append((math.ceil(x-width*math.cos(angle)+0.5*width*math.cos(alpha)), math.ceil(y+width*math.sin(angle)+0.5*width*math.sin(alpha))))
            p.append((math.ceil(x-width*math.cos(angle)-0.5*width*math.cos(alpha)), math.ceil(y+width*math.sin(angle)-0.5*width*math.sin(alpha))))
            for i in range(0, 3):
                cv2.line(self.color_after_crop, p[i], p[i+1], [0, 255, 0], 2)
            cv2.line(self.color_after_crop, p[3], p[0], [0, 255, 0], 2)

            self.color_after_crop = cv2.cvtColor(self.color_after_crop, cv2.COLOR_BGR2RGB)
            cv2.imshow("grasp", self.color_after_crop)
            cv2.imshow("depth", (self.ggcnn_input*128+128).astype(np.uint8))
            cv2.waitKey(1)
            self.GGCNNOutputPublish(x, y, angle, width)


    def GGCNNOutputPublish(self, x, y, angle, width):
        msg = String()
        msg.data = "["+str(x)+","+str(y)+","+str(angle)+","+str(width)+"]"
        self.publisher_.publish(msg)
        self.get_logger().info('Publishing: "%s"' % msg.data)

    def color_callback(self, msg):
        bridge = CvBridge()
        self.get_logger().info('I heard COLOR: "%s"' % msg.encoding)
        self.color_image = bridge.imgmsg_to_cv2(msg, "rgb8")
        picture_ready.set_color_ok(True)

    def align_callback(self, msg):
        bridge = CvBridge()
        self.get_logger().info('I heard ALIGN: "%s"' % msg.data[500])
        self.depth_image = bridge.imgmsg_to_cv2(msg, desired_encoding="16UC1")
        picture_ready.set_align_ok(True)
        # cv_bridge.getCvType()


def main(args=None):
    rclpy.init(args=args)


    GGCNNTool = GraspGenerater()
    colorimg_name = "color"
    # alignimg_name = "align"

    while rclpy.ok():
        rclpy.spin_once(GGCNNTool)
        if picture_ready.get_ggcnn_ok():
            picture_ready.refresh_ok()
            GGCNNTool.get_ggcnn_input([200, 300])
            GGCNNTool.get_ggcnn_output()

            # GGCNNTool.GGCNNOutputPublish()
            # GGCNNTool.color_image = cv2.cvtColor(GGCNNTool.color_image, cv2.COLOR_BGR2RGB)
            # cv2.imshow(colorimg_name, GGCNNTool.color_image)
            # cv2.waitKey(1)
        # rclpy.spin(minimal_publisher)
        # a = torch.rand(3, 3)
        # print(a)
        # Destroy the node explicitly
        # (optional - otherwise it will be done automatically
        # when the garbage collector destroys the node object)
    GGCNNTool.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
