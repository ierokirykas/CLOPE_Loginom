# Пробуем прочитать файл
file = open("CLOPE_Loginom\\mushrooms.txt","r") #хехе, грибы
Database = []
for item in file.readlines():
    item = item.replace('\n','').split(',')
    Database.append(item)

# Так, мы можем читать список. Теперь нужно начать его обработку.
# Для начала, пусть D - это Database. Состоит из множества "транзакций" в виде хар-рик грибов.
# Узнаем уникальные значения для каждой категории D(C_j)

unique_stats = [dict() for _ in range(len(Database[0]))]
for items in Database:
    index_counter = 0
    for item in items:
        if not(item in unique_stats[index_counter]):
            # print(item)
            unique_stats[index_counter][item] = 1
        else:
            unique_stats[index_counter][item] += 1
        index_counter += 1
# print(unique_stats)
unique_stats = [{key:val for key,val in sorted(_.items(), key=lambda item: -item[1])} for _ in unique_stats]
# print(unique_stats)
# Теперь мы видим разные характеристики, которые соответствуют своему индексу.
# Вместо set используем dict, чтобы считать их количество i вхождений. Occ(i,C_j)

S = sum([sum(_.values()) for _ in unique_stats])

# Сейчас он выводит общее кол-во объектов в Database. Позже мы сможем узнавать S(C_j)
# W(C) - это просто ширина кластера len(D(C))
# H(C) - вычисляем по формуле S(C)/W(C)
# Хмм, а как насчёт сделать визуализацию? Попробую seaborn
'''
import seaborn as sns # pip install seaborn
import matplotlib.pyplot as plt
keys = list(unique_stats[1].keys())
vals = list(unique_stats[1].values())
sns.barplot(x=keys,y=vals)
plt.show() 
'''
# Это всё классно, но теперь перейдём к скучной части...
# Как я понимаю, нужно сделать структуру кластера для его дальнейшей обработки.

class Cluster:
    def __init__(self):
        # Чё там у него должно быть?
        # Площадь, высота, ширина. Вроде всё
        self.area = 0
        self.height = 0
        self.width = 0
        # Ну и сами элементы кластера, очевидно =|
        self.transactions = {}
        # И количество транзакций
        self.counter = 0

    def add_transaction(self, transaction):
        # Как нам их добавлять? У нас транзакции типа {'e','t','c'...}
        # Хотя. Я вроде делал проход по транзакциям сверху... 
        # Можно сказать, что брал базу как "один большой кластер"
        for item in transaction:
            if not(item in self.transactions):
                # print(item)
                unique_stats[index_counter][item] = 1
            else:
                unique_stats[index_counter][item] += 1
        self.counter += 1