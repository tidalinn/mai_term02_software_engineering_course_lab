USE tripdb;
SET FOREIGN_KEY_CHECKS=0;
DROP TABLE RouteRide;
DROP TABLE Ride;
DROP TABLE Category;
DROP TABLE Route;
DROP TABLE User;

CREATE TABLE IF NOT EXISTS `User` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `first_name` VARCHAR(256) NOT NULL,
    `last_name` VARCHAR(256) NOT NULL,
    `login` VARCHAR(256) NOT NULL,
    `password` VARCHAR(256) NOT NULL,
    `email` VARCHAR(256) NULL,
    `phone` VARCHAR(1024) NULL,
    `socials` VARCHAR(1024) NULL,
    PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`)
);

CREATE TABLE IF NOT EXISTS `Route`(
    `id` int NOT NULL AUTO_INCREMENT,
    `route_name` text NOT NULL ,
    `creator_id` int NOT NULL ,
    `point_start` text NOT NULL ,
    `point_end` text NOT NULL ,
    `point_other` text NOT NULL ,
    PRIMARY KEY (`id`)
);
                        
CREATE TABLE IF NOT EXISTS `Category`(
    `id` int NOT NULL AUTO_INCREMENT,
    `name_of_category` varchar(1024) NOT NULL ,
    PRIMARY KEY (`id`))
;

CREATE TABLE IF NOT EXISTS `Ride`(
    `id` int NOT NULL AUTO_INCREMENT,
    `ride_name`    varchar(1024) NOT NULL,
    `creator_id` int NOT NULL,
    `category_id`  int NOT NULL,
    `description`  text NOT NULL,
    `ride_date` text NOT NULL,
    PRIMARY KEY (`id`),
    KEY `FK_2` (`category_id`),
    CONSTRAINT `FK_1` FOREIGN KEY `FK_2` (`category_id`) REFERENCES `Category` (`id`)
);

CREATE TABLE IF NOT EXISTS `RouteRide`(
    `route_id` int NOT NULL ,
    `ride_id`   int NOT NULL ,
    PRIMARY KEY (`route_id`, `ride_id`),
    KEY `FK_1` (`route_id`),
    CONSTRAINT `FK_4` FOREIGN KEY `FK_1` (`route_id`) REFERENCES `Route` (`id`),
    KEY `FK_3` (`ride_id`),
    CONSTRAINT `FK_5` FOREIGN KEY `FK_3` (`ride_id`) REFERENCES `Ride` (`id`)
);

SELECT * FROM Route
LIMIT 10;

SELECT * FROM RouteRide
LIMIT 10;

SELECT * FROM User
LIMIT 10;

SELECT * FROM Ride
LIMIT 10;

SELECT * FROM Category
LIMIT 10;

USE tripdb;
SET FOREIGN_KEY_CHECKS=0;
truncate table RouteRide;
truncate table Ride;
truncate table Category;
truncate table Route;
truncate table User;

use tripdb;
INSERT INTO `User`
(user_id, login, password, first_name, last_name, email, phone)
VALUES(1, 'trvl', '123---789', 'Random', 'Traveler', 'travel_me@yandex.ru', '89997776655');

INSERT INTO `User`
(id, login, password, first_name, last_name, email, phone)
VALUES(2, 'marie', 'DDDIII__)', 'Mariya', 'Loon', 'hey@google.com', '87779996677');

INSERT INTO Category
(name_of_category)
VALUES('Local');
	  
INSERT INTO Category
(name_of_category)
VALUES('International');
	  
INSERT INTO Category
(name_of_category)
VALUES('Short');

INSERT INTO Category
(name_of_category)
VALUES('Several Days');

INSERT INTO Ride
(ride_name, creator_id, category_id, description, ride_date)
VALUES('To Moscow', 1, 1, "Short", "24.05.2023");
      
INSERT INTO Ride
(ride_name, creator_id, category_id, description, ride_date)
VALUES('welcom to Korea', 1, 2, "International", "31.05.2023");

INSERT INTO Route
(route_name, creator_id, point_start, point_end, point_other)
VALUES('Serpukhov-Moscow', 1, 'Serpukhov', 'Moscow', '');

INSERT INTO Route
(route_name, creator_id, point_start, point_end, point_other)
VALUES('Moscow-Korea', 2, 'Moscow', 'Incheong', '31.05.2023');
 
INSERT INTO Route
(route_name, creator_id, point_start, point_end, point_other)
VALUES('Stavropol-Vladivostok', 3, 'Stavropol', 'Vladivostok', '12.06.2023');      

INSERT INTO RouteRide
(route_id, ride_id)
VALUES(1,2);

INSERT INTO RouteRide
(route_id, ride_id)
VALUES(1,1);

INSERT INTO RouteRide
(route_id, ride_id)
VALUES(2,2);