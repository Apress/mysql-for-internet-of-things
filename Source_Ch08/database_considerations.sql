CREATE DATABASE `plant_moisture`;

CREATE TABLE `plant_moisture`.`plants` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `moisture_raw` int(11) DEFAULT NULL,
  `date_saved` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=257 DEFAULT CHARSET=latin1;

CREATE DATABASE `read_scaling`;

CREATE TABLE `read_scaling`.`current_slave` (
  `server_id` int(11) DEFAULT NULL,
  `host` char(30) DEFAULT NULL,
  `port` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `read_scaling`.`current_slave` VALUES (0,NULL,0);
