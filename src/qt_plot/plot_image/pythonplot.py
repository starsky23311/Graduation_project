############################################ 条形图绘制（超调为例）############################################
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

############################################ 折线图绘制 ############################################
# import numpy as np
# import matplotlib.pyplot as plt
# x1=[20,33,51,79,101,121,132,145,162,182,203,219,232,243,256,270,287,310,325]
# y1=[49,48,48,48,48,87,106,123,155,191,233,261,278,284,297,307,341,319,341]
# x2=[31,52,73,92,101,112,126,140,153,175,186,196,215,230,240,270,288,300]
# y2=[48,48,48,48,49,89,162,237,302,378,443,472,522,597,628,661,690,702]
# x3=[30,50,70,90,105,114,128,137,147,159,170,180,190,200,210,230,243,259,284,297,311]
# y3=[48,48,48,48,66,173,351,472,586,712,804,899,994,1094,1198,1360,1458,1578,1734,1797,1892]
# x=np.arange(20,350)
# l1=plt.plot(x1,y1,'r--',label='type1')
# l2=plt.plot(x2,y2,'g--',label='type2')
# l3=plt.plot(x3,y3,'b--',label='type3')
# plt.plot(x1,y1,'ro-',x2,y2,'g+-',x3,y3,'b^-')
# plt.title('The Lasers in Three Conditions')
# plt.xlabel('row')
# plt.ylabel('column')
# plt.legend()
# plt.show()

############################################ 曲线图绘制 ############################################
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
key = []
value = []
i = int(0)
with open('/home/wzf/main_folder/MyProject/Graduation_project/src/qt_plot/plot_image/数据.txt') as f:
    for line in f.readlines():
        temp = line.split()
        if i % 2 == 0:
            key.append(np.float_(temp))
        elif i % 2 == 1:
            value.append(np.float_(temp))
        i = i + 1
#关闭文件
f.close()
# print(len(value))
# 设置刻度字体大小
plt.xticks(fontsize=13)
plt.yticks(fontsize=13)
# # 设置坐标标签字体大小
ax = plt.subplot(111)
ax.set_xlabel("iteration times", fontsize=15)
ax.set_ylabel("success rate", fontsize=15)
plt.plot(key[0], value[0], color='green', label='training accuracy')
plt.plot(key[0], value[0]+0.001, color='blue', label='threshold')
plt.legend() # 显示图例
plt.show()
