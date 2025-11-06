from seaborn import barplot # pip install seaborn
from matplotlib.pyplot import show

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
    
    # Приводим в правильный синтаксис
    def sort_cluster(self): 
        # Честно, я подсмотрел эту сортировку в инете. Сам бы до x:y не додумался :)
        self.transactions = {key:val for key,val in sorted(self.transactions.items(), key=lambda item: -item[1])}

    def visualize_cluster(self):
        self.sort_cluster()
        keys = list(self.transactions.keys())
        vals = list(self.transactions.values())
        barplot(x=keys,y=vals)
        show()

# Так. С кластерами разобрались. Теперь нужно сделать сам алгоритм.
class CLOPE:
    def __init__(self):
        # Чё там ему надобно?
        self.clusters = {}
        self.count_iterations = 0
        self.iterations = {}
        self.counter = 0
    # И самое основное - Profit(C,r). Который мы переделываем в DeltaAdd(C,t,r)
    # Где C - сама транзакция, t - номер кластера, r - отталкивание кластеров. В примере r = 2.6
    def deltaAdd(self,C,t,r):
        # Для начала нужно посчитать новую площадь
        area_new = self.clusters[t].area + len(C)
        # И ещё ща будем считать новые широты (ахах)
        width_new = self.clusters[t].width
        for item in C:
            if not(item in self.clusters[t].transactions):
                width_new += 1
            results = area_new * (self.clusters[t].counter + 1)/pow(width_new,r) 
            - self.clusters[t].area * self.clusters[t].counter/pow(self.clusters[t].width,r)
            # Чё-то типа такого, судя по методе.

            