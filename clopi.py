# Пробуем прочитать файл
file = open("CLOPE_Loginom\\mushrooms.txt","r")
Database = []
for item in file.readlines():
    item = item.replace('\n','').split(',')
    Database.append(item)

# Так, мы можем читать список. Теперь нужно начать его обработку.
# Для начала, пусть D - это Database. Состоит из множества "транзакций" в виде хар-рик грибов.
# Узнаем уникальные значения для каждой категории

unique_stats = [set() for _ in range(len(Database[0]))]
for items in Database:
    index_counter = 0
    for item in items:
        if not(item in unique_stats[index_counter]):
            # print(item)
            unique_stats[index_counter].add(item)
        index_counter += 1
print(unique_stats)

#Теперь мы видим разные характеристики, которые соответствуют своему индексу.