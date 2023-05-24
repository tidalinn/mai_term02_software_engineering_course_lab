## mai_term02_software_engineering_course
> Software Engineering course (Dzuba Dmitry Vladimirovich / Ivanov Andrey Leonidovich)


# Архитектура программного обеспечения

* Выполнила: Кухтенкова П.Г.
* Группа: M8O-114M-22
* Преподаватель курса: Дзюба Д.В.
* Преподаватель: Иванов А.Л.

## Этапы домашних заданий:

1. Проектирование программной архитектуры
    > Цель: Ознакомится с инструментами проектирования в формате Architecture As A Code. Получить практический навык в моделировании в нотации C4
2. Stateful сервисы
    > Цель: Получить практические навыки в построении сервисов, работающих с данными.
3. Database sharding
4. Data cache
5. CQRS

---

# Задание №02: Stateful сервис

*Вариант 7 - Сервис поиска попутчиков*	

## Задача
Разработать приложение на языке C++, осуществляющее хранение сущностей из предыдущего шага. Для выявленных в предыдущем задании вызовов между сервисами необходимо создать REST интерфейс.


```plantuml
@startuml
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml


Person(user, "Пользователь")

Container(user_service, "Сервис авторизации", "C++")    
Container(other_service, "Сторонний сервис", "C++") 

Rel(user, user_service, "Управление пользователем")
Rel(user, other_service, "Авторизация")
Rel(other_service,user_service, "/auth - проверка логина/пароля")

@enduml
```