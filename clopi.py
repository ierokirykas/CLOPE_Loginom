import seaborn as sns # pip install seaborn
import matplotlib.pyplot as plt

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
        # self.height = 0 не нужен по причине того, что градиент более эффективен
        self.gradient = 0
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
                self.transactions[item] = 1
            else:
                self.transactions[item] += 1
        self.counter += 1
        # Не забываем обновлять параметры кластера
        self.area += float(len(transaction)) # Они всё равно все одинакового размера. Но потом будем их укорачивать
        self.width = float(len(self.transactions))
        self.gradient = self.area/pow(self.width,2)

# Ща посмотрим как это выглядит
CC = Cluster()
CC.add_transaction(['a','b'])
CC.add_transaction(['a','b','c'])
CC.add_transaction(['a','c','d'])
CC.add_transaction(['d','e'])
CC.add_transaction(['d','e','f'])
print(CC.transactions)
print(f'Площадь = {CC.area} \nШирина = {CC.width} \nГрадиент = {CC.gradient}')

# Не хочу писать одно и тоже каждый раз, когда нужно визуализировать. Сделаем функцию
def visualize_cluster(transactions):
    transactions = sort_cluster(transactions)
    keys = list(transactions.keys())
    vals = list(transactions.values())
    sns.barplot(x=keys,y=vals)
    plt.show()
# Ещё нужно сделать сортировку
def sort_cluster(transactions): return {key:val for key,val in sorted(transactions.items(), key=lambda item: -item[1])}
visualize_cluster(CC.transactions)