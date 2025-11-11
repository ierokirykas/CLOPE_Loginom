from clopi import Cluster, CLOPE
# Пробуем прочитать файл

file = open("CLOPE_Loginom\\mushrooms.txt","r") #хехе, грибы
example = [item.replace('\n', '').split(',') for item in file.readlines()]
# print(mushroomsStart[:10])

example = [['a','b'],
                ['a','b','c'],
                ['a','c','d'],
                ['d','e'],
                ['d','e','f']
]

mushrooms = {}
for exIndex in range(0, len(example)):
    for index in range(0, len(example[exIndex])):
        if index == 0:
            mushrooms[exIndex] = [''] * len(example[exIndex])
        mushrooms[exIndex][index] = example[exIndex][index]
            
# print(mushrooms)
CC = CLOPE()
CC.add_cluster(mushrooms, repulsion= 3)
k = 0
while True:
    try:
        print(CC.clusters[k].transactions)
        k+=1
    except KeyError:
        break
# ИТОГО: Он разделил все транзакции на единичные кластеры. 
# Далее нам нужно искать более оптимальные конфигурации.