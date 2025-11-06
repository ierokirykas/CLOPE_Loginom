# Пробуем прочитать файл
file = open("CLOPE_Loginom\\mushrooms.txt","r")
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
print(unique_stats)

# Теперь мы видим разные характеристики, которые соответствуют своему индексу.
# Вместо set используем dict, чтобы считать их количество i вхождений. Occ(i,C_j)

S = sum([sum(_.values()) for _ in unique_stats])

# Сейчас он выводит общее кол-во объектов в Database. Позже мы сможем узнавать S(C_j)
# W(C) - это просто ширина кластера len(D(C))
# H(C) - вычисляем по формуле S(C)/W(C)
# Хмм, а как насчёт сделать визуализацию?
