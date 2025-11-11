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

    def del_transaction(self, transaction):
        for item in transaction:
            if self.transactions[item] == 0:
                del self.transactions[item]
            self.area -= float(len(transaction))
            self.width = float(len(self.transactions))
            self.counter -= 1
            return self.gradient
    
    # Приводим в правильный синтаксис
    def sort_cluster(self): 
        # Честно, я подсмотрел эту сортировку в инете. Сам бы до key:val  не додумался :)
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
        self.transactions = {}
        self.counttr = 0 #Счётчик транзакций
        self.max_cluster_number = 0 # Макс. номер кластера. Нужен для указания max_delta_index

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
            if width_new != 0:
                results = area_new * (self.clusters[t].counter + 1)/pow(width_new,r) 
            else:
                results = 0
            if self.clusters[t].width != 0:
                results_old = self.clusters[t].area * self.clusters[t].counter/pow(self.clusters[t].width,r)
            else:
                results_old = 0
            return results - results_old
            # Вооо, другое дело
        return results
    # И наконец добавляем транзакции
    def add_transaction(self, transaction, id, r=2.6, max_count_clusters = None):
        # id - номер транзакции. r - отталкивание кластеров
        self.counttr += 1
        max_delta = None
        max_delta_index = None

        # Проходим по каждому кластеру, смотрим, где транзакция будет эффективнее
        for t in self.clusters:
            delta = self.deltaAdd(transaction, t, r)
            if (delta > 0 or max_count_clusters is not None) and (max_delta is None or delta > max_delta) :
                max_delta = delta
                max_delta_index = t

        if max_count_clusters is None or len(self.clusters) < max_count_clusters:
            # print(self.max_cluster_number)
            self.clusters[self.max_cluster_number] = Cluster()
            if max_delta is None or self.deltaAdd(transaction, self.max_cluster_number, r) > max_delta:
                max_delta_index = self.max_cluster_number
                self.max_cluster_number += 1
            else:
                del self.clusters[self.max_cluster_number]
        self.transactions[id] = max_delta_index

        self.clusters[max_delta_index].add_transaction(transaction)

        return max_delta_index

        
    def add_cluster(self, transactions, repulsion = 2):
        keys = sorted(transactions.keys())
        # print(transactions[keys[0]])
        for item in keys:
            self.add_transaction(transactions[item],item)
            self.count_iterations = 1
    
    def next_step(self, transactions, repulsion = 2, max_count_clusters = None):
        # I like to move it, move it!
        moves = 0
        keys = sorted(transactions.keys())
        for id in keys:
            cluster_id = self.transactions[id]
            transaction = transactions[id]
            self.clusters[cluster_id].del_transaction(transaction)
            moves += int(self.add_transaction(transaction,id,repulsion,max_count_clusters) != cluster_id)

        self.count_iterations += 1

        return moves