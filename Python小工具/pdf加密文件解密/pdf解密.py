import os
import pikepdf

os.chdir(r"E:\BaiduNetdiskDownload")
pdf = pikepdf.open(r'.\深入理解计算机系统 第3版\深入理解计算机系统 第3版.pdf')
pdf.save(r'./深入理解计算机系统 第3版.pdf')