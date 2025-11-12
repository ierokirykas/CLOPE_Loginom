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
        # print(mushrooms[exIndex][index])
            
# print(mushrooms)
clope = CLOPE()
clope.add_cluster(mushrooms, repulsion = 1.2)
k = 0
while True:
    try:
        print(clope.clusters[k].transactions)
        k+=1
    except KeyError:
        print("__________")
        break
for i in range(5):
    clope.next_step(mushrooms, repulsion = 1.2)

    while True:
        try:
            print(clope.clusters[k].transactions, clope.clusters[k].gradient)
            k+=1
        except KeyError:
            print("__________")
            break

# Происходит странная вещь при repulsion = 1.2
# Он не может определиться, какой из 2х вариантов лучше
# ыыыы. Я не знаю. Выведи что-ли весь список. И проверь работу визуализатора. Нужен ли он тебе вообще?