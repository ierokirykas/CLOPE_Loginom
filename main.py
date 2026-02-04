from clopi import Cluster, CLOPE
import pandas as pd
import random
# Пробуем прочитать файл

file = open("mushrooms.txt","r")
mushroomsStart = [item.replace('\n', '').split(',') for item in file.readlines()]
# print(mushroomsStart[:10])

random.seed(42)
random.shuffle(mushroomsStart)
'''
example = [['a','b'],
            ['a','b','c'],
            ['a','c','d'],
            ['d','e'],
            ['d','e','f']
]
'''
mushrooms = {}
miss_count = 0
for exampleIndex in range(0, len(mushroomsStart)):
    for index in range(0, len(mushroomsStart[exampleIndex])):
        # Первый столбец -- признак (съедобные (e) или нет(p)). Данный столбец является целым классом. По этому столбцу
        # проверяется качество тестирования
        if index != 0:
            if mushroomsStart[exampleIndex][index] != '?':
                mushrooms[exampleIndex][index - 1] = mushroomsStart[exampleIndex][index] + str(index-1)
            else:
                # print('Пропущен объект. Номер транзакции:', exampleIndex, '. Номер объекта:', index)
                miss_count += 1
        else:
            mushrooms[exampleIndex] = [''] * 22

def get_count_clusters(clope):
    # Выводим распределение по кластерам съедобных и несъедобных грибов
    answ = []
    for item in range(0, clope.max_cluster_number):
        answ.append({'e': 0, 'p': 0})
    for itemTransact in clope.transactions:
        cluster = clope.transactions[itemTransact]
        if mushroomsStart[itemTransact][0] == 'e':
            answ[cluster]['e'] += 1
        else:
            answ[cluster]['p'] += 1

    return pd.DataFrame(answ)

clope = CLOPE()
repulsion = 1
noiseLimit = 0
clope.add_cluster(mushrooms, repulsion, noiseLimit)
df = get_count_clusters(clope)
print(df)
