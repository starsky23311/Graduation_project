############################################ 条形图绘制（超调为例8）############################################
# import matplotlib.pyplot as plt
# import matplotlib
# # 设置中文字体和负号正常显示
# # matplotlib.rcParams['font.sans-serif'] = ['SimHei']
# # matplotlib.rcParams['axes.unicode_minus'] = False
#
# label_list = ['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8']    # 横坐标刻度显示值
# num_list1 = [0.20, 0.30, 0.15, 0.35, 0.20, 0.30, 0.15, 0.35]      # 纵坐标值1
# x = range(len(num_list1))
# # 字体调整
# ax = plt.subplot(111)
# # 设置刻度字体大小
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# # # 设置坐标标签字体大小
# ax.set_xlabel("position", fontsize=15)
# ax.set_ylabel("overshoot", fontsize=15)
# # # 设置图例字体大小
# # ax.legend(..., fontsize=20)
#
# """
# 绘制条形图
# left:长条形中点横坐标
# height:长条形高度
# width:长条形宽度，默认值0.8
# label:为后面设置legend准备
# """
# rects1 = plt.bar(x=x, height=num_list1, width=0.6, alpha=0.8, color='red', label="1")
# plt.ylim(0, 1)     # y轴取值范围
# plt.ylabel("overshoot")
# """
# 设置x轴刻度显示值
# 参数一：中点坐标
# 参数二：显示值
# """
# # plt.xticks([index + 0.2 for index in x], label_list)
# plt.xticks([index for index in x], label_list)
# # plt.xlabel("position")
# # plt.title("4")
# # plt.legend()     # 设置题注
# # 编辑文本
# # for rect in rects1:
# #     height = rect.get_height()
# #     plt.text(rect.get_x() + rect.get_width() / 2, height+1, str(height), ha="center", va="bottom")
# for rect in rects1:
#     height = rect.get_height()
#     plt.text(rect.get_x() + rect.get_width() / 2, height+0.02, str(height), ha="center", va="bottom", size=13)
#
# plt.show()
# ############################################ 条形图绘制（超调为例4）############################################
# import matplotlib.pyplot as plt
# import numpy as np
# import matplotlib
# # 设置中文字体和负号正常显示
# # matplotlib.rcParams['font.sans-serif'] = ['SimHei']
# # matplotlib.rcParams['axes.unicode_minus'] = False
#
# label_list = ['P1', 'P2', 'P3', 'P4', 'P5', 'P6', 'P7', 'P8']    # 横坐标刻度显示值
# num_list1 = [0.02, 0.013, 0.02, 0.015, 0.018, 0.018, 0.02, 0.018]     # 纵坐标值1
# num_list2 = [0.065, 0.17, 0.05, 0.11, 0.115, 0.125, 0.10, 0.15]      # 纵坐标值2
# x = np.arange(len(num_list1))
# # 字体调整
# ax = plt.subplot(111)
# # 设置刻度字体大小
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# # # 设置坐标标签字体大小
# ax.set_xlabel("Position", fontsize=15)
# ax.set_ylabel("Data", fontsize=15)
# # # 设置图例字体大小
# # ax.legend(..., fontsize=20)
#
# """
# 绘制条形图
# left:长条形中点横坐标
# height:长条形高度
# width:长条形宽度，默认值0.8
# label:为后面设置legend准备
# """
# # rects1 = plt.bar(x=x-0.15, height=num_list1, width=0.3, alpha=0.8, color='red', label=chr(916)+"overshoot(m)")
# # rects2 = plt.bar(x=x+0.15, height=num_list2, width=0.3, alpha=0.8, color='blue', label=chr(916)+"time*0.01(s)")
# rects1 = plt.bar(x=x-0.15, height=num_list1, width=0.3, alpha=0.8, color='red', label="overshoot(m)")
# rects2 = plt.bar(x=x+0.15, height=num_list2, width=0.3, alpha=0.8, color='blue', label="time*0.01(s)")
# plt.ylim(0, 0.2)     # y轴取值范围
# # plt.ylabel("Overshoot(m)")
# """
# 设置x轴刻度显示值
# 参数一：中点坐标
# 参数二：显示值
# """
# # plt.xticks([index + 0.2 for index in x], label_list)
# plt.xticks([index for index in x], label_list)
# # plt.xlabel("position")
# # plt.title("4")
# plt.legend()     # 设置题注
# # 编辑文本
# # for rect in rects1:
# #     height = rect.get_height()
# #     plt.text(rect.get_x() + rect.get_width() / 2, height+1, str(height), ha="center", va="bottom")
# for rect in rects1:
#     height = rect.get_height()
#     plt.text(rect.get_x() + rect.get_width() / 2, height+0.005, str(height), ha="center", va="bottom", size=7)
# for rect in rects2:
#     height = rect.get_height()
#     plt.text(rect.get_x() + rect.get_width() / 2, height+0.005, str(height), ha="center", va="bottom", size=7)
# plt.show()
# ############################################ 条形图绘制（超调为例5）############################################
# import matplotlib.pyplot as plt
# import numpy as np
# import matplotlib
# # 设置中文字体和负号正常显示
# # matplotlib.rcParams['font.sans-serif'] = ['SimHei']
# # matplotlib.rcParams['axes.unicode_minus'] = False
#
# label_list = ['O1', 'O2', 'O3', 'O4', 'O5', 'O6']    # 横坐标刻度显示值
# num_list1 = [0.02, 0, 0, 0.01, 0.018, 0.018]     # 纵坐标值1
# num_list2 = [0.05, 0.175, 0.125, 0.11, 0.10, 0.125]      # 纵坐标值2
# x = np.arange(len(num_list1))
# # 字体调整
# ax = plt.subplot(111)
# # 设置刻度字体大小
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# # # 设置坐标标签字体大小
# ax.set_xlabel("Object", fontsize=15)
# ax.set_ylabel("Data", fontsize=15)
# # # 设置图例字体大小
# # ax.legend(..., fontsize=20)
#
# """
# 绘制条形图
# left:长条形中点横坐标
# height:长条形高度
# width:长条形宽度，默认值0.8
# label:为后面设置legend准备
# """
# # rects1 = plt.bar(x=x-0.15, height=num_list1, width=0.3, alpha=0.8, color='red', label=chr(916)+"overshoot(m)")
# # rects2 = plt.bar(x=x+0.15, height=num_list2, width=0.3, alpha=0.8, color='blue', label=chr(916)+"time*0.01(s)")
# rects1 = plt.bar(x=x-0.15, height=num_list1, width=0.3, alpha=0.8, color='red', label="overshoot(m)")
# rects2 = plt.bar(x=x+0.15, height=num_list2, width=0.3, alpha=0.8, color='blue', label="time*0.01(s)")
# plt.ylim(0, 0.2)     # y轴取值范围
# # plt.ylabel("Overshoot(m)")
# """
# 设置x轴刻度显示值
# 参数一：中点坐标
# 参数二：显示值
# """
# # plt.xticks([index + 0.2 for index in x], label_list)
# plt.xticks([index for index in x], label_list)
# # plt.xlabel("position")
# # plt.title("4")
# plt.legend()     # 设置题注
# # 编辑文本
# # for rect in rects1:
# #     height = rect.get_height()
# #     plt.text(rect.get_x() + rect.get_width() / 2, height+1, str(height), ha="center", va="bottom")
# for rect in rects1:
#     height = rect.get_height()
#     plt.text(rect.get_x() + rect.get_width() / 2, height+0.005, str(height), ha="center", va="bottom", size=7)
# for rect in rects2:
#     height = rect.get_height()
#     plt.text(rect.get_x() + rect.get_width() / 2, height+0.005, str(height), ha="center", va="bottom", size=7)
# plt.show()
############################################ 条形图绘制（调节时间4）############################################
# import matplotlib.pyplot as plt
# import matplotlib
# # 设置中文字体和负号正常显示
# # matplotlib.rcParams['font.sans-serif'] = ['SimHei']
# # matplotlib.rcParams['axes.unicode_minus'] = False
#
# label_list = ['PD', 'Add Slip', 'Add Adaptive', 'Add Consistence']    # 横坐标刻度显示值
# num_list1 = [14, 9.2, 7.9, 7.75]      # 纵坐标值1
# x = range(len(num_list1))
# # 字体调整
# ax = plt.subplot(111)
# # 设置刻度字体大小
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# # # 设置坐标标签字体大小
# ax.set_xlabel("Method", fontsize=15)
# ax.set_ylabel("Adjustment Time(s)", fontsize=15)
# # # 设置图例字体大小
# # ax.legend(..., fontsize=20)
#
# """
# 绘制条形图
# left:长条形中点横坐标
# height:长条形高度
# width:长条形宽度，默认值0.8
# label:为后面设置legend准备
# """
# rects1 = plt.bar(x=x, height=num_list1, width=0.4, alpha=0.8, color='blue', label="1")
# plt.ylim(0, 17)     # y轴取值范围
# # plt.ylabel("overshoot")
# """
# 设置x轴刻度显示值
# 参数一：中点坐标
# 参数二：显示值
# """
# # plt.xticks([index + 0.2 for index in x], label_list)
# plt.xticks([index for index in x], label_list)
# # plt.xlabel("position")
# # plt.title("4")
# # plt.legend()     # 设置题注
# # 编辑文本
# # for rect in rects1:
# #     height = rect.get_height()
# #     plt.text(rect.get_x() + rect.get_width() / 2, height+1, str(height), ha="center", va="bottom")
# for rect in rects1:
#     height = rect.get_height()
#     plt.text(rect.get_x() + rect.get_width() / 2, height+0.2, str(height), ha="center", va="bottom", size=13)
#
# plt.show()
############################################ 折线图绘制 ############################################
# import numpy as np
# import matplotlib.pyplot as plt
# x1 = range(1, 41)
# y1 = [0.00, 0.35, 0.40, 0.63, 0.65, 0.68, 0.69, 0.71, 0.72, 0.71, 0.71, 0.72, 0.73, 0.69, 0.71, 0.71, 0.72, 0.73, 0.73, 0.73,
#       0.73, 0.70, 0.74, 0.73, 0.73, 0.73, 0.71, 0.72, 0.73, 0.75, 0.75, 0.74, 0.75, 0.76, 0.76, 0.77, 0.70, 0.78, 0.79, 0.80 ]
# x2 = range(1, 41)
# y2 = [0.00, 0.12, 0.39, 0.41, 0.49, 0.57, 0.51, 0.57, 0.51, 0.59, 0.55, 0.56, 0.57, 0.57, 0.59, 0.67, 0.66, 0.67, 0.66, 0.68,
#       0.69, 0.75, 0.70, 0.70, 0.71, 0.73, 0.71, 0.59, 0.63, 0.65, 0.71, 0.73, 0.75, 0.73, 0.73, 0.72, 0.74, 0.73, 0.75, 0.76]
# # x3=[30,50,70,90,105,114,128,137,147,159,170,180,190,200,210,230,243,259,284,297,311]
# # y3=[48,48,48,48,66,173,351,472,586,712,804,899,994,1094,1198,1360,1458,1578,1734,1797,1892]
# x=np.arange(20,350)
# l1=plt.plot(x1, y1, 'r--',label='optimized frame')
# l2=plt.plot(x2, y2, 'b--',label='original frame')
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# plt.plot(x1, y1, 'ro-', x2, y2, 'b+-')
# ax = plt.subplot(111)
# ax.set_xlabel("iteration times", fontsize=15)
# ax.set_ylabel("success rate", fontsize=15)
# # plt.title('The Lasers in Three Conditions')
# # plt.xlabel('training accuracy')
# # plt.ylabel('column')
# plt.legend()
# plt.show()

