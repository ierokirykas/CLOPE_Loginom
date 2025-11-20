from seaborn import barplot # pip install seaborn
from matplotlib.pyplot import show

class Cluster:
    def __init__(self):
        self.area = 0 # Площадь гистограммы
        # self.height = 0 не нужен по причине того, что градиент более эффективен
        self.gradient = 0
        self.width = 0 #Ширина кластера
        self.transactions = {} #Структура кластера
        self.counter = 0 # Количество транзакций
         
    #Перебираем все элементы гистограммы и добавляем в кластер
    def add_transaction(self, transaction):
        # Просматриваем каждый элемент транзакции и добавляем в столбец кластера.
        # Если такого столбца нет - добавляем его в кластер 
        for item in transaction:
            if not(item in self.transactions):
                self.transactions[item] = 1
            else:
                self.transactions[item] += 1
        # +1 транзакция
        self.counter += 1
        
        # Обновляем новую площадь и ширину кластера
        self.area += float(len(transaction))
        self.width = float(len(self.transactions))
        self.gradient = self.area/pow(self.width,2)

    # Удаляем транзакцию из кластера. Убираем поочерёдно все элементы заданной транзкации из кластера
    def del_transaction(self, transaction):
        for item in transaction:
            if self.transactions[item] == 0:
                del self.transactions[item]
        
        # Обновляем метрики после удаления всех элементов
        self.area -= len(transaction)
        self.width = len(self.transactions)
        self.counter -= 1
        # Пересчитываем gradient
        if self.width > 0:
            self.gradient = self.area / (self.width ** 2)
        else:
            self.gradient = 0
    
    # Сортировка кластера (чисто для visualize cluster)
    def sort_cluster(self): 
        self.transactions = {key:val for key,val in sorted(self.transactions.items(), key=lambda item: -item[1])}

    # Визуализация кластера
    def visualize_cluster(self):
        self.sort_cluster()
        keys = list(self.transactions.keys())
        vals = list(self.transactions.values())
        barplot(x=keys,y=vals)
        show()


class CLOPE:
    def __init__(self):
        self.clusters = {} # Список кластеров
        self.count_iterations = 0 # Количество пройденных итераций (steps)
        self.transactions = {} # список добавленных транзакций с указанием кластеров
        self.counttr = 0 # Количество транзакций
        self.max_cluster_number = 0 # Максимальный номер кластера
        self.noise_clusters = {} # Список номеров шумовых кластеров

    ''' 
    Считаем изменение целевой функции при добавлении транзакции к кластеру
    C - транзакция
    t - номер кластера
    r - отталкивание (repulsion)
    '''
    def deltaAdd(self,C,t,r):

        area_new = self.clusters[t].area + len(C)
        width_new = self.clusters[t].width

        for item in C:
            if item not in self.clusters[t].transactions:
                width_new += 1

        if width_new == 0:
            return 0 

        new_value = area_new * (self.clusters[t].counter + 1) / (width_new ** r)
        old_value = self.clusters[t].area * self.clusters[t].counter / (self.clusters[t].width ** r) if self.clusters[t].width > 0 else 0
    
        return new_value - old_value

    '''
    Добавляем новую транзацию в алгоритм CLOPE
    Задача - распределить её по кластерам так, чтобы целевая функция приняла максимальное значение

    transaction - сама транзакция
    id - её номер
    r - repulsion (отталкивание)
    max_count_clusters - максимальный номер кластера
    '''
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

        # Добавляем транзакцию в новый кластер 
        if max_count_clusters is None or len(self.clusters) < max_count_clusters:
            self.clusters[self.max_cluster_number] = Cluster()
            if max_delta is None or self.deltaAdd(transaction, self.max_cluster_number, r) > max_delta:
                max_delta_index = self.max_cluster_number
                self.max_cluster_number += 1
            else:
                del self.clusters[self.max_cluster_number]
        # Запоминаем кластер с наилучшим значением
        self.transactions[id] = max_delta_index

        # Добавляем транзакцию в кластер
        self.clusters[max_delta_index].add_transaction(transaction)

        return max_delta_index

    '''
    Создание кластеров

    transactions - слайс с транзакциями
    repulsion - -//-
    '''
    def add_cluster(self, transactions, repulsion = 2,
            is_noise_reduction=-1, noise_median_threshold=0.75,max_count_clusters=None):
        keys = sorted(transactions.keys())
        for item in keys:
            self.add_transaction(transactions[item],item)

        # Получаем порог шума
        if is_noise_reduction < 0:
            is_noise_reduction = self.getLimit(noise_median_threshold)
        
        if is_noise_reduction > 0:
            self.delNoise(is_noise_reduction)

        self.count_iterations = 1
    
    '''
    Выполнение алгоритма CLOPE для следующей транзакции
    transactions - слайс с транзакциями
    repulsion - -//-
    max_count_clusters - -//-
    
    '''
    def next_step(self, transactions, repulsion = 2, 
                is_noise_reduction=-1, noise_median_threshold=0.75, max_count_clusters = None):
        # Удаляем шумовые кластеры
        if is_noise_reduction < 0:
            is_noise_reduction = self.getLimit(noise_median_threshold)
        self.delNoise(is_noise_reduction)

        # Считаем кол-во перемещений транзакций
        moves = 0
        keys = sorted(transactions.keys())
        # Для каждой транзакции ищем более подходящий кластер или создаём его
        for id in keys:
            cluster_id = self.transactions[id]
            transaction = transactions[id]
            if cluster_id in self.noise_clusters:
                moves += 0
            else:
                self.clusters[cluster_id].del_transaction(transaction)
                moves += int(self.add_transaction(transaction,id,repulsion,max_count_clusters) != cluster_id)

        self.count_iterations += 1
        self.delNoise(is_noise_reduction)
        
        return moves
    
    def delNoise(self,limit):
        # Удаляем шумовые кластеры
        new_clusters = {}
        for item in self.clusters:
            if self.clusters[item].counter > limit:
                new_clusters[item] = self.clusters[item]
            else:
                self.noise_clusters[item] = True
        self.clusters = new_clusters

    def getLimit(self, percent = 0.75):
        size_clusters = []
        for item in self.clusters:
            size_clusters.append(self.clusters[item].counter)
        sorted(size_clusters)
        cluster_median = int(len(size_clusters) * percent) + 1
        if len(size_clusters) < 5:
            limit = 10
        else:
            limit = size_clusters[cluster_median]
        return limit
