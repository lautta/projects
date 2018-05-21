SET FOREIGN_KEY_CHECKS = 0;
DROP TABLE IF EXISTS user;
DROP TABLE IF EXISTS award;
DROP TABLE IF EXISTS award_type;
SET FOREIGN_KEY_CHECKS = 1;

CREATE TABLE user (
id int(11) NOT NULL AUTO_INCREMENT,
email varchar(255) NOT NULL UNIQUE,
password varchar(255) NOT NULL,
role varchar(255) NOT NULL,
name varchar(255),
creation timestamp DEFAULT CURRENT_TIMESTAMP,
signature varchar(255),
PRIMARY KEY (id)
)ENGINE=InnoDB;

CREATE TABLE award_type (
id int(11) NOT NULL AUTO_INCREMENT,
name varchar(255) NOT NULL,
PRIMARY KEY (id)
)ENGINE=InnoDB;

CREATE TABLE award (
id int(11) NOT NULL AUTO_INCREMENT,
recipient varchar(255) NOT NULL,
rec_email varchar(255) NOT NULL,
awarder_id int(11) NOT NULL,
type_id int(11) NOT NULL,
awarded datetime,
AddTime timestamp DEFAULT CURRENT_TIMESTAMP,
PRIMARY KEY (id),
FOREIGN KEY (awarder_id) REFERENCES user (id) ON DELETE CASCADE ON UPDATE CASCADE,
FOREIGN KEY (type_id) REFERENCES award_type (id) ON DELETE CASCADE ON UPDATE CASCADE
)ENGINE=InnoDB;