############################################ 曲线图绘制 ############################################
# import numpy as np
# import matplotlib as mpl
# import matplotlib.pyplot as plt
# key = []
# value = []
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/系统稳定性测试/0-0.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key.append(np.float_(temp))
#         elif i % 2 == 1:
#             value.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# # print(len(value))
# # 设置刻度字体大小
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# # # 设置坐标标签字体大小
# ax = plt.subplot(111)
# ax.set_xlabel("iteration times", fontsize=15)
# ax.set_ylabel("success rate", fontsize=15)
# plt.plot(key[0], value[0], color='green', label='training accuracy')
# plt.plot(key[0], value[0]+0.001, color='blue', label='threshold')
# plt.legend() # 显示图例
# plt.show()

# ############################################ 曲线图绘制（系统稳定性测试） ############################################
# import numpy as np
# import matplotlib as mpl
# import matplotlib.pyplot as plt
# key0 = []
# value0 = []
# key1 = []
# value1 = []
# key2 = []
# value2 = []
# key3 = []
# value3 = []
# key4 = []
# value4 = []
# key5 = []
# value5 = []
# key6 = []
# value6 = []
# key7 = []
# value7 = []
# key8 = []
# value8 = []
# key9 = []
# value9 = []
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/11/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key0.append(np.float_(temp))
#         elif i % 2 == 1:
#             value0.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/12/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key1.append(np.float_(temp))
#         elif i % 2 == 1:
#             value1.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/13/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key2.append(np.float_(temp))
#         elif i % 2 == 1:
#             value2.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/14/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key3.append(np.float_(temp))
#         elif i % 2 == 1:
#             value3.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/15/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key4.append(np.float_(temp))
#         elif i % 2 == 1:
#             value4.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/16/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key5.append(np.float_(temp))
#         elif i % 2 == 1:
#             value5.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/7/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key6.append(np.float_(temp))
#         elif i % 2 == 1:
#             value6.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/8/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key7.append(np.float_(temp))
#         elif i % 2 == 1:
#             value7.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/边界鲁棒性测试/8.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key8.append(np.float_(temp))
#         elif i % 2 == 1:
#             value8.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# i = int(0)
# with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/伺服性能/3/数据.txt') as f:
#     for line in f.readlines():
#         temp = line.split()
#         if i % 2 == 0:
#             key9.append(np.float_(temp))
#         elif i % 2 == 1:
#             value9.append(np.float_(temp))
#         i = i + 1
# #关闭文件
# f.close()
# # print(len(value))
# # 设置刻度字体大小
# plt.xticks(fontsize=13)
# plt.yticks(fontsize=13)
# # # 设置坐标标签字体大小
# ax = plt.subplot(111)
# ax.set_xlabel("Time(s)", fontsize=15)
# ax.set_ylabel("Error2(m2)", fontsize=15)
# plt.plot(key0[0], value0[0], color='green', label='O1')
# plt.plot(key1[0], value1[0], color='blue', label='O2')
# plt.plot(key2[0], value2[0], color='red', label='O3')
# plt.plot(key3[0], value3[0], color='brown', label='O4')
# plt.plot(key4[0], value4[0], color='black', label='O5')
# plt.plot(key5[0], value5[0], color='yellow', label='O6')
# # plt.plot(key6[0], value6[0], color='pink', label='P7')
# # plt.plot(key7[0], value7[0], color='purple', label='P8')
# # plt.plot(key8[0], value8[0]*2.7, color='green', label='P')
# # plt.plot(key9[0], value9[0], color='blue', label='90 degree')
#
#
#
#
#
# plt.legend() # 显示图例
# plt.show()


import matplotlib.pyplot as plt
import numpy as np

p1=[5.44, 5.4, 5.4, 5.43, 5.4, 5.3, 5.32, 5.35, 5.3, 5.4, 5.44, 5.36, 5.34, 5.33, 5.32, 5.42]  # 数据点
p2=[3.32, 3.33, 3.43, 2.98, 3.21, 3.34, 3.32, 3.23, 3.1, 3.0, 3.05, 3.10, 3.05, 3.10, 3.22, 3.05]

#创建绘图图表对象，可以不显式创建，跟cv2中的cv2.namedWindow()用法差不多
plt.figure('Draw')
ax = plt.subplot(111)
ax.set_xlabel("X(mm)", fontsize=15)
ax.set_ylabel("Y(mm)", fontsize=15)
plt.scatter(p1, p2)  # scatter绘制散点图

plt.draw()  # 显示绘图
plt.show()
# plt.pause(10)  #显示10秒

# plt.savefig("easyplot.jpg")  #保存图象

# plt.close()   #关闭图表




