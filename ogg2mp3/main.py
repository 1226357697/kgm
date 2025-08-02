import os
from pydub import AudioSegment

def convert_ogg_to_mp3(directory):
    # 遍历指定目录
    for filename in os.listdir(directory):
        # 获取文件的完整路径
        file_path = os.path.join(directory, filename)

        # 判断是否是 .ogg 文件
        if file_path.endswith(".ogg"):
            try:
                # 加载 ogg 文件
                audio = AudioSegment.from_ogg(file_path)
                
                # 使用 os.path.splitext() 获取文件名和扩展名
                base_name, _ = os.path.splitext(filename)
                
                # 生成 mp3 文件的路径，保留文件的基础名字
                mp3_file = os.path.join(directory, base_name + ".mp3")

                # 转换并保存为 mp3 文件
                audio.export(mp3_file, format="mp3")
                print(f"转换成功: {file_path} -> {mp3_file}")
            except Exception as e:
                print(f"转换失败: {file_path} 错误: {e}")

# 输入目录路径
directory = input("请输入目录路径: ")
convert_ogg_to_mp3(directory)
