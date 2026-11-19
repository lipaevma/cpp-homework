ФизКат: учёт парка (ДЗ 2)

Команды

add scooter id name energy score - электросамокат 
add bike id name score - велосипед 
add ebike id name energy score - электровелосипед 
add helmet id name - шлем 
add dock id name energy - док-станция 
add Skateboard id name energy - скейтборд
energy - суммарная суточная энергия парка novice - техника для новичков 
list - все позиции по инвентарному номеру focus id - поставить позицию на витрину 
show_focus - показать витрину (removed, если позиции уже нет) 
remove id - убрать позицию max_energy - наибольший расход энергии среди позиций 
rejected - журнал отклонённых техосмотром quit - выход
Техосмотр отклоняет транспорт, если в имени есть broken (в любом регистре).

Наследование

InventoryItem и EnergyConsumer - абстрактные классы. От InventoryItem наследуются Transport (от него Bike, ElectricScooter, EBike), Skateboard,
 Helmet и DockStation. ElectricScooter, EBike и DockStation наследуются ещё и от EnergyConsumer.

Умные указатели

unique_ptr - новая позиция до техосмотра. При отказе удаляется, при успехе передаётся парку. 
shared_ptr - парк хранит позиции в vector<shared_ptr<InventoryItem>>. weak_ptr - витрина, указывает на позицию и не владеет ей.

Исключения

FleetError - общий класс (от std::exception). InspectionRejected - отказ техосмотра. 
ParseError - неверные данные (простота не в 1..10, пустой id, отрицательная энергия, неизвестный id, переполнение журнала). 
Ошибки печатаются только в catch в main.

Шаблон
Registry<T, Cap> - журнал до Cap записей. Используется в Park для журнала отказов техосмотра (команда rejected).

Творческая часть
Команда max_energy и журнал отказов с командой rejected. Также вид транспорта скейтборд.

Классы и функции
FleetError - запоминает текст ошибки, what() возвращает его. InspectionRejected, 
ParseError - ошибки техосмотра и данных. Registry - Add добавляет запись, Size даёт число записей, 
operator[] даёт запись по номеру, operator<< печатает журнал. EnergyConsumer - DailyEnergy даёт суточный расход энергии. 
InventoryItem - InventoryId, Name, Describe, Usable (подходит ли новичку, по умолчанию нет), 
AsConsumer (по умолчанию nullptr). Transport - хранит простоту, id и имя, проверяет их в конструкторе. 
Usable true, если простота от 6. Bike, ElectricScooter, EBike - виды транспорта. 
Describe печатает описание, у электрических ещё DailyEnergy и AsConsumer. 
Helmet, DockStation - вещи на балансе. У DockStation есть DailyEnergy и AsConsumer.
Skateboard - транспорт с длиной, проверяет длину в конструкторе.
ServiceCenter - Inspect ищет broken в имени и бросает InspectionRejected. 
operator<< и operator< для позиций - печать "id имя" и сравнение по id. 
Park - AddThing, AddTransport (с техосмотром и записью в журнал), Rejected, Remove, TotalEnergy, PrintNovice, PrintAll (по возрастанию id), Focus, ShowFocus, MaxEnergy. 
main - читает команды, создаёт объекты, вызывает методы парка, ловит ошибки.

Использование ИИ
Я пользовался ИИ (Claude) для объяснения теории, для продумывания логики и структуры классов, для реализации класса ошибок. ИИ помогал находить ошибки компиляции.
