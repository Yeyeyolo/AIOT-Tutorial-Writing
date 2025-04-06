import pandas as pd

# 读取原始txt文件
with open('stm32数据 1.txt', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# 设置表头
header = 'index,temperature,humidity,adcx\n'

# 处理数据行，跳过原始表头
data_lines = []
for line in lines[1:]:  # 跳过第一行表头
    if '；' in line:  # 跳过包含中文分号的行
        continue
    processed_line = line.strip().replace(';', ',') + '\n'
    data_lines.append(processed_line)

# 创建新的csv文件并写入数据
with open('stm32_data.csv', 'w', encoding='utf-8') as f:
    # 写入表头
    f.write(header)
    # 写入数据行
    f.writelines(data_lines)

# 验证数据是否正确读取
df = pd.read_csv('stm32_data.csv')
print("数据已成功转换为CSV格式！")
print("\n数据预览：")
print(df.head())
print("\n数据信息：")
print(df.info()) 