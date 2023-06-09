# Контекст решения

```plantuml
@startuml
!include https://raw.githubusercontent.com/plantuml-stdlib/C4-PlantUML/master/C4_Container.puml

Person(admin, "Администратор")
Person(moderator, "Модератор")
Person(user, "Пользователь")

System(fellow_travaler_search_site, "Сервис поиска попутчиков", "Сайт для поиска попутчиков")
Rel(admin, fellow_travaler_search_site, "Просмотр, добавление и редактирование информации о пользователях, маршрутах и поездках")
Rel(moderator, fellow_travaler_search_site, "Модерация контента и пользователей")
Rel(user, fellow_travaler_search_site, "Регистрация, поиск пользователей, создание/просмотр/получение информации о маршрутах и поездках, подключение пользователей к поездке")
@enduml
```

## Назначение систем
|Система| Описание|
|-------|---------|
|Сервис поиска попутчиков| Веб-интерфейс, обеспечивающий доступ к созданию поездок с привлечением попутчиков. Бэкенд сервиса реализован в виде микросервисной архитектуры и спроектирован на основе подхода "снизу-вверх"|