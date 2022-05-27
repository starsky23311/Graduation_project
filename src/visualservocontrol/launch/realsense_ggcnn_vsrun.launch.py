import os
import launch
from launch_ros.actions import Node
from launch_ros.actions import ComposableNodeContainer
from launch_ros.descriptions import ComposableNode
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    realsense_node = Node(
        package='realsense_node',
        executable='realsense_node',
        node_namespace='',
        output='screen'
        # parameter=[{'align_depth': True}]
    )
    ggcnn_node = Node(
        package='mycnn',
        executable='ggcnn_run',
        node_namespace='',
        output='screen'
    )
    vsrun_node = Node(
        package='visualservocontrol',
        executable='GGCNNBasedVisualServo',
        node_namespace='',
        output='screen'
    )
    return launch.LaunchDescription([realsense_node, ggcnn_node, vsrun_node])

