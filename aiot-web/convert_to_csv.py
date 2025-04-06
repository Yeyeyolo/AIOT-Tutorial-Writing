import pandas as pd


with open('stm32data 1.txt', 'r', encoding='utf-8') as f:
    lines = f.readlines()

header = 'index,temperature,humidity,adcx\n'

data_lines = []
for line in lines[1:]: 
    if '；' in line:  
        continue
    processed_line = line.strip().replace(';', ',') + '\n'
    data_lines.append(processed_line)


with open('stm32_data.csv', 'w', encoding='utf-8') as f:
 
    f.write(header)

    f.writelines(data_lines)

df = pd.read_csv('stm32_data.csv')
print("Data has been successfully converted to CSV format!")
print("\nData Preview:")
print(df.head())
print("\nData information:")
print(df.info()) 